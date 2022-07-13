#include <stdio.h>
#include <stdlib.h> /* size_t, NULL, malloc, free */
#include "TestDLIB.h"
#include "GenVector.h"

size_t GetNumOfElements(Vector *_ptr);
size_t GetSize(Vector *_ptr);
size_t GetOriginalSize(Vector *_ptr);
size_t GetBlockSize(Vector *_ptr);
void **GetArrayPtr(Vector *_ptr);

static void FreeSize_t(void *_item);
static int PrintIntVector(void* _element, size_t _index, void* _context);
static int SearchIntVector(void* _element, size_t _index, void* _context);

test TestCreateVectorGood1()
{
	test result = FAIL;
	Vector *ptr;
	size_t size = 10, blockSize = 10;
	
	ptr = VectorCreate(size, blockSize);
	
	if (ptr != NULL &&
	GetSize(ptr) == size &&
	GetOriginalSize(ptr) == size &&
	GetNumOfElements(ptr) == 0 &&
	GetArrayPtr(ptr) != NULL &&
	GetBlockSize(ptr) == blockSize)
	{
		result = PASS;
	}
	
	VectorDestroy(&ptr, NULL);
	if (ptr != NULL)
	{
		result = FAIL;
	}
 	return result;
}

test TestCreateVectorGood2()
{
	test result = FAIL;
	Vector *ptr;
	size_t size = 0, blockSize = 10;
	
	ptr = VectorCreate(size, blockSize);
	
	if (ptr != NULL &&
	GetSize(ptr) == size &&
	GetOriginalSize(ptr) == size &&
	GetNumOfElements(ptr) == 0 &&
	GetArrayPtr(ptr) != NULL &&
	GetBlockSize(ptr) == blockSize)
	{
		result = PASS;
	}
	
	VectorDestroy(&ptr, NULL);
 	return result;
}

test TestCreateVectorGood3()
{
	test result = FAIL;
	Vector *ptr;
	size_t size = 10, blockSize = 0;
	
	ptr = VectorCreate(size, blockSize);
	
	if (ptr != NULL &&
	GetSize(ptr) == size &&
	GetOriginalSize(ptr) == size &&
	GetNumOfElements(ptr) == 0 &&
	GetArrayPtr(ptr) != NULL &&
	GetBlockSize(ptr) == blockSize)
	{
		result = PASS;
	}
	
	VectorDestroy(&ptr, NULL);
 	return result;
}

test TestCreateVectorBad1()
{
	test result = FAIL;
	Vector *ptr;
	size_t size = 0, blockSize = 0;
	
	ptr = VectorCreate(size, blockSize);
	
	if (ptr == NULL)
	{
		result = PASS;
	}
	return result;
}

test TestCreateVectorBad2()
{
	test result = FAIL;
	Vector *ptr;
	size_t size = -1, blockSize = 0;
	
	ptr = VectorCreate(size, blockSize);
	
	if (ptr == NULL)
	{
		result = PASS;
	}
	return result;
}

test TestDestroyVectorGood()
{
	test result = PASS;
	Vector *ptr;
	size_t size = 10, blockSize = 10;
	size_t *sizePtr;
	
	ptr = VectorCreate(size, blockSize);
	sizePtr = (size_t*)malloc(sizeof(size_t) * 3);
	sizePtr[0] = size;
	VectorAppend(ptr, (void*)sizePtr);
	
	VectorDestroy(&ptr, FreeSize_t);
	if (ptr != NULL)
	{
		result = FAIL;
	}
 	return result;
}

test TestDestroyVectorBad()
{
	test result = FAIL;
	Vector *ptr;
	size_t size = 10, blockSize = 10;
	
	ptr = VectorCreate(size, blockSize);
	
	if (ptr != NULL &&
	GetSize(ptr) == size &&
	GetOriginalSize(ptr) == size &&
	GetNumOfElements(ptr) == 0 &&
	GetArrayPtr(ptr) != NULL &&
	GetBlockSize(ptr) == blockSize)
	{
		result = PASS;
	}
	
	VectorDestroy(&ptr, NULL);
	VectorDestroy(&ptr, NULL);
	if (ptr != NULL)
	{
		result = FAIL;
	}
 	return result;
}

