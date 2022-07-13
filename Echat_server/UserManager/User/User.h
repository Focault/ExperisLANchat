#include "GenListAPI.h"
#define LOGIN 1
#define LOGOUT 0

typedef enum UserResult UserResult;

typedef struct User
{
    char[MAX_NAME_LEN] m_userName;
    char[MAX_PASS_LEN] m_password;
    List* m_groups;
    char m_loginFlag;
} User;

User* CreateUser(char* _userName, char* _password);

UserResult UserLogin(User *_user);

List* UserGetGroups(User *_user);

UserResult UserAddGroup(User *_user, char *_groupName);

UserResult UserLeaveGroup(User *_user, char *_groupName);

void DestroyUser(void* _user);