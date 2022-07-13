#include <stdio.h> /* printf, size_t */
#include <stdlib.h> /* NULL, malloc, free */
#include "GenListAPI.h"
#include "ListFunctions.h"
#include "ListItr.h"
#include "../GenDoubleList/internal.h"
#include "TestDLIB.h"
#define FILL_INT_SIZE 10
#define STOP_ACT 0
#define RESUME_ACT 1

static void FillListInt(List *_ptr);
static void FreeIntList(void *_item);
static int PredicateEqualIntList(void* _element, void *_context);
static int ActionPrintIntList(void* _element, void *_context);
static int ActionPrintContextIntList(void* _element, void *_context);

test ListCreateGood()
{
	test result = FAIL;
	List *ptr;
	ptr = ListCreate();
	
	if (ptr != NULL && ListSize(ptr) == 0)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, NULL);
	return result;
}

test ListDestroyGood()
{
	test result = FAIL;
	List *ptr;
	ptr = ListCreate();
	FillListInt(ptr);
	ListDestroy(&ptr, FreeIntList);
	
	if (ptr == NULL)
	{
		result = PASS;
	}
	
	return result;
}

test ListDestroyBad()
{
	test result = FAIL;
	List *ptr;
	ptr = ListCreate();
	ListDestroy(&ptr, NULL);
	ListDestroy(&ptr, NULL);
	ListDestroy(NULL, NULL);
	
	if (ptr == NULL)
	{
		result = PASS;
	}
	
	return result;
}

test ListPushHeadGood()
{
	test result = FAIL;
	List *ptr;
	ListResult status;
	int data = 3;
	ptr = ListCreate();
	status = ListPushHead(ptr, (void*)&data);
	
	if (status == LIST_SUCCESS &&
	ListSize(ptr) == 1)
	{
		result = PASS;
	}

	ListDestroy(&ptr, NULL);
	return result;
}

test ListPushHeadBad1()
{
	test result = FAIL;
	ListResult status;
	int data = 3;
	status = ListPushHead(NULL, (void*)&data);
	
	if (status == LIST_UNINITIALIZED_ERROR)
	{
		result = PASS;
	}

	return result;
}

test ListPushHeadBad2()
{
	test result = FAIL;
	List *ptr;
	ListResult status;
	ptr = ListCreate();
	status = ListPushHead(ptr, NULL);
	
	if (status == LIST_NULL_ELEMENT_ERROR)
	{
		result = PASS;
	}

	ListDestroy(&ptr, NULL);
	return result;
}

test ListPushTailGood()
{
	test result = FAIL;
	List *ptr;
	ListResult status;
	int data = 3;
	ptr = ListCreate();
	status = ListPushTail(ptr, (void*)&data);
	
	if (status == LIST_SUCCESS &&
	ListSize(ptr) == 1)
	{
		result = PASS;
	}

	ListDestroy(&ptr, NULL);
	return result;
}

test ListPushTailBad1()
{
	test result = FAIL;
	ListResult status;
	int data = 3;
	status = ListPushTail(NULL, (void*)&data);
	
	if (status == LIST_UNINITIALIZED_ERROR)
	{
		result = PASS;
	}

	return result;
}

test ListPushTailBad2()
{
	test result = FAIL;
	List *ptr;
	ListResult status;
	ptr = ListCreate();
	status = ListPushTail(ptr, NULL);
	
	if (status == LIST_NULL_ELEMENT_ERROR)
	{
		result = PASS;
	}

	ListDestroy(&ptr, NULL);
	return result;
}

test ListPopHeadGood1()
{
	test result = FAIL;
	List *ptr;
	ListResult status;
	int data = 3, *popped;
	ptr = ListCreate();
	ListPushHead(ptr, (void*)&data);
	status = ListPopHead(ptr, (void*)&popped);
	
	if (status == LIST_SUCCESS &&
		*popped == data &&
		ListSize(ptr) == 0)
	{
		result = PASS;
	}

	ListDestroy(&ptr, NULL);
	return result;
}

test ListPopHeadGood2()
{
	test result = FAIL;
	List *ptr;
	ListResult status;
	int data = 3, *popped;
	ptr = ListCreate();
	ListPushTail(ptr, (void*)&data);
	status = ListPopHead(ptr, (void*)&popped);
	
	if (status == LIST_SUCCESS &&
		*popped == data &&
		ListSize(ptr) == 0)
	{
		result = PASS;
	}

	ListDestroy(&ptr, NULL);
	return result;
}

test ListPopHeadBad1()
{
	test result = FAIL;
	ListResult status;
	int *popped;
	status = ListPopHead(NULL, (void*)&popped);
	
	if (status == LIST_UNINITIALIZED_ERROR)
	{
		result = PASS;
	}

	return result;
}

