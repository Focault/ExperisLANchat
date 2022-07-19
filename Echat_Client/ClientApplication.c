#include <stdlib.h> /*malloc, free*/
#include <stdio.h>
#include <string.h> /* strcpy */
#include "Chat.h"
#include "ClientApplication.h"
#include "ClientNet.h"
#include "UI.h"
#include "Protocol.h"
#include "EchatLimits.h"
#include "ServerAddress.h"
#include "GenListAPI.h"
#include "ListFunctions.h"
#define MAX_PORT_DIGITS 5


struct Client {
        ClientNet* m_clientNet;
        List* m_groupsIP;
};

typedef struct Data {
        char m_groupname[MAX_GROUP_NAME_LEN];
        char m_IP[MAX_IP_LEN];
}Data;


static ClientResult HandleClientRequest(Client* _client, Protocol* _protocol);
static ClientResult HandleLogin(Client* _client, Protocol* _protocol);
static void GroupEntrance(Client* _client, Protocol* _protocol, ProtocolType _protocolType);
static char* GetGroupIP(List* _list, char* _groupName);
static ClientResult CloseAllChats(Client* _client);
static int FindGoupName(void* _element, void* _context);
static int ActionCloseChat(void* _element, void* _context);


Client* CreateClient()
{
    Client* clientPTR;
    
    if ((clientPTR = (Client*)malloc(sizeof(Client))) == NULL)
    {
        return NULL;
    }

    if ((clientPTR->m_groupsIP = ListCreate()) == NULL)
    {
        free(clientPTR);
        return NULL;
    }

    /* create socket and connect to server */
    if ((clientPTR->m_clientNet = InitializeClientNet(IPV4SERVER, SERVER_PORT)) == NULL)
    {
        free(clientPTR);
        return NULL;
    }

    return clientPTR;
}


ClientResult RunClient(Client* _client)
{
    MenuOptions option = REGISTER;
    ClientResult errcode;
    Protocol proto;

    if (_client == NULL)
    {
        return CLIENT_UNINITIALIZED;
    }
    
    while (option != EXIT)
    {
        option = WelcomeMenu();
        if (option == REGISTER)
        {
            GetUserDetails(proto.m_name, proto.m_password);
            proto.m_protocolType = REGISTRATION_REQUEST;
            if ((errcode = HandleClientRequest(_client, &proto)) != CLIENT_SUCCESS)
            {
                return errcode;
            }
            /* inform user */
            ClientRequestStatus(proto.m_reply);
        }
        else if (option == LOGIN)
        {
            GetUserDetails(proto.m_name, proto.m_password);
            proto.m_protocolType = LOGIN_REQUEST;
            if ((errcode = HandleClientRequest(_client, &proto)) != CLIENT_SUCCESS)
            {
                return errcode;
            }
            /* inform user */
            ClientRequestStatus(proto.m_reply);

            while(TRUE)
            {
                HandleLogin(_client, &proto);
                if (proto.m_protocolType == LOGOUT_NOTIFY)
                {
                    break;
                }
            }
        }
    }
    return CLIENT_SUCCESS;
}


void DestroyClient(Client** _client)
{
    if (_client != NULL && *_client != NULL)
    {
        DestroyClientNet(&(*_client)->m_clientNet);
        free(*_client);
        *_client = NULL;
    }
}



/* STATIC FUNCTIONS */


static ClientResult HandleClientRequest(Client* _client, Protocol* _protocol)
{
    char packedMSG[MAX_MESSAGE_LEN];
    size_t messageLen;
    
    /* pack and send to server */
    if (Pack(packedMSG, _protocol, &messageLen) != PACK_SUCCESS)
    {
        return CLIENT_PACKING_FAILED;
    }

    if (ClientNetSendMessage(_client->m_clientNet, packedMSG, messageLen) != CLIENT_NET_SUCCESS)
    {
        return CLIENT_SEND_FAILED;
    }

    if (_protocol->m_protocolType != GROUP_LEAVE && _protocol->m_protocolType != LOGOUT_NOTIFY && !(_protocol->m_protocolType == GROUP_LIST_RECIEVED && !_protocol->m_numOfGroupsLeft))
    {
        /* receive from server and unpack */
        if (ClientNetReceiveMessage(_client->m_clientNet, packedMSG, sizeof(packedMSG)) != CLIENT_NET_SUCCESS)
        {
            return CLIENT_RECEIVE_FAILED;
        }

        if (UnPack(packedMSG, _protocol) != PACK_SUCCESS)
        {
            return CLIENT_UNPACKING_FAILED;
        }
    }
    return CLIENT_SUCCESS;
}



