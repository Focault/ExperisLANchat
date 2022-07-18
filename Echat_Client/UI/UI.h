#ifndef __UI_H__
#define __UI_H__
#include "Protocol.h"


typedef enum MenuOptions {
            REGISTER,
            LOGIN,
            EXIT,
} MenuOptions;


typedef enum ChatOptions {
            LIST_GROUPS,
            CREATE_GROUP,
            JOIN_GROUP,
            LEAVE_GROUP,
            LOGOUT,
} ChatOptions;



/**
 * @brief Fucntion presents the app options to the user.
 * @return MenuOptions: function returns the option chosen by the user.
           Option 1: REGISTER - register to the app.
           Option 2: LOGIN - log in to the app.
           Option 3: EXIT - exit the app.
*/
MenuOptions WelcomeMenu();


/**
 * @brief Function gets data from user.
 * @param _username: a user provided username for creating an account.
 * @param _password: the password chosen by the user for the account.
 * @note: username and password must obey the app's rules.
*/
void GetUserDetails(char* _username, char* _password);


/**
 * @brief Function returns the user's request status: success or failure.
*/
void ClientRequestStatus(Reply _reply);


/**
 * @brief Fucntion presents the chat options to the user.
 * 
 * @return ChatOptions: function returns the option chosen by the user.
           Option 1: LIST_GROUPS - list all existing groups.
           Option 2: CREATE_GROUP - create a new group.
           Option 3: JOIN_GROUP - join an existing group.
           Option 4: LEAVE_GROUP - leave a group.
           Option 5: LOGOUT - leave chat and go back to welcome menu.
 */
ChatOptions ChatMenu();


/**
 * @brief Function gets group details from user.
 * @param _groupName: pointer to hold the user provided group details.
 */
void GetGroupName(char* _groupName);


/**
 * @brief Function presents the details of an existing group, and the number of remaining groups. 
 */
void ListGroupDetails(char* _groupName, int _numOfParticipants, int _numOfGroupsLeft);



#endif /* __UI_H__ */