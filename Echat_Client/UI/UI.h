#ifndef __UI_H__
#define __UI_H__
#include "Protocol.h"


typedef enum MenuOptions {
            REGISTER,
            LOGIN,
            EXIT,
            ILLEGAL_OPTION
} MenuOptions;



/**
 * @brief Fucntion presents the app options to the user.
 * @return MenuOptions: function returns the option chosen by the user.
           Option 1: REGISTER - register to the app.
           Option 2: LOGIN - log in to the app.
           Option 3: EXIT - exit the app.
           ILLEGAL_OPTION if option is neither of the above.
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







#endif /* __UI_H__ */