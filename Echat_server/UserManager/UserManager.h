#ifndef __USERMANAGER_H__
#define __USERMANAGER_H__

#include "GenListAPI.h"

typedef struct UserManager UserManager;

typedef enum UserManagerStatus
{
    USER_MANAGER_SUCCESS,
    USER_DUPLICATE,
    USER_ILLEGAL_INPUT,
    USER_WRONG_INPUT,
    USER_MANAGER_ALREADY_ACTIVE,
    USER_MANAGER_UNINITIALIZED,
    USER_MANGER_ALLOCATION_FAIL,
    USER_FILE_ERROR
} UserManagerStatus;

/**
 * @brief Create a new UserManager struct
 *
 * @return pointer to created userManeger or null on failure (failed allocation or file error)
 */
UserManager *CreateUserManager();

/**
 * @brief Destroy UserManager struct
 * @param _uM pointer to pointer to UserManager struct
 */
void DestroyUserManager(UserManager **_userManager);

/**
 * @brief add a new user to user's manager
 * 
 * @param _userManager pointer to UserManager struct
 * @param _userName  name of new user
 * @param _pass password of new user
 * @return UserManagerStatus 
 * 
 * @retval USER_MANAGER_SUCCESS - added user to user's manager
 * @retval USER_DUPLICATE - name user taken
 * @retval USER_ILLEGAL_INPUT - username or password illigal
 * @retval USER_MANAGER_UNINITIALIZED - one of the pointers are NULL
 * @retval USER_MANGER_ALLOCATION_FAIL - if allocation failed
 * @retval USER_FILE_ERROR - couldn't open\create file to register user
 */
UserManagerStatus UserRegister(UserManager *_userManager, char *_userName, char *_pass);

/**
 * @brief changes a status of user to LOGIN
 * 
 * @param _userManager pointer to UserManager struct
 * @param _userName  name of user
 * @param _pass password
 * @return UserManagerStatus 
 * 
 * @retval USER_MANAGER_SUCCESS - added user to user's manager
 * @retval USER_MANAGER_ALREADY_ACTIVE - user already active
 * @retval USER_WRONG_INPUT - username or password illigal
 * @retval USER_MANAGER_UNINITIALIZED - one of the pointers are NULL
 */
UserManagerStatus UserLogIn(UserManager *_userManager, char *_userName, char *_pass);

/**
 * @brief adds a name of group to a user's group connection list
 * 
 * @param _userManager pointer to UserManager struct
 * @param _userName name of user
 * @param _group name of group
 * @return UserManagerStatus
 * 
 * @retval USER_MANAGER_SUCCESS - added user to user's manager
 * @retval USER_MANAGER_UNINITIALIZED - one of the pointers are NULL
 * @retval USER_WRONG_INPUT - no user with that name
 */
UserManagerStatus UserJoinGroup(UserManager *_userManager, char *_userName, char *_group);

/**
 * @brief mark that user had left a group
 * 
 * @param _userManager - pointer to user manager
 * @param _userName - username
 * @param _group - group name
 * @return UserManagerStatus 
 *
 * @retval USER_MANAGER_SUCCESS - added user to user's manager
 * @retval USER_MANAGER_UNINITIALIZED - one of the pointers are NULL
 * @retval USER_WRONG_INPUT - no user with that name
 */
UserManagerStatus UserManagerLeaveGroup(UserManager *_userManager, char *_userName, char *_group);

/**
 * @brief mark that user had disconnected
 * 
 * @param _userManager - pointer to UserManager struct
 * @param _userName - name of user
 * @return pointer to list of groups user been connected to at logout
 * 
 * @warning if one of the pointers are NULL returns NULL
 */
List* UserExit(UserManager *_userManager, char *_userName);

#endif /*__USERMANAGER_H__*/
