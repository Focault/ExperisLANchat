#include <stdio.h>
#include <stdlib.h> /* malloc */
#include <string.h> /* strcpy, strcmp, strlen */
#include "Protocol.h"
#include "EchatLimits.h"

typedef enum test 
{
	PASS = 40,
	FAIL
} test;

typedef struct Overall
{
    int m_passed;
    int m_failed;
} Overall;

test TestRegistrationRequestProtocolGood1()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = REGISTRATION_REQUEST;
    strcpy(protocolPack.m_name, "Tommy4");
    strcpy(protocolPack.m_password, "123456789");
    
    if ((packedMessage = (void*)malloc(sizeof(MAX_MESSAGE_LEN))) == NULL) {return FAIL;}
    packStatus = Pack(packedMessage, &protocolPack, &packageLen);
    unpackStatus = UnPack(packedMessage, &protocolUnpack);

    if (packStatus == PACK_SUCCESS && unpackStatus == PACK_SUCCESS &&
        !strcmp(protocolPack.m_name, protocolUnpack.m_name) &&
        !strcmp(protocolPack.m_password, protocolUnpack.m_password) &&
        protocolPack.m_protocolType == protocolUnpack.m_protocolType &&
        packageLen == (strlen(protocolPack.m_name) + strlen(protocolPack.m_password) + 4))
    {
        result = PASS;
    }

    free(packedMessage);
    return result;
}

test TestRegistrationRequestProtocolGood2()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = REGISTRATION_REQUEST;
    strcpy(protocolPack.m_name, "T");
    strcpy(protocolPack.m_password, "1");
    
    if ((packedMessage = (void*)malloc(sizeof(MAX_MESSAGE_LEN))) == NULL) {return FAIL;}
    packStatus = Pack(packedMessage, &protocolPack, &packageLen);
    unpackStatus = UnPack(packedMessage, &protocolUnpack);

    if (packStatus == PACK_SUCCESS && unpackStatus == PACK_SUCCESS &&
        !strcmp(protocolPack.m_name, protocolUnpack.m_name) &&
        !strcmp(protocolPack.m_password, protocolUnpack.m_password) &&
        protocolPack.m_protocolType == protocolUnpack.m_protocolType &&
        packageLen == (strlen(protocolPack.m_name) + strlen(protocolPack.m_password) + 4))
    {
        result = PASS;
    }

    free(packedMessage);
    return result;
}

test TestRegistrationRequestProtocolGood3()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = REGISTRATION_REQUEST;
    strcpy(protocolPack.m_name, "Tomer123Tomer123Tomer123Tomer123");
    strcpy(protocolPack.m_password, "Tomer123Tomer123Tomer123Tomer123");
    
    if ((packedMessage = (void*)malloc(sizeof(MAX_MESSAGE_LEN))) == NULL) {return FAIL;}
    packStatus = Pack(packedMessage, &protocolPack, &packageLen);
    unpackStatus = UnPack(packedMessage, &protocolUnpack);

    if (packStatus == PACK_SUCCESS && unpackStatus == PACK_SUCCESS &&
        !strcmp(protocolPack.m_name, protocolUnpack.m_name) &&
        !strcmp(protocolPack.m_password, protocolUnpack.m_password) &&
        protocolPack.m_protocolType == protocolUnpack.m_protocolType &&
        packageLen == (strlen(protocolPack.m_name) + strlen(protocolPack.m_password) + 4))
    {
        result = PASS;
    }

    free(packedMessage);
    return result;
}

test TestLoginRequestProtocolGood1()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = LOGIN_REQUEST;
    strcpy(protocolPack.m_name, "Tommy4");
    strcpy(protocolPack.m_password, "123456789");
    
    if ((packedMessage = (void*)malloc(sizeof(MAX_MESSAGE_LEN))) == NULL) {return FAIL;}
    packStatus = Pack(packedMessage, &protocolPack, &packageLen);
    unpackStatus = UnPack(packedMessage, &protocolUnpack);

    if (packStatus == PACK_SUCCESS && unpackStatus == PACK_SUCCESS &&
        !strcmp(protocolPack.m_name, protocolUnpack.m_name) &&
        !strcmp(protocolPack.m_password, protocolUnpack.m_password) &&
        protocolPack.m_protocolType == protocolUnpack.m_protocolType &&
        packageLen == (strlen(protocolPack.m_name) + strlen(protocolPack.m_password) + 4))
    {
        result = PASS;
    }

    free(packedMessage);
    return result;
}

