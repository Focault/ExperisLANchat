#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__
#include <stddef.h> /* size_t */

/* General Note - This Server Works In Select, which mean that it can connect only to 1021 clients simultanusly */

typedef enum Server_Result
{
    SERVER_SUCCESS,
    SERVER_CONTINUED,
    SERVER_OVERFLOW,
    CONNECTION_CLOSED,
    SERVER_UNINITIALIZED_ERR,
    BUFFER_TOO_SMALL_ERR,
    ALLOCATION_FAILED_ERR,
    RECV_FAIL,
    ACCEPT_FAIL,
    SEND_FAIL,
    BIND_FAIL,
    LISTEN_FAIL,
    REUSE_FAIL,
    SOCKET_FAIL,
    SET_TO_NO_BLOCKING_FAIL
} Server_Result;

typedef struct Server Server;

/* returns 0 to kick client from server */
typedef int (*NewClient)(Server *_server, int _clientID, void *_context);
/* returns 0 to kick client from server */
typedef int (*GetMessage)(Server *_server, int _clientID, void *_message, size_t _bytesInMessage, void *_context);

typedef void (*CloseClient)(Server *_server, int _clientID, void *_context);

typedef void (*ServerFailed)(Server *_server, int _clientID, Server_Result _fail, void *_context);

/* GetMessage can't be NULL */
typedef struct Application
{
    NewClient m_newClient;
    GetMessage m_getMessage;
    CloseClient m_closeClient;
    ServerFailed m_serverFailed;
    void *m_context;
} Application;

/**
 * @brief creates a new server
 * 
 * @param _IP - server's IP address
 * @param _port - server's port (have to be at least 1024 and 64K at most)
 * @param _application - pointer to struct for application functions (all members can be NULL except for GetMessage)
 * @param _backLog - queue size for servers waiting to connect (have to be at least 1 and 1023 at most)
 * @return Server* 
 * @warning returns NULL if failed allocation, if _IP, _application or m_getMessage are NULL or if _port or _backLog are Illigal
 */
Server *InitializeIPv4Server(const char *_IP, int _port, Application *_application, size_t _backLog);

/**
 * @brief Runs The Server
 * 
 * @param _server - pointer to server
 * @param _bufferSize - max size of received message from client in bytes
 * @return Server_Result 
 * @retval SERVER_SUCCESS Got a signal to exit
 * @retval SERVER_UNINITIALIZED_ERR pointer to server is NULL
 * @retval BUFFER_TOO_SMALL_ERR Buffer is under 8 bytes
 * @retval ALLOCATION_FAILED_ERR failed to allocate buffer
 */
Server_Result RunServer(Server *_server, size_t _bufferSize);

/**
 * @brief Sends a message to client
 * 
 * @param _server - pointer to server
 * @param _clientID - the ID of the client
 * @param _message - pointer to message to send
 * @param _bytesInMessage - size of message to send in bytes
 * @return Bytes Sent 
 * @retval if _server or _message are NULL or if _bytesInMessage is 0 returns 0 
 */
size_t SendMessage(Server *_server, int _clientID, const void *_message, size_t _bytesInMessage);

/**
 * @brief Stops the run of the server
 * 
 * @param _server - pointer to server
 * @return Server_Result 
 * @retval SERVER_SUCCESS - stopped server
 * @retval SERVER_UNINITIALIZED_ERR pointer to server is NULL
 */
Server_Result StopServer(Server *_server);

/**
 * @brief print server statistics
 * 
 * @param _server - pointer to server
 */
void PrintStatistics(Server *_server);

/**
 * @brief Destroies the server (if _server is null won't execute)
 * 
 * @param _server - server to destroy
 */
void DestroyServer(Server *_server);

#endif /* __TCPSERVER_H__ */