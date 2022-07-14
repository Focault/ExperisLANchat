#ifndef __CLIENT_APP_H__
#define __CLIENT_APP_H__


typedef enum ClientResult {
            CLIENT_SUCCESS,
            CLIENT_UNINITIALIZED,
            CLIENT_PACKING_FAILED,
            CLIENT_UNPACKING_FAILED,
            CLIENT_SEND_FAILED,
            CLIENT_RECEIVE_FAILED

} ClientResult;


typedef struct Client Client;


/**
 * @brief Function creates a new client and connects it to the server.
 * @return Function returns a pointer to the newly created client.
 *          NULL if client allocation or initialization fails.
*/
Client* CreateClient();


/**
 * @brief Function allows communication between a client and server.
 * @param _client: pointer to the client.
 * @return Function returns CLIENT_SUCCESS on success.
          CLIENT_UNINITIALIZED if _client is null.
          CLIENT_PACKING_FAILED if packing the message to be sent fails.
          CLIENT_UNPACKING_FAILED if unpacking the received  message fails.
          CLIENT_SEND_FAILED if sending a message fails.
          CLIENT_RECEIVE_FAILED if receiving a message fails.
*/
ClientResult RunClient(Client* _client);


/**
 * @brief Function destroys a client.
 * @param _client: pointer to the client to be destroyed.
*/
void DestroyClient(Client** _client);



#endif /* __CLIENT_APP_H__ */