test TestLoginRequestProtocolGood2()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = LOGIN_REQUEST;
    strcpy(protocolPack.m_name, "T");
    strcpy(protocolPack.m_password, "1");
    
    if ((packedMessage = (void*)malloc(sizeof(MAX_MESSAGE_LEN))) == NULL) {return FAIL;}
    packStatus = Pack(packedMessage, &protocolPack, &packageLen);
    unpackStatus = UnPack(packedMessage, &protocolUnpack);

    if (packStatus == PACK_SUCCESS && unpackStatus == PACK_SUCCESS &&
        !strcmp(protocolPack.m_name, protocolUnpack.m_name) &&
        !strcmp(protocolPack.m_password, protocolUnpack.m_password) &&
        protocolPack.m_protocolType == protocolUnpack.m_protocolType &&
        packageLen == (strlen(protocolPack.m_name) + strlen(protocolPack.m_password) + 4))
    {
        result = PASS;
    }

    free(packedMessage);
    return result;
}

test TestLoginRequestProtocolGood3()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = LOGIN_REQUEST;
    strcpy(protocolPack.m_name, "Tomer123Tomer123Tomer123Tomer123");
    strcpy(protocolPack.m_password, "Tomer123Tomer123Tomer123Tomer123");
    
    if ((packedMessage = (void*)malloc(sizeof(MAX_MESSAGE_LEN))) == NULL) {return FAIL;}
    packStatus = Pack(packedMessage, &protocolPack, &packageLen);
    unpackStatus = UnPack(packedMessage, &protocolUnpack);

    if (packStatus == PACK_SUCCESS && unpackStatus == PACK_SUCCESS &&
        !strcmp(protocolPack.m_name, protocolUnpack.m_name) &&
        !strcmp(protocolPack.m_password, protocolUnpack.m_password) &&
        protocolPack.m_protocolType == protocolUnpack.m_protocolType &&
        packageLen == (strlen(protocolPack.m_name) + strlen(protocolPack.m_password) + 4))
    {
        result = PASS;
    }

    free(packedMessage);
    return result;
}

test TestRegistrationReplyProtocolGood()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = REGISTRATION_REPLY;
    protocolPack.m_reply = SUCCESS;
    
    if ((packedMessage = (void*)malloc(sizeof(MAX_MESSAGE_LEN))) == NULL) {return FAIL;}
    packStatus = Pack(packedMessage, &protocolPack, &packageLen);
    unpackStatus = UnPack(packedMessage, &protocolUnpack);

    if (packStatus == PACK_SUCCESS && unpackStatus == PACK_SUCCESS &&
        protocolPack.m_reply == protocolUnpack.m_reply &&
        protocolPack.m_protocolType == protocolUnpack.m_protocolType &&
        packageLen == 4)
    {
        result = PASS;
    }

    free(packedMessage);
    return result;
}

test TestLoginReplyProtocolGood()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = LOGIN_REPLY;
    protocolPack.m_reply = SUCCESS;
    
    if ((packedMessage = (void*)malloc(sizeof(MAX_MESSAGE_LEN))) == NULL) {return FAIL;}
    packStatus = Pack(packedMessage, &protocolPack, &packageLen);
    unpackStatus = UnPack(packedMessage, &protocolUnpack);

    if (packStatus == PACK_SUCCESS && unpackStatus == PACK_SUCCESS &&
        protocolPack.m_reply == protocolUnpack.m_reply &&
        protocolPack.m_protocolType == protocolUnpack.m_protocolType &&
        packageLen == 4)
    {
        result = PASS;
    }

    free(packedMessage);
    return result;
}

