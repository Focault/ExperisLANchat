#include "UserManager.h"
#include "../User/User.h"
#include<stdio.h>
#include<stdlib.h>
#include <string.h>

#define MAX_USERS_NUM 1000

struct UserManager{
HashMap* m_users;
char* m_UsersDataBase;
};

size_t HashFunc(void* _key)
{
  char* key = (char*)_key;
  
  int i = 0 ;
   size_t index = 0 ;
   while(*(key+i)!= '\0' & *(key+i)!= '\n')
    {
       index += *(key+i);
       i++;
    }

 return (index + i);
}

int IsKeysEquals(void* _key1, void* _key2)
{
  char* key1;
  char* key2;
  int i= 0;
  
  key1 = (char*)_key1;
  key2 = (char*)_key2;
  
  while(*(key1 + i) != '\0' && *(key2 + i) != '\0')
  {
    if( *(key1 + i) != *(key2 + i))
    return 0;
    i++;
  }
  
if( (*(key1 + i) != *(key2 + i) )
  && (*(key2 + i) == 0 && *(key1 + i) != '\n') && ( (*(key1 + i) == '\n') && (*(key2 + i) != 0) ) )
    {
     return 0;
    }
    
    return 1;
}

UserManager* CreateUserManager(char* _fileUsersData)
{
   UserManager* userManager;
   HashMap* users;
   User* user;
   FILE* fp;
   char* userName;
   char* userNameDyn;
   char pass[20];
   int bytesName;
   int bytesPass;
   
   if(_fileUsersData == NULL)
     {
       return NULL;
     }
   
   
   if( (userManager = (UserManager*)malloc(sizeof(UserManager))) == NULL)
   {
      return NULL;
   
   }
   
   if( (users = HashMapCreate( MAX_USERS_NUM, HashFunc, IsKeysEquals)) == NULL)
   {
      free(userManager);
      return NULL;
   
   }
   
   
   if( (userName = (char*)malloc(30*sizeof(char))) == NULL)
   {
      HashMapDestroy(&users, NULL,NULL);
      free(userManager);
      return NULL;
   }
   
   
   if(_fileUsersData != NULL)
   {
     
     if( (fp = fopen(_fileUsersData, "a+") )!= NULL)
     {
       
       while(!feof(fp))
       {
       
             if( (fgets(userName ,256, fp)) !=  NULL)
             {
             userNameDyn = (char*)malloc((strlen(userName)-1)* sizeof(char));
             
             strcpy(userNameDyn, userName);
             
              fgets(pass , 256, fp);
           
             user = CreateUser( userNameDyn, pass);
             HashMapInsert(users, userName, user);
             
             }
             
             
             
      }
      }
       fclose(fp); 
   }
      
    
   
   
   userManager -> m_UsersDataBase = _fileUsersData;
   userManager -> m_users = users;
   
   return userManager;
 
}


UserManagerStatus UserRegister(UserManager* _userManager, char* _userName, char* _pass)
{
   User* user;
   void* duplicate;
   FILE *fp;
   char* userName;

 if(_userManager == NULL ||  _userName == NULL || _pass == NULL)
   {
     return USER_MANAGER_UNINITIALIZED;
   }


 userName = (char*)malloc( (strlen(_userName) + 1 )* sizeof(char));
   
     strcpy(userName, _userName);
   *(userName + strlen(_userName)) ='\0';
   

   if( (user = CreateUser(userName, _pass)) == NULL)
     {
       USER_MANAGER_REGISTRATION_FAILED;
     }
     

 if( HashMapFind(_userManager -> m_users, _userName, &duplicate)
== MAP_SUCCESS)
   {
     return USER_DUPLICATE;
   }

  
   
     
     HashMapInsert(_userManager -> m_users, userName, user);

      
     if( (fp = fopen(_userManager -> m_UsersDataBase, "a+")) != NULL)
     {
        fputs(userName , fp);
       fputc('\n',fp);
        fputs( _pass, fp);
       fputc('\n',fp);
        fclose(fp);    
     }
     
     return SUCCESS;
}



/*int main()
{

 UserManager*  um;
 
 um= CreateUserManager("fileUsersData.txt");
 UserRegister(um, "Sofia", "pass");
 
}*/

