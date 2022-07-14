#include "ServerApplication.h"

int main(int argc, char const *argv[])
{
    ServerApp *serverApp;
    serverApp = InitializeServerApp();
    RunServerApp(serverApp);
    DestroyServerApp(&serverApp);
    return 0;
}
