#include <stdio.h>
#include <stdlib.h> /*malloc, free*/
#include "ClientApplication.h"
#include "ClientNet.h"
#include "UI.h"
#include "EchatLimits.h"
#include "ServerAddress.h"


struct Client {
        ClientNet* m_clientNet;
};


static ClientResult CheckClientDetails(Client* _client, ProtocolType _packProtocol);


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

    if (_client == NULL)
    {
        return CLIENT_UNINITIALIZED;
    }

    option = WelcomeMenu();
    if (option == REGISTER)
    {
        if ((errcode = CheckClientDetails(_client, REGISTRATION_REQUEST)) != CLIENT_SUCCESS)
        {
            return errcode;
        }
    }
    else if (option == LOGIN)
    {
        if ((errcode = CheckClientDetails(_client, LOGIN_REQUEST)) != CLIENT_SUCCESS)
        {
            return errcode;
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


static ClientResult CheckClientDetails(Client* _client, ProtocolType _packProtocol)
{
    Protocol ptcl;
    char packedMSG[MAX_MESSAGE_LEN];
    ClientResult errcode;
    size_t messageLen;
    
    GetUserDetails(ptcl.m_name, ptcl.m_password);

    /* pack and send to server */
    ptcl.m_protocolType = _packProtocol;
    if (Pack(packedMSG, &ptcl, &messageLen) != PACK_SUCCESS)
    {
        return CLIENT_PACKING_FAILED;
    }

    if (ClientNetSendMessage(_client->m_clientNet, packedMSG, messageLen) != CLIENT_NET_SUCCESS)
    {
        return CLIENT_SEND_FAILED;
    }

    /* receive from server and unpack */
    if (ClientNetReceiveMessage(_client->m_clientNet, packedMSG, sizeof(packedMSG)) != CLIENT_NET_SUCCESS)
    {
        return CLIENT_RECEIVE_FAILED;
    }

    if (UnPack(packedMSG, &ptcl) != PACK_SUCCESS)
    {
        return CLIENT_UNPACKING_FAILED;
    }

    /* inform user */
    ClientRequestStatus(ptcl.m_reply);

    return CLIENT_SUCCESS;
}







