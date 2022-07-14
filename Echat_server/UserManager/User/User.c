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

User *CreateUser(const char *_userName, const char *_password)
{
	User *user = NULL;
	if (_userName != NULL && _password != NULL)
	{
		if ((user = (User *)calloc(1, sizeof(User))) == NULL)
		{
			return NULL;
		}
		if ((user->m_groups = ListCreate()) == NULL)
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

	if ((groupName = (char *)malloc(strlen(_groupName) * sizeof(char))) == NULL)
	{
		return USER_UNINITIALIZED_ERROR;
	}

	strcpy(groupName, _groupName);

	ListPushHead(_user->m_groups, groupName);

	return USER_SUCCESS;
}

int NameGroupEquals(void *_element, void *_context)
{
	char *nameGroup1;
	char *nameGroup2;
	int i = 0;

	nameGroup1 = (char *)_element;
	nameGroup2 = (char *)_context;

	while (*(nameGroup1 + i) != '\0' || *(nameGroup2 + i) != '\0')
	{
		if (*(nameGroup1 + i) != *(nameGroup2 + i))
			return 0;
		i++;
	}

	if ((*(nameGroup1 + i) == '\0' && *(nameGroup2 + i) != '\0') || (*(nameGroup2 + i) == '\0' && *(nameGroup1 + i) != '\0'))
	{
		return 0;
	}

	return 1;
}

UserResult UserLeaveGroup(User *_user, char *_groupName)
{
	ListItr begin;
	ListItr end;
	ListItr iter;

	if (_user == NULL || _groupName == NULL)
	{
		return USER_UNINITIALIZED_ERROR;
	}

	begin = ListItrBegin(_user->m_groups);
	end = ListItrEnd(_user->m_groups);

	iter = ListItrFindFirst(begin, end, NameGroupEquals, _groupName);

	if (iter == end)
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

/*int main()
{
User* user;
ListItr begin;

List* groups;
user = CreateUser("Name", "Pass");
printf("%s\n", GetUserName(user));
groups = UserGetGroups(user);
UserAddGroup(user, "Group");
begin = ListItrBegin(groups);
printf("%d\n", NameGroupEquals(ListItrGet(begin), "Group"));
UserAddGroup(user, "Group");



printf("%s\n", (char*)ListItrGet(begin));


DestroyUser(user);
}*/
