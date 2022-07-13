#include <stdio.h>
#include <stdlib.h>
#include "BSTree.h"
#include "TestDLIB.h"
#define FILL_INT_SIZE 10
#define STOP_ACT 0
#define RESUME_ACT 1

static int LessComparatorBSTInt(void* _compared, void* _with);
static void FillBSTInt(BSTree *_ptr);
static void FreeIntBST(void *_item);
static int PredicateEqualIntBST(void* _element, void *_context);
static int ActionPrintIntBST(void* _element, void *_context);
static int ActionPrintContextIntBST(void* _element, void *_context);

test BSTreeCreateGood()
{
	test result = FAIL;
	BSTree *ptr;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	
	if (ptr != NULL)
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, NULL);
	return result;
}

test BSTreeCreateBad()
{
	test result = FAIL;
	BSTree *ptr;
	ptr = BSTreeCreate(NULL);
	
	if (ptr == NULL)
	{
		result = PASS;
	}

	return result;
}

test BSTreeDestroyGood()
{
	test result = FAIL;
	BSTree *ptr;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	FillBSTInt(ptr);
	BSTreeDestroy(&ptr, FreeIntBST);
	
	if (ptr == NULL)
	{
		result = PASS;
	}
	return result;
}

test BSTreeDestroyBad()
{
	test result = FAIL;
	BSTree *ptr;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	FillBSTInt(ptr);
	BSTreeDestroy(&ptr, FreeIntBST);
	BSTreeDestroy(&ptr, FreeIntBST);
	
	if (ptr == NULL)
	{
		result = PASS;
	}
	return result;
}

test BSTreeInsertGood()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr;
	int data = 3;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	itr = BSTreeInsert(ptr, (void*)&data);
	
	if (itr != NULL)
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, NULL);
	return result;
}

test BSTreeInsertBad1()
{
	test result = FAIL;
	BSTreeItr itr;
	int data = 3;
	itr = BSTreeInsert(NULL, (void*)&data);
	
	if (itr == NULL)
	{
		result = PASS;
	}

	return result;
}

test BSTreeInsertBad2()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr1, itr2;
	int data1 = 3, data2 = 5, data3 = 6, data4 = 1, data5 = 6;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	BSTreeInsert(ptr, (void*)&data1);
	BSTreeInsert(ptr, (void*)&data2);
	BSTreeInsert(ptr, (void*)&data3);
	BSTreeInsert(ptr, (void*)&data4);
	itr1 = BSTreeInsert(ptr, (void*)&data5);
	itr2 = BSTreeItrEnd(ptr);
	
	if (BSTreeItrEquals(itr1, itr2))
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, NULL);
	return result;
}

test BSTreeInsertBad3()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr1, itr2;
	int data1 = 3, data2 = 5, data3 = 6, data4 = 1, data5 = 3;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	BSTreeInsert(ptr, (void*)&data1);
	BSTreeInsert(ptr, (void*)&data2);
	BSTreeInsert(ptr, (void*)&data3);
	BSTreeInsert(ptr, (void*)&data4);
	itr1 = BSTreeInsert(ptr, (void*)&data5);
	itr2 = BSTreeItrEnd(ptr);
	
	if (BSTreeItrEquals(itr1, itr2))
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, NULL);
	return result;
}

test BSTreeInsertBad4()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr1, itr2;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	itr1 = BSTreeInsert(ptr, NULL);
	itr2 = BSTreeItrEnd(ptr);
	
	if (BSTreeItrEquals(itr1, itr2))
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, NULL);
	return result;
}

test BSTreeFindFirstGood1()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr;
	int context = 30;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	FillBSTInt(ptr);
	itr = BSTreeFindFirst(ptr, PredicateEqualIntBST, (void*)&context);
	
	if (itr != NULL && *(int*)BSTreeItrGet(itr) == context)
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, FreeIntBST);
	return result;
}

test BSTreeFindFirstGood2()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr;
	int context = 60;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	FillBSTInt(ptr);
	itr = BSTreeFindFirst(ptr, PredicateEqualIntBST, (void*)&context);
	
	if (itr != NULL && BSTreeItrGet(itr) == NULL)
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, FreeIntBST);
	return result;
}

test BSTreeFindFirstGood3()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr;
	int context = 12;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	FillBSTInt(ptr);
	itr = BSTreeFindFirst(ptr, PredicateEqualIntBST, (void*)&context);
	
	if (itr != NULL && BSTreeItrGet(itr) == NULL)
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, FreeIntBST);
	return result;
}

