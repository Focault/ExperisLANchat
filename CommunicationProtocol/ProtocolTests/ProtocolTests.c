#include <stdio.h>
#include <stdlib.h> /* malloc */
#include <string.h> /* strcpy, strcmp, strlen */
#include <arpa/inet.h> /* uint32_t, uint16_t */
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

test TestLoginReplyProtocolGood1()
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

test TestLoginReplyProtocolGood2()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = LOGIN_REPLY;
    protocolPack.m_reply = LOGIN_FAIL_ALREADY_ACTIVE;
    
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

test TestLoginReplyProtocolGood3()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = LOGIN_REPLY;
    protocolPack.m_reply = LOGIN_FAIL_WRONG_INPUT;
    
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

test TestGroupJoinRequestProtocolGood1()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = GROUP_JOIN_REQUEST;
    strcpy(protocolPack.m_name, "Tomer1234");
    strcpy(protocolPack.m_groupName, "My Little Ponny Admiration Club");
    
    if ((packedMessage = (void*)malloc(sizeof(MAX_MESSAGE_LEN))) == NULL) {return FAIL;}
    packStatus = Pack(packedMessage, &protocolPack, &packageLen);
    unpackStatus = UnPack(packedMessage, &protocolUnpack);

    if (packStatus == PACK_SUCCESS && unpackStatus == PACK_SUCCESS &&
        !strcmp(protocolPack.m_name, protocolUnpack.m_name) &&
        !strcmp(protocolPack.m_groupName, protocolUnpack.m_groupName) &&
        protocolPack.m_protocolType == protocolUnpack.m_protocolType &&
        packageLen == (strlen(protocolPack.m_name) + strlen(protocolPack.m_groupName) + 4))
    {
        result = PASS;
    }

    free(packedMessage);
    return result;
}

test TestGroupJoinRequestProtocolGood2()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = GROUP_JOIN_REQUEST;
    strcpy(protocolPack.m_name, "T");
    strcpy(protocolPack.m_groupName, "M");
    
    if ((packedMessage = (void*)malloc(sizeof(MAX_MESSAGE_LEN))) == NULL) {return FAIL;}
    packStatus = Pack(packedMessage, &protocolPack, &packageLen);
    unpackStatus = UnPack(packedMessage, &protocolUnpack);

    if (packStatus == PACK_SUCCESS && unpackStatus == PACK_SUCCESS &&
        !strcmp(protocolPack.m_name, protocolUnpack.m_name) &&
        !strcmp(protocolPack.m_groupName, protocolUnpack.m_groupName) &&
        protocolPack.m_protocolType == protocolUnpack.m_protocolType &&
        packageLen == (strlen(protocolPack.m_name) + strlen(protocolPack.m_groupName) + 4))
    {
        result = PASS;
    }

    free(packedMessage);
    return result;
}

test TestGroupJoinRequestProtocolGood3()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = GROUP_JOIN_REQUEST;
    strcpy(protocolPack.m_name, "Tomer123Tomer123Tomer123Tomer123");
    strcpy(protocolPack.m_groupName, "This Name Group is the maximum length of 70 charachters long - wow!!!!");
    
    if ((packedMessage = (void*)malloc(sizeof(MAX_MESSAGE_LEN))) == NULL) {return FAIL;}
    packStatus = Pack(packedMessage, &protocolPack, &packageLen);
    unpackStatus = UnPack(packedMessage, &protocolUnpack);

    if (packStatus == PACK_SUCCESS && unpackStatus == PACK_SUCCESS &&
        !strcmp(protocolPack.m_name, protocolUnpack.m_name) &&
        !strcmp(protocolPack.m_groupName, protocolUnpack.m_groupName) &&
        protocolPack.m_protocolType == protocolUnpack.m_protocolType &&
        packageLen == (strlen(protocolPack.m_name) + strlen(protocolPack.m_groupName) + 4))
    {
        result = PASS;
    }

    free(packedMessage);
    return result;
}

