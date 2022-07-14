#ifndef __USER_H__
#define __USER_H__

#include "GenListAPI.h"


typedef enum UserResult 
{
USER_SUCCESS,
USER_UNINITIALIZED_ERROR,
USER_INVALID_DATA_ERROR

}UserResult;

typedef struct User User;

/** 
 * @brief Create a new User with given name and password characteristics.
 * @param[in] _userName
 * 						  
 * @param[in]  password 
 *  
 * @return newly created user or null on failure
 */

User* CreateUser(char* _userName, char* _password);

/** 
 * @brief put user's flag login  in LOGIN 
 * @param[in] pointer to user struct
 * 						  
 * 
 * @return status  
 */



UserResult UserLogin(User *_user);


/** 
 * @brief return list of names of groups that a user are in
 * @param[in] pointer to user struct
 * 						  
 * 
 * @return NULL if pointer to user is NULL, pointer to list groups  
 */
List* UserGetGroups(User *_user);

/** 
 * @brief add to user's list group name of group
 * @param[in] pointer to user struct
 * @param[in]name of group 
 * 
 * @return Status  
 */



UserResult UserAddGroup(User *_user, char *_groupName);


/** 
 * @brief remove from user's list group name of group
 * @param[in] pointer to user struct
 * @param[in]name of group  
 * 
 * @return Status  
 */



UserResult UserLeaveGroup(User *_user, char *_groupName);

/** 
 * @brief free a memory that was allocated to user struct
 * @param[in] pointer to user struct
 *   
 * 
 * @return none  
 */


void DestroyUser(void* _user);

/** 
 * @brief return name of user 
 * @param[in] pointer to user struct
 *   
 * @return NULL if pointer to user is NULL
 * @return userName   
 */


char* GetUserName(User* _user);

/** 
 * @brief return user's password  
 * @param[in] pointer to user struct
 *   
 * @return NULL if pointer to user is NULL
 * @return userName   
 */
char* GetUserPass(User* _user);
/** 
 * @brief check if user logged on  
 * @param[in] pointer to user struct
 *   
 * @return -1 if pointer to user is NULL
 * @return 1 if   user logged on 
 * @return 1 if   user aren't logg on 
 */

int IsLoggedInUser(User* _user);


#endif /*__USER_H__*/
