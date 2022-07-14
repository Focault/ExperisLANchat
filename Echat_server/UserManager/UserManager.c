#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "User.h"
#include "UserManager.h"
#include "EchatLimits.h"
#define USER_DATA_BASE "UserManager/UserInfo.txt"
#define HASH_MAP_SIZE 1000

struct UserManager
{
	HashMap *m_users;
};

static int IsKeysEquals(const void *_key1, const void *_key2);
static size_t HashFunc(const void *_key);
static void DestroyUserName(void* _username);
static UserManager *CalibrateHashMap(UserManager *_userManager);

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
	
	if ((fp = fopen(USER_DATA_BASE, "a+")) != NULL)
	{
	    while (!feof(fp))
	    {	
			if ((userName = (char*)malloc(MAX_NAME_LEN * sizeof(char))) == NULL)
			{
				HashMap_Destroy(&_userManager->m_users, DestroyUserName, DestroyUser);
				free(_userManager);
				return NULL;
			}
			if ((fgets(userName, MAX_NAME_LEN, fp)) != NULL)
			{
				fgets(pass, MAX_PASSWORD_LEN, fp);
				user = CreateUser(userName, pass);
				HashMap_Insert(_userManager->m_users, (void*)userName, (void*)user);
			}
	    }
		fclose(fp);
	}
	return _userManager;
}