test TestGroupJoinReplyProtocolGood1()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = GROUP_JOIN_REPLY;
    protocolPack.m_reply = SUCCESS;
    strcpy(protocolPack.m_groupName, "My Little Ponny Admiration Club");
    strcpy(protocolPack.m_udpIP, "224.0.0.200");
    protocolPack.m_port = (int32_t)11000;
    
    if ((packedMessage = (void*)malloc(sizeof(MAX_MESSAGE_LEN))) == NULL) {return FAIL;}
    packStatus = Pack(packedMessage, &protocolPack, &packageLen);
    unpackStatus = UnPack(packedMessage, &protocolUnpack);

    if (packStatus == PACK_SUCCESS && unpackStatus == PACK_SUCCESS &&
        protocolPack.m_reply == protocolUnpack.m_reply &&
        !strcmp(protocolPack.m_groupName, protocolUnpack.m_groupName) &&
        !strcmp(protocolPack.m_udpIP, protocolUnpack.m_udpIP) &&
        protocolPack.m_port == protocolUnpack.m_port &&
        protocolPack.m_protocolType == protocolUnpack.m_protocolType &&
        packageLen == (strlen(protocolPack.m_udpIP) + strlen(protocolPack.m_groupName) + 11))
    {
        result = PASS;
    }

    free(packedMessage);
    return result;
}

test TestGroupJoinReplyProtocolGood2()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = GROUP_JOIN_REPLY;
    protocolPack.m_reply = GROUP_FAIL_GROUP_NOT_FOUND;
    strcpy(protocolPack.m_groupName, "M");
    strcpy(protocolPack.m_udpIP, "8.8.8.8");
    protocolPack.m_port = 1024;
    
    if ((packedMessage = (void*)malloc(sizeof(MAX_MESSAGE_LEN))) == NULL) {return FAIL;}
    packStatus = Pack(packedMessage, &protocolPack, &packageLen);
    unpackStatus = UnPack(packedMessage, &protocolUnpack);

    if (packStatus == PACK_SUCCESS && unpackStatus == PACK_SUCCESS &&
        protocolPack.m_reply == protocolUnpack.m_reply &&
        !strcmp(protocolPack.m_groupName, protocolUnpack.m_groupName) &&
        !strcmp(protocolPack.m_udpIP, protocolUnpack.m_udpIP) &&
        protocolPack.m_port == protocolUnpack.m_port &&
        protocolPack.m_protocolType == protocolUnpack.m_protocolType &&
        packageLen == (strlen(protocolPack.m_udpIP) + strlen(protocolPack.m_groupName) + 11))
    {
        result = PASS;
    }

    free(packedMessage);
    return result;
}

test TestGroupJoinReplyProtocolGood3()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = GROUP_JOIN_REPLY;
    protocolPack.m_reply = SUCCESS;
    strcpy(protocolPack.m_groupName, "This Name Group is the maximum length of 70 charachters long - wow!!!!");
    strcpy(protocolPack.m_udpIP, "224.224.224.224");
    protocolPack.m_port = DEC_64KB;
    
    if ((packedMessage = (void*)malloc(sizeof(MAX_MESSAGE_LEN))) == NULL) {return FAIL;}
    packStatus = Pack(packedMessage, &protocolPack, &packageLen);
    unpackStatus = UnPack(packedMessage, &protocolUnpack);

    if (packStatus == PACK_SUCCESS && unpackStatus == PACK_SUCCESS &&
        protocolPack.m_reply == protocolUnpack.m_reply &&
        !strcmp(protocolPack.m_groupName, protocolUnpack.m_groupName) &&
        !strcmp(protocolPack.m_udpIP, protocolUnpack.m_udpIP) &&
        protocolPack.m_port == protocolUnpack.m_port &&
        protocolPack.m_protocolType == protocolUnpack.m_protocolType &&
        packageLen == (strlen(protocolPack.m_udpIP) + strlen(protocolPack.m_groupName) + 11))
    {
        result = PASS;
    }

    free(packedMessage);
    return result;
}

test TestGroupLeaveProtocolGood1()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = GROUP_LEAVE;
    strcpy(protocolPack.m_name, "Tomer1234");
    strcpy(protocolPack.m_groupName, "My Little Ponny Admiration Club");
    
    if ((packedMessage = (void*)malloc(sizeof(MAX_MESSAGE_LEN))) == NULL) {return FAIL;}
    packStatus = Pack(packedMessage, &protocolPack, &packageLen);
    unpackStatus = UnPack(packedMessage, &protocolUnpack);

    if (packStatus == PACK_SUCCESS && unpackStatus == PACK_SUCCESS &&
        !strcmp(protocolPack.m_name, protocolUnpack.m_name) &&
        !strcmp(protocolPack.m_groupName, protocolUnpack.m_groupName) &&
        protocolPack.m_protocolType == protocolUnpack.m_protocolType &&
        packageLen == (strlen(protocolPack.m_name) + strlen(protocolPack.m_groupName) + 4))
    {
        result = PASS;
    }

    free(packedMessage);
    return result;
}

