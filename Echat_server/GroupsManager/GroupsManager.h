#ifndef __GROUPSMANAGER_H__
#define __GROUPSMANAGER_H__
#include <arpa/inet.h> /* uint32_t, uint16_t */
#include "GenListAPI.h"
#include "EchatLimits.h"

typedef struct GroupsManager GroupsManager;

typedef struct GroupInfo
{
    char m_groupName[MAX_GROUP_NAME_LEN];
    uint16_t m_usersInGroup;
} GroupInfo;

typedef enum GroupManager_Result
{
    GROUP_MANAGER_SUCCESS,
    GROUP_MANAGER_ALLOCATION_FAIL,
    GROUP_MANAGER_UNINITIALIZED,
    GROUP_MANAGER_DUPLICATE,
    GROUP_MANAGER_GROUPS_OVERFLOW,
    GROUP_MANAGER_GROUP_NOT_FOUND,
    GROUP_MANAGER_GROUP_AT_FULL_CAPACITY
} GroupManager_Result;

/**
 * @brief Create a Group Manager struct
 * 
 * @return GroupsManager* 
 * 
 * @warning returns NULL if failed allocation
 */
GroupsManager* CreateGroupManager();

/**
 * @brief returns a list of GroupInfo structs for all of the groups
 * 
 * @param _groupManager - pointer to groupManager struct
 * @return List* - pointer to a list of GroupInfo structs for all of the groups in the app
 * 
 * @warning if groupManager is NULL, allocation failed or no groups exist returns NULL
 * @details //! Demolition of the list is done by DemolishGroupList function and calling it is user's responsibility
 */
List* ListGroups(GroupsManager *_groupManager);

/**
 * @brief Demolishes List of Groups' information
 * 
 * @param _groupList - pointer to pointer to a list of GroupInfo structs for all of the groups in the app
 */
void DemolishGroupList(List** _groupList);

/**
 * @brief Join an existent group (update counter)
 * 
 * @param _groupManager - pointer to groupManager struct
 * @param _groupName - group name user had joined
 * @return GroupManager_Result 
 * 
 * @retval GROUP_MANAGER_SUCCESS - succesfuly joined group
 * @retval GROUP_MANAGER_UNINITIALIZED - one of the pointers are NULL
 * @retval GROUP_MANAGER_GROUP_NOT_FOUND - no group of that name
 * @retval GROUP_MANAGER_GROUP_AT_FULL_CAPACITY - group is at full capacity - can't add more users
 */
GroupManager_Result JoinGroup(GroupsManager *_groupManager, const char *_groupName);

/**
 * @brief leave an existent group (update counter)
 * 
 * @param _groupManager - pointer to groupManager struct
 * @param _groupName - group name user had left
 * @return GroupManager_Result 
 * 
 * @retval GROUP_MANAGER_SUCCESS - succesfuly left group
 * @retval GROUP_MANAGER_UNINITIALIZED - one of the pointers are NULL
 * @retval GROUP_MANAGER_GROUP_NOT_FOUND - no group of that name
 */
GroupManager_Result LeaveGroup(GroupsManager *_groupManager, const char *_groupName);

/**
 * @brief update that user had left for each of the groups in the list (Whipes the List clean but doesn't destroy it)
 * 
 * @param _groupManager - pointer to groupManager struct
 * @param _groupNames - list of groups names (contains only strings)
 * @return GroupManager_Result 
 * @retval GROUP_MANAGER_SUCCESS - succesfully updated groups
 * @retval GROUP_MANAGER_UNINITIALIZED - one of the pointers are NULL
 */
GroupManager_Result UserExitAllGroups(GroupsManager *_groupManager, List *_groupNames);

/**
 * @brief Get the Group Details
 * 
 * @param _groupManager - pointer to groupManager struct
 * @param _groupName - the name of the group for which the details requested
 * @param _udpIP - pointer to store group UDP IPv4 at (have to be at least MAX_IP_LEN long)
 * @param _port - pointer to store port number to
 * @return GroupManager_Result 
 * 
 * @retval GROUP_MANAGER_SUCCESS - succesfuly grabed group's details
 * @retval GROUP_MANAGER_UNINITIALIZED - one of the pointers are NULL
 * @retval GROUP_MANAGER_GROUP_NOT_FOUND - no group of that name
 */
GroupManager_Result GetGroupDetails(GroupsManager *_groupManager, const char *_groupName, char **_udpIP, uint32_t *_port);

/**
 * @brief Create a new group and update it's population to 1
 * 
 * @param _groupManager - pointer to groupManager struct
 * @param _groupName - the name of the group you wish to create
 * @param _udpIP - pointer to store group UDP IPv4 at (have to be at least MAX_IP_LEN long)
 * @param _port - pointer to store port number to
 * @return GroupManager_Result 
 * 
 * @retval GROUP_MANAGER_SUCCESS - succesfuly created group
 * @retval GROUP_MANAGER_UNINITIALIZED - one of the pointers are NULL
 * @retval GROUP_MANAGER_DUPLICATE - a group of that name already exists
 * @retval GROUP_MANAGER_GROUPS_OVERFLOW - can't create new group because number of groups is at limit
 * @retval GROUP_MANAGER_ALLOCATION_FAIL - allocation failed
 */
GroupManager_Result CreateNewGroup(GroupsManager *_groupManager, const char *_groupName, char **_udpIP, uint32_t *_port);

/**
 * @brief Destroy Group Manager
 * 
 * @param _groupManager - pointer to pointer to group manager struct
 */
void DestroyGroupManager(GroupsManager **_groupManager);

#endif /* __GROUPSMANAGER_H__ */