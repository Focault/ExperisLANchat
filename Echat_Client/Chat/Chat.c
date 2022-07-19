#include <stdio.h> /* sprintf, fgets */
#include <stdlib.h> /* system */
#include <sys/types.h> /* pid_t */
#include <signal.h> /* kill */
#include <string.h> /* strcmp, atoi */
#include <errno.h>
#include "Chat.h"
#define COMMAND_LEN 150

void RunChat(char* _IPaddress, char* _port, char* _username)
{
    char sender[COMMAND_LEN];
    char receiver[COMMAND_LEN];
    /* char resizeWindows[] = "xdotool windowsize $WINDOW_ID_GOES_HERE 500 500 | xdotool search --onlyvisible --name Sender.out"; */
    char ip[20], port[20], username[20];

    if (_IPaddress != NULL && _username != NULL)
    {
        sprintf(ip, "%s", _IPaddress);
        sprintf(port, "%s", _port);
        sprintf(username, "%s", _username);

        sprintf(sender, "gnome-terminal -e \"../Chat/Sender.out %s %s %s\"", ip, port, _username);
        sprintf(receiver, "gnome-terminal -e \"../Chat/Receiver.out %s %s %s\"", ip, port, _username);

        system(receiver);
        system(sender);
        /* sleep(1);
        system(resizeWindows); */
    }
}


void CloseChat(char* _IPaddress)
{
    FILE* fp;
    char line[10], pid1[10], pid2[10];
    pid_t recvPID, sendPID;

    if ((fp = fopen(PID_FILE_NAME, "r")) == NULL)
    {
        printf("errno %d\n", errno);
        return;
    }

    while (!feof(fp))
    {
        fgets(line, 10, fp);
        if (!strcmp(line, _IPaddress))
        {
            fgets(pid1, 10, fp);
            fgets(pid1, 10, fp);
            fgets(pid2, 10, fp);
            break;
        }
    }
    fclose(fp);
    
    recvPID = atoi(pid1);
    sendPID = atoi(pid2);
    kill(recvPID, SIGTERM);
    kill(sendPID, SIGTERM);
}