test TestGroupLeaveProtocolGood2()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = GROUP_LEAVE;
    strcpy(protocolPack.m_name, "T");
    strcpy(protocolPack.m_groupName, "M");
    
    if ((packedMessage = (void*)malloc(sizeof(MAX_MESSAGE_LEN))) == NULL) {return FAIL;}
    packStatus = Pack(packedMessage, &protocolPack, &packageLen);
    unpackStatus = UnPack(packedMessage, &protocolUnpack);

    if (packStatus == PACK_SUCCESS && unpackStatus == PACK_SUCCESS &&
        !strcmp(protocolPack.m_name, protocolUnpack.m_name) &&
        !strcmp(protocolPack.m_groupName, protocolUnpack.m_groupName) &&
        protocolPack.m_protocolType == protocolUnpack.m_protocolType &&
        packageLen == (strlen(protocolPack.m_name) + strlen(protocolPack.m_groupName) + 4))
    {
        result = PASS;
    }

    free(packedMessage);
    return result;
}

test TestGroupLeaveProtocolGood3()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = GROUP_LEAVE;
    strcpy(protocolPack.m_name, "Tomer123Tomer123Tomer123Tomer123");
    strcpy(protocolPack.m_groupName, "This Name Group is the maximum length of 70 charachters long - wow!!!!");
    
    if ((packedMessage = (void*)malloc(sizeof(MAX_MESSAGE_LEN))) == NULL) {return FAIL;}
    packStatus = Pack(packedMessage, &protocolPack, &packageLen);
    unpackStatus = UnPack(packedMessage, &protocolUnpack);

    if (packStatus == PACK_SUCCESS && unpackStatus == PACK_SUCCESS &&
        !strcmp(protocolPack.m_name, protocolUnpack.m_name) &&
        !strcmp(protocolPack.m_groupName, protocolUnpack.m_groupName) &&
        protocolPack.m_protocolType == protocolUnpack.m_protocolType &&
        packageLen == (strlen(protocolPack.m_name) + strlen(protocolPack.m_groupName) + 4))
    {
        result = PASS;
    }

    free(packedMessage);
    return result;
}

test TestGroupListRequestProtocolGood()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = GROUP_LIST_REQUEST;
    
    if ((packedMessage = (void*)malloc(sizeof(MAX_MESSAGE_LEN))) == NULL) {return FAIL;}
    packStatus = Pack(packedMessage, &protocolPack, &packageLen);
    unpackStatus = UnPack(packedMessage, &protocolUnpack);

    if (packStatus == PACK_SUCCESS && unpackStatus == PACK_SUCCESS &&
        protocolPack.m_protocolType == protocolUnpack.m_protocolType &&
        packageLen == 2)
    {
        result = PASS;
    }

    free(packedMessage);
    return result;
}

test TestGroupListReplyProtocolGood1()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = GROUP_LIST_REPLY;
    protocolPack.m_numOfGroupsLeft = 60;
    strcpy(protocolPack.m_groupName, "My Little Ponny Admiration Club");
    protocolPack.m_usersInGroup = 500;
    
    if ((packedMessage = (void*)malloc(sizeof(MAX_MESSAGE_LEN))) == NULL) {return FAIL;}
    packStatus = Pack(packedMessage, &protocolPack, &packageLen);
    unpackStatus = UnPack(packedMessage, &protocolUnpack);

    if (packStatus == PACK_SUCCESS && unpackStatus == PACK_SUCCESS &&
        protocolPack.m_numOfGroupsLeft == protocolUnpack.m_numOfGroupsLeft &&
        !strcmp(protocolPack.m_groupName, protocolUnpack.m_groupName) &&
        protocolPack.m_usersInGroup == protocolUnpack.m_usersInGroup &&
        protocolPack.m_protocolType == protocolUnpack.m_protocolType &&
        packageLen == (strlen(protocolPack.m_groupName) + 8))
    {
        result = PASS;
    }

    free(packedMessage);
    return result;
}

