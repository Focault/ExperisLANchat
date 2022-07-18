#include <stdio.h> /* FILE */
#include <signal.h> /* pid_t */
#include "Chat.h"
#define COMMAND_LEN 150


void RunChat(char* _IPaddress, char* _port, char* _username, char* _groupname)
{
    char sender[COMMAND_LEN];
    char receiver[COMMAND_LEN];
    char ip[20], port[20], username[20], groupname[50];

    if (_IPaddress != NULL && _username != NULL)
    {
        sprintf(ip, "%s", _IPaddress);
        sprintf(port, "%s", _port);
        sprintf(username, "%s", _username);
        sprintf(groupname, "%s", _groupname);

        sprintf(sender, "gnome-terminal -e \"./Sender.out %s %s %s %s\"", ip, port, _username, groupname);
        sprintf(receiver, "gnome-terminal -e \"./Receiver.out %s %s %s %s\"", ip, port, _username, groupname);

        system(receiver);
        system(sender);
    }
}


void CloseChat(char* _groupName)
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