test ListPopHeadBad2()
{
	test result = FAIL;
	List *ptr;
	ListResult status;
	int data = 3;
	ptr = ListCreate();
	ListPushHead(ptr, (void*)&data);
	status = ListPopHead(ptr, NULL);
	
	if (status == LIST_POINTER_UNINITIALIZED_ERROR)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, NULL);
	return result;
}

test ListPopHeadBad3()
{
	test result = FAIL;
	List *ptr;
	ListResult status;
	int *popped;
	ptr = ListCreate();
	status = ListPopHead(ptr, (void*)&popped);
	
	if (status == LIST_UNDERFLOW_ERROR)
	{
		result = PASS;
	}

	ListDestroy(&ptr, NULL);
	return result;
}

test ListPopTailGood1()
{
	test result = FAIL;
	List *ptr;
	ListResult status;
	int data = 3, *popped;
	ptr = ListCreate();
	ListPushTail(ptr, (void*)&data);
	status = ListPopTail(ptr, (void*)&popped);
	
	if (status == LIST_SUCCESS &&
		*popped == data &&
		ListSize(ptr) == 0)
	{
		result = PASS;
	}

	ListDestroy(&ptr, NULL);
	return result;
}

test ListPopTailGood2()
{
	test result = FAIL;
	List *ptr;
	ListResult status;
	int data = 3, *popped;
	ptr = ListCreate();
	ListPushHead(ptr, (void*)&data);
	status = ListPopTail(ptr, (void*)&popped);
	
	if (status == LIST_SUCCESS &&
		*popped == data &&
		ListSize(ptr) == 0)
	{
		result = PASS;
	}

	ListDestroy(&ptr, NULL);
	return result;
}

test ListPopTailBad1()
{
	test result = FAIL;
	ListResult status;
	int *popped;
	status = ListPopTail(NULL, (void*)&popped);
	
	if (status == LIST_UNINITIALIZED_ERROR)
	{
		result = PASS;
	}

	return result;
}

test ListPopTailBad2()
{
	test result = FAIL;
	List *ptr;
	ListResult status;
	int data = 3;
	ptr = ListCreate();
	ListPushTail(ptr, (void*)&data);
	status = ListPopTail(ptr, NULL);
	
	if (status == LIST_POINTER_UNINITIALIZED_ERROR)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, NULL);
	return result;
}

test ListPopTailBad3()
{
	test result = FAIL;
	List *ptr;
	ListResult status;
	int *popped;
	ptr = ListCreate();
	status = ListPopTail(ptr, (void*)&popped);
	
	if (status == LIST_UNDERFLOW_ERROR)
	{
		result = PASS;
	}

	ListDestroy(&ptr, NULL);
	return result;
}

test ListSizeGood1()
{
	test result = FAIL;
	List *ptr;
	int data1 = 3, data2 = 2, data3 = 78;
	size_t count;
	ptr = ListCreate();
	ListPushTail(ptr, (void*)&data1);
	ListPushTail(ptr, (void*)&data2);
	ListPushTail(ptr, (void*)&data3);
	count = ListSize(ptr);
	
	if (count == 3)
	{
		result = PASS;
	}

	ListDestroy(&ptr, NULL);
	return result;
}

test ListSizeGood2()
{
	test result = FAIL;
	List *ptr;
	size_t count;
	ptr = ListCreate();
	count = ListSize(ptr);
	
	if (count == 0)
	{
		result = PASS;
	}

	ListDestroy(&ptr, NULL);
	return result;
}

test ListSizeBad()
{
	test result = FAIL;
	size_t count;
	count = ListSize(NULL);
	
	if (count == 0)
	{
		result = PASS;
	}

	return result;
}

/* ListItr Tests */

test ListItrBeginGood1()
{
	test result = FAIL;
	List *ptr;
	ListItr itr;
	ptr = ListCreate();
	itr = ListItrBegin(ptr);
	
	if ((Node*)itr == &(ptr->m_tail))
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, NULL);
	return result;
}

test ListItrBeginGood2()
{
	test result = FAIL;
	List *ptr;
	ListItr itr;
	int data = 3;
	ptr = ListCreate();
	ListPushHead(ptr, (void*)&data);
	itr = ListItrBegin(ptr);
	
	if ((Node*)itr == ptr->m_head.m_next)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, NULL);
	return result;
}

test ListItrBeginBad()
{
	test result = FAIL;
	ListItr itr;
	itr = ListItrBegin(NULL);
	
	if (itr == NULL)
	{
		result = PASS;
	}
	
	return result;
}

