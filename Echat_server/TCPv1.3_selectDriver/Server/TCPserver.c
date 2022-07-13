#include <unistd.h>     /* close, fcntl, select */
#include <stdio.h>      /* printf, scanf */
#include <stdlib.h>     /* calloc, malloc, free */
#include <string.h>     /* memset, strlen, strncmp */
#include <netinet/in.h> /* sockaddr_in */
#include <sys/types.h>  /* ssize_t, sockaddr, select */
#include <sys/socket.h> /* socket, send, recv */
#include <arpa/inet.h>  /* htons, inet_addr */
#include <fcntl.h>      /* fcntl */
#include <errno.h>      /* errno */
#include <sys/time.h>   /* select */
#include "TCPserver.h"
#include "ListFunctions.h"
#include "ListItr.h"
#include "GenListAPI.h"
#define FAIL -1
#define SUCCESS 1
#define MAGIC_NUM 8275429569
#define BUFFER_MIN 8
#define TRUE 1
#define MAX_BACK_LOG 1023
#define FD_RANGE 1024
#define MIN_PORT 1024
#define MAX_PORT 65536 /* 64K */
#define MAX_ACTIVE_CLIENTS 1000
#define STOP 0
#define MIN_CLIENT_ID 2
#define MAX_CLIENT_ID 1023


typedef struct Statistics
{
    size_t m_proccessedClients;
    size_t m_totalReadBytes;
    size_t m_totalSentBytes;
    size_t m_acceptErrors;
    size_t m_receveErrors;
    size_t m_sendErrors;
    size_t m_totalErrors;
    size_t m_kikcedUsers;
} Statistics;

struct Server
{
    List *m_list;
    int m_listenSocket;
    struct sockaddr_in m_listenSin;
    socklen_t m_listenSinLen;
    Application *m_application;
    char m_stopFlag;
    size_t m_activeClients;
    fd_set m_masterFD;
    fd_set m_proxyFD;
    /* Statistics */
    Statistics m_statistics;
    /* Magic Num */
    size_t m_magicN;
};

typedef struct Client
{
    size_t m_clientIdx;
    int m_socket;
    struct sockaddr_in m_clientSin;
    socklen_t m_clientSinLen;
} Client;

typedef struct Context
{
    Server *m_server;
    char *m_buffer;
    size_t m_bufferSize;
    int m_clients2Serve;
    char m_kill;
} Context;

static int InitializeServerIPv4Connection(Server *_server, const char *_IP, int _port, size_t _backLog);
static int SetToNoBlocking(int _socket);
static int CreateListenSocket(Server *_server);
static int BindListeningSocket(Server *_server, const char *_IP, int _port);
static Server_Result NonBlockingAcceptError(Server *_server);
static Server_Result CheckRunParameters(Server *_server, size_t _bufferSize);
static void CheckOnConnectedClients(Server *_server, size_t _bufferSize, void *_buffer, int _clients2Serve);
static void DestroyClient(void *_client);
static Server_Result ReceiveMessage(Server *_server, size_t _bufferSize, void *_buffer, const Client *_client);
static int ReceiveMessageFromEach(void* _element, void* _context);
static Server_Result CheckForNewClients(Server *_server);
static void CheckForExitSignal(Server *_server);
static int CheckInitializeParameters(const char *_IP, int _port, Application *_application, size_t _backLog);
static void ReportFailToApp(Server *_server, int _clientID, Server_Result _fail);
static void ReportCloseClient2App(Server *_server, int _clientID);
static void ReportNewClient2App(Server *_server, Client *_client);
static int FindClientByID(void * _client, void* _clientID);

Server *InitializeIPv4Server(const char *_IP, int _port, Application *_application, size_t _backLog)
{
    Server *newServer = NULL;
    if (CheckInitializeParameters(_IP, _port, _application, _backLog) && (newServer = (Server*)calloc(1, sizeof(Server))) != NULL)
    {
        if ((newServer->m_list = ListCreate()) == NULL)
        {
            free(newServer);
            return NULL;
        }
        newServer->m_application = _application;
        if (InitializeServerIPv4Connection(newServer, _IP, _port, _backLog) < 0)
        {
            ListDestroy(&(newServer->m_list), NULL);
            free(newServer);
            return NULL;
        }
    }
    return newServer;
}

