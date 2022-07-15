#include <stdio.h>
#include "UI.h"
#include "EchatLimits.h"



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
    printf("Please enter your username:\nUsername must be between %d and %d characters. Use letters and numbers only.\n", MIN_NAME_LEN, MAX_NAME_LEN);
    scanf("%s", _username);

    printf("Please enter your password:\nPassword must be between %d and %d characters. Use letters and numbers only\n", MIN_PASSWORD_LEN, MAX_PASSWORD_LEN);
    scanf("%s", _password);
}


void ClientRequestStatus(Reply _reply)
{
    switch (_reply)
    {
    case SUCCESS:
        printf("Registration successful!");
        break;

    case REG_FAIL_USR_EXIST:
        printf("Username already exists. Please re-register.\n");
        break;

    case LOGIN_FAIL_ALREADY_ACTIVE:
        printf("Username already logged in.\n");
        break;
    
    case LOGIN_FAIL_WRONG_INPUT:
        printf("Wrong username or password. Try again\n");
        break;

    default:
        break;
    }
}