test ListItrEndGood1()
{
	test result = FAIL;
	List *ptr;
	ListItr itr;
	ptr = ListCreate();
	itr = ListItrEnd(ptr);
	
	if ((Node*)itr == &(ptr->m_tail))
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, NULL);
	return result;
}

test ListItrEndGood2()
{
	test result = FAIL;
	List *ptr;
	ListItr itr;
	int data = 3;
	ptr = ListCreate();
	ListPushHead(ptr, (void*)&data);
	itr = ListItrEnd(ptr);
	
	if ((Node*)itr == &(ptr->m_tail))
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, NULL);
	return result;
}

test ListItrEndBad()
{
	test result = FAIL;
	ListItr itr;
	itr = ListItrEnd(NULL);
	
	if (itr == NULL)
	{
		result = PASS;
	}
	
	return result;
}

test ListItrEqualsGood1()
{
	test result = FAIL;
	List *ptr;
	ListItr itr1, itr2;
	ptr = ListCreate();
	itr1 = ListItrBegin(ptr);
	itr2 = ListItrEnd(ptr);
	
	if (ListItrEquals(itr1, itr2))
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, NULL);
	return result;
}

test ListItrEqualsGood2()
{
	test result = FAIL;
	List *ptr;
	ListItr itr1, itr2;
	int data = 3;
	ptr = ListCreate();
	ListPushHead(ptr, (void*)&data);
	itr1 = ListItrBegin(ptr);
	itr2 = ListItrEnd(ptr);
	
	if (!ListItrEquals(itr1, itr2))
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, NULL);
	return result;
}

test ListItrEqualsBad1()
{
	test result = FAIL;
	List *ptr;
	ListItr itr;
	ptr = ListCreate();
	itr = ListItrBegin(ptr);
	
	if (ListItrEquals(NULL ,itr) == PTR_NULL)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, NULL);
	return result;
}

test ListItrEqualsBad2()
{
	test result = FAIL;
	List *ptr;
	ListItr itr;
	ptr = ListCreate();
	itr = ListItrBegin(ptr);
	
	if (ListItrEquals(itr, NULL) == PTR_NULL)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, NULL);
	return result;
}

test ListItrNextGood1()
{
	test result = FAIL;
	List *ptr;
	ListItr itr1, itr2;
	ptr = ListCreate();
	itr1 = ListItrBegin(ptr);
	itr2 = ListItrNext(itr1);
	
	if ((Node*)itr1 == (Node*)itr2 && (Node*)itr1 == &(ptr->m_tail))
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, NULL);
	return result;
}

test ListItrNextGood2()
{
	test result = FAIL;
	List *ptr;
	ListItr itr1, itr2;
	int data = 3;
	ptr = ListCreate();
	ListPushHead(ptr, (void*)&data);
	itr1 = ListItrBegin(ptr);
	itr2 = ListItrNext(itr1);
	
	if ((Node*)itr1 == ptr->m_head.m_next &&
		(Node*)itr2 == &(ptr->m_tail))
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, NULL);
	return result;
}

test ListItrNextBad()
{
	test result = FAIL;
	ListItr itr;
	itr = ListItrNext(NULL);
	
	if (itr == NULL)
	{
		result = PASS;
	}
	
	return result;
}

test ListItrPrevGood1()
{
	test result = FAIL;
	List *ptr;
	ListItr itr1, itr2;
	ptr = ListCreate();
	itr1 = ListItrBegin(ptr);
	itr2 = ListItrPrev(itr1);
	
	if ((Node*)itr1 == (Node*)itr2 && (Node*)itr1 == ptr->m_head.m_next)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, NULL);
	return result;
}

test ListItrPrevGood2()
{
	test result = FAIL;
	List *ptr;
	ListItr itr1, itr2;
	int data = 3;
	ptr = ListCreate();
	ListPushHead(ptr, (void*)&data);
	itr1 = ListItrEnd(ptr);
	itr2 = ListItrPrev(itr1);
	
	if ((Node*)itr1 == &(ptr->m_tail) && (Node*)itr2 == ptr->m_head.m_next)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, NULL);
	return result;
}

test ListItrPrevBad()
{
	test result = FAIL;
	ListItr itr;
	itr = ListItrPrev(NULL);
	
	if (itr == NULL)
	{
		result = PASS;
	}
	
	return result;
}

test ListItrGetGood()
{
	test result = FAIL;
	List *ptr;
	ListItr itr;
	int data = 3;
	void *get;
	ptr = ListCreate();
	ListPushHead(ptr, (void*)&data);
	itr = ListItrBegin(ptr);
	get = ListItrGet(itr);
	
	if (*(int*)get == data)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, NULL);
	return result;
}