test TestAppendGood1()
{
	test result = FAIL;
	Vector *ptr;
	VectorResult status;
	size_t size = 1, blockSize = 0;
	int item = 10;
	
	ptr = VectorCreate(size, blockSize);
	status = VectorAppend(ptr, (void*)&item);
	
	if (status == VECTOR_SUCCESS &&
		GetNumOfElements(ptr) == 1 &&
		GetSize(ptr) == size &&
		*(int*)((GetArrayPtr(ptr))[0]) == item)
	{
		result = PASS;
	}
	VectorDestroy(&ptr, NULL);
	return result;
}

test TestAppendGood2()
{
	test result = FAIL;
	Vector *ptr;
	VectorResult status;
	size_t size = 1, blockSize = 2;
	int item1 = 10, item2 = 20;
	
	ptr = VectorCreate(size, blockSize);
	VectorAppend(ptr, (void*)&item1);
	status = VectorAppend(ptr, (void*)&item2);
	
	if (status == VECTOR_SUCCESS &&
	GetNumOfElements(ptr) == 2 &&
	GetSize(ptr) == size + blockSize &&
	*(int*)((GetArrayPtr(ptr))[0]) == item1 &&
	*(int*)((GetArrayPtr(ptr))[1]) == item2)
	{
		result = PASS;
	}

	VectorDestroy(&ptr, NULL);
	return result;
}

test TestAppendGood3()
{
	test result = FAIL;
	Vector *ptr;
	VectorResult status;
	size_t size = 1, blockSize = 1;
	int item = 10;
	
	ptr = VectorCreate(size, blockSize);
	VectorAppend(ptr, (void*)&item);
	VectorAppend(ptr, (void*)&item);
	status = VectorAppend(ptr, (void*)&item);
	
	if (status == VECTOR_SUCCESS &&
	GetNumOfElements(ptr) == 3 &&
	GetSize(ptr) == size + 2*blockSize &&
	*(int*)((GetArrayPtr(ptr))[2]) == item &&
	GetOriginalSize(ptr) == 1)
	{
		result = PASS;
	}
	VectorDestroy(&ptr, NULL);
	return result;
}

test TestAppendBad1()
{
	test result = FAIL;
	Vector *ptr = NULL;
	VectorResult status;
	int item = 10;
	
	status = VectorAppend(ptr, (void*)&item);
	
	if (status == VECTOR_UNITIALIZED_ERROR)
	{
		result = PASS;
	}
	return result;
}

test TestAppendBad2()
{
	test result = FAIL;
	Vector *ptr;
	VectorResult status;
	size_t size = 1, blockSize = 0;
	int item = 10;
	
	ptr = VectorCreate(size, blockSize);
	VectorAppend(ptr, (void*)&item);
	status = VectorAppend(ptr, (void*)&item);
	
	if (status == VECTOR_OVERFLOW &&
	GetNumOfElements(ptr) == size &&
	GetSize(ptr) == size &&
	GetOriginalSize(ptr) == size)
	{
		result = PASS;
	}
	VectorDestroy(&ptr, NULL);
	return result;
}

test TestAppendBad3()
{
	test result = FAIL;
	Vector *ptr;
	VectorResult status;
	size_t size = 1, blockSize = -3;
	int item = 10;
	
	ptr = VectorCreate(size, blockSize);
	VectorAppend(ptr, (void*)&item);
	status = VectorAppend(ptr, (void*)&item);
	
	if (status == VECTOR_ALLOCATION_ERROR &&
	GetNumOfElements(ptr) == size &&
	GetSize(ptr) == size &&
	GetOriginalSize(ptr) == size)
	{
		result = PASS;
	}
	VectorDestroy(&ptr, NULL);
	return result;
}