test TestGroupListReplyProtocolGood2()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = GROUP_LIST_REPLY;
    protocolPack.m_numOfGroupsLeft = 0;
    strcpy(protocolPack.m_groupName, "M");
    protocolPack.m_usersInGroup = 1;
    
    if ((packedMessage = (void*)malloc(sizeof(MAX_MESSAGE_LEN))) == NULL) {return FAIL;}
    packStatus = Pack(packedMessage, &protocolPack, &packageLen);
    unpackStatus = UnPack(packedMessage, &protocolUnpack);

    if (packStatus == PACK_SUCCESS && unpackStatus == PACK_SUCCESS &&
        protocolPack.m_numOfGroupsLeft == protocolUnpack.m_numOfGroupsLeft &&
        !strcmp(protocolPack.m_groupName, protocolUnpack.m_groupName) &&
        protocolPack.m_usersInGroup == protocolUnpack.m_usersInGroup &&
        protocolPack.m_protocolType == protocolUnpack.m_protocolType &&
        packageLen == (strlen(protocolPack.m_groupName) + 8))
    {
        result = PASS;
    }

    free(packedMessage);
    return result;
}

test TestGroupListReplyProtocolGood3()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = GROUP_LIST_REPLY;
    protocolPack.m_numOfGroupsLeft = MAX_GROUP_NUM;
    strcpy(protocolPack.m_groupName, "This Name Group is the maximum length of 70 charachters long - wow!!!!");
    protocolPack.m_usersInGroup = MAX_MEMBERS_IN_GROUP;
    
    if ((packedMessage = (void*)malloc(sizeof(MAX_MESSAGE_LEN))) == NULL) {return FAIL;}
    packStatus = Pack(packedMessage, &protocolPack, &packageLen);
    unpackStatus = UnPack(packedMessage, &protocolUnpack);

    if (packStatus == PACK_SUCCESS && unpackStatus == PACK_SUCCESS &&
        protocolPack.m_numOfGroupsLeft == protocolUnpack.m_numOfGroupsLeft &&
        !strcmp(protocolPack.m_groupName, protocolUnpack.m_groupName) &&
        protocolPack.m_usersInGroup == protocolUnpack.m_usersInGroup &&
        protocolPack.m_protocolType == protocolUnpack.m_protocolType &&
        packageLen == (strlen(protocolPack.m_groupName) + 8))
    {
        result = PASS;
    }

    free(packedMessage);
    return result;
}

test TestGroupListRecievedProtocolGood()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = GROUP_LIST_RECIEVED;
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

test TestCreateGroupRequestProtocolGood1()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = CREATE_GROUP_REQUEST;
    strcpy(protocolPack.m_name, "Tomer1234");
    strcpy(protocolPack.m_groupName, "My Little Ponny Admiration Club");
    
    if ((packedMessage = (void*)malloc(sizeof(MAX_MESSAGE_LEN))) == NULL) {return FAIL;}
    packStatus = Pack(packedMessage, &protocolPack, &packageLen);
    unpackStatus = UnPack(packedMessage, &protocolUnpack);

    if (packStatus == PACK_SUCCESS && unpackStatus == PACK_SUCCESS &&
        !strcmp(protocolPack.m_name, protocolUnpack.m_name) &&
        !strcmp(protocolPack.m_groupName, protocolUnpack.m_groupName) &&
        protocolPack.m_protocolType == protocolUnpack.m_protocolType &&
        packageLen == (strlen(protocolPack.m_name) + strlen(protocolPack.m_groupName) + 4))
    {
        result = PASS;
    }

    free(packedMessage);
    return result;
}

