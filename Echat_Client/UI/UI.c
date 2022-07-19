#include <stdio.h>
#include "UI.h"
#include "EchatLimits.h"
#define BOLD "\x1B[1m"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define NORMAL "\x1B[0m"

static void GetAndCheckUsername(char* _username);
static void GetAndCheckPassword(char* _password);


MenuOptions WelcomeMenu()
{
    int option;
    printf(BOLD);
    printf("Welcome to eChat!\n");
    putchar('\n');
    printf("What would like to do?\n1. Register.\n2. Login.\n3. Exit.\n");
    printf(NORMAL);
    putchar('\n');
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
        printf(RED);
        printf(BOLD);
            printf("Illegal option! Try again.\n");
            printf(NORMAL);
            
            putchar('\n');
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
        putchar('\n');
        printf(BOLD);
        printf(GREEN);
        printf("You're in!\n");
        printf(NORMAL);
        
        break;

    case REG_FAIL_USR_EXIST:
        printf(RED);
        printf(BOLD);
        printf("Username already exists. Please re-register.\n");
        printf(NORMAL);
        putchar('\n');
        break;

    case LOGIN_FAIL_ALREADY_ACTIVE:
        printf(BOLD);
        printf(RED);
        printf("User already logged in.\n");
        printf(NORMAL);
        putchar('\n');
        break;
    
    case LOGIN_FAIL_WRONG_INPUT:
        printf(BOLD);
        printf(RED);
        printf("Wrong username or password. Try again\n");
        printf(NORMAL);
        putchar('\n');
        break;
    
    case GROUP_FAIL_GROUP_NOT_FOUND:
        printf(BOLD);  
        printf(RED);
        printf("Group not found. Try again\n");
        printf(NORMAL);
        putchar('\n');
        break;
    
    case CREATE_GROUP_FAIL_OVERFLOW:
        printf(BOLD);
        printf(RED);
        printf("Can't create new groups. Please choose a group from the groups' list\n");
        printf(NORMAL);
        putchar('\n');
        break;
    
    case CREATE_GROUP_FAIL_ILLIGAL_INPUT:
        printf(BOLD);
        printf(RED);
        printf("Illegal group name. Try again\n");
        printf(NORMAL);
        putchar('\n');
        break;
    
    case CREATE_GROUP_FAIL_DUPLICATE: 
        printf(BOLD);
        printf(RED);
        printf("Create group error: group name already exists. Try again\n");
        printf(NORMAL);
        putchar('\n');
        break;

    default:
        break;
    }
    
}


ChatOptions ChatMenu()
{
    int option;
    printf(BOLD);
    printf("What would like to do?\n1. List existing groups.\n2. Create a new group.\n3. Join a group.\n4. Leave a group.\n5. Logout\n");
    putchar('\n');
    printf(NORMAL);
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
            printf(BOLD);
            printf(RED);
            printf("Illegal option! Try again.\n");
            printf(NORMAL);
            putchar('\n');
            return ChatMenu();
    }
}


void GetGroupName(char* _groupName)
{
    int i=0;
    putchar('\n');
    printf(BOLD);
    printf("Please enter a group name. Name must be between %d and %d characters. Use letters and numbers only.\n", MIN_GROUP_NAME_LEN, MAX_GROUP_NAME_LEN);
    printf(NORMAL);
    putchar('\n');
    scanf("%s", _groupName);

    while (_groupName[i] != '\0')
    {
        if ((_groupName[i] > 9 && _groupName[i] < 65) || (_groupName[i] > 90 && _groupName[i] < 97) || (_groupName[i] > 122))
        {
            printf(BOLD);
            printf(RED);
            printf("Illegal name.\n");
            printf(NORMAL);
            putchar('\n');
            GetAndCheckUsername(_groupName);
        }
        i++;
    }
}


void ListGroupDetails(char* _groupName, int _numOfParticipants, int _numOfGroupsLeft)
{
    putchar('\n');
    printf(BOLD);
    printf("Group: %s Number of participants: %d\n", _groupName, _numOfParticipants);
    putchar('\n');
}


static void GetAndCheckUsername(char* _username)
{
    int i=0;
    putchar('\n');
    printf(BOLD);
    printf("Please enter your username:\nUsername must be between %d and %d characters. Use letters and numbers only.\n", MIN_NAME_LEN, MAX_NAME_LEN);
    putchar('\n');
    printf(NORMAL);
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
    putchar('\n');
    printf(BOLD);
    printf("Please enter your password:\nPassword must be between %d and %d characters. Use letters and numbers only\n", MIN_PASSWORD_LEN, MAX_PASSWORD_LEN);
    printf(NORMAL);
    putchar('\n');
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

