#include <arpa/inet.h> /* uint32_t */
#include <stdlib.h> /* size_t, malloc */
#include "EchatLimits.h"
#include "Group.h"

struct Group 
{
    char m_groupName[MAX_GROUP_NAME_LEN];
    char m_udpIP[MAX_IP_LEN];
    uint32_t m_port;
    size_t m_size;
    size_t m_magicNum;
}; 