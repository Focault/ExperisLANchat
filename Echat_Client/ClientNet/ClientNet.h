#ifndef __CLIENTNET_H__
#define __CLIENTNET_H__
#include <arpa/inet.h>


typedef enum ClientNetResult {
            CLIENT_NET_SUCCESS,
            CLIENT_NET_SOCKET_FAIL,
            CLIENT_NET_CONNECT_FAIL,
            UNINITIALIZED_CLIENT_NET,
            UNINITIALIZED_MSG, 
            CLIENT_SEND_FAIL,
            CLIENT_RECEIVE_FAIL
}ClientNetResult;



typedef struct ClientNet ClientNet;


/**
 * @brief Function creates a client net and connects it to a server.
 * @param _IPaddress: IP address that the net will connect to.
 * @param _port: port number that the net will connect to.
 * @return Function returns pointer to the newly created client net on success.
 *         NULL if client net allocation fails, if socket function fails or if connect function fails
 */
ClientNet* InitializeClientNet(char* _IPaddress, uint32_t _port);


/**
 * @brief Function sends a message to a server.
 * @param _clientNet: pointer to the client net.
 * @param _msg: message to send to the server.
 * @param _msgSize: size of the message.
 * @return Function returns CLIENT_NET_SUCCESS if the message is sent successfully.
          UNINITIALIZED_CLIENT_NET if _clientNet is null.
          UNINITIALIZED_MSG if _msg is null.
          CLIENT_SEND_FAIL if sending the message fails.
*/
ClientNetResult ClientNetSendMessage(ClientNet* _clientNet, const void* _msg, size_t _msgSize);


/**
 * @brief Function receives a message from a server.
 * @param _clientNet: pointer to the client net.
 * @param _buffer: pointer to buffer to hold the incoming message.
 * @param _bufferSize: size of _buffer.
 * @return Function returns CLIENT_NET_SUCCESS if the message is sent successfully.
          UNINITIALIZED_CLIENT_NET if _clientNet is null.
          UNINITIALIZED_MSG if _buffer is null.
          CLIENT_RECEIVE_FAIL if receiving the message fails.
*/
ClientNetResult ClientNetReceiveMessage(ClientNet* _clientNet, char* _buffer, size_t _bufferSize);


/**
 * @brief Function destroys the client net.
 * @param _clientNet: pointer to the client net to be destroyed.
*/
void DestroyClientNet(ClientNet** _clientNet);


#endif /* __CLIENTNET_H__ */