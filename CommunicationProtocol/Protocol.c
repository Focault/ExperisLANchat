#include <string.h> /* memcpy, strncpy */
#include "Protocol.h"
#include "EchatLimits.h"
#define FIRST_ENTRY_OFFSET 2
#define STR_ENTRY_LEN(Max) (Max) + 2
#define ENTRY_LEN(Max) (Max) + 1

static void PackProtocolType(void *_packedMessage, char _protocolType);
static void PackLength(void *_packedMessage, char _length, size_t *_packetLen);
/* Returns New offset */
static unsigned char PackString(void *_packedMessage, unsigned char _offSet, const char *_str);
/* Returns New offset */
static unsigned char PackChar(void *_packedMessage, unsigned char _offSet, unsigned char _char);
static void UnpackProtocolType(const char *_packedMessage, Protocol *_params);
static unsigned char UnpackLength(const char *_packedMessage);
/* Returns New offset */
static unsigned char UnpackString(const char *_packedMessage, unsigned char _offSet, char *_str);
/* Returns New offset */
static unsigned char UnpackChar(const void *_packedMessage, unsigned char _offSet, unsigned char *_char);

PackResult Pack(void *_packedMessage, const Protocol *_params, size_t *_packetLen)
{
    unsigned char nextOffset;
    if (_packedMessage == NULL || _params == NULL)
    {
        return PACK_FAIL;
    }
    PackProtocolType(_packedMessage, (char)(_params->m_protocolType));
    switch (_params->m_protocolType)
    {
    case REGISTRATION_REQUEST:
    case LOGIN_REQUEST:
        /* char* Username, char* Password */
        nextOffset = PackString(_packedMessage, FIRST_ENTRY_OFFSET, _params->m_name);
        nextOffset = PackString(_packedMessage, nextOffset, _params->m_password);
        PackLength(_packedMessage, (nextOffset - FIRST_ENTRY_OFFSET), _packetLen);
        break;
    case LOGOUT_NOTIFY:
        /* Username */
        nextOffset = PackString(_packedMessage, FIRST_ENTRY_OFFSET, _params->m_name);
        PackLength(_packedMessage, (nextOffset - FIRST_ENTRY_OFFSET), _packetLen);
        break;
    case GROUP_JOIN_REPLY:
    case CREATE_GROUP_REPLY:
        /* Reply, char* GroupName, char* udpIP */
        break;
    case REGISTRATION_REPLY:
    case LOGIN_REPLY:
    case GROUP_LIST_RECIEVED:
        /* Reply */
        nextOffset = PackChar(_packedMessage, FIRST_ENTRY_OFFSET, _params->m_reply);
        PackLength(_packedMessage, (nextOffset - FIRST_ENTRY_OFFSET), _packetLen);
        break;
    case GROUP_JOIN_REQUEST:
    case CREATE_GROUP_REQUEST:
    case GROUP_LEAVE:
        /* char* Username, char* GroupName */
    case GROUP_LIST_REQUEST:
        break;
    default:
        return PACK_FAIL;
    }
    return PACK_SUCCESS;
}

PackResult UnPack(const void *_packedMessage, Protocol *_params)
{
    unsigned char packetLen, nextOffset;
    if (_packedMessage == NULL || _params == NULL)
    {
        return PACK_FAIL;
    }
    UnpackProtocolType((char*)_packedMessage, _params);
    if ((packetLen = UnpackLength((char*)_packedMessage)))
    {
        switch (_params->m_protocolType)
        {
        case REGISTRATION_REQUEST:
        case LOGIN_REQUEST:
            /* char* Username, char* Password */
            nextOffset = UnpackString((char*)_packedMessage, FIRST_ENTRY_OFFSET, _params->m_name);
            if (!(packetLen -= (nextOffset - FIRST_ENTRY_OFFSET))) {return PACK_FAIL;}
            UnpackString((char*)_packedMessage, nextOffset, _params->m_password);
            break;
        case LOGOUT_NOTIFY:
            /* Username */
            UnpackString((char*)_packedMessage, FIRST_ENTRY_OFFSET, _params->m_name);
            break;
        case GROUP_JOIN_REPLY:
        case CREATE_GROUP_REPLY:
            /* Reply, char* GroupName, char* udpIP */
            break;
        case REGISTRATION_REPLY:
        case LOGIN_REPLY:
        case GROUP_LIST_RECIEVED:
            /* Reply */
            UnpackChar((char*)_packedMessage, FIRST_ENTRY_OFFSET, &_params->m_reply);
            break;
        case GROUP_JOIN_REQUEST:
        case CREATE_GROUP_REQUEST:
        case GROUP_LEAVE:
            /* char* Username, char* GroupName */
        case GROUP_LIST_REQUEST:
            break;
        default:
            return PACK_FAIL;
        }
    }
    return PACK_SUCCESS;
}

/* Static Functions */

static void PackProtocolType(void *_packedMessage, char _protocolType)
{ 
    memcpy(_packedMessage, (void*)&_protocolType, 1);
}

static void PackLength(void *_packedMessage, char _length, size_t *_packetLen)
{
    memcpy((void*)((char*)_packedMessage + 1), (void*)&_length, 1);
    if (_packetLen != NULL)
    {
        *_packetLen = _length + FIRST_ENTRY_OFFSET;
    }
}

/* Returns New offset */
static unsigned char PackString(void *_packedMessage, unsigned char _offSet, const char *_str)
{
    unsigned char entryLength = strlen(_str);
    if (!entryLength)
    {
        return PackChar(_packedMessage, _offSet, *_str);
    }
    memcpy((void*)((char*)_packedMessage + _offSet), (void*)&entryLength, 1);
    memcpy((void*)((char*)_packedMessage + _offSet + 1), (void*)_str, entryLength);
    return ENTRY_LEN(entryLength + _offSet);
}

/* Returns New offset */
static unsigned char PackChar(void *_packedMessage, unsigned char _offSet, unsigned char _char)
{
    unsigned char entryLength = 1;
    memcpy((void*)((char*)_packedMessage + _offSet), (void*)&entryLength, 1);
    memcpy((void*)((char*)_packedMessage + _offSet + 1), (void*)&_char, entryLength);
    return ENTRY_LEN(entryLength + _offSet);
}

static void UnpackProtocolType(const char *_packedMessage, Protocol *_params)
{
    _params->m_protocolType = (ProtocolType)(_packedMessage[0]);
}

static unsigned char UnpackLength(const char *_packedMessage)
{
    return (_packedMessage[1]);
}

/* Returns New offset */
static unsigned char UnpackString(const char *_packedMessage, unsigned char _offSet, char *_str)
{
    unsigned char newOffset, strLength = _packedMessage[_offSet];
    if (strLength == 1)
    {
        newOffset = UnpackChar(_packedMessage, _offSet, (unsigned char*)_str);
        _str[strLength] = '\0';
        return newOffset;
    }
    strncpy(_str, (_packedMessage + _offSet + 1), strLength);
    _str[strLength] = '\0';
    return _offSet + ENTRY_LEN(strLength);
}

/* Returns New offset */
static unsigned char UnpackChar(const void *_packedMessage, unsigned char _offSet, unsigned char *_char)
{
    *_char = *((char*)_packedMessage + _offSet + 1);
    return STR_ENTRY_LEN(_offSet);
}