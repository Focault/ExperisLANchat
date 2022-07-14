#include"User.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../../../GenStructures/GenListFuncs/GenListFuncs.h"
#include "../../../GenStructures/GenList/ListIterator.h"
#define LOGIN 1
#define LOGOUT 0

#define MAX_NAME_LEN 20
#define MAX_PASS_LEN 10

#define USER_MAGIC_NUMBER 892350

 struct User
{
    char m_userName[MAX_NAME_LEN];
    char m_password[MAX_PASS_LEN];
    List* m_groups;
    char m_loginFlag;
    size_t m_magicNum;
};



User* CreateUser(char* _userName, char* _password)
{
	  User* user;
	  List* groups;

	   if(_userName == NULL || _password == NULL)
	    {
	       return NULL;
	    }
	    
	    
	   if( (user = (User*)calloc(1,sizeof(User))) == NULL)
	    {
	       return NULL;
	    }
	     
	   if( (groups = ListCreate()) == NULL)
	    {
	        free(user);
		return NULL;
	    }
	      
	       strcpy(user -> m_userName, _userName);
	       strcpy(user ->  m_password,_password);
	     user -> m_groups = groups;
	     user -> m_loginFlag = LOGOUT;
	     user -> m_magicNum = USER_MAGIC_NUMBER;
    

}

int IsLoggedInUser(User* _user)
{
   if(_user == NULL)
    {
      return -1;
    }
    
    return _user -> m_loginFlag;


}

void DestroyUser(void* _user)
{
	  User* user;

	   if(_user == NULL )
	     {
	       return;
	     }
	       
	    user = (User*)_user;
	    
	    if(user -> m_magicNum != USER_MAGIC_NUMBER)
	    {
	    return;
	    }
	    
	    ListDestroy( &(user -> m_groups) , NULL);
	    user -> m_magicNum = 0;
	    free(user);
  
}

UserResult UserLogin(User* _user)
{
	  if(_user == NULL)
	  {
	    return USER_UNINITIALIZED_ERROR;
	  }
	  
	  _user -> m_loginFlag = LOGIN;
	   return USER_SUCCESS;
}


List* UserGetGroups(User *_user)
{
   if(_user == NULL)
    {
      NULL;
    }
    
   return _user -> m_groups;
 
}

UserResult UserAddGroup(User *_user, char *_groupName)
{
   char* groupName;

  if(_user == NULL || _groupName == NULL)
  {
    return USER_UNINITIALIZED_ERROR;
  }

   if( (groupName = (char*)malloc(strlen(_groupName)*sizeof(char))) == NULL)
     {
       return USER_UNINITIALIZED_ERROR;
     }
   
   strcpy(groupName, _groupName);


  ListPushHead(_user -> m_groups, groupName);

   return USER_SUCCESS;
}



int NameGroupEquals(void* _element, void* _context)
{
  char* nameGroup1;
  char* nameGroup2;
  int i= 0;
  
  nameGroup1 = (char*)_element;
  nameGroup2 = (char*)_context;
  
  while(*(nameGroup1 + i) != '\0' || *(nameGroup2 + i) != '\0')
  {
    if( *(nameGroup1 + i) != *(nameGroup2 + i))
    return 0;
    i++;
  }
  
  
  if( (*(nameGroup1 + i) == '\0' && *(nameGroup2 + i) != '\0')
  || (*(nameGroup2 + i) == '\0' && *(nameGroup1 + i) != '\0'))
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
	 
	 if(_user == NULL ||  _groupName == NULL)
	   {
	     return USER_UNINITIALIZED_ERROR;
	   }
	   
	   begin = ListItrBegin(_user -> m_groups);
	   end = ListItrEnd(_user -> m_groups);



	iter = ListItrFindFirst(begin, end, NameGroupEquals, _groupName);


	if(iter == end)
	 {
	   return USER_INVALID_DATA_ERROR;
	 }
	 
	 ListItrRemove(iter);
	 
	 return USER_SUCCESS;
}


char* GetUserPass(User* _user)
{
if(_user == NULL)
   {
     return NULL;
   }
   
   return _user -> m_password;
}

char* GetUserName(User* _user)
{
  if(_user == NULL)
   {
     return NULL;
   }
   
   return _user -> m_userName;

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