test BSTreeFindFirstGood4()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr;
	int context = 1;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	FillBSTInt(ptr);
	itr = BSTreeFindFirst(ptr, PredicateEqualIntBST, (void*)&context);
	
	if (itr != NULL && BSTreeItrGet(itr) == NULL)
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, FreeIntBST);
	return result;
}

test BSTreeFindFirstBad1()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr;
	int context = 1;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	FillBSTInt(ptr);
	itr = BSTreeFindFirst(ptr, NULL, (void*)&context);
	
	if (itr == NULL)
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, FreeIntBST);
	return result;
}

test BSTreeFindFirstBad2()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	FillBSTInt(ptr);
	itr = BSTreeFindFirst(ptr, PredicateEqualIntBST, NULL);
	
	if (itr != NULL && *(int*)BSTreeItrGet(itr) == 4)
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, FreeIntBST);
	return result;
}

test BSTreeFindFirstBad3()
{
	test result = FAIL;
	BSTreeItr itr;
	int context = 1;
	itr = BSTreeFindFirst(NULL, PredicateEqualIntBST, (void*)&context);
	
	if (itr == NULL)
	{
		result = PASS;
	}

	return result;
}

test BSTreeItrBeginGood1()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	itr = BSTreeItrBegin(ptr);
	
	if (itr != NULL && BSTreeItrGet(itr) == NULL)
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, FreeIntBST);
	return result;
}

test BSTreeItrBeginGood2()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	FillBSTInt(ptr);
	itr = BSTreeItrBegin(ptr);
	
	if (itr != NULL && *(int*)BSTreeItrGet(itr) == 4)
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, FreeIntBST);
	return result;
}

test BSTreeItrBeginBad()
{
	test result = FAIL;
	BSTreeItr itr;
	itr = BSTreeItrBegin(NULL);
	
	if (itr == NULL)
	{
		result = PASS;
	}

	return result;
}

test BSTreeItrEndGood1()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	itr = BSTreeItrEnd(ptr);
	
	if (itr != NULL && BSTreeItrGet(itr) == NULL)
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, FreeIntBST);
	return result;
}

test BSTreeItrEndGood2()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	FillBSTInt(ptr);
	itr = BSTreeItrEnd(ptr);
	
	if (itr != NULL && BSTreeItrGet(itr) == NULL)
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, FreeIntBST);
	return result;
}

test BSTreeItrEndBad()
{
	test result = FAIL;
	BSTreeItr itr;
	itr = BSTreeItrEnd(NULL);
	
	if (itr == NULL)
	{
		result = PASS;
	}

	return result;
}

test BSTreeItrEqualsGood1()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr1, itr2;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	itr1 = BSTreeItrBegin(ptr);
	itr2 = BSTreeItrBegin(ptr);
	
	if (BSTreeItrEquals(itr1, itr2))
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, FreeIntBST);
	return result;
}

test BSTreeItrEqualsGood2()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr1, itr2;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	itr1 = BSTreeItrBegin(ptr);
	FillBSTInt(ptr);
	itr2 = BSTreeItrBegin(ptr);
	
	if (!BSTreeItrEquals(itr1, itr2))
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, FreeIntBST);
	return result;
}

test BSTreeItrEqualsBad1()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	itr = BSTreeItrBegin(ptr);
	
	if (!BSTreeItrEquals(itr, NULL))
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, FreeIntBST);
	return result;
}

test BSTreeItrEqualsBad2()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	itr = BSTreeItrBegin(ptr);
	
	if (!BSTreeItrEquals(NULL, itr))
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, FreeIntBST);
	return result;
}

test BSTreeItrNextGood1()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	FillBSTInt(ptr);
	itr = BSTreeItrBegin(ptr);
	itr = BSTreeItrNext(itr);
	
	if (itr != NULL && *(int*)BSTreeItrGet(itr) == 5)
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, FreeIntBST);
	return result;
}

test BSTreeItrNextGood2()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	itr = BSTreeItrBegin(ptr);
	itr = BSTreeItrNext(itr);
	
	if (itr != NULL && BSTreeItrGet(itr) == NULL)
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, FreeIntBST);
	return result;
}

