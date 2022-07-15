#include <stdio.h>
#include"ClientApplication.h"


int main(int argc, char const *argv[])
{
    Client* client_ptr;

    client_ptr = CreateClient();
    RunClient(client_ptr);
    DestroyClient(&client_ptr);

    return 0;
}

