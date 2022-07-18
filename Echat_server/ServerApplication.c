#include <stdlib.h> /* calloc */
#include <string.h> /* strncpy */
#include "ServerApplication.h"
#include "TCPserver.h"
#include "ServerAddress.h"
#include "EchatLimits.h"
#include "Protocol.h"
#include "UserManager.h"
#include "GenListAPI.h"
#include "ListItr.h"
#include "GroupsManager.h"
#define BACK_LOG 1000

struct ServerApp
{
    Server *m_server;
    UserManager *m_userManager;
    GroupsManager *m_groupManager;
    Application m_application;
    List* m_groupInfoList;
    ListItr m_read;
    unsigned char m_groupsLeft;
};

static int GetMessageAndProccess(Server *_server, int _clientID, void *_message, size_t _bytesInMessage, void *_context);
static void FailedServer(Server *_server, int _clientID, Server_Result _fail, void *_context);
static void RegistrationRequestAndReply(Server *_server, int _clientID, ServerApp *_context, Protocol *_protocol);
static void LoginRequestAndReply(Server *_server, int _clientID, ServerApp *_context, Protocol *_protocol);
static void LogOutNotify(ServerApp *_context, Protocol *_protocol);
static void GroupJoinRequestAndReply(Server *_server, int _clientID, ServerApp *_context, Protocol *_protocol);
static void GroupCreateRequestAndReply(Server *_server, int _clientID, ServerApp *_context, Protocol *_protocol);
static void GroupLeaveNotify(ServerApp *_context, Protocol *_protocol);
static void SendGroupListToClient(Server *_server, int _clientID, ServerApp *_context, Protocol *_protocol);

ServerApp* InitializeServerApp()
{
    ServerApp *serverApp;
    if ((serverApp = (ServerApp*)calloc(1, sizeof(ServerApp))) != NULL)
    {
        serverApp->m_application.m_getMessage = GetMessageAndProccess;
        serverApp->m_application.m_context = (void*)serverApp;
        serverApp->m_application.m_serverFailed = FailedServer;
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
        if ((serverApp->m_groupManager = CreateGroupManager()) == NULL)
        {
            DestroyServer(serverApp->m_server);
            DestroyUserManager(&serverApp->m_userManager);
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
        DestroyGroupManager(&((*_serverApp)->m_groupManager));
        free(*_serverApp);
        *_serverApp = NULL;
    }
}

/* Static Functions */

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
            LoginRequestAndReply(_server, _clientID, (ServerApp*)_context, &messageProtocol);
            break;
        case GROUP_JOIN_REQUEST:
            GroupJoinRequestAndReply(_server, _clientID, (ServerApp*)_context, &messageProtocol);
            break;
        case CREATE_GROUP_REQUEST:
            GroupCreateRequestAndReply(_server, _clientID, (ServerApp*)_context, &messageProtocol);
            break;
        case GROUP_LEAVE:
            GroupLeaveNotify((ServerApp*)_context, &messageProtocol);
            break;
        case GROUP_LIST_REQUEST:
        case GROUP_LIST_RECIEVED:
            SendGroupListToClient(_server, _clientID, (ServerApp*)_context, &messageProtocol);
            break;
        case LOGOUT_NOTIFY:
            LogOutNotify((ServerApp*)_context, &messageProtocol);
        default:
            return FALSE;
    }
    return TRUE;
}

static void SendGroupListToClient(Server *_server, int _clientID, ServerApp *_context, Protocol *_protocol)
{
    size_t packageLength;
    char buffer[MAX_MESSAGE_LEN];
    const GroupInfo *details;
    if (_protocol->m_protocolType == GROUP_LIST_REQUEST)
    {
        if (_context->m_groupInfoList != NULL)
        {
            return;
        }
        _context->m_groupsLeft = 0;
        if ((_context->m_groupInfoList = ListGroups(_context->m_groupManager)) != NULL)
        {
            _context->m_read = ListItrBegin(_context->m_groupInfoList);
            _context->m_groupsLeft = (unsigned char)ListSize(_context->m_groupInfoList);
        }
    }
    if (_protocol->m_protocolType == GROUP_LIST_RECIEVED)
    {
        _context->m_read = ListItrNext(_context->m_read);
        --_context->m_groupsLeft;
    }
    _protocol->m_numOfGroupsLeft = _context->m_groupsLeft;
    _protocol->m_protocolType = GROUP_LIST_REPLY;
    if (_context->m_groupsLeft)
    {
        details = (GroupInfo*)ListItrGet(_context->m_read);
        strncpy(_protocol->m_groupName, details->m_groupName, MAX_GROUP_NAME_LEN);
        _protocol->m_usersInGroup = details->m_usersInGroup;
    } else {
        DemolishGroupList(&_context->m_groupInfoList);
    }
    Pack((void*)buffer, _protocol, &packageLength);
    SendMessage(_server, _clientID, (void*)buffer, packageLength);
}

static void GroupLeaveNotify(ServerApp *_context, Protocol *_protocol)
{
    UserManagerLeaveGroup(_context->m_userManager, _protocol->m_name, _protocol->m_groupName);
    LeaveGroup(_context->m_groupManager, _protocol->m_groupName);
}

static void LogOutNotify(ServerApp *_context, Protocol *_protocol)
{
    List *groups = UserExit(_context->m_userManager, _protocol->m_name);
    UserExitAllGroups(_context->m_groupManager, groups);
}