test BSTreeItrNextBad()
{
	test result = FAIL;
	BSTreeItr itr;
	itr = BSTreeItrNext(NULL);
	
	if (itr == NULL)
	{
		result = PASS;
	}

	return result;
}

test BSTreeItrPrevGood1()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	FillBSTInt(ptr);
	itr = BSTreeItrBegin(ptr);
	itr = BSTreeItrPrev(itr);
	
	if (itr != NULL && *(int*)BSTreeItrGet(itr) == 4)
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, FreeIntBST);
	return result;
}

test BSTreeItrPrevGood2()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	FillBSTInt(ptr);
	itr = BSTreeItrEnd(ptr);
	itr = BSTreeItrPrev(itr);
	
	if (itr != NULL && *(int*)BSTreeItrGet(itr) == 50)
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, FreeIntBST);
	return result;
}

test BSTreeItrPrevGood3()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	itr = BSTreeItrBegin(ptr);
	itr = BSTreeItrPrev(itr);
	
	if (itr != NULL && BSTreeItrGet(itr) == NULL)
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, FreeIntBST);
	return result;
}

test BSTreeItrPrevBad()
{
	test result = FAIL;
	BSTreeItr itr;
	itr = BSTreeItrPrev(NULL);
	
	if (itr == NULL)
	{
		result = PASS;
	}

	return result;
}

test BSTreeItrRemoveGood1()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr;
	void *removed;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	FillBSTInt(ptr);
	itr = BSTreeItrBegin(ptr);
	removed = BSTreeItrRemove(itr);
	itr = BSTreeItrBegin(ptr);
	
	if (*(int*)removed == 4 && *(int*)BSTreeItrGet(itr) == 5)
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, FreeIntBST);
	return result;
}

test BSTreeItrRemoveGood2()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr;
	void *removed;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	FillBSTInt(ptr);
	itr = BSTreeItrBegin(ptr);
	itr = BSTreeItrNext(itr);
	removed = BSTreeItrRemove(itr);
	itr = BSTreeItrBegin(ptr);
	
	if (*(int*)removed == 5 && *(int*)BSTreeItrGet(itr) == 4)
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, FreeIntBST);
	return result;
}

test BSTreeItrRemoveBad1()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr;
	void *removed;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	itr = BSTreeItrBegin(ptr);
	removed = BSTreeItrRemove(itr);
	
	if (removed == NULL && BSTreeItrGet(itr) == NULL)
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, FreeIntBST);
	return result;
}

test BSTreeItrRemoveBad2()
{
	test result = FAIL;
	void *removed;
	removed = BSTreeItrRemove(NULL);
	
	if (removed == NULL)
	{
		result = PASS;
	}

	return result;
}

test BSTreeItrGetGood1()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr;
	void *got;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	FillBSTInt(ptr);
	itr = BSTreeItrBegin(ptr);
	got = BSTreeItrGet(itr);
	
	if (itr != NULL && *(int*)got == 4)
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, FreeIntBST);
	return result;
}

test BSTreeItrGetGood2()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr;
	void *got;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	itr = BSTreeItrBegin(ptr);
	got = BSTreeItrGet(itr);
	
	if (itr != NULL && got == NULL)
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, FreeIntBST);
	return result;
}

test BSTreeItrGetBad()
{
	test result = FAIL;
	void *got;
	got = BSTreeItrGet(NULL);
	
	if (got == NULL)
	{
		result = PASS;
	}

	return result;
}

test BSTreeForEachGood1()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	FillBSTInt(ptr);
	printf("\n\nIf prints Tree PreOrder ForEachGood1 Passed!\n\n");
	itr = BSTreeForEach(ptr, BSTREE_TRAVERSAL_PREORDER, ActionPrintIntBST, NULL);
	printf("\n\n");
	
	if (BSTreeItrGet(itr) == NULL)
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, FreeIntBST);
	return result;
}

test BSTreeForEachGood2()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	FillBSTInt(ptr);
	printf("\n\nIf prints Tree INOrder ForEachGood2 Passed!\n\n");
	itr = BSTreeForEach(ptr, BSTREE_TRAVERSAL_INORDER, ActionPrintIntBST, NULL);
	printf("\n\n");
	
	if (BSTreeItrGet(itr) == NULL)
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, FreeIntBST);
	return result;
}