test TestCreateGroupRequestProtocolGood2()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = CREATE_GROUP_REQUEST;
    strcpy(protocolPack.m_name, "T");
    strcpy(protocolPack.m_groupName, "M");
    
    if ((packedMessage = (void*)malloc(sizeof(MAX_MESSAGE_LEN))) == NULL) {return FAIL;}
    packStatus = Pack(packedMessage, &protocolPack, &packageLen);
    unpackStatus = UnPack(packedMessage, &protocolUnpack);

    if (packStatus == PACK_SUCCESS && unpackStatus == PACK_SUCCESS &&
        !strcmp(protocolPack.m_name, protocolUnpack.m_name) &&
        !strcmp(protocolPack.m_groupName, protocolUnpack.m_groupName) &&
        protocolPack.m_protocolType == protocolUnpack.m_protocolType &&
        packageLen == (strlen(protocolPack.m_name) + strlen(protocolPack.m_groupName) + 4))
    {
        result = PASS;
    }

    free(packedMessage);
    return result;
}

test TestCreateGroupRequestProtocolGood3()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = CREATE_GROUP_REQUEST;
    strcpy(protocolPack.m_name, "Tomer12 Tomer12 Tomer12 Tomer123");
    strcpy(protocolPack.m_groupName, "This Name Group is the maximum length of 70 charachters long - wow!!!!");
    
    if ((packedMessage = (void*)malloc(sizeof(MAX_MESSAGE_LEN))) == NULL) {return FAIL;}
    packStatus = Pack(packedMessage, &protocolPack, &packageLen);
    unpackStatus = UnPack(packedMessage, &protocolUnpack);

    if (packStatus == PACK_SUCCESS && unpackStatus == PACK_SUCCESS &&
        !strcmp(protocolPack.m_name, protocolUnpack.m_name) &&
        !strcmp(protocolPack.m_groupName, protocolUnpack.m_groupName) &&
        protocolPack.m_protocolType == protocolUnpack.m_protocolType &&
        packageLen == (strlen(protocolPack.m_name) + strlen(protocolPack.m_groupName) + 4))
    {
        result = PASS;
    }

    free(packedMessage);
    return result;
}

test TestCreateGroupReplyProtocolGood1()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = CREATE_GROUP_REPLY;
    protocolPack.m_reply = SUCCESS;
    strcpy(protocolPack.m_groupName, "My Little Ponny Admiration Club");
    strcpy(protocolPack.m_udpIP, "224.0.0.200");
    protocolPack.m_port = 11000;
    
    if ((packedMessage = (void*)malloc(sizeof(MAX_MESSAGE_LEN))) == NULL) {return FAIL;}
    packStatus = Pack(packedMessage, &protocolPack, &packageLen);
    unpackStatus = UnPack(packedMessage, &protocolUnpack);

    if (packStatus == PACK_SUCCESS && unpackStatus == PACK_SUCCESS &&
        protocolPack.m_reply == protocolUnpack.m_reply &&
        !strcmp(protocolPack.m_groupName, protocolUnpack.m_groupName) &&
        !strcmp(protocolPack.m_udpIP, protocolUnpack.m_udpIP) &&
        protocolPack.m_port == protocolUnpack.m_port &&
        protocolPack.m_protocolType == protocolUnpack.m_protocolType &&
        packageLen == (strlen(protocolPack.m_udpIP) + strlen(protocolPack.m_groupName) + 11))
    {
        result = PASS;
    }

    free(packedMessage);
    return result;
}

test TestCreateGroupReplyProtocolGood2()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = CREATE_GROUP_REPLY;
    protocolPack.m_reply = CREATE_GROUP_FAIL_OVERFLOW;
    strcpy(protocolPack.m_groupName, "M");
    strcpy(protocolPack.m_udpIP, "8.8.8.8");
    protocolPack.m_port = 1024;
    
    if ((packedMessage = (void*)malloc(sizeof(MAX_MESSAGE_LEN))) == NULL) {return FAIL;}
    packStatus = Pack(packedMessage, &protocolPack, &packageLen);
    unpackStatus = UnPack(packedMessage, &protocolUnpack);

    if (packStatus == PACK_SUCCESS && unpackStatus == PACK_SUCCESS &&
        protocolPack.m_reply == protocolUnpack.m_reply &&
        !strcmp(protocolPack.m_groupName, protocolUnpack.m_groupName) &&
        !strcmp(protocolPack.m_udpIP, protocolUnpack.m_udpIP) &&
        protocolPack.m_port == protocolUnpack.m_port &&
        protocolPack.m_protocolType == protocolUnpack.m_protocolType &&
        packageLen == (strlen(protocolPack.m_udpIP) + strlen(protocolPack.m_groupName) + 11))
    {
        result = PASS;
    }

    free(packedMessage);
    return result;
}