static ClientResult HandleLogin(Client* _client, Protocol* _protocol)
{
    int chatOption;
    ClientResult errcode;
    char leaveIP[10];
    
    chatOption = ChatMenu();
    switch (chatOption)
    {
        case LIST_GROUPS:
            _protocol->m_protocolType = GROUP_LIST_REQUEST;
            if ((errcode = HandleClientRequest(_client, _protocol) != CLIENT_SUCCESS))
            {
                return errcode;
            }
            
            while (_protocol->m_numOfGroupsLeft)
            {
                ListGroupDetails(_protocol->m_groupName, _protocol->m_usersInGroup, _protocol->m_numOfGroupsLeft);
                _protocol->m_reply = SUCCESS;
                _protocol->m_protocolType = GROUP_LIST_RECIEVED;
                if ((errcode = HandleClientRequest(_client, _protocol) != CLIENT_SUCCESS))
                {
                    return errcode;
                }
            }
            break;

        case CREATE_GROUP:
            GroupEntrance(_client, _protocol, CREATE_GROUP_REQUEST);
            break;

        case JOIN_GROUP:
            GroupEntrance(_client, _protocol, GROUP_JOIN_REQUEST);
            break;

        case LEAVE_GROUP:
            GetGroupName(_protocol->m_groupName);
            _protocol->m_protocolType = GROUP_LEAVE;
            if ((errcode = HandleClientRequest(_client, _protocol) != CLIENT_SUCCESS))
            {
                return errcode;
            }

            if (_protocol->m_reply != SUCCESS)
            {
                ClientRequestStatus(_protocol->m_reply);
                break;
            }
            strcpy(leaveIP, GetGroupIP(_client->m_groupsIP, _protocol->m_groupName));
            CloseChat(leaveIP);
            break;

        case LOGOUT:
            _protocol->m_protocolType = LOGOUT_NOTIFY;
            if ((errcode = HandleClientRequest(_client, _protocol) != CLIENT_SUCCESS))
            {
                return errcode;
            }
            CloseAllChats(_client);
            break;      
    }
    return errcode;
}


static void GroupEntrance(Client* _client, Protocol* _protocol, ProtocolType _protocolType)
{
    char port[MAX_PORT_DIGITS];
    Data* data;

    GetGroupName(_protocol->m_groupName);
    _protocol->m_protocolType = _protocolType;
    if (HandleClientRequest(_client, _protocol) != CLIENT_SUCCESS)
    {
        return;
    }

    if (_protocol->m_reply != SUCCESS)
    {
        ClientRequestStatus(_protocol->m_reply);
        return;
    }

    if ((data = (Data*)malloc(sizeof(Data))) == NULL)
    {
        return;
    }
    strcpy(data->m_groupname, _protocol->m_groupName);
    strcpy(data->m_IP, _protocol->m_udpIP);
    if (ListPushTail(_client->m_groupsIP, data) != LIST_SUCCESS)
    {
        return;
    }

    snprintf(port, MAX_PORT_DIGITS, "%d", _protocol->m_port);
    RunChat(_protocol->m_udpIP, port, _protocol->m_name); 
}


static char* GetGroupIP(List* _list, char* _groupName)
{
    ListItr begin, end, target;
    void* data;

    begin = ListItrBegin(_list);
    end = ListItrEnd(_list);

    target = ListItrFindFirst(begin, end, FindGoupName, _groupName);
    if (target != end)
    {
        data = ListItrGet(target);
        
    }
    return ((Data*)data)->m_IP;
}


static ClientResult CloseAllChats(Client* _client)
{
    ListItr begin, end;

    begin = ListItrBegin(_client->m_groupsIP);
    end = ListItrEnd(_client->m_groupsIP);

    if (ListItrForEach(begin, end, ActionCloseChat, NULL) != end)
    {
        return CLIENT_CLOSE_CHAT_FAILED;
    }
    return CLIENT_SUCCESS;
}


static int FindGoupName(void* _element, void* _context)
{
    return !strcmp(((Data*)_element)->m_groupname, (char*)_context);
}


static int ActionCloseChat(void* _element, void* _context)
{
    CloseChat(((Data*)_element)->m_IP);
    return 1;
}
