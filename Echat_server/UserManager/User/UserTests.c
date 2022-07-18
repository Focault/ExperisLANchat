#include"User.h"
#include <stdio.h>



CreateTest1()
{
  User* user;
  if( (user = CreateUser("Sofia", "pass")) == NULL)
   {
     printf("CreateTest1 fail\n");
     return;
     DestroyUser(user);
   }

 printf("CreateTest1 pass\n");
 DestroyUser(user);
}


CreateTest2()
{
  User* user;
  user = CreateUser("Sofia", "pass");
  if( strcmp(GetUserName(user), "Sofia") ||strcmp(GetUserPass(user), "pass"))
   {
     printf("CreateTest2 fail\n");
     DestroyUser(user);
     return;
   }

 printf("CreateTest2 pass\n");
 DestroyUser(user);
}


LoginTest1()
{

 User* user;
  user = CreateUser("Sofia", "pass");
  UserLogin(user);
  if( !IsLoggedInUser(user))
   {
     printf("LoginTest1 fail\n");
     DestroyUser(user);
     return;
   }

 printf("LoginTest1 pass\n");
 DestroyUser(user);
}


LogoutTest1()
{

 User* user;
  user = CreateUser("Sofia", "pass");
  UserLogin(user);
  UserLogout(user);
  if( IsLoggedInUser(user))
   {
     printf("LogoutTest1 fail\n");
     DestroyUser(user);
     return;
   }

 printf("LogoutTest1 pass\n");
 DestroyUser(user);
}


GetGroupsTest1()
{

 User* user;
 List* groups;
  user = CreateUser("Sofia", "pass");
 
  if( (groups = UserGetGroups(user)) == NULL)
   {
     printf("GetGroupsTest1 fail\n");
     DestroyUser(user);
     return;
   }

 printf("GetGroupsTest1 pass\n");
 DestroyUser(user);
}




AddGroupTest1()
{
User* user;
 List* groups;
  user = CreateUser("Sofia", "pass");
 
  if( UserAddGroup(user, "programmers") != USER_SUCCESS)
   {
     printf("AddGroupTest1 fail\n");
     DestroyUser(user);
     return;
   }

 printf("AddGroupTest1 pass\n");
 DestroyUser(user);
 }


AddGroupTest2()
{
User* user;
 List* groups;
  user = CreateUser("Sofia", "pass");
 UserAddGroup(user, "programmers");
  if( UserAddGroup(user, "programmers") != USER_INVALID_DATA_ERROR)
   {
     printf("AddGroupTest2 fail\n");
     DestroyUser(user);
     return;
   }

 printf("AddGroupTest2 pass\n");
 DestroyUser(user);
 }

LeaveGroupTest1()
{
User* user;
 List* groups;
  user = CreateUser("Sofia", "pass");
 UserAddGroup(user, "programmers");
  if( UserLeaveGroup(user, "programmers") != USER_SUCCESS)
   {
     printf("LeaveGroupTest1 fail\n");
     DestroyUser(user);
     return;
   }

 printf("LeaveGroupTest1 pass\n");
 DestroyUser(user);
 }


LeaveGroupTest2()
{
User* user;
 List* groups;
  user = CreateUser("Sofia", "pass");
 
  if( UserLeaveGroup(user, "programmers") != USER_INVALID_DATA_ERROR)
   {
     printf("LeaveGroupTest2 fail\n");
     DestroyUser(user);
     return;
   }

 printf("LeaveGroupTest2 pass\n");
 DestroyUser(user);
 }

GetNameTest1()
{
 User* user;
 user = CreateUser("Sofia", "pass");
 
  if( strcmp(GetUserName(user), "Sofia") )
   {
     printf("GetNameTest1 fail\n");
     DestroyUser(user);
     return;
   }

 printf("GetNameTest1 pass\n");
 DestroyUser(user);
}


GetPassTest1()
{
 User* user;
 user = CreateUser("Sofia", "pass");
 
  if( strcmp(GetUserPass(user), "pass") )
   {
     printf("GetPassTest1 fail\n");
     DestroyUser(user);
     return;
   }

 printf("GetPassTest1 pass\n");
 DestroyUser(user);
}

int main()
{
 CreateTest1();
 CreateTest2();
 LoginTest1();
 LogoutTest1();
 GetGroupsTest1();
 AddGroupTest1();
 AddGroupTest2();
 LeaveGroupTest1();
 LeaveGroupTest2();
 GetNameTest1();
 GetPassTest1();
}