test ListItrGetBad1()
{
	test result = FAIL;
	List *ptr;
	ListItr itr;
	void *get;
	ptr = ListCreate();
	itr = ListItrBegin(ptr);
	get = ListItrGet(itr);
	
	if (get == NULL)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, NULL);
	return result;
}

test ListItrGetBad2()
{
	test result = FAIL;
	void *get;
	get = ListItrGet(NULL);
	
	if (get == NULL)
	{
		result = PASS;
	}
	
	return result;
}

test ListItrSetGood()
{
	test result = FAIL;
	List *ptr;
	ListItr itr;
	int data = 3, set = 8;
	void *get;
	ptr = ListCreate();
	ListPushHead(ptr, (void*)&data);
	itr = ListItrBegin(ptr);
	get = ListItrSet(itr, (void*)&set);
	
	if (*(int*)get == data && *(int*)ptr->m_head.m_next->m_data == set)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, NULL);
	return result;
}

test ListItrSetBad1()
{
	test result = FAIL;
	List *ptr;
	ListItr itr;
	int set = 8;
	void *get;
	ptr = ListCreate();
	itr = ListItrBegin(ptr);
	get = ListItrSet(itr, (void*)&set);
	
	if (get == NULL)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, NULL);
	return result;
}

test ListItrSetBad2()
{
	test result = FAIL;
	List *ptr;
	ListItr itr;
	int data = 3;
	void *get;
	ptr = ListCreate();
	ListPushHead(ptr, (void*)&data);
	itr = ListItrBegin(ptr);
	get = ListItrSet(itr, NULL);
	
	if (get == NULL)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, NULL);
	return result;
}

test ListItrSetBad3()
{
	test result = FAIL;
	int set = 8;
	void *get;
	get = ListItrSet(NULL, (void*)&set);
	
	if (get == NULL)
	{
		result = PASS;
	}
	
	return result;
}

test ListItrInsertBeforeGood1()
{
	test result = FAIL;
	List *ptr;
	ListItr itr1, itr2;
	int set = 8;
	ptr = ListCreate();
	itr1 = ListItrBegin(ptr);
	itr2 = ListItrInsertBefore(itr1, (void*)&set);
	
	if (((Node*)itr2)->m_prev == &(ptr->m_head) && 
		((Node*)itr2)->m_next == (Node*)itr1 && 
		((Node*)itr1)->m_prev == (Node*)itr2 &&
		((Node*)itr1) == &(ptr->m_tail) &&
		*(int*)ptr->m_head.m_next->m_data == set)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, NULL);
	return result;
}

test ListItrInsertBeforeGood2()
{
	test result = FAIL;
	List *ptr;
	ListItr itr1, itr2;
	int data = 3, set = 8;
	ptr = ListCreate();
	ListPushHead(ptr, (void*)&data);
	itr1 = ListItrBegin(ptr);
	itr2 = ListItrInsertBefore(itr1, (void*)&set);
	
	if (((Node*)itr2)->m_prev == &(ptr->m_head) && 
		((Node*)itr2)->m_next == (Node*)itr1 && 
		((Node*)itr1)->m_prev == (Node*)itr2 &&
		((Node*)itr1)->m_next == &(ptr->m_tail) &&
		*(int*)ptr->m_head.m_next->m_data == set)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, NULL);
	return result;
}

test ListItrInsertBeforeBad1()
{
	test result = FAIL;
	List *ptr;
	ListItr itr1, itr2;
	ptr = ListCreate();
	itr1 = ListItrBegin(ptr);
	itr2 = ListItrInsertBefore(itr1, NULL);
	
	if (itr2 == NULL && 
		((Node*)itr1)->m_prev == &(ptr->m_head) &&
		((Node*)itr1) == &(ptr->m_tail))
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, NULL);
	return result;
}

test ListItrInsertBeforeBad2()
{
	test result = FAIL;
	ListItr itr;
	int set = 8;
	itr = ListItrInsertBefore(NULL, (void*)&set);
	
	if (itr == NULL)
	{
		result = PASS;
	}
	
	return result;
}

test ListItrRemoveGood()
{
	test result = FAIL;
	List *ptr;
	ListItr itr;
	int data = 3;
	void *removed;
	ptr = ListCreate();
	ListPushHead(ptr, (void*)&data);
	itr = ListItrBegin(ptr);
	removed = ListItrRemove(itr);
	
	if (ptr->m_head.m_next == &(ptr->m_tail) &&
		ptr->m_tail.m_prev == &(ptr->m_head) &&
		*(int*)removed == data)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, NULL);
	return result;
}

test ListItrRemoveBad1()
{
	test result = FAIL;
	List *ptr;
	ListItr itr;
	void *removed;
	ptr = ListCreate();
	itr = ListItrBegin(ptr);
	removed = ListItrRemove(itr);
	
	if (ptr->m_head.m_next == &(ptr->m_tail) &&
		ptr->m_tail.m_prev == &(ptr->m_head) &&
		removed == NULL)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, NULL);
	return result;
}

