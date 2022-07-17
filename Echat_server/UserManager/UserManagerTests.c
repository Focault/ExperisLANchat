#include "UserManager.h"
#include "User/User.h"
#include <stdio.h>

void CreateTest()
{
  UserManager*  um;
 
 if( (um = CreateUserManager()) == NULL)
  {
    printf("CreateTest fail\n");
    return;
  }
 printf("CreateTest pass\n");
}

void UserRegisterTest1()
{
UserManager*  um;

 um = CreateUserManager();
  if(UserRegister(um, "userName", "pass") != USER_MANAGER_SUCCESS|| UserRegister(um, "userNew", "pass") != USER_MANAGER_SUCCESS)
   {
    printf("UserRegisterTest1 fail\n");
    return;
   }
   printf("UserRegisterTest1 pass\n");
   
}


void UserRegisterTest2()
{
UserManager*  um;

 um = CreateUserManager();
  if(UserRegister(um, "userName", "pass") != USER_DUPLICATE
  ||UserRegister(um, "userNew", "pass") != USER_DUPLICATE)
   {
    printf("UserRegisterTest2 fail\n");
    return;
   }
   printf("UserRegisterTest2 pass\n");
   
}

void UserLoginTest1()
{
UserManager* um;

um = CreateUserManager();
UserRegister(um, "Sofia", "pass");
  
 if(UserLogIn(um, "Sofia", "pass") != USER_MANAGER_SUCCESS)
   {
    printf("UserLoginTest1 fail\n");
    return;
   }
   printf("UserLoginTest1 pass\n");

}
void UserLoginTest2()
{
UserManager* um;

um = CreateUserManager();
UserRegister(um, "Sofia", "pass");
  
 if(UserLogIn(um, "Sofia", "passs") != USER_WRONG_INPUT)
   {
    printf("UserLoginTest2 fail\n");
    return;
   }
   printf("UserLoginTest2 pass\n");


}


void UserJoinGroupTest1()
{
UserManager* um;

um = CreateUserManager();
UserRegister(um, "Sofia", "pass");
 UserLogIn(um, "Sofia", "pass");
 
 /*UserJoinGroup(UserManager *_userManager, char *_userName, char *_group)*/
  if(UserManagerJoinGroup(um, "Sofia", "Drivers") != USER_MANAGER_SUCCESS)
   {
    printf("UserJoinGroupTest1 fail\n");
    return;
   }
   printf("UserJoinGroupTest1 pass\n");

}

void UserJoinGroupTest2()
{
UserManager* um;

um = CreateUserManager();
UserRegister(um, "Sofia", "pass");
 UserLogIn(um, "Sofia", "pass");
 UserLogOut(um, "Sofia");
 
  if(UserManagerJoinGroup(um, "Sofia", "Drivers") != USER_WRONG_INPUT)
   {
    printf("UserJoinGroupTest2 fail\n");
    return;
   }
   printf("UserJoinGroupTest2 pass\n");

}


void UserLeaveGroupTest1()
{
UserManager* um;

um = CreateUserManager();
UserRegister(um, "Sofia", "pass");
 UserLogIn(um, "Sofia", "pass");

UserManagerJoinGroup(um, "Sofia", "Drivers");
  if(UserManagerLeaveGroup(um, "Sofia", "Drivers") != USER_MANAGER_SUCCESS)
   {
    printf("UserLeaveGroupTest1 fail\n");
    return;
   }
   printf("UserLeaveGroupTest1 pass\n");

}



void UserLeaveGroupTest2()
{
UserManager* um;

um = CreateUserManager();
UserRegister(um, "Sofia", "pass");
 UserLogIn(um, "Sofia", "pass");


  if(UserManagerLeaveGroup(um, "Sofia", "Programmers") != USER_INVALID_DATA_ERROR)
   {
    printf("UserLeaveGroupTest2 fail\n");
    return;
   }
   printf("UserLeaveGroupTest2 pass\n");

}



int main()
{

CreateTest();
UserRegisterTest1();
UserRegisterTest2(); 
 UserLoginTest1();
 UserLoginTest2();
 UserJoinGroupTest1();
 UserJoinGroupTest2();
 UserLeaveGroupTest1();
 UserLeaveGroupTest2();
}

