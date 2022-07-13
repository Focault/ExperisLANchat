#ifndef __LISTITR_H__
#define __LISTITR_H__
#include "GenListAPI.h"
#define PTR_NULL -1

typedef void* ListItr;

/** 
 * @brief Get itertator to the list's beginning
 *
 * @params _list : list to return begin iterator, pointing at first element
 *                 or at the end if list is empty
 * @return iterator pointing at the list's beginning (or NULL if _list is NULL)
 */
ListItr ListItrBegin(const List* _list);

/** 
 * @brief Get itertator to the list end
 *
 * @params _list : list to return end iterator
 * @return an iterator pointing at the list's end (or NULL if _list is NULL)
 */
ListItr ListItrEnd(const List* _list);

/** 
 * @brief retruns none zero if _a and _b iterators refer to same node
 * @warning return -1 (PTR_NULL) if one of the pointers are NULL
 */
int ListItrEquals(const ListItr _a, const ListItr _b);

/** 
 * @brief Get itertator to the next element from a given iterator
 * @warning if _itr is end iterator it will be returned (or NULL if _itr is NULL)
 */
ListItr ListItrNext(ListItr _itr);

/** 
 * @brief Get itertator to the previous element
 * @warning if _itr is begin iterator it will be returned (or NULL if _itr is NULL)
 */
ListItr ListItrPrev(ListItr _itr);

/** 
 * @brief Get data from the list node the iterator is pointing to
 *
 * @params _itr : A list iterator
 * @return the data the iterator is pointing at or NULL if pointing to the end (or if _itr is NULL)
 */
void* ListItrGet(ListItr _itr);

/** 
 * @brief Set data at the node where the iterator is pointing at
 * @return the data from the node to be changed or NULL if one of the argvs are NULL or if pointing to end
 */
void* ListItrSet(ListItr _itr, void* _element);

/** 
 * @brief Inserts a new node before node the iterator is pointing at
 * @return an iterator pointing at the element inserted or NULL on error
 */
ListItr ListItrInsertBefore(ListItr _itr, void* _element);

/** 
 * @brief Removes the node the iterator is pointing at
 *
 * @params _itr : A list iterator
 * @return the removed data
 * @warning if _itr is NULL or tail node returns NULL
 */
void* ListItrRemove(ListItr _itr);

#endif /* __LISTITR_H__ */