test TestAppendBad4()
{
	test result = FAIL;
	Vector *ptr;
	VectorResult status;
	size_t size = 1, blockSize = 0;
	
	ptr = VectorCreate(size, blockSize);
	status = VectorAppend(ptr, NULL);
	
	if (status == VECTOR_UNITIALIZED_ERROR)
	{
		result = PASS;
	}
	VectorDestroy(&ptr, NULL);
	return result;
}

test TestAppendBad5()
{
	test result = FAIL;
	Vector *ptr;
	VectorResult status;
	size_t size = 1, blockSize = -1;
	int item = 10;
	
	ptr = VectorCreate(size, blockSize);
	VectorAppend(ptr, (void*)&item);
	status = VectorAppend(ptr, (void*)&item);
	
	if (status == VECTOR_ALLOCATION_ERROR &&
	GetNumOfElements(ptr) == size &&
	GetSize(ptr) == size &&
	GetOriginalSize(ptr) == size)
	{
		result = PASS;
	}
	VectorDestroy(&ptr, NULL);
	return result;
}


test TestGetGood()
{
	test result = FAIL;
	Vector *ptr;
	VectorResult status;
	size_t size = 4, blockSize = 0, index = 0;
	int item1 = 10, item2 = 20, item3 = 30, *targetPtr;
	
	ptr = VectorCreate(size, blockSize);
	VectorAppend(ptr, (void*)&item1);
	VectorAppend(ptr, (void*)&item2);
	VectorAppend(ptr, (void*)&item3);
	status = VectorGet(ptr, index, (void*)&targetPtr);
	
	if (status == VECTOR_SUCCESS &&
	*targetPtr == item1)
	{
		result = PASS;
	}
	VectorDestroy(&ptr, NULL);
	return result;
}

test TestGetBad1()
{
	test result = FAIL;
	Vector *ptr = NULL;
	VectorResult status;
	int *targetPtr;
	size_t index = 0;
	
	status = VectorGet(ptr, index, (void*)&targetPtr);
	
	if (status == VECTOR_UNITIALIZED_ERROR)
	{
		result = PASS;
	}
	return result;
}

test TestGetBad2()
{
	test result = FAIL;
	Vector *ptr;
	VectorResult status;
	size_t size = 4, blockSize = 0;
	size_t index = 0;
	
	ptr = VectorCreate(size, blockSize);
	status = VectorGet(ptr, index, NULL);
	
	if (status == VECTOR_UNITIALIZED_ERROR)
	{
		result = PASS;
	}
	VectorDestroy(&ptr, NULL);
	return result;
}

test TestGetBad3()
{
	test result = FAIL;
	Vector *ptr = NULL;
	VectorResult status;
	size_t index = 0;
	
	status = VectorGet(ptr, index, NULL);
	
	if (status == VECTOR_UNITIALIZED_ERROR)
	{
		result = PASS;
	}
	return result;
}

test TestGetBad4()
{
	test result = FAIL;
	Vector *ptr;
	VectorResult status;
	size_t size = 4, blockSize = 0, index = 3;
	int item1 = 10, item2 = 20, item3 = 30, *targetPtr;
	
	ptr = VectorCreate(size, blockSize);
	VectorAppend(ptr, (void*)&item1);
	VectorAppend(ptr, (void*)&item2);
	VectorAppend(ptr, (void*)&item3);
	status = VectorGet(ptr, index, (void*)&targetPtr);
	
	if (status == VECTOR_INDEX_OUT_OF_BOUNDS_ERROR)
	{
		result = PASS;
	}
	VectorDestroy(&ptr, NULL);
	return result;
}

test TestGetBad5()
{
	test result = FAIL;
	Vector *ptr;
	VectorResult status;
	size_t size = 4, blockSize = 0, index = 0;
	int *targetPtr;
	
	ptr = VectorCreate(size, blockSize);
	status = VectorGet(ptr, index, (void*)&targetPtr);
	
	if (status == VECTOR_INDEX_OUT_OF_BOUNDS_ERROR)
	{
		result = PASS;
	}
	VectorDestroy(&ptr, NULL);
	return result;
}