test BSTreeForEachGood3()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	FillBSTInt(ptr);
	printf("\n\nIf prints Tree POSTOrder ForEachGood3 Passed!\n\n");
	itr = BSTreeForEach(ptr, BSTREE_TRAVERSAL_POSTORDER, ActionPrintIntBST, NULL);
	printf("\n\n");
	
	if (BSTreeItrGet(itr) == NULL)
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, FreeIntBST);
	return result;
}

test BSTreeForEachGood4()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr;
	int context = 40;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	FillBSTInt(ptr);
	printf("\n\nIf prints Tree Only 40 ForEachGood4 Passed!\n\n");
	itr = BSTreeForEach(ptr, BSTREE_TRAVERSAL_POSTORDER, ActionPrintContextIntBST, (void*)&context);
	printf("\n\n");
	
	if (BSTreeItrGet(itr) == NULL)
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, FreeIntBST);
	return result;
}

test BSTreeForEachBad1()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	FillBSTInt(ptr);
	itr = BSTreeForEach(ptr, BSTREE_TRAVERSAL_INORDER, ActionPrintContextIntBST, NULL);
	
	if (*(int*)BSTreeItrGet(itr) == 4)
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, FreeIntBST);
	return result;
}

test BSTreeForEachBad2()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	FillBSTInt(ptr);
	itr = BSTreeForEach(ptr, BSTREE_TRAVERSAL_INORDER, NULL, NULL);
	
	if (itr == NULL)
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, FreeIntBST);
	return result;
}

test BSTreeForEachBad3()
{
	test result = FAIL;
	BSTree *ptr;
	BSTreeItr itr;
	ptr = BSTreeCreate(LessComparatorBSTInt);
	FillBSTInt(ptr);
	itr = BSTreeForEach(ptr, BSTREE_TRAVERSAL_INORDER * 100, ActionPrintIntBST, NULL);
	
	if (itr == NULL)
	{
		result = PASS;
	}

	BSTreeDestroy(&ptr, FreeIntBST);
	return result;
}

test BSTreeForEachBad4()
{
	test result = FAIL;
	BSTreeItr itr;
	itr = BSTreeForEach(NULL, BSTREE_TRAVERSAL_INORDER, ActionPrintIntBST, NULL);
	
	if (itr == NULL)
	{
		result = PASS;
	}

	return result;
}

