#include <stdlib.h> /* malloc */
#include <arpa/inet.h> /* uint32_t, uint16_t */
#include <stdio.h> /* snprintf */
#include <string.h> /* strncmp, strncpy, strnlen */
#include "EchatLimits.h"
#include "GenListAPI.h"
#include "ListItr.h"
#include "ListFunctions.h"
#include "GenQueue.h"
#include "HashMapAPI.h"
#include "GroupsManager.h"
#include "Group.h"
#define DEFAULT_UDP_PORT 2000

struct GroupsManager
{
    HashMap *m_groups;
    Queue *m_addresses;
    List *m_lastList;
};

typedef struct ConnectionInfo
{
    char m_udpIP[MAX_IP_LEN];
    uint32_t m_port;
} ConnectionInfo;

typedef struct GroupContext
{
    ListItr m_prev;
    GroupsManager *m_groupManager;
    List *m_groupNames;
} GroupContext;


/* function that returns the index of the element from its key */
static size_t GroupsHashFunc(const void* _key);
/* if the data is equal returns TRUE[1], if not returns FALSE[0] */
static int GroupsEqualityFunc(const void* _firstKey, const void* _secondKey);
static void GroupHashKeyDestroy(void* _key);
/* Returns TRUE[1] on success and FALSE[0] upon fail */
static int InitializeAddressPoll(Queue *_queue);
/* Returns TRUE[1] on success and FALSE[0] upon fail */
static int AddAddressToPoll(Queue *_queue, char *_udpIP, uint32_t _port);
/* if returns 0 ForEach will stop the iteration */
static int CreateGroupListAction(const void* _key, void* _value, void* _context);
static void DestroyGroupInfo(void* _groupInfo);
static void DestroyConnectionInfo(void* _connectionInfo);
static int ExitAllGroupsActionFunc(void* _element, void* _context);
static GroupManager_Result CreateNewGroupHandler(GroupsManager *_groupManager, const char *_groupName, char **_udpIP, uint32_t *_port);
static void GroupDecreaseAndEraseIfEmpty(GroupsManager *_groupManager, const char *_groupName, Group *_group);

GroupsManager* CreateGroupManager()
{
    GroupsManager *manager;
    if ((manager = (GroupsManager*)malloc(sizeof(GroupsManager))) != NULL)
    {
        if ((manager->m_groups = HashMap_Create((MAX_GROUP_NUM / 2), GroupsHashFunc, GroupsEqualityFunc)) == NULL)
        {
            free(manager);
            return NULL;
        }
        if ((manager->m_addresses = QueueCreate(MAX_GROUP_NUM)) == NULL)
        {
            HashMap_Destroy(&manager->m_groups, NULL, NULL);
            free(manager);
            return NULL;
        }
        if (InitializeAddressPoll(manager->m_addresses) == FALSE)
        {
            HashMap_Destroy(&manager->m_groups, NULL, NULL);
            QueueDestroy(&manager->m_addresses, NULL);
            free(manager);
            return NULL;
        }
        manager->m_lastList = NULL;
    }
    return manager;
}

List* ListGroups(GroupsManager *_groupManager)
{
    List *groups = NULL;
    if (_groupManager != NULL && ((groups = ListCreate()) != NULL))
    {
        if (_groupManager->m_lastList != NULL)
        {
            DemolishGroupList(&_groupManager->m_lastList);
        }
        HashMap_ForEach(_groupManager->m_groups, CreateGroupListAction, (void*)groups);
        _groupManager->m_lastList = groups;
    }
    return groups;
}

void DemolishGroupList(List** _groupList)
{
    if (_groupList != NULL && *_groupList != NULL)
    {
        ListDestroy(_groupList, DestroyGroupInfo);
    }
}

GroupManager_Result JoinGroup(GroupsManager *_groupManager, const char *_groupName)
{
    Group *group;
    if (_groupManager == NULL || _groupName == NULL)
    {
        return GROUP_MANAGER_UNINITIALIZED;
    }
    if (HashMap_Find(_groupManager->m_groups, _groupName, (void*)&group) == MAP_KEY_NOT_FOUND_ERROR)
    {
        return GROUP_MANAGER_GROUP_NOT_FOUND;
    }
    if (GroupIncreasedSize(group) == GROUP_IS_AT_FULL_CAPACITY)
    {
        return GROUP_MANAGER_GROUP_AT_FULL_CAPACITY;
    }
    return GROUP_MANAGER_SUCCESS;
}