test TestSetGood()
{
	test result = FAIL;
	Vector *ptr;
	VectorResult status;
	size_t size = 4, blockSize = 0, index = 0;
	int item1 = 10, item2 = 20, item3 = 30, setItem = 600;
	
	ptr = VectorCreate(size, blockSize);
	VectorAppend(ptr, (void*)&item1);
	VectorAppend(ptr, (void*)&item2);
	VectorAppend(ptr, (void*)&item3);
	status = VectorSet(ptr, index, (void*)&setItem);
	
	if (status == VECTOR_SUCCESS &&
	*(int*)((GetArrayPtr(ptr))[index]) == setItem)
	{
		result = PASS;
	}
	VectorDestroy(&ptr, NULL);
	return result;
}

test TestSetBad1()
{
	test result = FAIL;
	Vector *ptr = NULL;
	VectorResult status;
	int item = 600;
	size_t index = 0;
	
	status = VectorSet(ptr, index, (void*)&item);
	
	if (status == VECTOR_UNITIALIZED_ERROR)
	{
		result = PASS;
	}
	VectorDestroy(&ptr, NULL);
	return result;
}

test TestSetBad2()
{
	test result = FAIL;
	Vector *ptr;
	VectorResult status;
	size_t size = 4, blockSize = 0, index = 3;
	int item1 = 10, item2 = 20, item3 = 30, setItem = 600;
	
	ptr = VectorCreate(size, blockSize);
	VectorAppend(ptr, (void*)&item1);
	VectorAppend(ptr, (void*)&item2);
	VectorAppend(ptr, (void*)&item3);
	status = VectorSet(ptr, index, (void*)&setItem);
	
	if (status == VECTOR_INDEX_OUT_OF_BOUNDS_ERROR)
	{
		result = PASS;
	}
	VectorDestroy(&ptr, NULL);
	return result;
}

test TestSetBad3()
{
	test result = FAIL;
	Vector *ptr;
	VectorResult status;
	size_t size = 4, blockSize = 0, index = 0;
	int setItem = 600;
	
	ptr = VectorCreate(size, blockSize);
	status = VectorSet(ptr, index, (void*)&setItem);
	
	if (status == VECTOR_INDEX_OUT_OF_BOUNDS_ERROR)
	{
		result = PASS;
	}
	VectorDestroy(&ptr, NULL);
	return result;
}

test TestSetBad4()
{
	test result = FAIL;
	Vector *ptr;
	VectorResult status;
	size_t size = 4, blockSize = 0, index = 1;
	int item1 = 10, item2 = 20, item3 = 30;
	
	ptr = VectorCreate(size, blockSize);
	VectorAppend(ptr, (void*)&item1);
	VectorAppend(ptr, (void*)&item2);
	VectorAppend(ptr, (void*)&item3);
	status = VectorSet(ptr, index, NULL);
	
	if (status == VECTOR_UNITIALIZED_ERROR)
	{
		result = PASS;
	}
	VectorDestroy(&ptr, NULL);
	return result;
}

test TestSizeGood()
{
	test result = FAIL;
	Vector *ptr;
	size_t size = 4, blockSize = 0, numOfItems;
	int item1 = 10, item2 = 20, item3 = 30;
	
	ptr = VectorCreate(size, blockSize);
	VectorAppend(ptr, (void*)&item1);
	VectorAppend(ptr, (void*)&item2);
	VectorAppend(ptr, (void*)&item3);
	numOfItems = VectorSize(ptr);
	
	if (numOfItems == 3 &&
	GetNumOfElements(ptr) == 3)
	{
		result = PASS;
	}
	VectorDestroy(&ptr, NULL);
	return result;
}

test TestSizeBad1()
{
	test result = FAIL;
	Vector *ptr = NULL;
	size_t numOfItems;
	
	numOfItems = VectorSize(ptr);
	
	if (numOfItems == 0)
	{
		result = PASS;
	}
	return result;
}

