#include <arpa/inet.h> /* uint32_t */
#include <stdlib.h>	   /* size_t, malloc */
#include <string.h>    /* strcmp */
#include "EchatLimits.h"
#include "Group.h"
#define GROUP_MAGIC_NUMBER 59312752485

struct Group
{
	char m_groupName[MAX_GROUP_NAME_LEN];
	char m_udpIP[MAX_IP_LEN];
	uint32_t m_port;
	size_t m_size;
	size_t m_magicNum;
};

Group *CreateGroup(char *_groupName, char *_udpIP, uint32_t _port)
{
	Group *group;
	if (_groupName == NULL || _udpIP == NULL || _port < MIN_PORT_NUM || _port > DEC_64KB)
	{
		return NULL;
	}

	if ((group = (Group *)malloc(sizeof(Group))) == NULL)
	{
		return NULL;
	}

	strcpy(group->m_groupName, _groupName);
	strcpy(group->m_udpIP, _udpIP);
	group->m_port = _port;
	group->m_size = 1;
	group->m_magicNum = GROUP_MAGIC_NUMBER;

	return group;
}

void DestroyGroup(void *_group)
{
	if ((Group *)_group != NULL && ((Group *)_group)->m_magicNum == GROUP_MAGIC_NUMBER)
	{
		((Group *)_group)->m_magicNum = 0;
		free((Group *)_group);
	}
}

GroupResult GroupIncreaseSize(Group *_group)
{

	if (_group == NULL)
	{
		return GROUP_UNINITIALIZED;
	}

	if (_group->m_size == MAX_MEMBERS_IN_GROUP)
	{
		return GROUP_IS_AT_FULL_CAPACITY;
	}

	_group->m_size += 1;

	return GROUP_SUCCESS;
}

GroupResult GroupDecreaseSize(Group *_group)
{

	if (_group == NULL)
	{
		return GROUP_UNINITIALIZED;
	}

	_group->m_size -= 1;

	return GROUP_SUCCESS;
}

GroupResult GroupGetDetails(Group *_group, char **_udpIP, uint32_t *_port)
{

	if (_group == NULL || _udpIP == NULL || _port == NULL)
	{
		return GROUP_UNINITIALIZED;
	}

	strncpy(*_udpIP, _group->m_udpIP, MAX_IP_LEN);
	*_port = _group->m_port;

	return GROUP_SUCCESS;
}

size_t GroupGetSize(Group *_group)
{
	if (_group == NULL)
	{
		return 0;
	}

	return _group->m_size;
}

switch_t IsGroupEmpty(Group *_group)
{
	if (_group != NULL)
	{
		return (_group->m_size == 0);
	}
	return FALSE;
}