void TestBST(void)
{	
	printf("BSTreeCreateGood:");
	PrintTestResult(BSTreeCreateGood());

	printf("BSTreeCreateBad:");
	PrintTestResult(BSTreeCreateBad());
	
	printf("BSTreeDestroyGood:");
	PrintTestResult(BSTreeDestroyGood());
	
	printf("BSTreeDestroyBad:");
	PrintTestResult(BSTreeDestroyBad());
	
	printf("BSTreeInsertGood:");
	PrintTestResult(BSTreeInsertGood());
	
	printf("BSTreeInsertBad1:");
	PrintTestResult(BSTreeInsertBad1());
	
	printf("BSTreeInsertBad2:");
	PrintTestResult(BSTreeInsertBad2());
	
	printf("BSTreeInsertBad3:");
	PrintTestResult(BSTreeInsertBad3());
	
	printf("BSTreeInsertBad4:");
	PrintTestResult(BSTreeInsertBad4());
	
	printf("BSTreeFindFirstGood1:");
	PrintTestResult(BSTreeFindFirstGood1());
	
	printf("BSTreeFindFirstGood2:");
	PrintTestResult(BSTreeFindFirstGood2());
	
	printf("BSTreeFindFirstGood3:");
	PrintTestResult(BSTreeFindFirstGood3());
	
	printf("BSTreeFindFirstGood4:");
	PrintTestResult(BSTreeFindFirstGood4());
	
	printf("BSTreeFindFirstBad1:");
	PrintTestResult(BSTreeFindFirstBad1());
	
	printf("BSTreeFindFirstBad2:");
	PrintTestResult(BSTreeFindFirstBad2());
	
	printf("BSTreeFindFirstBad3:");
	PrintTestResult(BSTreeFindFirstBad3());
	
	printf("BSTreeItrBeginGood1:");
	PrintTestResult(BSTreeItrBeginGood1());
	
	printf("BSTreeItrBeginGood2:");
	PrintTestResult(BSTreeItrBeginGood2());
	
	printf("BSTreeItrBeginBad:");
	PrintTestResult(BSTreeItrBeginBad());
	
	printf("BSTreeItrEndGood1:");
	PrintTestResult(BSTreeItrEndGood1());
	
	printf("BSTreeItrEndGood2:");
	PrintTestResult(BSTreeItrEndGood2());
	
	printf("BSTreeItrEndBad:");
	PrintTestResult(BSTreeItrEndBad());
	
	printf("BSTreeItrEqualsGood1:");
	PrintTestResult(BSTreeItrEqualsGood1());
	
	printf("BSTreeItrEqualsGood2:");
	PrintTestResult(BSTreeItrEqualsGood2());
	
	printf("BSTreeItrEqualsBad1:");
	PrintTestResult(BSTreeItrEqualsBad1());

	printf("BSTreeItrEqualsBad2:");
	PrintTestResult(BSTreeItrEqualsBad2());

	printf("BSTreeItrNextGood1:");
	PrintTestResult(BSTreeItrNextGood1());
	
	printf("BSTreeItrNextGood2:");
	PrintTestResult(BSTreeItrNextGood2());
	
	printf("BSTreeItrNextBad:");
	PrintTestResult(BSTreeItrNextBad());

	printf("BSTreeItrPrevGood1:");
	PrintTestResult(BSTreeItrPrevGood1());
	
	printf("BSTreeItrPrevGood2:");
	PrintTestResult(BSTreeItrPrevGood2());
	
	printf("BSTreeItrPrevGood3:");
	PrintTestResult(BSTreeItrPrevGood3());

	printf("BSTreeItrPrevBad:");
	PrintTestResult(BSTreeItrPrevBad());

	printf("BSTreeItrRemoveGood1:");
	PrintTestResult(BSTreeItrRemoveGood1());
	
	printf("BSTreeItrRemoveGood2:");
	PrintTestResult(BSTreeItrRemoveGood2());
	
	printf("BSTreeItrRemoveBad1:");
	PrintTestResult(BSTreeItrRemoveBad1());

	printf("BSTreeItrRemoveBad2:");
	PrintTestResult(BSTreeItrRemoveBad2());
	
	printf("BSTreeItrGetGood1:");
	PrintTestResult(BSTreeItrGetGood1());

	printf("BSTreeItrGetGood2:");
	PrintTestResult(BSTreeItrGetGood2());

	printf("BSTreeItrGetBad:");
	PrintTestResult(BSTreeItrGetBad());

	printf("BSTreeForEachGood1:");
	PrintTestResult(BSTreeForEachGood1());
	
	printf("BSTreeForEachGood2:");
	PrintTestResult(BSTreeForEachGood2());
	
	printf("BSTreeForEachGood3:");
	PrintTestResult(BSTreeForEachGood3());
	
	printf("BSTreeForEachGood4:");
	PrintTestResult(BSTreeForEachGood4());
	
	printf("BSTreeForEachBad1:");
	PrintTestResult(BSTreeForEachBad1());

	printf("BSTreeForEachBad2:");
	PrintTestResult(BSTreeForEachBad2());
	
	printf("BSTreeForEachBad3:");
	PrintTestResult(BSTreeForEachBad3());

	printf("BSTreeForEachBad4:");
	PrintTestResult(BSTreeForEachBad4());
}

/* Static Functions */

static int LessComparatorBSTInt(void* _compared, void* _with)
{
	if (*(int*)_compared < *(int*)_with)
	{
		return 1;
	}
	if (*(int*)_compared > *(int*)_with)
	{
		return -1;
	}
	return 0;
}

static void FillBSTInt(BSTree *_ptr)
{
    register int i;
    int *valPtr;
    int arr[FILL_INT_SIZE] = {30, 17, 4, 5, 6, 40, 16, 41, 39, 50};
    for (i = 0; i < FILL_INT_SIZE; ++i)
    {
        valPtr = (int*)malloc(sizeof(int));
        *valPtr = arr[i];
        BSTreeInsert(_ptr, (void*)valPtr);
    }
}

static void FreeIntBST(void *_item)
{
    free((int*)_item);
}

static int PredicateEqualIntBST(void* _element, void *_context)
{
	if (_context == NULL)
	{
		return 1;
	}
	return (*(int*)_element == *(int*)_context);
}

static int ActionPrintIntBST(void* _element, void *_context)
{
	if (_element != NULL)
	{
		printf("Element: %d\n", *(int*)_element);
		return RESUME_ACT;
	}
	return STOP_ACT;
}

static int ActionPrintContextIntBST(void* _element, void *_context)
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