test ListItrRemoveBad2()
{
	test result = FAIL;
	void *removed;
	removed = ListItrRemove(NULL);
	
	if (removed == NULL)
	{
		result = PASS;
	}
	
	return result;
}

/* List Function Tests */

test ListItrFindFirstGood1()
{
	test result = FAIL;
	List *ptr;
	ListItr itrB, itrE, itrR;
	void *get;
	int data = 30;
	ptr = ListCreate();
	FillListInt(ptr);
	itrB = ListItrBegin(ptr);
	itrE = ListItrEnd(ptr);
	itrR = ListItrFindFirst(itrB, itrE, PredicateEqualIntList, (void*)&data);
	get = ListItrGet(itrR);
	
	if (*(int*)((Node*)itrR)->m_data == data &&
		*(int*)get == data && 
		ListItrEquals(itrB, itrR) == 0 &&
		ListItrEquals(itrR, itrE) == 0)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, FreeIntList);
	return result;
}

test ListItrFindFirstGood2()
{
	test result = FAIL;
	List *ptr;
	ListItr itrB, itrE, itrR;
	void *get;
	int data = 30;
	ptr = ListCreate();
	FillListInt(ptr);
	itrB = ListItrBegin(ptr);
	itrB = ListItrNext(itrB);
	itrE = ListItrEnd(ptr);
	itrE = ListItrPrev(itrE);
	itrR = ListItrFindFirst(itrB, itrE, PredicateEqualIntList, (void*)&data);
	get = ListItrGet(itrR);
	
	if (*(int*)((Node*)itrR)->m_data == data &&
		*(int*)get == data && 
		ListItrEquals(itrB, itrR) == 0 &&
		ListItrEquals(itrR, itrE) == 0)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, FreeIntList);
	return result;
}

test ListItrFindFirstGood3()
{
	test result = FAIL;
	List *ptr;
	ListItr itrB, itrE, itrR;
	int data = 3;
	ptr = ListCreate();
	FillListInt(ptr);
	itrB = ListItrBegin(ptr);
	itrE = ListItrEnd(ptr);
	itrR = ListItrFindFirst(itrB, itrE, PredicateEqualIntList, (void*)&data);
	
	if (ListItrEquals(itrB, itrR) == 0 &&
		ListItrEquals(itrR, itrE) != 0)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, FreeIntList);
	return result;
}

test ListItrFindFirstBad1()
{
	test result = FAIL;
	List *ptr;
	ListItr itrE, itrR;
	int data = 3;
	ptr = ListCreate();
	FillListInt(ptr);
	itrE = ListItrEnd(ptr);
	itrR = ListItrFindFirst(NULL, itrE, PredicateEqualIntList, (void*)&data);
	
	if (itrR == NULL)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, FreeIntList);
	return result;
}

test ListItrFindFirstBad2()
{
	test result = FAIL;
	List *ptr;
	ListItr itrB, itrR;
	int data = 3;
	ptr = ListCreate();
	FillListInt(ptr);
	itrB = ListItrBegin(ptr);
	itrR = ListItrFindFirst(itrB, NULL, PredicateEqualIntList, (void*)&data);
	
	if (itrR == NULL)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, FreeIntList);
	return result;
}

test ListItrFindFirstBad3()
{
	test result = FAIL;
	List *ptr;
	ListItr itrB, itrE, itrR;
	int data = 3;
	ptr = ListCreate();
	FillListInt(ptr);
	itrB = ListItrBegin(ptr);
	itrE = ListItrEnd(ptr);
	itrR = ListItrFindFirst(itrB, itrE, NULL, (void*)&data);
	
	if (itrR == NULL)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, FreeIntList);
	return result;
}

test ListItrCountIfGood1()
{
	test result = FAIL;
	List *ptr;
	ListItr itrB, itrE;
	int data = 40;
	size_t count;
	ptr = ListCreate();
	FillListInt(ptr);
	itrB = ListItrBegin(ptr);
	itrE = ListItrEnd(ptr);
	count = ListItrCountIf(itrB, itrE, PredicateEqualIntList, (void*)&data);
	
	if (count == 2)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, FreeIntList);
	return result;
}

test ListItrCountIfGood2()
{
	test result = FAIL;
	List *ptr;
	ListItr itrB, itrE;
	int data = 40;
	size_t count;
	ptr = ListCreate();
	FillListInt(ptr);
	itrB = ListItrBegin(ptr);
	itrB = ListItrNext(itrB);
	itrE = ListItrEnd(ptr);
	itrE = ListItrPrev(itrE);
	itrE = ListItrPrev(itrE);
	itrE = ListItrPrev(itrE);
	itrE = ListItrPrev(itrE);
	count = ListItrCountIf(itrB, itrE, PredicateEqualIntList, (void*)&data);
	
	if (count == 1)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, FreeIntList);
	return result;
}

