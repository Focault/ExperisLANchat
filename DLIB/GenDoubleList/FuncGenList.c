#include <stdlib.h> /* NULL, malloc, free */
#include "GenListAPI.h"
#include "internal.h"
#define EMPTY 1
#define NOT_EMPTY 0

static ListResult PushCheck(List* _list, void* _item);
static ListResult PopCheck(List* _list, void** _pItem);
static int ListIsEmpty(List *_list);

List *ListCreate()
{
	List *ptr;
	if ((ptr = (List*)malloc(sizeof(List))) != NULL)
	{
		ptr->m_head.m_next = &(ptr->m_tail);
		ptr->m_head.m_prev = &(ptr->m_head);
		ptr->m_tail.m_prev = &(ptr->m_head);
		ptr->m_tail.m_next = &(ptr->m_tail);
		ptr->m_tail.m_data = NULL;
	}
	return ptr;
}

void ListDestroy(List** _pList, void (*_elementDestroy)(void* _item))
{
	Node *ptr;
	if (_pList != NULL && *_pList != NULL)
	{
		ptr = (*_pList)->m_head.m_next;
		while (ptr != &((*_pList)->m_tail))
		{
			if (_elementDestroy != NULL)
			{
				_elementDestroy(ptr->m_data);
			}
			ptr = ptr->m_next;
			free(ptr->m_prev);
		}
		free(*_pList);
		*_pList = NULL;
	}
}

ListResult ListPushHead(List* _list, void* _item)
{
	ListResult status;
	Node *insert;
	if ((status = PushCheck(_list, _item)) != LIST_SUCCESS)
	{
		return status;
	}
	insert = (Node*)malloc(sizeof(Node));
	if (insert == NULL)
	{
		return LIST_ALLOCATION_ERROR;
	}
	insert->m_data = _item;
	Push(insert, _list->m_head.m_next);
	return LIST_SUCCESS;
}

ListResult ListPushTail(List* _list, void* _item)
{
	ListResult status;
	Node *insert;
	if ((status = PushCheck(_list, _item)) != LIST_SUCCESS)
	{
		return status;
	}
	insert = (Node*)malloc(sizeof(Node));
	if (insert == NULL)
	{
		return LIST_ALLOCATION_ERROR;
	}
	insert->m_data = _item;
	Push(insert, &(_list->m_tail));
	return LIST_SUCCESS;
}

ListResult ListPopHead(List* _list, void** _pItem)
{
	ListResult status;
	if ((status = PopCheck(_list, _pItem)) != LIST_SUCCESS)
	{
		return status;
	}
	*_pItem = _list->m_head.m_next->m_data;
	Pop(_list->m_head.m_next);
	return LIST_SUCCESS;
}

ListResult ListPopTail(List* _list, void** _pItem)
{
	ListResult status;
	if ((status = PopCheck(_list, _pItem)) != LIST_SUCCESS)
	{
		return status;
	}
	*_pItem = _list->m_tail.m_prev->m_data;
	Pop(_list->m_tail.m_prev);
	return LIST_SUCCESS;
}

size_t ListSize(const List* _list)
{
	size_t count = 0;
	Node *ptr;
	if (_list != NULL)
	{
		ptr = _list->m_head.m_next;
		while (ptr != &_list->m_tail)
		{
			++count;
			ptr = ptr->m_next;
		}
	}
	return count;
}

/* static functions */

static int ListIsEmpty(List *_list)
{
	if (_list->m_head.m_next == &(_list->m_tail))
	{
		return EMPTY;
	}
	return NOT_EMPTY;
}

static ListResult PopCheck(List* _list, void** _pItem)
{
	if (_list == NULL)
	{
		return LIST_UNINITIALIZED_ERROR;
	}
	if (_pItem == NULL)
	{
		return LIST_POINTER_UNINITIALIZED_ERROR;
	}
	if (ListIsEmpty(_list))
	{
		return LIST_UNDERFLOW_ERROR;
	}
	return LIST_SUCCESS;
}

static ListResult PushCheck(List* _list, void* _item)
{
	if (_list == NULL)
	{
		return LIST_UNINITIALIZED_ERROR;
	}
	if (_item == NULL)
	{
		return LIST_NULL_ELEMENT_ERROR;
	}
	return LIST_SUCCESS;
}