GroupManager_Result LeaveGroup(GroupsManager *_groupManager, const char *_groupName)
{
    Group *group;
    if (_groupManager == NULL || _groupName == NULL)
    {
        return GROUP_MANAGER_UNINITIALIZED;
    }
    if (HashMap_Find(_groupManager->m_groups, _groupName, (void*)&group) == MAP_KEY_NOT_FOUND_ERROR)
    {
        return GROUP_MANAGER_GROUP_NOT_FOUND;
    }
    GroupDecreaseAndEraseIfEmpty(_groupManager, _groupName, group);
    return GROUP_MANAGER_SUCCESS;
}

GroupManager_Result UserExitAllGroups(GroupsManager *_groupManager, List *_groupNames)
{
    ListItr begin, end;
    GroupContext context = {NULL, _groupManager, _groupNames};
    if (_groupManager == NULL || _groupNames == NULL)
    {
        return GROUP_MANAGER_UNINITIALIZED;
    }
    begin = ListItrBegin(_groupNames);
    end = ListItrEnd(_groupNames);
    ListItrForEach(begin, end, ExitAllGroupsActionFunc, (void*)&context);
    ListItrRemove(ListItrBegin(_groupNames));
    return GROUP_MANAGER_SUCCESS;
}

GroupManager_Result GetGroupDetails(GroupsManager *_groupManager, const char *_groupName, char **_udpIP, uint32_t *_port)
{
    Group *group;
    if (_groupManager == NULL || _groupName == NULL || _udpIP == NULL || _port == NULL)
    {
        return GROUP_MANAGER_UNINITIALIZED;
    }
    if (HashMap_Find(_groupManager->m_groups, _groupName, (void*)&group) == MAP_KEY_NOT_FOUND_ERROR)
    {
        return GROUP_MANAGER_GROUP_NOT_FOUND;
    }
    GroupGetDetails(group, _udpIP, _port);
    return GROUP_MANAGER_SUCCESS;
}

GroupManager_Result CreateNewGroup(GroupsManager *_groupManager, const char *_groupName, char **_udpIP, uint32_t *_port)
{
    Group *group;
    if (_groupManager == NULL || _groupName == NULL || _udpIP == NULL || _port == NULL)
    {
        return GROUP_MANAGER_UNINITIALIZED;
    }
    if (HashMap_Find(_groupManager->m_groups, _groupName, (void*)&group) != MAP_KEY_NOT_FOUND_ERROR)
    {
        return GROUP_MANAGER_DUPLICATE;
    }
    if (HashMap_Size(_groupManager->m_groups) == MAX_GROUP_NUM)
    {
        return GROUP_MANAGER_GROUPS_OVERFLOW;
    }
    return CreateNewGroupHandler(_groupManager, _groupName, _udpIP, _port);
}

void DestroyGroupManager(GroupsManager **_groupManager)
{
    if (_groupManager != NULL && *_groupManager != NULL)
    {
        if ((*_groupManager)->m_lastList != NULL)
        {
            DemolishGroupList(&(*_groupManager)->m_lastList);
        }
        QueueDestroy(&(*_groupManager)->m_addresses, DestroyConnectionInfo);
        HashMap_Destroy(&(*_groupManager)->m_groups, GroupHashKeyDestroy, DestroyGroup);
        free(*_groupManager);
        *_groupManager = NULL;
    }
}

/* Static Functions */

static GroupManager_Result CreateNewGroupHandler(GroupsManager *_groupManager, const char *_groupName, char **_udpIP, uint32_t *_port)
{
    Group *group;
    ConnectionInfo *address;
    char *groupKey;
    size_t keyLen;
    keyLen = strnlen(_groupName, MAX_GROUP_NAME_LEN);
    QueueRemove(_groupManager->m_addresses, (void*)&address);
    if ((group = CreateGroup(_groupName, address->m_udpIP, address->m_port)) == NULL)
    {
        QueueInsert(_groupManager->m_addresses, (void*)address);
        return GROUP_MANAGER_ALLOCATION_FAIL;
    }
    if (groupKey = (char*)malloc(keyLen) == NULL)
    {
        QueueInsert(_groupManager->m_addresses, (void*)address);
        DestroyGroup((void*)group);
        return GROUP_MANAGER_ALLOCATION_FAIL;
    }
    if (strncpy(groupKey, _groupName, keyLen) == NULL ||
        HashMap_Insert(_groupManager->m_groups, groupKey, (void*)group) != MAP_SUCCESS)
    {
        QueueInsert(_groupManager->m_addresses, (void*)address);
        DestroyGroup((void*)group);
        free(groupKey);
        return GROUP_MANAGER_ALLOCATION_FAIL;
    }
    strncpy(*_udpIP, address->m_udpIP, MAX_IP_LEN);
    *_port = address->m_port;
    DestroyConnectionInfo((void*)address);
    return GROUP_MANAGER_SUCCESS;
}