Server_Result RunServer(Server *_server, size_t _bufferSize)
{
    Server_Result status;
    void *buffer;
    int activity;
    if ((status = CheckRunParameters(_server, _bufferSize)) != SERVER_SUCCESS)
    {
        return status;
    }
    if ((buffer = (void*)calloc(1, _bufferSize)) == NULL)
    {
        return ALLOCATION_FAILED_ERR;
    }
    _server->m_stopFlag = 0;
    while (!_server->m_stopFlag)
    {
        _server->m_proxyFD = _server->m_masterFD;
        activity = select(FD_RANGE, &_server->m_proxyFD, NULL, NULL, NULL);
        if (FD_ISSET(STDIN_FILENO, &_server->m_proxyFD))
        {
            CheckForExitSignal(_server);
        }
        if (FD_ISSET(_server->m_listenSocket, &_server->m_proxyFD))
        {
            while (status == SERVER_SUCCESS)
            {
                status = CheckForNewClients(_server);
            }
            status = SERVER_SUCCESS;
            if (!(--activity)) {continue;};
        }
        CheckOnConnectedClients(_server, _bufferSize, buffer, activity);
    }
    free(buffer);
    return SERVER_SUCCESS;
}

size_t SendMessage(Server *_server, int _clientID, const void *_message, size_t _bytesInMessage)
{
    Client *remove;
    ListItr begin, end, destroy;
    ssize_t sentBytes = 0;
    if (_server != NULL && _message != NULL && _clientID >= MIN_CLIENT_ID && _clientID <= MAX_CLIENT_ID && _bytesInMessage)
    {
        sentBytes = send(_clientID, _message, _bytesInMessage, 0);
        if (sentBytes < 0)
        {
            ReportFailToApp(_server, _clientID, SEND_FAIL);
            ++_server->m_statistics.m_sendErrors;
            ++_server->m_statistics.m_totalErrors;
            perror("Server send failed");
            --_server->m_activeClients;
            ++_server->m_statistics.m_kikcedUsers;
            close(_clientID);
            FD_CLR(_clientID, &_server->m_masterFD);
            begin = ListItrBegin(_server->m_list);
            end = ListItrEnd(_server->m_list);
            destroy = ListItrFindFirst(begin, end, FindClientByID, (void*)&_clientID);
            remove = (Client*)ListItrRemove(destroy);
            free(remove);
            ReportCloseClient2App(_server, _clientID);
            return 0;
        }
        _server->m_statistics.m_totalSentBytes += sentBytes;
        printf("Sent Message To Client Descriptor #%d of %lu bytes\n", _clientID, sentBytes);
    }
    return sentBytes;
}

Server_Result StopServer(Server *_server)
{
    if (_server == NULL)
    {
        return SERVER_UNINITIALIZED_ERR;
    }
    _server->m_stopFlag = TRUE;
    return SERVER_SUCCESS;
}

void PrintStatistics(Server *_server)
{
    if (_server != NULL)
    {
        printf("\n\n**************** Server Statistics ****************\n\n"); 
        printf("Total Readed Bytes: %lu\nTotal Sent Bytes: %lu\nAccept Errors: %lu\nReceve Errors: %lu\nSend Errors: %lu\nTotal Errors: %lu\nKicked Users: %lu\nOverall proccessed clients: %lu",
               _server->m_statistics.m_totalReadBytes, _server->m_statistics.m_totalSentBytes, _server->m_statistics.m_acceptErrors, _server->m_statistics.m_receveErrors, 
               _server->m_statistics.m_sendErrors, _server->m_statistics.m_totalErrors, _server->m_statistics.m_kikcedUsers, _server->m_statistics.m_proccessedClients);
        printf("\n\n**************** ***************** ****************\n\n");
    }
}

void DestroyServer(Server *_server)
{
    if (_server != NULL && _server->m_magicN == MAGIC_NUM)
    {
        _server->m_magicN = 0;
        PrintStatistics(_server);
        ListDestroy(&_server->m_list, DestroyClient);
        free(_server);
    }
}

/* Static Functions */

static int FindClientByID(void * _client, void* _clientID)
{
    if (((Client*)_client)->m_socket == *(int*)_clientID)
    {
        return TRUE;
    }
    return 0;
}

static void CheckForExitSignal(Server *_server)
{
    char signal[4] = "goon";
    scanf("%s", signal);
    if (!strncmp(signal, "exit", 4))
    {
        _server->m_stopFlag = 1;
    }
}

/**
 * @brief checks if a new client wants to connect to server
 * 
 * @param _server - pointer to server
 * @return Server_Result 
 * @retval SERVER_SUCCESS new client connected
 * @retval SERVER_CONTINUED no client want to connect
 * @retval ACCEPT_FAIL if accept failed
 * @retval ALLOCATION_FAILED_ERR if allocation of client failed
 * @retval SERVER_OVERFLOW server is full and can't accept new clients
 */
