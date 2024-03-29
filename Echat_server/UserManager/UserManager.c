#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HashMapAPI.h"
#include "User/User.h"
#include "User.h"
#include "UserManager.h"
#include "EchatLimits.h"
#define USER_DATA_BASE "UserInfo.txt"
#define HASH_MAP_SIZE 1000

struct UserManager
{
	HashMap *m_users;
};

static int IsKeysEquals(const void *_key1, const void *_key2);
static size_t HashFunc(const void *_key);
static void DestroyUserName(void* _username);
static UserManager *CalibrateHashMap(UserManager *_userManager);
static void RemoveNewLine(char *_str);

UserManager *CreateUserManager()
{
	UserManager *userManager;
	

	if ((userManager = (UserManager*)malloc(sizeof(UserManager))) == NULL)
	{
		return NULL;
	}
	if ((userManager->m_users = HashMap_Create(HASH_MAP_SIZE, HashFunc, IsKeysEquals)) == NULL)
	{
		free(userManager);
		return NULL;
	}

	return CalibrateHashMap(userManager);
}

UserManagerStatus UserRegister(UserManager *_userManager, char *_userName, char *_pass)
{
	FILE *fp;
	User *user, *duplicate;
	char *userName;
	size_t usernameLen;

	if (_userManager == NULL || _userName == NULL || _pass == NULL)
	{
		return USER_MANAGER_UNINITIALIZED;
	}
	if (HashMap_Find(_userManager->m_users, _userName, (void**)&duplicate) == MAP_SUCCESS)
	{
		return USER_DUPLICATE;
	}
	usernameLen = strlen(_userName);
	if ((userName = (char*)malloc((usernameLen + 1) * sizeof(char))) == NULL)
	{
		return USER_MANGER_ALLOCATION_FAIL;
	}
	strcpy(userName, _userName);
	userName[usernameLen] = '\0';

	if ((user = CreateUser(userName, _pass)) == NULL)
	{
		return USER_ILLEGAL_INPUT;
	}
	if (HashMap_Insert(_userManager->m_users, userName, user) != MAP_SUCCESS)
	{
		return USER_MANGER_ALLOCATION_FAIL;
	}

	if ((fp = fopen(USER_DATA_BASE, "a+")) == NULL)
	{
		return USER_FILE_ERROR;
	}
	fputs(userName, fp);
	fputc('\n', fp);
	fputs(_pass, fp);
	fputc('\n', fp);
	fclose(fp);
	return USER_MANAGER_SUCCESS;
}

void DestroyUserManager(UserManager **_userManager)
{
	if (_userManager != NULL && *_userManager != NULL)
	{
		HashMap_Destroy(&(*_userManager)->m_users, DestroyUserName, DestroyUser);
		free(*_userManager);
		*_userManager = NULL;
	}
}

UserManagerStatus UserLogIn(UserManager *_userManager, char *_userName, char *_pass)
{
	User *user;
	if (_userManager == NULL || _userName == NULL || _pass == NULL)
	{
		return USER_MANAGER_UNINITIALIZED;
	}
	if (HashMap_Find(_userManager->m_users, _userName, (void**)&user) != MAP_SUCCESS ||
		strcmp(GetUserPass(user), _pass))
	{
		return USER_WRONG_INPUT;
	}
	if (UserLogin(user) == USER_ALREADY_ACTIVE)
	{
		return USER_MANAGER_ALREADY_ACTIVE;
	}
	return USER_MANAGER_SUCCESS;
}



UserManagerStatus UserLogOut(UserManager *_userManager, char *_userName)
{
	User *user;
	if (_userManager == NULL || _userName == NULL )
	{
		return USER_MANAGER_UNINITIALIZED;
	}
	if (HashMap_Find(_userManager->m_users, _userName, (void**)&user) )
	{
		return USER_WRONG_INPUT;
	}
	if (UserLogout(user) == USER_ALREADY_NOT_ACTIVE)
	{
		return USER_MANAGER_ALREADY_NOT_ACTIVE;
	}
	return USER_MANAGER_SUCCESS;
}






/* Static Functions */

static size_t HashFunc(const void *_key)
{
	const char *key = (char *)_key;
	int i = 0;
	size_t index = 0;

	while (*(key + i) != '\0' && *(key + i) != '\n')
	{
		index += *(key + i);
		i++;
	}

	return (index + i);
}

static int IsKeysEquals(const void *_key1, const void *_key2)
{
	return !strcmp((char*)_key1, (char*)_key2);
}

static void DestroyUserName(void* _username)
{
	free((char*)_username);
}

static UserManager *CalibrateHashMap(UserManager *_userManager)
{
	User *user;
	FILE *fp;
	char *userName;
	char pass[20];
	
	if ((fp = fopen(USER_DATA_BASE, "r")) != NULL)
	{
	    while (!feof(fp))
	    {	
			if ((userName = (char*)malloc(MAX_NAME_LEN * sizeof(char))) == NULL)
			{
				HashMap_Destroy(&_userManager->m_users, DestroyUserName, DestroyUser);
				free(_userManager);
				return NULL;
			}
			if ((fgets(userName, MAX_NAME_LEN, fp)) != NULL &&
				fgets(pass, MAX_PASSWORD_LEN, fp) != NULL)
			{
				RemoveNewLine(userName);
				RemoveNewLine(pass);
				user = CreateUser(userName, pass);
				HashMap_Insert(_userManager->m_users, (void*)userName, (void*)user);
			}
	    }
		fclose(fp);
	}
	return _userManager;
}

static void RemoveNewLine(char *_str)
{
	int len = strlen(_str);
	if (_str[len - 1] == '\n')
	{
		_str[len - 1] = '\0';
	}
}



UserManagerStatus UserManagerJoinGroup(UserManager *_userManager, char *_userName, char *_group)
{
  User* user;
 
 
   if( _userManager == NULL || _userName == NULL ||  _group == NULL)
    {
      return USER_MANAGER_UNINITIALIZED;
    }
    
    if(HashMap_Find(_userManager->m_users, _userName, (void**)&user) != MAP_SUCCESS)
    {
      return USER_WRONG_INPUT;
    }
    
    if(!IsLoggedInUser(user))
    {
      return USER_WRONG_INPUT;
    }
       return UserAddGroup(user, _group);

}


UserManagerStatus UserManagerLeaveGroup(UserManager *_userManager, char *_userName, char *_group)
{

  User* user;
 
 
   if( _userManager == NULL || _userName == NULL ||  _group == NULL)
    {
      return USER_MANAGER_UNINITIALIZED;
    }
    
    if(HashMap_Find(_userManager->m_users, _userName, (void**)&user) != MAP_SUCCESS)
    {
      return USER_WRONG_INPUT;
    }
    
   return UserLeaveGroup(user, _group);

}

List* UserExit(UserManager *_userManager, char *_userName)
{
  List* userGroups;
  User* user;
  

  if( _userManager == NULL || _userName == NULL )
    {
      return NULL;
    }
    
     if(HashMap_Find(_userManager->m_users, _userName, (void**)&user) != MAP_SUCCESS)
    {
      return NULL;
    }
    
    if(!IsLoggedInUser(user))
     {
       return NULL;
     }
    
     userGroups = UserGetGroups(user);
     
     if(UserLogout(user) != USER_SUCCESS)
      {
        return NULL; 
      }
      
      return userGroups;
 
}
