#include <stdlib.h> /* calloc */
#include "ServerApplication.h"
#include "TCPserver.h"
#include "ServerAddress.h"
#include "EchatLimits.h"
#include "Protocol.h"
#include "UserManager.h"
#define BACK_LOG 1000

struct ServerApp
{
    Server *m_server;
    UserManager *m_userManager;
    Application m_application;
};

static int GetMessageAndProccess(Server *_server, int _clientID, void *_message, size_t _bytesInMessage, void *_context);
static void RegistrationRequestAndReply(Server *_server, int _clientID, ServerApp *_context, Protocol *_protocol);
static void LoginRequestAndReply(Server *_server, int _clientID, ServerApp *_context, Protocol *_protocol);

ServerApp* InitializeServerApp()
{
    ServerApp *serverApp;
    if ((serverApp = (ServerApp*)calloc(1, sizeof(ServerApp))) != NULL)
    {
        serverApp->m_application.m_getMessage = GetMessageAndProccess;
        serverApp->m_application.m_context = (void*)serverApp;
        if ((serverApp->m_server = InitializeIPv4Server(IPV4SERVER, SERVER_PORT, &serverApp->m_application, BACK_LOG)) == NULL)
        {
            free(serverApp);
            return NULL;
        }
        if ((serverApp->m_userManager = CreateUserManager()) == NULL)
        {
            DestroyServer(serverApp->m_server);
            free(serverApp);
            return NULL;
        }
    }
    return serverApp;
}

ServerAppResult RunServerApp(ServerApp *_serverApp)
{
    Server_Result serverStatus;
    if (_serverApp == NULL)
    {
        return SERVER_APP_UNINITIALIZED;
    }
    if ((serverStatus = RunServer(_serverApp->m_server, MAX_MESSAGE_LEN)) == ALLOCATION_FAILED_ERR)
    {
        return SERVER_APP_ALLOCATION_FAIL;
    }
    return SERVER_APP_SUCCESS;
}

void DestroyServerApp(ServerApp **_serverApp)
{
    if (_serverApp != NULL && *_serverApp != NULL)
    {
        DestroyServer((*_serverApp)->m_server);
        DestroyUserManager(&((*_serverApp)->m_userManager));
        free(*_serverApp);
    }
}

static int GetMessageAndProccess(Server *_server, int _clientID, void *_message, size_t _bytesInMessage, void *_context)
{
    Protocol messageProtocol;
    UnPack(_message, &messageProtocol);
    switch (messageProtocol.m_protocolType)
    {
        case REGISTRATION_REQUEST:
            RegistrationRequestAndReply(_server, _clientID, (ServerApp*)_context, &messageProtocol);
            break;
        case LOGIN_REQUEST:
            /* char* Username, char* Password */
            break;
        case LOGOUT_NOTIFY:
            /* Username */
            break;
        case GROUP_JOIN_REQUEST:
            /* char* Username, char* GroupName */
            break;
        case CREATE_GROUP_REQUEST:
            /* char* Username, char* GroupName */
            break;
        case GROUP_LEAVE:
            /* char* Username, char* GroupName */
            break;
        case GROUP_LIST_REQUEST:
            break;
        default:
            return FALSE;
    }
    return TRUE;
}

static void RegistrationRequestAndReply(Server *_server, int _clientID, ServerApp *_context, Protocol *_protocol)
{
    size_t packageLength;
    char buffer[MAX_MESSAGE_LEN];
    UserManagerStatus status;
    status = UserRegister(_context->m_userManager, _protocol->m_name, _protocol->m_password);
    if (status == USER_MANAGER_REGISTRATION_FAILED || status == USER_DUPLICATE)
    {
        _protocol->m_reply = REG_FAIL_USR_EXIST;
    } else {
        _protocol->m_reply = SUCCESS;
    }
    _protocol->m_protocolType = REGISTRATION_REPLY;
    Pack((void*)buffer, &_protocol, &packageLength);
    SendMessage(_server, _clientID, (void*)buffer, packageLength);
}

static void LoginRequestAndReply(Server *_server, int _clientID, ServerApp *_context, Protocol *_protocol)
{
    size_t packageLength;
    char buffer[MAX_MESSAGE_LEN];
    UserManagerStatus status;
    status = UserLogIn(_context->m_userManager, _protocol->m_name, _protocol->m_password);
    if (status == USER_MANAGER_REGISTRATION_FAILED || status == USER_DUPLICATE)
    {
        _protocol->m_reply = REG_FAIL_USR_EXIST;
    } else {
        _protocol->m_reply = SUCCESS;
    }
    _protocol->m_protocolType = LOGIN_REPLY;
    Pack((void*)buffer, &_protocol, &packageLength);
    SendMessage(_server, _clientID, (void*)buffer, packageLength);
}