static Server_Result CheckForNewClients(Server *_server)
{
    Client client, *clientPtr;
    client.m_clientSinLen = sizeof(client.m_clientSin);
    client.m_socket = accept(_server->m_listenSocket, (struct sockaddr *)&client.m_clientSin, &client.m_clientSinLen);
    if (client.m_socket < 0)
    {
        return NonBlockingAcceptError(_server);
    } 
    if (_server->m_activeClients >= MAX_ACTIVE_CLIENTS)
    {
        close(client.m_socket);
        return SERVER_OVERFLOW;
    }
    if ((clientPtr = (Client*)malloc(sizeof(Client))) == NULL ||
        ListPushTail(_server->m_list, (void*)clientPtr) != LIST_SUCCESS)
    {
        close(client.m_socket);
        ++_server->m_statistics.m_totalErrors;
        ReportFailToApp(_server, 0, ALLOCATION_FAILED_ERR);
        return ALLOCATION_FAILED_ERR; 
    }
    memcpy(clientPtr, &client, sizeof(client));
    clientPtr->m_clientIdx = ++_server->m_statistics.m_proccessedClients;
    ++_server->m_activeClients;
    FD_SET(client.m_socket, &_server->m_masterFD);
    ReportNewClient2App(_server, clientPtr);
    return SERVER_SUCCESS;
}

static void ReportNewClient2App(Server *_server, Client *_client)
{
    void *popped;
    if (_server->m_application->m_newClient != NULL)
    {
        if (!_server->m_application->m_newClient(_server, _client->m_socket, _server->m_application->m_context))
        {
            close(_client->m_socket);
            --_server->m_activeClients;
            FD_CLR(_client->m_socket, &_server->m_masterFD);
            ListPopTail(_server->m_list, &popped);
        }
    }
}

static void CheckOnConnectedClients(Server *_server, size_t _bufferSize, void *_buffer, int _clients2Serve)
{
    ListItr read, end, destroy;
    Context context;
    read = ListItrBegin(_server->m_list);
    end = ListItrEnd(_server->m_list);
    context.m_server = _server;
    context.m_buffer = _buffer;
    context.m_bufferSize = _bufferSize;
    context.m_clients2Serve = _clients2Serve;
    while (!ListItrEquals(read, end))
    {
        context.m_kill = STOP;
        destroy = ListItrForEach(read, end, ReceiveMessageFromEach, (void*)&context);
        if (!context.m_kill)
        {
            return;
        }
        read = ListItrNext(destroy);
        ListItrRemove(destroy);
        --_server->m_activeClients;
        ++_server->m_statistics.m_kikcedUsers;
    }
}

static int ReceiveMessageFromEach(void* _element, void* _context)
{
    if (FD_ISSET(((Client*)_element)->m_socket, &((Context*)_context)->m_server->m_proxyFD))
    {
        --((Context*)_context)->m_clients2Serve;
        if (ReceiveMessage(((Context*)_context)->m_server, ((Context*)_context)->m_bufferSize, ((Context*)_context)->m_buffer, (Client*)_element) != SERVER_SUCCESS)
        {
            close(((Client*)_element)->m_socket);
            FD_CLR(((Client*)_element)->m_socket, &((Context*)_context)->m_server->m_masterFD);
            free((Client*)_element);
            ReportCloseClient2App(((Context*)_context)->m_server, ((Client*)_element)->m_socket);
            ((Context*)_context)->m_kill = TRUE;
            return STOP;
        }
        if (!((Context*)_context)->m_clients2Serve)
        {
            return STOP;
        }
    }
    return SUCCESS;
}

static void ReportCloseClient2App(Server *_server, int _clientID)
{
    if (_server->m_application->m_closeClient != NULL)
    {
        _server->m_application->m_closeClient(_server, _clientID, _server->m_application->m_context);
    }
}

static void DestroyClient(void *_client)
{
    close(((Client*)_client)->m_socket);
    free((Client*)_client);
}

static Server_Result ReceiveMessage(Server *_server, size_t _bufferSize, void *_buffer, const Client *_client)
{
    ssize_t readBytes;
    /* receving message from client */
    readBytes = recv(_client->m_socket, _buffer, _bufferSize, 0);
    if (readBytes < 0)
    {
        ++_server->m_statistics.m_receveErrors;
        ++_server->m_statistics.m_totalErrors;
        perror("Server recv failed");
        ReportFailToApp(_server, _client->m_socket, RECV_FAIL);
        return RECV_FAIL;
    }
    if (readBytes == 0)
    {
        perror("Server recv failed - connection closed");
        return CONNECTION_CLOSED;
    }
    printf("Recived New Messege From Client #%lu with %lu bytes\n", _client->m_clientIdx, readBytes);
    _server->m_statistics.m_totalReadBytes += readBytes;
    if (!(_server->m_application->m_getMessage(_server, _client->m_socket, _buffer, readBytes, _server->m_application->m_context)))
    {
        return CONNECTION_CLOSED;
    }
    return SERVER_SUCCESS;
}

