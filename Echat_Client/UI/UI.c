#include <stdio.h>
#include "UI.h"
#include "EchatLimits.h"


static void GetAndCheckUsername(char* _username);
static void GetAndCheckPassword(char* _password);


MenuOptions WelcomeMenu()
{
    int option;
    
    printf("Welcome to eChat!\n");
    printf("What would like to do?\n1. Register.\n2. Login.\n3. Exit.\n");
    scanf("%d", &option);

    switch (option)
    {
        case 1:
            return REGISTER;
        
        case 2:
            return LOGIN;

        case 3:
            return EXIT;
        
        default:
            printf("Illegal option! Try again.\n");
            return WelcomeMenu();
    }
}


void GetUserDetails(char* _username, char* _password)
{
    GetAndCheckUsername(_username);
    GetAndCheckPassword(_password);
}


void ClientRequestStatus(Reply _reply)
{
    switch (_reply)
    {
    case SUCCESS:
        printf("You're in!\n");
        break;

    case REG_FAIL_USR_EXIST:
        printf("Username already exists. Please re-register.\n");
        break;

    case LOGIN_FAIL_ALREADY_ACTIVE:
        printf("User already logged in.\n");
        break;
    
    case LOGIN_FAIL_WRONG_INPUT:
        printf("Wrong username or password. Try again\n");
        break;
    
    case GROUP_FAIL_GROUP_NOT_FOUND:
        printf("Group not found. Try again\n");
        break;
    
    case CREATE_GROUP_FAIL_OVERFLOW:
        printf("Can't create new groups. Please choose a group from the groups' list\n");
        break;
    
    case CREATE_GROUP_FAIL_ILLIGAL_INPUT:
        printf("Illegal group name. Try again\n");
        break;
    
    case CREATE_GROUP_FAIL_DUPLICATE: 
        printf("Create group error: group name already exists. Try again\n");
        break;

    default:
        break;
    }
}


ChatOptions ChatMenu()
{
    int option;
    
    printf("What would like to do?\n1. List existing groups.\n2. Create a new group.\n3. Join a group.\n4. Leave a group.\n5. Logout\n");
    scanf("%d", &option);

    switch (option)
    {
        case 1:
            return LIST_GROUPS;
        
        case 2:
            return CREATE_GROUP;
        
        case 3:
            return JOIN_GROUP;

        case 4:
            return LEAVE_GROUP;
        
        case 5:
            return LOGOUT;
        
        default:
            printf("Illegal option! Try again.\n");
            return ChatMenu();
    }
}


void GetGroupName(char* _groupName)
{
    int i=0;
    printf("Please enter a group name. Name must be between %d and %d characters. Use letters and numbers only.\n", MIN_GROUP_NAME_LEN, MAX_GROUP_NAME_LEN);
    scanf("%s", _groupName);

    while (_groupName[i] != '\0')
    {
        if ((_groupName[i] > 9 && _groupName[i] < 65) || (_groupName[i] > 90 && _groupName[i] < 97) || (_groupName[i] > 122))
        {
            printf("Illegal name.\n");
            GetAndCheckUsername(_groupName);
        }
        i++;
    }
}


void ListGroupDetails(char* _groupName, int _numOfParticipants, int _numOfGroupsLeft)
{
    printf("Group: %s Number of participants: %d\n", _groupName, _numOfParticipants);
}


static void GetAndCheckUsername(char* _username)
{
    int i=0;
    printf("Please enter your username:\nUsername must be between %d and %d characters. Use letters and numbers only.\n", MIN_NAME_LEN, MAX_NAME_LEN);
    scanf("%s", _username);

    /* while (_username[i] != '\0')
    {
        if ((_username[i] > 9 && _username[i] < 65) || (_username[i] > 90 && _username[i] < 97) || (_username[i] > 122))
        {
            printf("Illegal username.\n");
            GetAndCheckUsername(_username);
        }
        i++;
    } */
}



static void GetAndCheckPassword(char* _password)
{
    int i=0;
    printf("Please enter your password:\nPassword must be between %d and %d characters. Use letters and numbers only\n", MIN_PASSWORD_LEN, MAX_PASSWORD_LEN);
    scanf("%s", _password);

    /* while (_password[i] != '\0')
    {
        if ((_password[i] > 9 && _password[i] < 65) || (_password[i] > 90 && _password[i] < 97) || (_password[i] > 122))
        {
            printf("Illegal password.\n");
            GetAndCheckPassword(_password);
        }
        i++;
    } */
}

