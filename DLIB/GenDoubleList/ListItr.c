#include <stdlib.h> /* malloc */
#include "GenListAPI.h"
#include "internal.h"
#include "ListItr.h"

ListItr ListItrBegin(const List* _list)
{
    if (_list == NULL)
    {
        return NULL;
    }
    return _list->m_head.m_next;
}

ListItr ListItrEnd(const List* _list)
{
    if (_list == NULL)
    {
        return NULL;
    }
    return (void*)&(_list->m_tail);
}

int ListItrEquals(const ListItr _a, const ListItr _b)
{
    if (_a == NULL || _b == NULL)
    {
        return PTR_NULL;
    }
    if ((Node*)_a == (Node*)_b)
    {
        return 1;
    }
    return 0;
}

ListItr ListItrNext(ListItr _itr)
{
    if (_itr == NULL)
    {
        return NULL;
    }
    return ((Node*)_itr)->m_next;
}

ListItr ListItrPrev(ListItr _itr)
{
    if (_itr == NULL)
    {
        return NULL;
    }
    if (((Node*)_itr)->m_prev->m_prev == ((Node*)_itr)->m_prev)
    {
        return _itr;
    }
    return ((Node*)_itr)->m_prev;
}

void* ListItrGet(ListItr _itr)
{
    if (_itr == NULL || ((Node*)_itr)->m_next == (Node*)_itr)
    {
        return NULL;
    }
    return ((Node*)_itr)->m_data;
}

void* ListItrSet(ListItr _itr, void* _element)
{
    void *oldD;
    if (_itr == NULL || _element == NULL || ((Node*)_itr)->m_next == ((Node*)_itr))
    {
        return NULL;
    }
    oldD = ((Node*)_itr)->m_data;
    ((Node*)_itr)->m_data = _element;
    return oldD;
}

ListItr ListItrInsertBefore(ListItr _itr, void* _element)
{
	Node *insert;
    if (_itr == NULL || _element == NULL || (insert = (Node*)malloc(sizeof(Node))) == NULL)
    {
        return NULL;
    }
    insert->m_data = _element;
	Push(insert, (Node*)_itr);
    return insert;
}

void* ListItrRemove(ListItr _itr)
{
    void *oldD;
    if (_itr == NULL || ((Node*)_itr) == ((Node*)_itr)->m_next)
    {
        return NULL;
    }
    oldD = ((Node*)_itr)->m_data;
    Pop(((Node*)_itr));
    return oldD;
}