/* function that returns the index of the element from its key */
static size_t GroupsHashFunc(const void* _key)
{
    size_t index = 0, length = strnlen((char*)_key, MAX_GROUP_NAME_LEN);
    register int i;
    for (i = 0; i < length; ++i)
    {
        index += (size_t)(((char*)_key)[i]);
    }
    return (index * length);
}

/* if the data is equal returns TRUE[1], if not returns FALSE[0] */
static int GroupsEqualityFunc(const void* _firstKey, const void* _secondKey)
{
    return !strncmp((char*)_firstKey, (char*)_secondKey, MAX_GROUP_NAME_LEN);
}

static void GroupHashKeyDestroy(void* _key)
{
    free((char*)_key);
}

/* Returns TRUE[1] on success and FALSE[0] upon fail */
static int InitializeAddressPoll(Queue *_queue)
{
    const char fixedAddress = "224.0.0.";
    char fullAddress[MAX_IP_LEN];
    register unsigned char i;
    for (i = 0; i <= MAX_GROUP_NUM; ++i)
    {
        snprintf(fullAddress, MAX_IP_LEN, "%s%c", fixedAddress, i);
        if (!AddAddressToPoll(_queue, fullAddress, DEFAULT_UDP_PORT))
        {
            return FALSE;
        }
    }
    return TRUE;
}

/* Returns TRUE[1] on success and FALSE[0] upon fail */
static int AddAddressToPoll(Queue *_queue, char *_udpIP, uint32_t _port)
{
    ConnectionInfo *address;
    if ((address = (ConnectionInfo*)malloc(sizeof(ConnectionInfo))) == NULL || 
        QueueInsert(_queue, (void*)address) != QUEUE_SUCCESS)
    {
        return FALSE;
    }
    strncpy(address->m_udpIP, _udpIP, MAX_IP_LEN);
    address->m_port = _port;
    return TRUE;
}

/* if returns 0 ForEach will stop the iteration */
static int CreateGroupListAction(const void* _key, void* _value, void* _context)
{
    GroupInfo *info;
    if ((info = (GroupInfo*)malloc(sizeof(GroupInfo))) != NULL)
    {
        strncpy(info->m_groupName, (char*)_key, MAX_GROUP_NAME_LEN);
        info->m_usersInGroup = (uint16_t)GroupGetSize((Group*)_value);
        ListPushTail((List*)_context, (void*)info);
    }
    return TRUE;
}

static void DestroyGroupInfo(void* _groupInfo)
{
    free((GroupInfo*)_groupInfo);
}

static void DestroyConnectionInfo(void* _connectionInfo)
{
    free((ConnectionInfo*)_connectionInfo);
}

static int ExitAllGroupsActionFunc(void* _element, void* _context)
{
    Group *group;
    if (HashMap_Find(((GroupContext*)_context)->m_groupManager->m_groups, (char*)_element, (void**)&group) == MAP_SUCCESS)
    {
        GroupDecreaseAndEraseIfEmpty(((GroupContext*)_context)->m_groupManager, (char*)_element, group);
        if (((GroupContext*)_context)->m_prev == NULL)
        {
            ((GroupContext*)_context)->m_prev = ListItrBegin(((GroupContext*)_context)->m_groupNames);
        } else {
            ListItrRemove(((GroupContext*)_context)->m_prev);
            ((GroupContext*)_context)->m_prev = ListItrNext(((GroupContext*)_context)->m_prev);
        }
    }
    return TRUE;
}

static void GroupDecreaseAndEraseIfEmpty(GroupsManager *_groupManager, const char *_groupName, Group *_group)
{
    char *key, *udpIP;
    uint32_t port;
    GroupDecreasedSize(_group);
    if (IsGroupEmpty(_group))
    {
        HashMap_Remove(_groupManager->m_groups, _groupName, (void*)&key, NULL);
        GroupGetDetails(_group, &udpIP, &port);
        AddAddressToPoll(_groupManager->m_addresses, udpIP, port);
        DestroyGroup((void*)_group);
    }
}