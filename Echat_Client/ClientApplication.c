#include <stdlib.h> /*malloc, free*/
#include <stdio.h>
#include "Chat.h"
#include "ClientApplication.h"
#include "ClientNet.h"
#include "UI.h"
#include "Protocol.h"
#include "EchatLimits.h"
#include "ServerAddress.h"
#define MAX_PORT_DIGITS 5


struct Client {
        ClientNet* m_clientNet;
};


static ClientResult HandleClientRequest(Client* _client, Protocol* _protocol);
static ClientResult HandleLogin(Client* _client, Protocol* _protocol);
static void GroupEntrance(Client* _client, Protocol* _protocol, ProtocolType _protocolType);


Client* CreateClient()
{
    Client* clientPTR;
    
    if ((clientPTR = (Client*)malloc(sizeof(Client))) == NULL)
    {
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
    MenuOptions option;
    ClientResult errcode;
    Protocol proto;

    if (_client == NULL)
    {
        return CLIENT_UNINITIALIZED;
    }

    option = WelcomeMenu();
    while (option != EXIT)
    {
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
        RunClient(_client);
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

            CloseChat(_protocol->m_groupName); /* */
            break;

        case LOGOUT:
            _protocol->m_protocolType = LOGOUT_NOTIFY;
            if ((errcode = HandleClientRequest(_client, _protocol) != CLIENT_SUCCESS))
            {
                return errcode;
            }
            /* close chat */
            break;      
    }
    return errcode;
}


static void GroupEntrance(Client* _client, Protocol* _protocol, ProtocolType _protocolType)
{
    char port[MAX_PORT_DIGITS];

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

    snprintf(port, MAX_PORT_DIGITS, "%d", _protocol->m_port);
    RunChat(_protocol->m_udpIP, port, _protocol->m_name, _protocol->m_groupName);
}






