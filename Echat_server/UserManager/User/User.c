#include <stdlib.h> /* calloc */
#include <string.h> /* strncpy */
#include "GenListAPI.h"
#include "ListItr.h"
#include "ListFunctions.h"
#include "User.h"
#include "EchatLimits.h"
#define USER_MAGIC_NUMBER 892350473

struct User
{
	char m_userName[MAX_NAME_LEN];
	char m_password[MAX_PASSWORD_LEN];
	List *m_groups;
	switch_t m_loginFlag;
	size_t m_magicNum;
};

static int NameGroupEquals(void *_element, void *_context);
static ListItr IsGroupExistInList(List *_list, char *_groupName);

User *CreateUser(const char *_userName, const char *_password)
{
	User *user = NULL;
	if (_userName != NULL && _password != NULL)
	{
		if ((user = (User *)calloc(1, sizeof(User))) == NULL)
		{
			return NULL;
		}
		if ((user -> m_groups = ListCreate()) == NULL)
		{
			free(user);
			return NULL;
		}
		strncpy(user->m_userName, _userName, MAX_NAME_LEN);
		strncpy(user->m_password, _password, MAX_PASSWORD_LEN);
		user->m_magicNum = USER_MAGIC_NUMBER;
	}
	return user;
}

int IsLoggedInUser(User *_user)
{
	if (_user == NULL)
	{
		return PTR_NULL;
	}
	return _user->m_loginFlag;
}

void DestroyUser(void *_user)
{
	if (_user != NULL && ((User *)_user)->m_magicNum == USER_MAGIC_NUMBER)
	{
		ListDestroy(&((User *)_user)->m_groups, NULL);
		((User *)_user)->m_magicNum = 0;
		free((User *)_user);
	}
}

UserResult UserLogin(User *_user)
{
	if (_user == NULL)
	{
		return USER_UNINITIALIZED_ERROR;
	}
	if (_user->m_loginFlag == TRUE)
	{
		return USER_ALREADY_ACTIVE;
	}
	_user->m_loginFlag = TRUE;
	return USER_SUCCESS;
}



UserResult UserLogout(User *_user)
{
	if (_user == NULL)
	{
		return USER_UNINITIALIZED_ERROR;
	}
	if (_user->m_loginFlag == FALSE)
	{
		return USER_ALREADY_NOT_ACTIVE;
	}
	_user->m_loginFlag = FALSE;
	return USER_SUCCESS;
}



List *UserGetGroups(User *_user)
{
	if (_user == NULL)
	{
		return NULL;
	}
	return _user->m_groups;
}

UserResult UserAddGroup(User *_user, char *_groupName)
{
	char *groupName;

	if (_user == NULL || _groupName == NULL)
	{
		return USER_UNINITIALIZED_ERROR;
	}

	if (IsGroupExistInList(_user-> m_groups, _groupName) == NULL)
	{
		if ((groupName = (char*)malloc(strlen(_groupName) * sizeof(char))) == NULL ||
			ListPushHead(_user->m_groups, groupName) != LIST_SUCCESS)
		{
			return USER_ALLOCATION_FAILED;
		}

		strcpy(groupName, _groupName);
	}
	return USER_SUCCESS;
}

UserResult UserLeaveGroup(User *_user, char *_groupName)
{
	ListItr iter;

	if (_user == NULL || _groupName == NULL)
	{
		return USER_UNINITIALIZED_ERROR;
	}

	if ((iter = IsGroupExistInList(_user->m_groups, _groupName)) == NULL)
	{
	  return USER_INVALID_DATA_ERROR;
	}
	ListItrRemove(iter);
	return USER_SUCCESS;
}

char *GetUserPass(User *_user)
{
	if (_user == NULL)
	{
		return NULL;
	}
	return _user->m_password;
}

char *GetUserName(User *_user)
{
	if (_user == NULL)
	{
		return NULL;
	}
	return _user->m_userName;
}

/* Static Functions */

static int NameGroupEquals(void *_groupName1, void *_groupName2)
{
	return !strcmp((char*)_groupName1, (char*)_groupName2);
}

static ListItr IsGroupExistInList(List *_list, char *_groupName)
{
	ListItr begin, end, iter;

	begin = ListItrBegin(_list);
	end = ListItrEnd(_list);

	iter = ListItrFindFirst(begin, end, NameGroupEquals, _groupName);

	if (!ListItrEquals(iter, end))
	{
		return iter;
	}
	return NULL;
}