test TestCapacityGood()
{
	test result = FAIL;
	Vector *ptr;
	size_t size = 1, blockSize = 4, capacity;
	int item1 = 10, item2 = 20, item3 = 30;
	
	ptr = VectorCreate(size, blockSize);
	VectorAppend(ptr, (void*)&item1);
	VectorAppend(ptr, (void*)&item2);
	VectorAppend(ptr, (void*)&item3);
	capacity = VectorCapacity(ptr);
	
	if (capacity == size + blockSize &&
	GetSize(ptr) == capacity)
	{
		result = PASS;
	}
	VectorDestroy(&ptr, NULL);
	return result;
}

test TestCapacityBad()
{
	test result = FAIL;
	Vector *ptr = NULL;
	size_t capacity;
	
	capacity = VectorCapacity(ptr);
	
	if (capacity == 0)
	{
		result = PASS;
	}
	return result;
}

test TestRemoveVectorGood1()
{
	test result = FAIL;
	Vector *ptr;
	VectorResult status;
	size_t size = 4, blockSize = 0;
	int item1 = 10, item2 = 20, item3 = 30, *lastItemPtr;
	
	ptr = VectorCreate(size, blockSize);
	VectorAppend(ptr, (void*)&item1);
	VectorAppend(ptr, (void*)&item2);
	VectorAppend(ptr, (void*)&item3);
	status = VectorRemove(ptr, (void*)&lastItemPtr);
	
	if (status == VECTOR_SUCCESS &&
	*lastItemPtr == item3 &&
	GetNumOfElements(ptr) == 2 &&
	GetSize(ptr) == size)
	{
		result = PASS;
	}
	VectorDestroy(&ptr, NULL);
	return result;
}

test TestRemoveVectorGood2()
{
	test result = FAIL;
	Vector *ptr;
	VectorResult status;
	size_t size = 1, blockSize = 1;
	int item1 = 10, item2 = 20, item3 = 30, *lastItemPtr;
	
	ptr = VectorCreate(size, blockSize);
	VectorAppend(ptr, (void*)&item1);
	VectorAppend(ptr, (void*)&item2);
	VectorAppend(ptr, (void*)&item3);
	VectorRemove(ptr, (void*)&lastItemPtr);
	status = VectorRemove(ptr, (void*)&lastItemPtr);
	
	if (status == VECTOR_SUCCESS &&
	*lastItemPtr == item2 &&
	GetNumOfElements(ptr) == 1 &&
	GetSize(ptr) == size + blockSize)
	{
		result = PASS;
	}
	VectorDestroy(&ptr, NULL);
	return result;
}

test TestRemoveVectorGood3()
{
	test result = FAIL;
	Vector *ptr;
	VectorResult status;
	size_t size = 2, blockSize = 1;
	int item1 = 10, item2 = 20, item3 = 30, *lastItemPtr;
	
	ptr = VectorCreate(size, blockSize);
	VectorAppend(ptr, (void*)&item1);
	VectorAppend(ptr, (void*)&item2);
	VectorAppend(ptr, (void*)&item3);
	VectorRemove(ptr, (void*)&lastItemPtr);
	VectorRemove(ptr, (void*)&lastItemPtr);
	status = VectorRemove(ptr, (void*)&lastItemPtr);
	
	if (status == VECTOR_SUCCESS &&
	*lastItemPtr == item1 &&
	GetNumOfElements(ptr) == 0 &&
	GetSize(ptr) == size)
	{
		result = PASS;
	}
	VectorDestroy(&ptr, NULL);
	return result;
}

test TestRemoveVectorBad1()
{
	test result = FAIL;
	Vector *ptr = NULL;
	VectorResult status;
	int *lastItemPtr;
	
	status = VectorRemove(ptr, (void*)&lastItemPtr);
	
	if (status == VECTOR_UNITIALIZED_ERROR)
	{
		result = PASS;
	}
	return result;
}

