#ifndef __USERMANAGER_H__
#define __USERMANAGER_H__


#include "HashMapAPI.h"

typedef struct UserManager UserManager;


typedef enum UserManagerStatus{
	SUCCESS,
	USER_DUPLICATE,
	USER_ILLEGAL_INPUT,
	USER_WRONG_INPUT,
	USER_ALREADY_ACTIVE,
	USER_MANAGER_UNINITIALIZED,
	USER_MANAGER_REGISTRATION_FAILED

}UserManagerStatus;

/** 
 * @brief Create a new UserManager struct
 * @param[in] ??
 * 						  
  *  
 * @return pointer to  created userManeger or null on failure
 */


UserManager* CreateUserManager();/* file */

/** 
 * @brief Free a memory that alloceted to UserManager struct
 * @param[in] pointer to pointer to UserManager struct
 * 						  
  *  
 * @return none
 */


void DestroyUserManager(UserManager** _uM);

/** 
 * @brief add a new user to user's HashMap
 * @param[in] pointer to UserManager struct, name of new user
 * 						  
  *  
 * @return Status
 */



UserManagerStatus UserRegister(UserManager* _userManager, char* _userName, char* _pass);

/** 
 * @brief call a function of user struct  that changes a status of user to LOGIN
 * @param[in] pointer to UserManager struct, name of user, password
 * 						  
  *  
 * @return Status
 */


UserManagerStatus UserLogIn(UserManager* _userManager, char* _userName, char* _pass);


/** 
 * @brief call a function of user struct  that adds a name of group to a user's group List
 * @param[in] pointer to UserManager struct, name of user, name of group
 * 						  
  *  
 * @return Status
 */
UserManagerStatus UserJoinGroup(UserManager* _userManager, char* _userName, char* _group);


/** 
 * @brief call a function of user struct  that removes a name of group from a user's group List
 * @param[in] pointer to UserManager struct, name of user, name of group
 * 						  
  *  
 * @return Status
 */

UserManagerStatus UserManagerLeaveGroup(UserManager* _userManager, char* _userName, char* _group);

/** 
 * @brief call a function of user struct  that changes a status of user to LOGOUT
 * @param[in] pointer to UserManager struct, name of user, password
 * 						  
  *  
 * @return Status
 */

UserManagerStatus UserExit(UserManager* _userManager, char* _userName);

/*GetUserGroups*/


#endif /*__USERMANAGER_H__*/