test TestLogoutNotifyProtocolGood1()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = LOGOUT_NOTIFY;
    strcpy(protocolPack.m_name, "Tommy4");
    
    if ((packedMessage = (void*)malloc(sizeof(MAX_MESSAGE_LEN))) == NULL) {return FAIL;}
    packStatus = Pack(packedMessage, &protocolPack, &packageLen);
    unpackStatus = UnPack(packedMessage, &protocolUnpack);

    if (packStatus == PACK_SUCCESS && unpackStatus == PACK_SUCCESS &&
        !strcmp(protocolPack.m_name, protocolUnpack.m_name) &&
        protocolPack.m_protocolType == protocolUnpack.m_protocolType &&
        packageLen == (strlen(protocolPack.m_name) + 3))
    {
        result = PASS;
    }

    free(packedMessage);
    return result;
}

test TestLogoutNotifyProtocolGood2()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = LOGOUT_NOTIFY;
    strcpy(protocolPack.m_name, "T");
    
    if ((packedMessage = (void*)malloc(sizeof(MAX_MESSAGE_LEN))) == NULL) {return FAIL;}
    packStatus = Pack(packedMessage, &protocolPack, &packageLen);
    unpackStatus = UnPack(packedMessage, &protocolUnpack);

    if (packStatus == PACK_SUCCESS && unpackStatus == PACK_SUCCESS &&
        !strcmp(protocolPack.m_name, protocolUnpack.m_name) &&
        protocolPack.m_protocolType == protocolUnpack.m_protocolType &&
        packageLen == (strlen(protocolPack.m_name) + 3))
    {
        result = PASS;
    }

    free(packedMessage);
    return result;
}

test TestLogoutNotifyProtocolGood3()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = LOGOUT_NOTIFY;
    strcpy(protocolPack.m_name, "Tomer123Tomer123Tomer123Tomer123");
    
    if ((packedMessage = (void*)malloc(sizeof(MAX_MESSAGE_LEN))) == NULL) {return FAIL;}
    packStatus = Pack(packedMessage, &protocolPack, &packageLen);
    unpackStatus = UnPack(packedMessage, &protocolUnpack);

    if (packStatus == PACK_SUCCESS && unpackStatus == PACK_SUCCESS &&
        !strcmp(protocolPack.m_name, protocolUnpack.m_name) &&
        protocolPack.m_protocolType == protocolUnpack.m_protocolType &&
        packageLen == (strlen(protocolPack.m_name) + 3))
    {
        result = PASS;
    }

    free(packedMessage);
    return result;
}

Overall PrintTestResult(test _result)
{
	static Overall recap;
    if (_result == PASS)
	{
		printf(" PASSED!\n");
        ++recap.m_passed;
	}
	if (_result == FAIL)
	{
		printf(" *** FAILED! ***\n");
        ++recap.m_failed;
	}
    return recap;
}

int main(int argc, char const *argv[])
{
    printf("TestRegistrationRequestProtocolGood1:");
    PrintTestResult(TestRegistrationRequestProtocolGood1());

    printf("TestRegistrationRequestProtocolGood2:");
    PrintTestResult(TestRegistrationRequestProtocolGood2());

    printf("TestRegistrationRequestProtocolGood3:");
    PrintTestResult(TestRegistrationRequestProtocolGood3());

    printf("TestLoginRequestProtocolGood1:");
    PrintTestResult(TestLoginRequestProtocolGood1());

    printf("TestLoginRequestProtocolGood2:");
    PrintTestResult(TestLoginRequestProtocolGood2());

    printf("TestLoginRequestProtocolGood3:");
    PrintTestResult(TestLoginRequestProtocolGood3());

    printf("TestRegistrationReplyProtocolGood:");
    PrintTestResult(TestRegistrationReplyProtocolGood());

    printf("TestLoginReplyProtocolGood:");
    PrintTestResult(TestLoginReplyProtocolGood());

    printf("TestLogoutNotifyProtocolGood1:");
    PrintTestResult(TestLogoutNotifyProtocolGood1());

    printf("TestLogoutNotifyProtocolGood2:");
    PrintTestResult(TestLogoutNotifyProtocolGood2());

    printf("TestLogoutNotifyProtocolGood3:");
    PrintTestResult(TestLogoutNotifyProtocolGood3());

    return 0;
}
