#ifndef __GROUP_H__
#define __GROUP_H__
#include <arpa/inet.h> /* uint32_t */
#include <stddef.h> /* size_t */
#include "EchatLimits.h"

typedef struct Group Group; 

typedef enum GroupResult
{
    GROUP_SUCCESS,
    GROUP_UNINITIALIZED,
    GROUP_IS_AT_FULL_CAPACITY
} GroupResult;

/**
 * @brief Create a Group
 * 
 * @param _groupName - pointer to group name
 * @param _udpIP - pointer to UDP IP for group
 * @param _port - port number for group
 * @return Group* 
 * 
 * @warning if one of the pointers are NULL or failed allocation returns NULL
 */
Group *CreateGroup(char *_groupName, char *_udpIP, uint32_t _port);

/**
 * @brief Increase Group size by 1
 * 
 * @param _group - poiner to group struct
 * @return GroupResult 
 * 
 * @retval GROUP_SUCCESS - group increased size counter successfully
 * @retval GROUP_UNINITIALIZED - group pointer is uninitialized 
 * @retval GROUP_IS_AT_FULL_CAPACITY - group is at MAX_MEMBERS_IN_GROUP and can't add another member
 */
GroupResult GroupIncreasedSize(Group *_group);

/**
 * @brief Decrease Group size by 1
 * 
 * @param _group - poiner to group struct
 * @return GroupResult 
 * 
 * @retval GROUP_SUCCESS - group decreased size counter successfully
 * @retval GROUP_UNINITIALIZED - group pointer is uninitialized 
 */
GroupResult GroupDecreasedSize(Group *_group);

/**
 * @brief Get Group's connectivity details
 * 
 * @param _group - pointer to group struct
 * @param _udpIP - pointer to pointer to string to store IP into
 * @param _port - pointer to uint32_t to store port into
 * @return GroupResult 
 * 
 * @retval GROUP_SUCCESS - grabed group details successfuly
 * @retval GROUP_UNINITIALIZED - one of the pointers is uninitialized 
 */
GroupResult GroupGetDetails(Group *_group, char **_udpIP, uint32_t *_port);

/**
 * @brief return size of group
 * 
 * @param _group - pointer to group struct
 * @return size_t - size of group
 */
size_t GroupGetSize(Group *_group);

/**
 * @brief tells if group is empty
 * 
 * @param _group - pointer to group struct
 * @return switch_t - TRUE[1] if empty and FALSE[0] if not empty
 */
switch_t IsGroupEmpty(Group *_group);

/**
 * @brief Destroies a group struct
 * 
 * @param _group - pointer to group casted to void*
 */
void DestroyGroup(void *_group);

#endif /* __GROUP_H__ */
