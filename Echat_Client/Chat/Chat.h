#ifndef __CHAT_H__
#define __CHAT_H__
#define PID_FILE_NAME "ProcessIDs"


/**
 * @brief Function runs a chatroom.
 * @param _IPaddress Pointer to the IP address of the chatroom. Can't be null.
 * @param _port Port number of the chatroom. Can't be null.
 * @param _username Username of the user joining the chatroom.
*/
void RunChat(char* _IPaddress, char* _port, char* _username);


/**
 * @brief Function closes an open chatroom.
*/
void CloseChat();




#endif /* __CHAT_H__ */