static Server_Result CheckRunParameters(Server *_server, size_t _bufferSize)
{
    if (_server == NULL)
    {
        return SERVER_UNINITIALIZED_ERR;
    }
    if (_bufferSize < BUFFER_MIN)
    {
        return BUFFER_TOO_SMALL_ERR;
    }
    return SERVER_SUCCESS;
}

static Server_Result NonBlockingAcceptError(Server *_server)
{
    if (errno != EAGAIN && errno != EWOULDBLOCK)
    {
        _server->m_statistics.m_acceptErrors++;
        _server->m_statistics.m_totalErrors++;
        perror("Accept Error");
        ReportFailToApp(_server, 0, ACCEPT_FAIL);
        return ACCEPT_FAIL;
    }
    return SERVER_CONTINUED;
}

static int InitializeServerIPv4Connection(Server *_server, const char *_IP, int _port, size_t _backLog)
{
    SetToNoBlocking(STDIN_FILENO);
    if (CreateListenSocket(_server) < 0 || BindListeningSocket(_server, _IP, _port) < 0)
    {
        return FAIL;
    }
    /* listening to clients */
    if (listen(_server->m_listenSocket, _backLog) < 0)
    {
        perror("listen failed");
        ReportFailToApp(_server, 0, LISTEN_FAIL);
        return FAIL;
    }
    _server->m_magicN = MAGIC_NUM;
    FD_ZERO(&_server->m_masterFD);
    FD_SET(STDIN_FILENO, &_server->m_masterFD);
    FD_SET(_server->m_listenSocket, &_server->m_masterFD);
    return SUCCESS;
}

static void ReportFailToApp(Server *_server, int _clientID, Server_Result _fail)
{
    if (_server->m_application->m_serverFailed != NULL)
    {
        _server->m_application->m_serverFailed(_server, _clientID, _fail, _server->m_application->m_context);
    }
}

static int BindListeningSocket(Server *_server, const char *_IP, int _port)
{
    /* Creating socket address struct */
    _server->m_listenSinLen = sizeof(_server->m_listenSin);
    memset((void*)&(_server->m_listenSin), 0, _server->m_listenSinLen);
    _server->m_listenSin.sin_family = AF_INET;
    _server->m_listenSin.sin_addr.s_addr = inet_addr(_IP);
    _server->m_listenSin.sin_port = htons(_port);
    /* Binding the server port */
    if (bind(_server->m_listenSocket, (struct sockaddr *)&(_server->m_listenSin), _server->m_listenSinLen) < 0)
    {
        perror("Server Bind failed");
        ReportFailToApp(_server, 0, BIND_FAIL);
        return FAIL;
    }
    return SUCCESS;
}

static int CreateListenSocket(Server *_server)
{
    int optval = 1;
    /* Create New Socket */
    if ((_server->m_listenSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Server socket failed");
        ReportFailToApp(_server, 0, SOCKET_FAIL);
        return FAIL;
    }
    printf("Server Listening Socket Initialized! socket Descriptor - %d\n", _server->m_listenSocket);
    if (SetToNoBlocking(_server->m_listenSocket) == FAIL)
    {
        ReportFailToApp(_server, 0, SET_TO_NO_BLOCKING_FAIL);
        return FAIL;
    }
    /* Declaring the port as reusable */
    if (setsockopt(_server->m_listenSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
    {
        perror("reuse failed");
        ReportFailToApp(_server, 0, REUSE_FAIL);
        return FAIL;
    }
    return SUCCESS;
}

static int SetToNoBlocking(int _socket)
{
    int flags;
    if (FAIL == (flags = fcntl(_socket, F_GETFL)))
    {
        perror("error at fcntl. F_GETFL");
        return FAIL;
    }
    if(FAIL == fcntl(_socket, F_SETFL, flags | O_NONBLOCK))
    {
        perror("error at fcntl. F_SETFL.");
        return FAIL;
    }
    printf("Server Socket Descriptor #%d Configured for non Blocking!\n", _socket);
    return SUCCESS;
}

static int CheckInitializeParameters(const char *_IP, int _port, Application *_application, size_t _backLog)
{
    if (_IP == NULL || _application == NULL || _application->m_getMessage == NULL || 
        !_backLog || _backLog > MAX_BACK_LOG || _port < MIN_PORT || _port > MAX_PORT)
    {
        return STOP;
    }
    return SUCCESS;
}