test TestRemoveVectorBad2()
{
	test result = FAIL;
	Vector *ptr;
	VectorResult status;
	size_t size = 1, blockSize = 1;
	int item = 3;
	
	ptr = VectorCreate(size, blockSize);
	VectorAppend(ptr, (void*)&item);
	status = VectorRemove(ptr, NULL);
	
	if (status == VECTOR_UNITIALIZED_ERROR)
	{
		result = PASS;
	}
	VectorDestroy(&ptr, NULL);
	return result;
}

test TestRemoveVectorBad3()
{
	test result = FAIL;
	Vector *ptr = NULL;
	VectorResult status;
	
	status = VectorRemove(ptr, NULL);
	
	if (status == VECTOR_UNITIALIZED_ERROR)
	{
		result = PASS;
	}
	return result;
}

test TestRemoveVectorBad4()
{
	test result = FAIL;
	Vector *ptr;
	VectorResult status;
	size_t size = 1, blockSize = 1;
	int *lastItemPtr;
	
	ptr = VectorCreate(size, blockSize);
	status = VectorRemove(ptr, (void*)&lastItemPtr);
	
	if (status == VECTOR_UNDERFLOW)
	{
		result = PASS;
	}
	VectorDestroy(&ptr, NULL);
	return result;
}

test TestVectorForEachGood1()
{
	test result = FAIL;
	Vector *ptr;
	size_t size = 2, blockSize = 1, forHowMuch;
	int item1 = 10, item2 = 20, item3 = 30;
	
	ptr = VectorCreate(size, blockSize);
	VectorAppend(ptr, (void*)&item1);
	VectorAppend(ptr, (void*)&item2);
	VectorAppend(ptr, (void*)&item3);
	printf("\n\n");
	forHowMuch = VectorForEach(ptr, PrintIntVector, NULL);

	printf("\nIf printed 10, 20, 30 VectorForEachGood1 Passed!\n\n");
	
	if (forHowMuch == 3)
	{
		result = PASS;
	}
	VectorDestroy(&ptr, NULL);
	return result;
}

test TestVectorForEachGood2()
{
	test result = FAIL;
	Vector *ptr;
	size_t size = 2, blockSize = 1, forHowMuch;
	int item1 = 10, item2 = 20, item3 = 30, context = item1;
	
	ptr = VectorCreate(size, blockSize);
	VectorAppend(ptr, (void*)&item1);
	VectorAppend(ptr, (void*)&item2);
	VectorAppend(ptr, (void*)&item3);
	forHowMuch = VectorForEach(ptr, SearchIntVector, (void*)&context);
	
	if (forHowMuch == 0)
	{
		result = PASS;
	}
	VectorDestroy(&ptr, NULL);
	return result;
}

test TestVectorForEachBad1()
{
	test result = FAIL;
	Vector *ptr;
	size_t size = 2, blockSize = 1, forHowMuch;
	int item1 = 10, item2 = 20, item3 = 30;
	
	ptr = VectorCreate(size, blockSize);
	VectorAppend(ptr, (void*)&item1);
	VectorAppend(ptr, (void*)&item2);
	VectorAppend(ptr, (void*)&item3);
	forHowMuch = VectorForEach(ptr, NULL, NULL);
	
	if (forHowMuch == 0)
	{
		result = PASS;
	}
	VectorDestroy(&ptr, NULL);
	return result;
}

test TestVectorForEachBad2()
{
	test result = FAIL;
	Vector *ptr = NULL;
	size_t forHowMuch;
	
	forHowMuch = VectorForEach(ptr, PrintIntVector, NULL);
	
	if (forHowMuch == 0)
	{
		result = PASS;
	}
	return result;
}

