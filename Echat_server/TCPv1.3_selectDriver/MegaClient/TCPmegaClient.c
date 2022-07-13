#include <unistd.h> /* close */
#include <stdio.h> /* printf */
#include <string.h> /* memset, strlen */
#include <stdlib.h> /* rand */
#include <netinet/in.h> /* sockaddr_in */
#include <sys/types.h> /* ssize_t, sockaddr */
#include <sys/socket.h> /* socket, sendto, recvfrom */
#include <arpa/inet.h> /* inet_addr, htons */
#define MAX_BUFFER 40
#define PRECENT 100
#define CONNECT_PROB 30
#define DISCONNECT_PROB 5
#define INTERACT_PROB 30
#define CLIENT_NUM 1000
#define ITER_NUM 500
#define SERVER_PORT 2000
#define INET4IP "127.0.0.1"

typedef struct MetaData
{
    ssize_t m_readBytes;
    ssize_t m_sentBytes;
} MetaData;

static int InitializeClientIPv4Connection(struct sockaddr_in *_sin, socklen_t _sinLen, size_t _idx);
static MetaData SendAndRecieveMessage(int _sock, const char *_data, size_t _idx);

int main(int argc, char const *argv[])
{
    struct sockaddr_in sin;
    MetaData bytes;
    char data[] = "PING";
    socklen_t sinLen = sizeof(sin);
    int n, sock[CLIENT_NUM] = {0};
    size_t i;

    for (n = 0; n < ITER_NUM; ++n)
    {
        for (i = 0; i < CLIENT_NUM; ++i)
        {
            if (!sock[i] && (rand() % PRECENT) < CONNECT_PROB)
            {
                if ((sock[i] = InitializeClientIPv4Connection(&sin, sinLen, i)) < 0)
                {
                    sock[i] = 0;
                    continue;
                }
            }
            if (sock[i])
            {
                if ((rand() % PRECENT) < DISCONNECT_PROB)
                {
                    close(sock[i]);
                    sock[i] = 0;
                    continue;
                }
                if ((rand() % PRECENT) < INTERACT_PROB)
                {
                    bytes = SendAndRecieveMessage(sock[i], data, i);
                    if (bytes.m_readBytes <= 0 || bytes.m_sentBytes < 0)
                    {
                        close(sock[i]);
                        sock[i] = 0;
                        continue;
                    }
                }
            }

        }
    }
    for (i = 0; i < CLIENT_NUM; ++i)
    {
        if (sock[i])
        {
            close(sock[i]);
        }
        sock[i] = 0;
    }
    return 0;
}

/* Static Functions */

static int InitializeClientIPv4Connection(struct sockaddr_in *_sin, socklen_t _sinLen, size_t _idx)
{
    /* Create New Socket */
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("client socket failed");
        return sock;
    }
    printf("Client #%lu Socket Initialized! socket descriptor - %d\n", _idx, sock);
    /* Creating socket address struct */
    memset(_sin, 0, _sinLen);
    (*_sin).sin_family = AF_INET;
    (*_sin).sin_addr.s_addr = inet_addr(INET4IP);
    (*_sin).sin_port = htons(SERVER_PORT);
    /* connect to server */
    if ((connect(sock, (struct sockaddr*)_sin, _sinLen)) < 0)
    {
        perror("connection failed");
        sock = -1;
    }
    return sock;
}

static MetaData SendAndRecieveMessage(int _sock, const char *_data, size_t _idx)
{
    MetaData result;
    char buffer[MAX_BUFFER] = {0};
    size_t dataLen = strlen(_data);
    /* sending message to server */
    printf("Client #%lu Sending Messege To Server: %s\n", _idx, _data);
    result.m_sentBytes = send(_sock, _data, dataLen, 0);
    if (result.m_sentBytes < 0)
    {
        perror("client send failed");
        return result;
    }
    /* receving message from server */
    printf("Client #%lu Waiting For a message from Server\n", _idx);
    result.m_readBytes = recv(_sock, buffer, sizeof(buffer), 0);
    if (result.m_readBytes < 0)
    {
        perror("client recv failed");
        return result;
    }
    if (result.m_readBytes == 0)
    {
        perror("client recv failed - connection closed");
        return result;
    }
    printf("Client #%lu Recived New Messege From Server: %s\n", _idx, buffer);
    return result;
}



