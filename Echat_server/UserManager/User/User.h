#ifndef __USER_H__
#define __USER_H__

#include "GenListAPI.h"


typedef enum UserResult
{
    USER_SUCCESS,
    USER_UNINITIALIZED_ERROR,
    USER_INVALID_DATA_ERROR,
    USER_ALREADY_ACTIVE,
    USER_ALREADY_NOT_ACTIVE,
    USER_ALLOCATION_FAILED
} UserResult;

typedef struct User User;

/**
 * @brief Create a new User with given name and password characteristics.
 * 
 * @param _userName - username
 * @param _password - password
 * @return User* 
 * @warning returns NULL if one of the pointers are NULL or if failed allocation
 */
User *CreateUser(const char *_userName, const char *_password);

/**
 * @brief marks user as LOGIN
 * 
 * @param _user pointer to user struct
 * @return UserResult 
 * @retval USER_SUCCESS
 * @retval USER_UNINITIALIZED_ERROR - pointer is NULL
 * @retval USER_ALREADY_ACTIVE - if user is already connected
 */
UserResult UserLogin(User *_user);
/**
 * @brief marks user as LOGOUT
 * 
 * @param _user pointer to user struct
 * @return UserResult 
 * @retval USER_SUCCESS
 * @retval USER_UNINITIALIZED_ERROR - pointer is NULL
 * @retval USER_ALREADY_NOT_ACTIVE - if user is already exit
 */
UserResult UserLogout(User *_user);

/**
 * @brief return list of names of groups that a user are in
 * @param[in] pointer to user struct
 *
 *
 * @return NULL if pointer to user is NULL, pointer to list groups
 */
List *UserGetGroups(User *_user);

/**
 * @brief mark user as connected to group
 * 
 * @param _user pointer to user struct
 * @param _groupName name of group
 * @return UserResult 
 * @retval USER_SUCCESS
 * @retval USER_UNINITIALIZED_ERROR - one of the pointers is NULL
 * @retval USER_ALLOCATION_FAILED - if failed to mark
 */
UserResult UserAddGroup(User *_user, char *_groupName);

/**
 * @brief mark that user had left a group
 * 
 * @param _user pointer to user struct
 * @param _groupName name of group
 * @return UserResult 
 * @retval USER_SUCCESS
 * @retval USER_UNINITIALIZED_ERROR - one of the pointers is NULL
 */
UserResult UserLeaveGroup(User *_user, char *_groupName);

/**
 * @brief Destroy user struct
 * 
 * @param _user pointer to user struct
 */
void DestroyUser(void *_user);

/**
 * @brief Get the User Name
 * 
 * @param _user pointer to user struct
 * @return char* 
 * @warning NULL if pointer to user is NULL
 */
char *GetUserName(User *_user);

/**
 * @brief Get the User Password
 * 
 * @param _user - pointer to user struct
 * @return char*
 * @warning NULL if pointer to user is NULL
 */
char *GetUserPass(User *_user);

/**
 * @brief checks if user is logged in
 * 
 * @param _user pointer to user struct
 * @return int 
 * @retval PTR_NULL[-1] - if pointer to user is NULL
 * @retval TRUE[1] - if user is logged in
 * @retval FALSE[0] - if user is disconnected
 */
int IsLoggedInUser(User *_user);

#endif /*__USER_H__*/