void TestVector(void)
{
	printf("TestCreateVectorGood1:");
	PrintTestResult(TestCreateVectorGood1());
	
	printf("TestCreateVectorGood2:");
	PrintTestResult(TestCreateVectorGood2());
	
	printf("TestCreateVectorGood3:");
	PrintTestResult(TestCreateVectorGood3());
	
	printf("TestCreateVectorBad1:");
	PrintTestResult(TestCreateVectorBad1());
	
	printf("TestCreateVectorBad2:");
	PrintTestResult(TestCreateVectorBad2());
	
	printf("TestDestroyVectorGood:");
	PrintTestResult(TestDestroyVectorGood());
	
	printf("TestDestroyVectorBad:");
	PrintTestResult(TestDestroyVectorBad());
	
	printf("TestAppendGood1:");
	PrintTestResult(TestAppendGood1());
	
	printf("TestAppendGood2:");
	PrintTestResult(TestAppendGood2());
	
	printf("TestAppendGood3:");
	PrintTestResult(TestAppendGood3());
	
	printf("TestAppendBad1:");
	PrintTestResult(TestAppendBad1());
	
	printf("TestAppendBad2:");
	PrintTestResult(TestAppendBad2());
	
	printf("TestAppendBad3:");
	PrintTestResult(TestAppendBad3());

	printf("TestAppendBad4:");
	PrintTestResult(TestAppendBad4());
	
	printf("TestAppendBad5:");
	PrintTestResult(TestAppendBad5());
	
	printf("TestGetGood:");
	PrintTestResult(TestGetGood());
	
	printf("TestGetBad1:");
	PrintTestResult(TestGetBad1());
	
	printf("TestGetBad2:");
	PrintTestResult(TestGetBad2());
	
	printf("TestGetBad3:");
	PrintTestResult(TestGetBad3());
	
	printf("TestGetBad4:");
	PrintTestResult(TestGetBad4());
	
	printf("TestGetBad5:");
	PrintTestResult(TestGetBad5());
	
	printf("TestSetGood:");
	PrintTestResult(TestSetGood());
	
	printf("TestSetBad1:");
	PrintTestResult(TestSetBad1());
	
	printf("TestSetBad2:");
	PrintTestResult(TestSetBad2());
	
	printf("TestSetBad3:");
	PrintTestResult(TestSetBad3());

	printf("TestSetBad4:");
	PrintTestResult(TestSetBad4());
	
	printf("TestSizeGood:");
	PrintTestResult(TestSizeGood());
	
	printf("TestSizeBad1:");
	PrintTestResult(TestSizeBad1());

	printf("TestCapacityGood:");
	PrintTestResult(TestCapacityGood());
	
	printf("TestCapacityBad:");
	PrintTestResult(TestCapacityBad());
	
	printf("TestRemoveVectorGood1:");
	PrintTestResult(TestRemoveVectorGood1());
	
	printf("TestRemoveVectorGood2:");
	PrintTestResult(TestRemoveVectorGood2());
	
	printf("TestRemoveVectorGood3:");
	PrintTestResult(TestRemoveVectorGood3());
	
	printf("TestRemoveVectorBad1:");
	PrintTestResult(TestRemoveVectorBad1());
	
	printf("TestRemoveVectorBad2:");
	PrintTestResult(TestRemoveVectorBad2());
	
	printf("TestRemoveVectorBad3:");
	PrintTestResult(TestRemoveVectorBad3());
	
	printf("TestRemoveVectorBad4:");
	PrintTestResult(TestRemoveVectorBad4());

	printf("TestVectorForEachGood1:");
	PrintTestResult(TestVectorForEachGood1());
	
	printf("TestVectorForEachGood2:");
	PrintTestResult(TestVectorForEachGood2());
	
	printf("TestVectorForEachBad1:");
	PrintTestResult(TestVectorForEachBad1());
	
	printf("TestVectorForEachBad2:");
	PrintTestResult(TestVectorForEachBad2());
}

/* static functions for tests */

static void FreeSize_t(void *_item)
{
	free((size_t*)_item);
}

static int PrintIntVector(void* _element, size_t _index, void* _context)
{
	printf("\nElement %lu: %d\n", _index, *(int*)_element);
	return 1;
}

static int SearchIntVector(void* _element, size_t _index, void* _context)
{
	if (*(int*)_element == *(int*)_context)
	{
		return 0;
	}
	return 1;
}