test ListItrCountIfGood3()
{
	test result = FAIL;
	List *ptr;
	ListItr itrB, itrE;
	int data = 3;
	size_t count;
	ptr = ListCreate();
	FillListInt(ptr);
	itrB = ListItrBegin(ptr);
	itrE = ListItrEnd(ptr);
	count = ListItrCountIf(itrB, itrE, PredicateEqualIntList, (void*)&data);
	
	if (count == 0)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, FreeIntList);
	return result;
}

test ListItrCountIfBad1()
{
	test result = FAIL;
	List *ptr;
	ListItr itrE;
	int data = 3;
	size_t count;
	ptr = ListCreate();
	FillListInt(ptr);
	itrE = ListItrEnd(ptr);
	count = ListItrCountIf(NULL, itrE, PredicateEqualIntList, (void*)&data);
	
	if (count == 0)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, FreeIntList);
	return result;
}

test ListItrCountIfBad2()
{
	test result = FAIL;
	List *ptr;
	ListItr itrB;
	int data = 3;
	size_t count;
	ptr = ListCreate();
	FillListInt(ptr);
	itrB = ListItrBegin(ptr);
	count = ListItrCountIf(itrB, NULL, PredicateEqualIntList, (void*)&data);
	
	if (count == 0)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, FreeIntList);
	return result;
}

test ListItrCountIfBad3()
{
	test result = FAIL;
	List *ptr;
	ListItr itrB, itrE;
	int data = 3;
	size_t count;
	ptr = ListCreate();
	FillListInt(ptr);
	itrB = ListItrBegin(ptr);
	itrE = ListItrEnd(ptr);
	count = ListItrCountIf(itrB, itrE, NULL, (void*)&data);
	
	if (count == 0)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, FreeIntList);
	return result;
}

test ListItrForEachGood1()
{
	test result = FAIL;
	List *ptr;
	ListItr itrB, itrE, itrR;
	ptr = ListCreate();
	FillListInt(ptr);
	itrB = ListItrBegin(ptr);
	itrE = ListItrEnd(ptr);
	printf("\n\nShould Print Whole List:\n");
	itrR = ListItrForEach(itrB, itrE, ActionPrintIntList, NULL);
	printf("\n\n");
	
	if (ListItrEquals(itrB, itrR) == 0 &&
		ListItrEquals(itrR, itrE) != 0)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, FreeIntList);
	return result;
}

test ListItrForEachGood2()
{
	test result = FAIL;
	List *ptr;
	ListItr itrB, itrE, itrR;
	ptr = ListCreate();
	FillListInt(ptr);
	itrB = ListItrBegin(ptr);
	itrB = ListItrNext(itrB);
	itrE = ListItrEnd(ptr);
	itrE = ListItrPrev(itrE);
	printf("\n\nShould Print redacted List:\n");
	itrR = ListItrForEach(itrB, itrE, ActionPrintIntList, NULL);
	printf("\n\n");
	
	if (ListItrEquals(itrB, itrR) == 0 &&
		ListItrEquals(itrR, itrE) != 0)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, FreeIntList);
	return result;
}

test ListItrForEachGood3()
{
	test result = FAIL;
	List *ptr;
	ListItr itrB, itrE, itrR;
	int data = 40;
	ptr = ListCreate();
	FillListInt(ptr);
	itrB = ListItrBegin(ptr);
	itrE = ListItrEnd(ptr);
	printf("\n\nShould Print only 40's:\n");
	itrR = ListItrForEach(itrB, itrE, ActionPrintContextIntList, (void*)&data);
	printf("\n\n");
	
	if (ListItrEquals(itrB, itrR) == 0 &&
		ListItrEquals(itrR, itrE) != 0)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, FreeIntList);
	return result;
}

test ListItrForEachBad1()
{
	test result = FAIL;
	List *ptr;
	ListItr itrE, itrR;
	int data = 3;
	ptr = ListCreate();
	FillListInt(ptr);
	itrE = ListItrEnd(ptr);
	itrR = ListItrForEach(NULL, itrE, ActionPrintIntList, (void*)&data);
	
	if (itrR == NULL)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, FreeIntList);
	return result;
}

test ListItrForEachBad2()
{
	test result = FAIL;
	List *ptr;
	ListItr itrB, itrR;
	int data = 3;
	ptr = ListCreate();
	FillListInt(ptr);
	itrB = ListItrBegin(ptr);
	itrR = ListItrForEach(itrB, NULL, ActionPrintIntList, (void*)&data);
	
	if (itrR == NULL)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, FreeIntList);
	return result;
}

