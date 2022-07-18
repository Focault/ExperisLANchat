#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h> /* exit */
#include <string.h> /* memeset */
#include <sys/socket.h> /* socket */
#include <netinet/in.h> 
#include "Chat.h"
#define MESSAGE_LENGTH 1000
#define ARGS_COUNT 4
#define TRUE 1


int main(int argc, char const *argv[])
{
    int sock, reuse, sin_len;
    ssize_t read_bytes;
    struct sockaddr_in sin;
    struct ip_mreq mc_request; 
    char message[MESSAGE_LENGTH];
    FILE* fp;
    time_t cur_time;

    if (argc < ARGS_COUNT)
    {
        printf("ARGUMENTS COUNT ERROR\n");
    }
    
    if ((fp = fopen(PID_FILE_NAME, "a+")) != NULL)
    {
        fprintf(fp, "%d", getpid());
        fputc('\n', fp);
    }
    
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("SOCKET FAIL\n");
        exit(0);
    }

    /*allow the same port to receive multiple datagrams*/
    reuse = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse)) < 0)
    {
        printf("SETSOCKOPT REUSEADDR FAIL\n");
        close(sock);
        exit(0);
    }

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(argv[1]);
    sin.sin_port = htons(atoi(argv[2])); /*port*/
    if (bind(sock, (struct sockaddr*)&sin, sizeof(sin)) < 0)
    {
        printf("BIND FAIL\n");
        close(sock);
        exit(0);
    }

    /*allow clients to join multicast group*/
    mc_request.imr_multiaddr.s_addr = inet_addr(argv[1]); /*multicast IP*/
    mc_request.imr_interface.s_addr = INADDR_ANY;
    if(setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&mc_request, sizeof(mc_request)) < 0)
    {
        printf("SETSOCKOPT IP_ADD_MEMBERSHIP FAIL\n");
        close(sock);
        exit(0);
    }

    while (TRUE)
    {
        sin_len = sizeof(sin);
        read_bytes = recvfrom(sock, message, sizeof(message), 0, (struct sockaddr*)&sin, (socklen_t*)&sin_len);
        if (read_bytes < 0)
        {
            printf("RECV FAIL\n");      
            exit(0);
        }

        /* add date and time */
        printf("%s: %s\n", argv[3], message);
    }

    return 0;
}

#endif