static void GroupJoinRequestAndReply(Server *_server, int _clientID, ServerApp *_context, Protocol *_protocol)
{
    size_t packageLength;
    char buffer[MAX_MESSAGE_LEN];
    UserManagerStatus userStatus;
    GroupManager_Result groupStatus;
    groupStatus = JoinGroup(_context->m_groupManager, _protocol->m_groupName);
    switch (groupStatus)
    {
    case GROUP_MANAGER_GROUP_NOT_FOUND:
        _protocol->m_reply = GROUP_FAIL_GROUP_NOT_FOUND;
        break;
    case GROUP_MANAGER_GROUP_AT_FULL_CAPACITY:
        _protocol->m_reply = GROUP_FAIL_GROUP_AT_FULL_CAPACITY;
        break;
    }
    if (groupStatus == GROUP_MANAGER_SUCCESS)
    {
        if ((userStatus = UserManagerJoinGroup(_context->m_userManager, _protocol->m_name, _protocol->m_groupName)) != USER_MANAGER_SUCCESS)
        {
            _protocol->m_reply = GROUP_FAIL_WRONG_USERNAME;
            LeaveGroup(_context->m_groupManager, _protocol->m_groupName);
        }
    }
    _protocol->m_protocolType = GROUP_JOIN_REPLY;
    if (groupStatus == GROUP_MANAGER_SUCCESS && userStatus == USER_MANAGER_SUCCESS)
    {
        GetGroupDetails(_context->m_groupManager, _protocol->m_groupName, _protocol->m_udpIP, &_protocol->m_port);
        _protocol->m_reply = SUCCESS;
    }
    Pack((void*)buffer, _protocol, &packageLength);
    SendMessage(_server, _clientID, (void*)buffer, packageLength);
}

static void GroupCreateRequestAndReply(Server *_server, int _clientID, ServerApp *_context, Protocol *_protocol)
{
    size_t packageLength;
    char buffer[MAX_MESSAGE_LEN];
    UserManagerStatus userStatus;
    GroupManager_Result groupStatus;
    if ((userStatus = UserManagerJoinGroup(_context->m_userManager, _protocol->m_name, _protocol->m_groupName)) != USER_MANAGER_SUCCESS)
    {
        _protocol->m_reply = CREATE_GROUP_FAIL_WRONG_USERNAME;
    }
    _protocol->m_protocolType = GROUP_JOIN_REPLY;
    if (userStatus == USER_MANAGER_SUCCESS)
    {
        if ((groupStatus = CreateNewGroup(_context->m_groupManager, _protocol->m_groupName, _protocol->m_udpIP, &_protocol->m_port)) != GROUP_MANAGER_SUCCESS)
        {
            switch (groupStatus)
            {
            case GROUP_MANAGER_GROUPS_OVERFLOW:
                _protocol->m_reply = CREATE_GROUP_FAIL_OVERFLOW;
                break;
            case GROUP_MANAGER_DUPLICATE:
                _protocol->m_reply = CREATE_GROUP_FAIL_DUPLICATE;
                break;
            case GROUP_MANAGER_UNINITIALIZED:
                _protocol->m_reply = CREATE_GROUP_FAIL_ILLIGAL_INPUT;
                break;
            default:
                _protocol->m_reply = CREATE_GROUP_FAIL_TRY_AGAIN;
            }
            UserManagerLeaveGroup(_context->m_userManager, _protocol->m_name, _protocol->m_groupName);
        } else {
            _protocol->m_reply = SUCCESS;
        }
    }
    Pack((void*)buffer, _protocol, &packageLength);
    SendMessage(_server, _clientID, (void*)buffer, packageLength);
}

static void RegistrationRequestAndReply(Server *_server, int _clientID, ServerApp *_context, Protocol *_protocol)
{
    size_t packageLength;
    char buffer[MAX_MESSAGE_LEN];
    UserManagerStatus status;
    status = UserRegister(_context->m_userManager, _protocol->m_name, _protocol->m_password);
    switch (status)
    {
    case USER_MANAGER_SUCCESS:
        _protocol->m_reply = SUCCESS;
        break;
    case USER_DUPLICATE:
        _protocol->m_reply = REG_FAIL_USR_EXIST;
        break;
    case USER_MANGER_ALLOCATION_FAIL:
    case USER_FILE_ERROR:
        _protocol->m_reply = REG_FAIL_TRY_AGAIN;
        break;
    default:
    case USER_ILLEGAL_INPUT:
        _protocol->m_reply = REG_FAIL_ILLIGAL_INPUT;
        break;
    }
    _protocol->m_protocolType = REGISTRATION_REPLY;
    Pack((void*)buffer, _protocol, &packageLength);
    SendMessage(_server, _clientID, (void*)buffer, packageLength);
}

static void LoginRequestAndReply(Server *_server, int _clientID, ServerApp *_context, Protocol *_protocol)
{
    size_t packageLength;
    char buffer[MAX_MESSAGE_LEN];
    UserManagerStatus status;
    status = UserLogIn(_context->m_userManager, _protocol->m_name, _protocol->m_password);
    switch (status)
    {
    case USER_MANAGER_SUCCESS:
        _protocol->m_reply = SUCCESS;
        break;
    case USER_MANAGER_ALREADY_ACTIVE:
        _protocol->m_reply = LOGIN_FAIL_ALREADY_ACTIVE;
        break;
    case USER_WRONG_INPUT:
    default:
        _protocol->m_reply = LOGIN_FAIL_WRONG_INPUT;
        break;
    }
    _protocol->m_protocolType = LOGIN_REPLY;
    Pack((void*)buffer, _protocol, &packageLength);
    SendMessage(_server, _clientID, (void*)buffer, packageLength);
}

static void FailedServer(Server *_server, int _clientID, Server_Result _fail, void *_context)
{
    PrintStatistics(_server);
}