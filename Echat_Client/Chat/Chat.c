#include <stdio.h> /* FILE */
#include <signal.h> /* pid_t */
#include "Chat.h"
#define COMMAND_LEN 110


void RunChat(char* _IPaddress, char* _port, char* _username)
{
    char sender[COMMAND_LEN];
    char receiver[COMMAND_LEN];
    char ip[20], port[20], username[20];

    if (_IPaddress != NULL && _username != NULL)
    {
        sprintf(ip, "%s", _IPaddress);
        sprintf(port, "%s", _port);
        sprintf(username, "%s", _username);

        sprintf(sender, "gnome-terminal -e \"./Sender.out %s %s %s; \"", ip, port, _username);
        sprintf(receiver, "gnome-terminal -e \"./Receiver.out %s %s %s; \"", ip, port, _username);

        system(receiver);
        system(sender);
    }
}


void CloseChat()
{
    int pid;
    FILE* fp;
    
    if ((fp = fopen(PID_FILE_NAME, "r")) != NULL)
    {
        while (!feof(fp))
        {
            fscanf(fp, "%d", &pid);
            system("kill -9 " + pid);
        }
    }
}

