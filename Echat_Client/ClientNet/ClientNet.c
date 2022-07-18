#include <unistd.h> /* close */
#include <stdio.h>
#include <stdlib.h> /*malloc, free*/
#include <string.h> /*memset, strlen*/
#include <sys/socket.h> /*socket*/
#include <sys/select.h> /*select*/
#include <netinet/in.h> /*struct sockaddr_in*/
#include <arpa/inet.h> /* inet_addr, htons */
#include "ClientNet.h"
#include "EchatLimits.h"




struct ClientNet{
        int m_socket;
        char* m_IP;
        uint32_t m_port;
        struct sockaddr_in m_sin;
};


static ClientNetResult Initialize(ClientNet* _clientNet);


ClientNet* InitializeClientNet(char* _IPaddress, uint32_t _port)
{
    ClientNet* netPTR;
    
    if ((netPTR = (ClientNet*)malloc(sizeof(ClientNet))) == NULL)
    {
        return NULL;
    }

    netPTR->m_IP = _IPaddress;
    netPTR->m_port = _port;

    if (Initialize(netPTR) != CLIENT_NET_SUCCESS)
    {
        free(netPTR);
        return NULL;
    }
    
    return netPTR;
}


ClientNetResult ClientNetSendMessage(ClientNet* _clientNet, const void* _msg, size_t _msgSize)
{
    int sent_bytes;

    if (_clientNet == NULL)
    {
        return UNINITIALIZED_CLIENT_NET;
    }

    if (_msg == NULL)
    {
        return UNINITIALIZED_MSG;
    }

    sent_bytes = send(_clientNet->m_socket, _msg, _msgSize, 0);
    if (sent_bytes < 0)
    {
        return CLIENT_SEND_FAIL;
    }

    return CLIENT_NET_SUCCESS;
}


ClientNetResult ClientNetReceiveMessage(ClientNet* _clientNet, char* _buffer, size_t _bufferSize)
{
    int read_bytes;

    if (_clientNet == NULL)
    {
        return UNINITIALIZED_CLIENT_NET;
    }

    if (_buffer == NULL)
    {
        return UNINITIALIZED_MSG;
    }

    read_bytes = recv(_clientNet->m_socket, _buffer, _bufferSize, 0);
    if (read_bytes < 0)
    {
        return CLIENT_RECEIVE_FAIL;
    }

    return CLIENT_NET_SUCCESS;
}


void DestroyClientNet(ClientNet** _clientNet)
{
    if (_clientNet != NULL && *_clientNet != NULL)
    {
        close((*_clientNet)->m_socket);
        free(*_clientNet);
        *_clientNet = NULL;
    }
}


/* STATIC FUNCTIONS */

static ClientNetResult Initialize(ClientNet* _clientNet)
{

    if ((_clientNet->m_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        return CLIENT_NET_SOCKET_FAIL;
    }

    memset(&_clientNet->m_sin, 0, sizeof(_clientNet->m_sin));
    _clientNet->m_sin.sin_family = AF_INET;
    _clientNet->m_sin.sin_addr.s_addr = inet_addr(_clientNet->m_IP);
    _clientNet->m_sin.sin_port = htons(_clientNet->m_port);

    if (connect(_clientNet->m_socket, (struct sockaddr*)&_clientNet->m_sin, sizeof(_clientNet->m_sin)) < 0)
    {
        return CLIENT_NET_CONNECT_FAIL;
    }

    return CLIENT_NET_SUCCESS;
}





