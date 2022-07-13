#include <stdio.h>
#include <string.h> /* strcpy */
#include "TCPserver.h"
#define QUEUE_S 1000
#define SERVER_ITER 5000000
#define BUFFER_S 4096
#define SERVER_PORT 2000
#define INET4IP "127.0.0.1"

static int PingPong(Server *_server, int _clientID, void *_message, size_t _bytesInMessage, void *_context);
static int NewClientCounter(Server *_server, int _clientID, void *_context);
static void ClosedClientsCounter(Server *_server, int _clientID, void *_context);
static void FailCounter(Server *_server, int _clientID, Server_Result _fail, void *_context);

typedef struct Counter
{
    size_t m_newClients;
    size_t m_closedClients;
    size_t m_fail;
} Counter;

int main(int argc, char const *argv[])
{
    Application app;
    Server *server;
    Counter count = {0};
    app.m_context = &count;
    app.m_closeClient = ClosedClientsCounter;
    app.m_getMessage = PingPong;
    app.m_newClient = NewClientCounter;
    app.m_serverFailed = FailCounter;

    server = InitializeIPv4Server(INET4IP, SERVER_PORT, &app, QUEUE_S);

    RunServer(server, BUFFER_S);

    DestroyServer(server);

    printf("\n\nOverall Proccessed Clients: %lu\nOverall Closed Clients: %lu\nFatal Fails: %lu\n\n", count.m_newClients, count.m_closedClients, count.m_fail);

    return 0;
}

static int PingPong(Server *_server, int _clientID, void *_message, size_t _bytesInMessage, void *_context)
{
    char response[] = "PONG";
    printf("Recieved Message From User #%d: %s\n", _clientID, (char*)_message);
    SendMessage(_server, _clientID, (void*)response, 5);
    return 1;
}

static int NewClientCounter(Server *_server, int _clientID, void *_context)
{
    ((Counter*)_context)->m_newClients++;
    return 1;
}

static void ClosedClientsCounter(Server *_server, int _clientID, void *_context)
{
    ((Counter*)_context)->m_closedClients++;
}

static void FailCounter(Server *_server, int _clientID, Server_Result _fail, void *_context)
{
    ((Counter*)_context)->m_fail++;
}