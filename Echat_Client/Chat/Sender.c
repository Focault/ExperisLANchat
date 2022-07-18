#include <stdio.h>
#include <stdlib.h> /* exit */
#include <string.h> /* memeset */
#include <sys/socket.h>
#include <netinet/in.h>
#include "Chat.h"
#define MESSAGE_LENGTH 1000
#define ARGS_COUNT 5
#define TRUE 1


int main(int argc, char const *argv[])
{
    int sock, pid; 
    ssize_t sent_bytes;
    struct sockaddr_in sin;
    struct in_addr local_interface;
    char message[MESSAGE_LENGTH - 50], finalMessage[MESSAGE_LENGTH], loop;
    FILE* fp;
    
    if (argc < ARGS_COUNT)
    {
        printf("ARGUMENTS COUNT ERROR\n");
    }
    
    pid = getpid();
    if ((fp = fopen(PID_FILE_NAME, "a+")) != NULL)
    {
        fputs("argv[4]", fp);
        fputc('\n', fp);
        fputs((char*)&pid, fp);
        fputc('\n', fp);
    }

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("SOCK FAIL\n");
        exit(0);
    }

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(argv[1]); /*multicast IP*/
    sin.sin_port = htons(atoi(argv[2])); /*port*/

    while (TRUE)
    {
        printf("New message: ");
        fgets(message, MESSAGE_LENGTH, stdin);
        sprintf(finalMessage, "%s: %s", argv[3], message);
        sent_bytes = sendto(sock, finalMessage, sizeof(finalMessage), 0, (struct sockaddr*)&sin, sizeof(sin));
        if (sent_bytes < 0)
        {
            printf("SEND FAIL\n");
            exit(0);
        }
    }

    return 0;
}

