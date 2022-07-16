#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__
#include <stddef.h> /* size_t */
#include "EchatLimits.h"

typedef unsigned char reply_t;

typedef enum PackResult
{
    /* Pack/Unpack Results */
    PACK_SUCCESS,
    PACK_FAIL
} PackResult;

typedef enum Reply
{
    /* Protocol Results */
    SUCCESS,
    /* Registration Protocols Results */
    REG_FAIL_USR_EXIST,
    /* Login Protocol Results */
    LOGIN_FAIL_ALREADY_ACTIVE,
    LOGIN_FAIL_WRONG_INPUT,
    /* Group Protocol Results */
    GROUP_FAIL_GROUP_NOT_FOUND,
    /* Create Group Protocol Results */
    CREATE_GROUP_FAIL_OVERFLOW,
    CREATE_GROUP_FAIL_ILLIGAL_INPUT,
    CREATE_GROUP_FAIL_DUPLICATE
} Reply;

typedef enum ProtocolType
{
    /* Registration Protocols */
    REGISTRATION_REQUEST, /* char* Username, char* Password */
    REGISTRATION_REPLY,   /* Reply */
    /* Login Protocols */
    LOGIN_REQUEST,        /* char* Username, char* Password */
    LOGIN_REPLY,          /* Reply */
    LOGOUT_NOTIFY,        /* char* Username */
    /* Group Protocols */
    GROUP_JOIN_REQUEST,   /* char* Username, char* GroupName */
    GROUP_JOIN_REPLY,     /* Reply, char* GroupName, char* udpIP, int port */
    GROUP_LEAVE,          /* char* Username, char* GroupName */
    GROUP_LIST_REQUEST,   /*  */
    GROUP_LIST_REPLY,     /* unsigned char numOfGroupsLeft, char* GroupName, unsigned int usersInGroup */
    GROUP_LIST_RECIEVED,  /* Reply */
    /* Create Group Protocols */
    CREATE_GROUP_REQUEST, /* char* Username, char* GroupName */
    CREATE_GROUP_REPLY    /* Reply, char* GroupName, char* udpIP, int port */
} ProtocolType;

typedef struct Protocol
{
    ProtocolType m_protocolType;
    reply_t m_reply;
    char m_name[MAX_NAME_LEN];
    char m_password[MAX_PASSWORD_LEN];
    char m_groupName[MAX_GROUP_NAME_LEN];
    char m_udpIP[MAX_IP_LEN];
    unsigned int m_port;
    unsigned int m_usersInGroup;
    unsigned char m_numOfGroupsLeft;
    unsigned char m_offset;
    switch_t m_finishedPack;
} Protocol;

/**
 * @brief Packs a message for TCP transmission
 * 
 * @param _PackedMessage - pointer to buffer for packed message (Have to be at least MAX_MESSAGE_LEN long!)
 * @param _params - pointer to protocol parameters - Only members relevent to protocol will be packed (see ProtocolType)
 * @param _packetLen[optional] - will present real message len after packing
 * @return PackResult 
 * @retval PACK_SUCCESS - packed message to buffer
 * @retval PACK_FAIL - couldn't pack message - if one of the pointers or the relevant members are uninitiallized
 * 
 * @warning //! The Protocol Type in the struct must be set to desired protocol
 */
PackResult Pack(void *_packedMessage, const Protocol *_params, size_t *_packetLen);

/**
 * @brief Unpacks a message received through TCP transmission to Protocol struct
 * 
 * @param _PackedMessage - pointer to recieved message
 * @param _params - pointer to Protocol struct to write message contents to
 * @return PackResult 
 * @retval PACK_SUCCESS - unpacked message to buffer 
 * @retval PACK_FAIL - couldn't unpack message - if one of the pointers or the relevant members are unset
 */
PackResult UnPack(const void *_packedMessage, Protocol *_params);

#endif /* __PROTOCOL_H__ */