test TestCreateGroupReplyProtocolGood3()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = CREATE_GROUP_REPLY;
    protocolPack.m_reply = CREATE_GROUP_FAIL_ILLIGAL_INPUT;
    strcpy(protocolPack.m_groupName, "This Name Group is the maximum length of 70 charachters long - wow!!!!");
    strcpy(protocolPack.m_udpIP, "224.224.224.224");
    protocolPack.m_port = DEC_64KB;
    
    if ((packedMessage = (void*)malloc(sizeof(MAX_MESSAGE_LEN))) == NULL) {return FAIL;}
    packStatus = Pack(packedMessage, &protocolPack, &packageLen);
    unpackStatus = UnPack(packedMessage, &protocolUnpack);

    if (packStatus == PACK_SUCCESS && unpackStatus == PACK_SUCCESS &&
        protocolPack.m_reply == protocolUnpack.m_reply &&
        !strcmp(protocolPack.m_groupName, protocolUnpack.m_groupName) &&
        !strcmp(protocolPack.m_udpIP, protocolUnpack.m_udpIP) &&
        protocolPack.m_port == protocolUnpack.m_port &&
        protocolPack.m_protocolType == protocolUnpack.m_protocolType &&
        packageLen == (strlen(protocolPack.m_udpIP) + strlen(protocolPack.m_groupName) + 11))
    {
        result = PASS;
    }

    free(packedMessage);
    return result;
}

test TestCreateGroupReplyProtocolGood4()
{
    test result = FAIL;
    PackResult packStatus, unpackStatus;
    Protocol protocolPack, protocolUnpack;
    void *packedMessage;
    size_t packageLen;
    protocolPack.m_protocolType = CREATE_GROUP_REPLY;
    protocolPack.m_reply = CREATE_GROUP_FAIL_DUPLICATE;
    strcpy(protocolPack.m_groupName, "My Little Ponny Admiration Club");
    strcpy(protocolPack.m_udpIP, "224.0.0.200");
    protocolPack.m_port = 11000;
    
    if ((packedMessage = (void*)malloc(sizeof(MAX_MESSAGE_LEN))) == NULL) {return FAIL;}
    packStatus = Pack(packedMessage, &protocolPack, &packageLen);
    unpackStatus = UnPack(packedMessage, &protocolUnpack);

    if (packStatus == PACK_SUCCESS && unpackStatus == PACK_SUCCESS &&
        protocolPack.m_reply == protocolUnpack.m_reply &&
        !strcmp(protocolPack.m_groupName, protocolUnpack.m_groupName) &&
        !strcmp(protocolPack.m_udpIP, protocolUnpack.m_udpIP) &&
        protocolPack.m_port == protocolUnpack.m_port &&
        protocolPack.m_protocolType == protocolUnpack.m_protocolType &&
        packageLen == (strlen(protocolPack.m_udpIP) + strlen(protocolPack.m_groupName) + 11))
    {
        result = PASS;
    }

    free(packedMessage);
    return result;
}

test TestPackProtocolBad1()
{
    test result = FAIL;
    PackResult packStatus;
    void *packedMessage;
    size_t packageLen;
    
    if ((packedMessage = (void*)malloc(sizeof(MAX_MESSAGE_LEN))) == NULL) {return FAIL;}
    packStatus = Pack(packedMessage, NULL, &packageLen);

    if (packStatus == PACK_FAIL)
    {
        result = PASS;
    }

    free(packedMessage);
    return result;
}

test TestPackProtocolBad2()
{
    test result = FAIL;
    Protocol packProtocol;
    PackResult packStatus;
    size_t packageLen;
    
    packStatus = Pack(NULL, &packProtocol, &packageLen);

    if (packStatus == PACK_FAIL)
    {
        result = PASS;
    }

    return result;
}

test TestUnPackProtocolBad1()
{
    test result = FAIL;
    PackResult packStatus;
    void *packedMessage;
    
    if ((packedMessage = (void*)malloc(sizeof(MAX_MESSAGE_LEN))) == NULL) {return FAIL;}
    packStatus = UnPack(packedMessage, NULL);

    if (packStatus == PACK_FAIL)
    {
        result = PASS;
    }

    free(packedMessage);
    return result;
}