test ListItrForEachBad3()
{
	test result = FAIL;
	List *ptr;
	ListItr itrB, itrE, itrR;
	int data = 3;
	ptr = ListCreate();
	FillListInt(ptr);
	itrB = ListItrBegin(ptr);
	itrE = ListItrEnd(ptr);
	itrR = ListItrForEach(itrB, itrE, NULL, (void*)&data);
	
	if (itrR == NULL)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, FreeIntList);
	return result;
}

test ListItrForEachBad4()
{
	test result = FAIL;
	List *ptr;
	ListItr itrB, itrE, itrR;
	ptr = ListCreate();
	FillListInt(ptr);
	itrB = ListItrBegin(ptr);
	itrE = ListItrEnd(ptr);
	itrR = ListItrForEach(itrB, itrE, ActionPrintContextIntList, NULL);
	
	if (ListItrEquals(itrB, itrR) != 0)
	{
		result = PASS;
	}
	
	ListDestroy(&ptr, FreeIntList);
	return result;
}

void TestList(void)
{
	printf("ListCreateGood:");
	PrintTestResult(ListCreateGood());
	
	printf("ListDestroyGood:");
	PrintTestResult(ListDestroyGood());
	
	printf("ListDestroyBad:");
	PrintTestResult(ListDestroyBad());
	
	printf("ListPushHeadGood:");
	PrintTestResult(ListPushHeadGood());
	
	printf("ListPushHeadBad1:");
	PrintTestResult(ListPushHeadBad1());
	
	printf("ListPushHeadBad2:");
	PrintTestResult(ListPushHeadBad2());

	printf("ListPushTailGood:");
	PrintTestResult(ListPushTailGood());
	
	printf("ListPushTailBad1:");
	PrintTestResult(ListPushTailBad1());

	printf("ListPushTailBad2:");
	PrintTestResult(ListPushTailBad2());
	
	printf("ListPopHeadGood1:");
	PrintTestResult(ListPopHeadGood1());

	printf("ListPopHeadGood2:");
	PrintTestResult(ListPopHeadGood2());
	
	printf("ListPopHeadBad1:");
	PrintTestResult(ListPopHeadBad1());
	
	printf("ListPopHeadBad2:");
	PrintTestResult(ListPopHeadBad2());

	printf("ListPopHeadBad3:");
	PrintTestResult(ListPopHeadBad3());
	
	printf("ListPopTailGood1:");
	PrintTestResult(ListPopTailGood1());

	printf("ListPopTailGood2:");
	PrintTestResult(ListPopTailGood2());
	
	printf("ListPopTailBad1:");
	PrintTestResult(ListPopTailBad1());
	
	printf("ListPopTailBad2:");
	PrintTestResult(ListPopTailBad2());

	printf("ListPopTailBad3:");
	PrintTestResult(ListPopTailBad3());
	
	printf("ListSizeGood1:");
	PrintTestResult(ListSizeGood1());
	
	printf("ListSizeGood2:");
	PrintTestResult(ListSizeGood2());
	
	printf("ListSizeBad:");
	PrintTestResult(ListSizeBad());

	/* ListItr Tests */

	printf("ListItrBeginGood1:");
	PrintTestResult(ListItrBeginGood1());
	
	printf("ListItrBeginGood2:");
	PrintTestResult(ListItrBeginGood2());
	
	printf("ListItrBeginBad:");
	PrintTestResult(ListItrBeginBad());

	printf("ListItrEndGood1:");
	PrintTestResult(ListItrEndGood1());
	
	printf("ListItrEndGood2:");
	PrintTestResult(ListItrEndGood2());
	
	printf("ListItrEndBad:");
	PrintTestResult(ListItrEndBad());
	
	printf("ListItrEqualsGood1:");
	PrintTestResult(ListItrEqualsGood1());

	printf("ListItrEqualsGood2:");
	PrintTestResult(ListItrEqualsGood2());
	
	printf("ListItrEqualsBad1:");
	PrintTestResult(ListItrEqualsBad1());
	
	printf("ListItrEqualsBad2:");
	PrintTestResult(ListItrEqualsBad2());

	printf("ListItrNextGood1:");
	PrintTestResult(ListItrNextGood1());
	
	printf("ListItrNextGood2:");
	PrintTestResult(ListItrNextGood2());
	
	printf("ListItrNextBad:");
	PrintTestResult(ListItrNextBad());

	printf("ListItrPrevGood1:");
	PrintTestResult(ListItrPrevGood1());
	
	printf("ListItrPrevGood2:");
	PrintTestResult(ListItrPrevGood2());
	
	printf("ListItrPrevBad:");
	PrintTestResult(ListItrPrevBad());

	printf("ListItrGetGood:");
	PrintTestResult(ListItrGetGood());
	
	printf("ListItrGetBad1:");
	PrintTestResult(ListItrGetBad1());

	printf("ListItrGetBad2:");
	PrintTestResult(ListItrGetBad2());

	printf("ListItrSetGood:");
	PrintTestResult(ListItrSetGood());
	
	printf("ListItrSetBad1:");
	PrintTestResult(ListItrSetBad1());

	printf("ListItrSetBad2:");
	PrintTestResult(ListItrSetBad2());

	printf("ListItrSetBad3:");
	PrintTestResult(ListItrSetBad3());

	printf("ListItrInsertBeforeGood1:");
	PrintTestResult(ListItrInsertBeforeGood1());

	printf("ListItrInsertBeforeGood2:");
	PrintTestResult(ListItrInsertBeforeGood2());
	
	printf("ListItrInsertBeforeBad1:");
	PrintTestResult(ListItrInsertBeforeBad1());
	
	printf("ListItrInsertBeforeBad2:");
	PrintTestResult(ListItrInsertBeforeBad2());

	printf("ListItrRemoveGood:");
	PrintTestResult(ListItrRemoveGood());
	
	printf("ListItrRemoveBad1:");
	PrintTestResult(ListItrRemoveBad1());

	printf("ListItrRemoveBad2:");
	PrintTestResult(ListItrRemoveBad2());

	/* List Functions Tests */

	printf("ListItrFindFirstGood1:");
	PrintTestResult(ListItrFindFirstGood1());

	printf("ListItrFindFirstGood2:");
	PrintTestResult(ListItrFindFirstGood2());

	printf("ListItrFindFirstGood3:");
	PrintTestResult(ListItrFindFirstGood3());
	
	printf("ListItrFindFirstBad1:");
	PrintTestResult(ListItrFindFirstBad1());
	
	printf("ListItrFindFirstBad2:");
	PrintTestResult(ListItrFindFirstBad2());

	printf("ListItrFindFirstBad3:");
	PrintTestResult(ListItrFindFirstBad3());

	printf("ListItrCountIfGood1:");
	PrintTestResult(ListItrCountIfGood1());

	printf("ListItrCountIfGood2:");
	PrintTestResult(ListItrCountIfGood2());

	printf("ListItrCountIfGood3:");
	PrintTestResult(ListItrCountIfGood3());
	
	printf("ListItrCountIfBad1:");
	PrintTestResult(ListItrCountIfBad1());
	
	printf("ListItrCountIfBad2:");
	PrintTestResult(ListItrCountIfBad2());

	printf("ListItrCountIfBad3:");
	PrintTestResult(ListItrCountIfBad3());

	printf("ListItrForEachGood1:");
	PrintTestResult(ListItrForEachGood1());

	printf("ListItrForEachGood2:");
	PrintTestResult(ListItrForEachGood2());

	printf("ListItrForEachGood3:");
	PrintTestResult(ListItrForEachGood3());
	
	printf("ListItrForEachBad1:");
	PrintTestResult(ListItrForEachBad1());
	
	printf("ListItrForEachBad2:");
	PrintTestResult(ListItrForEachBad2());

	printf("ListItrForEachBad3:");
	PrintTestResult(ListItrForEachBad3());

	printf("ListItrForEachBad4:");
	PrintTestResult(ListItrForEachBad4());
}

/* Static Test Functions */

static void FillListInt(List *_ptr)
{
    register int i;
    int *valPtr;
    int arr[FILL_INT_SIZE] = {4, 5, 6, 40, 17, 30, 16, 40, 39, 50};
    for (i = 0; i < FILL_INT_SIZE; ++i)
    {
        valPtr = (int*)malloc(sizeof(int));
        *valPtr = arr[i];
        ListPushHead(_ptr, (void*)valPtr);
    }
}

static void FreeIntList(void *_item)
{
    free((int*)_item);
}

static int PredicateEqualIntList(void* _element, void *_context)
{
	return (*(int*)_element == *(int*)_context);
}

static int ActionPrintIntList(void* _element, void *_context)
{
	if (_element != NULL)
	{
		printf("Element: %d\n", *(int*)_element);
		return RESUME_ACT;
	}
	return STOP_ACT;
}

static int ActionPrintContextIntList(void* _element, void *_context)
{
	if (_element != NULL && _context != NULL)
	{
		if (*(int*)_element == *(int*)_context)
		{
			printf("Context = Element = %d\n", *(int*)_element);
		}
		return RESUME_ACT;
	}
	return STOP_ACT;
}