test TestUnPackProtocolBad2()
{
    test result = FAIL;
    Protocol packProtocol;
    PackResult packStatus;
    
    packStatus = UnPack(NULL, &packProtocol);

    if (packStatus == PACK_FAIL)
    {
        result = PASS;
    }

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

    printf("TestLoginReplyProtocolGood1:");
    PrintTestResult(TestLoginReplyProtocolGood1());

    printf("TestLoginReplyProtocolGood2:");
    PrintTestResult(TestLoginReplyProtocolGood2());

    printf("TestLoginReplyProtocolGood3:");
    PrintTestResult(TestLoginReplyProtocolGood3());

    printf("TestLogoutNotifyProtocolGood1:");
    PrintTestResult(TestLogoutNotifyProtocolGood1());

    printf("TestLogoutNotifyProtocolGood2:");
    PrintTestResult(TestLogoutNotifyProtocolGood2());

    printf("TestLogoutNotifyProtocolGood3:");
    PrintTestResult(TestLogoutNotifyProtocolGood3());

    printf("TestGroupJoinRequestProtocolGood1:");
    PrintTestResult(TestGroupJoinRequestProtocolGood1());

    printf("TestGroupJoinRequestProtocolGood2:");
    PrintTestResult(TestGroupJoinRequestProtocolGood2());

    printf("TestGroupJoinRequestProtocolGood3:");
    PrintTestResult(TestGroupJoinRequestProtocolGood3());

    printf("TestGroupJoinReplyProtocolGood1:");
    PrintTestResult(TestGroupJoinReplyProtocolGood1());

    printf("TestGroupJoinReplyProtocolGood2:");
    PrintTestResult(TestGroupJoinReplyProtocolGood2());

    printf("TestGroupLeaveProtocolGood3:");
    PrintTestResult(TestGroupLeaveProtocolGood3());

    printf("TestGroupLeaveProtocolGood1:");
    PrintTestResult(TestGroupLeaveProtocolGood1());

    printf("TestGroupLeaveProtocolGood2:");
    PrintTestResult(TestGroupLeaveProtocolGood2());

    printf("TestGroupLeaveProtocolGood3:");
    PrintTestResult(TestGroupLeaveProtocolGood3());

    printf("TestGroupListRequestProtocolGood:");
    PrintTestResult(TestGroupListRequestProtocolGood());

    printf("TestGroupListReplyProtocolGood1:");
    PrintTestResult(TestGroupListReplyProtocolGood1());

    printf("TestGroupListReplyProtocolGood2:");
    PrintTestResult(TestGroupListReplyProtocolGood2());

    printf("TestGroupListReplyProtocolGood3:");
    PrintTestResult(TestGroupListReplyProtocolGood3());

    printf("TestGroupListRecievedProtocolGood:");
    PrintTestResult(TestGroupListRecievedProtocolGood());

    printf("TestCreateGroupRequestProtocolGood1:");
    PrintTestResult(TestCreateGroupRequestProtocolGood1());

    printf("TestCreateGroupRequestProtocolGood2:");
    PrintTestResult(TestCreateGroupRequestProtocolGood2());

    printf("TestCreateGroupRequestProtocolGood3:");
    PrintTestResult(TestCreateGroupRequestProtocolGood3());

    printf("TestCreateGroupReplyProtocolGood1:");
    PrintTestResult(TestCreateGroupReplyProtocolGood1());

    printf("TestCreateGroupReplyProtocolGood2:");
    PrintTestResult(TestCreateGroupReplyProtocolGood2());

    printf("TestCreateGroupReplyProtocolGood3:");
    PrintTestResult(TestCreateGroupReplyProtocolGood3());

    printf("TestCreateGroupReplyProtocolGood4:");
    PrintTestResult(TestCreateGroupReplyProtocolGood4());

    printf("TestPackProtocolBad1:");
    PrintTestResult(TestPackProtocolBad1());

    printf("TestPackProtocolBad2:");
    PrintTestResult(TestPackProtocolBad2());

    printf("TestUnPackProtocolBad1:");
    PrintTestResult(TestUnPackProtocolBad1());

    printf("TestUnPackProtocolBad2:");
    PrintTestResult(TestUnPackProtocolBad2());

    return 0;
}
