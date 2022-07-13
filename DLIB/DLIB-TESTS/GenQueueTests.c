#include <stdlib.h> /* size_t, malloc, free, NULL */
#include <stdio.h>
#include "TestDLIB.h"
#include "GenQueue.h"
#define FILL_INT_SIZE 10
#define FILL_PERSON_SIZE 5

typedef struct HowMuchOfThisAge
{
	size_t m_age;
	size_t m_howMuch;
} HowMuchOfThisAge;

typedef struct Person 
{
	int m_id;
	char m_name[128];
	int m_age;
} Person;

Person personsTest[5] = {{12345, "Tomer", 23}, 
                         {26458, "Aviya", 25}, 
                         {34383, "Shira", 25},
                         {42932, "Angela", 25}, 
                         {12341, "Greg", 45}};

size_t GetQueueSize(Queue *_queue);
size_t GetHead(Queue *_queue);
size_t GetTail(Queue *_queue);
size_t GetNItems(Queue *_queue);

static void FillQueueInt(Queue *_queue);
static void FreeIntQueue(void *_item);
static int PrintIntQueue(const void *_elem, void * _context);
static int SearchIntQueue(const void *_elem, void * _context);
static void FillQueuePerson(Queue *_queue);
static int AgeCount(const void *_elem, void * _context);

test TestCreateQueueGood()
{
	test result = FAIL;
	Queue *ptr;
	size_t size = 3;
	
	ptr = QueueCreate(size);

	if (ptr != NULL && 
		GetQueueSize(ptr) == size && 
		GetHead(ptr) == 0 && 
		GetTail(ptr) == 0 && 
		GetNItems(ptr) == 0)
	{
		result = PASS;
	}	

	QueueDestroy(&ptr, NULL);
	return result;
}

test TestCreateQueueBad1()
{
	test result = FAIL;
	Queue *ptr;
	size_t size = 0;
	
	ptr = QueueCreate(size);

	if (ptr == NULL)
	{
		result = PASS;
	}	

	return result;
}

test TestCreateQueueBad2()
{
	test result = FAIL;
	Queue *ptr;
	size_t size = -1;
	
	ptr = QueueCreate(size);

	if (ptr == NULL)
	{
		result = PASS;
	}	

	return result;
}

test TestDestroyQueueGood()
{
	test result = FAIL;
	Queue *ptr;
	size_t size = FILL_INT_SIZE;
	
	ptr = QueueCreate(size);
	FillQueueInt(ptr);
	QueueDestroy(&ptr, FreeIntQueue);

	if (ptr == NULL)
	{
		result = PASS;
	}	

	return result;
}

test TestDestroyQueueBad()
{
	test result = FAIL;
	Queue *ptr;
	size_t size = 3;
	
	ptr = QueueCreate(size);
	QueueDestroy(&ptr, NULL);
	QueueDestroy(&ptr, NULL);

	if (ptr == NULL)
	{
		result = PASS;
	}	

	return result;
}

test TestInsertQueueGood()
{
	test result = FAIL;
	Queue *ptr;
	QueueResult status;
	size_t size = 3;
	int item = 3;
	
	ptr = QueueCreate(size);
	status = QueueInsert(ptr, (void*)&item);
	
	if (ptr != NULL &&
		status == QUEUE_SUCCESS &&
		GetHead(ptr) == 0 &&
		GetTail(ptr) == 1 &&
		GetNItems(ptr) == 1 &&
		GetQueueSize(ptr) == size)
	{
		result = PASS;
	}	

	QueueDestroy(&ptr, NULL);
	return result;
}

test TestInsertQueueBad1()
{
	test result = FAIL;
	QueueResult status;
	int item = 3;
	
	status = QueueInsert(NULL, (void*)&item);
	
	if (status == QUEUE_UNINITIALIZED_ERROR)
	{
		result = PASS;
	}	

	return result;
}

test TestInsertQueueBad2()
{
	test result = FAIL;
	Queue *ptr;
	QueueResult status;
	size_t size = 1;
	
	ptr = QueueCreate(size);
	status = QueueInsert(ptr, NULL);
	
	if (status == QUEUE_DATA_UNINITIALIZED_ERROR)
	{
		result = PASS;
	}	

	QueueDestroy(&ptr, NULL);
	return result;
}

test TestInsertQueueBad3()
{
	test result = FAIL;
	Queue *ptr;
	QueueResult status;
	size_t size = 1;
	int item1 = 3, item2 = 4;
	
	ptr = QueueCreate(size);
	QueueInsert(ptr, (void*)&item1);
	status = QueueInsert(ptr, (void*)&item2);
	
	if (status == QUEUE_OVERFLOW_ERROR)
	{
		result = PASS;
	}	

	QueueDestroy(&ptr, NULL);
	return result;
}

test TestRemoveQueueGood()
{
	test result = FAIL;
	Queue *ptr;
	QueueResult status;
	size_t size = 3;
	int item = 3, *inserted;
	
	ptr = QueueCreate(size);
	QueueInsert(ptr, (void*)&item);
	status = QueueRemove(ptr, (void*)&inserted);
	
	if (status == QUEUE_SUCCESS &&
		*inserted == item &&
		GetHead(ptr) == 1 &&
		GetTail(ptr) == 1 &&
		GetNItems(ptr) == 0 &&
		GetQueueSize(ptr) == size)
	{
		result = PASS;
	}	

	QueueDestroy(&ptr, NULL);
	return result;
}

test TestRemoveQueueBad1()
{
	test result = FAIL;
	QueueResult status;
	int *inserted;
	
	status = QueueRemove(NULL, (void*)&inserted);
	
	if (status == QUEUE_UNINITIALIZED_ERROR)
	{
		result = PASS;
	}	

	return result;
}

test TestRemoveQueueBad2()
{
	test result = FAIL;
	Queue *ptr;
	QueueResult status;
	size_t size = 3;
	int item = 3;
	
	ptr = QueueCreate(size);
	QueueInsert(ptr, (void*)&item);
	status = QueueRemove(ptr, NULL);
	
	if (status == QUEUE_DATA_UNINITIALIZED_ERROR)
	{
		result = PASS;
	}	

	QueueDestroy(&ptr, NULL);
	return result;
}

test TestRemoveQueueBad3()
{
	test result = FAIL;
	Queue *ptr;
	QueueResult status;
	size_t size = 3;
	int *inserted;
	
	ptr = QueueCreate(size);
	status = QueueRemove(ptr, (void*)&inserted);
	
	if (status == QUEUE_DATA_NOT_FOUND_ERROR)
	{
		result = PASS;
	}	

	QueueDestroy(&ptr, NULL);
	return result;
}

test TestEmptyGood1()
{
	test result = FAIL;
	Queue *ptr;
	size_t size = 3, isEmpty;
	
	ptr = QueueCreate(size);
	isEmpty = QueueIsEmpty(ptr);
	
	if (isEmpty == EMPTY)
	{
		result = PASS;
	}

	QueueDestroy(&ptr, NULL);
	return result;
}

test TestEmptyGood2()
{
	test result = FAIL;
	Queue *ptr;
	size_t isEmpty, size = 3;
	int item = 3;
	
	ptr = QueueCreate(size);
	QueueInsert(ptr, (void*)&item);
	isEmpty = QueueIsEmpty(ptr);
	
	if (isEmpty == NOT_EMPTY)
	{
		result = PASS;
	}	

	QueueDestroy(&ptr, NULL);
	return result;
}

test TestEmptyBad()
{
	test result = FAIL;
	size_t isEmpty;
	
	isEmpty = QueueIsEmpty(NULL);
	
	if (isEmpty == PTR_NULL)
	{
		result = PASS;
	}	

	return result;
}

test TestForEachGood1()
{
	test result = FAIL;
	Queue *ptr;
	size_t size = FILL_INT_SIZE, forHowMuch;
	
	ptr = QueueCreate(size);
	FillQueueInt(ptr);
    printf("If prints Queue TestForEachGood1 Passed!\n\n");
	forHowMuch = QueueForEach(ptr, PrintIntQueue, NULL);
    printf("\n\n");

	if (forHowMuch == FILL_INT_SIZE)
	{
		result = PASS;
	}	

	QueueDestroy(&ptr, FreeIntQueue);
	return result;
}

test TestForEachGood2()
{
	test result = FAIL;
	Queue *ptr;
	size_t size = FILL_INT_SIZE, forHowMuch;
	int searchFor = 40;
	
	ptr = QueueCreate(size);
	FillQueueInt(ptr);
	forHowMuch = QueueForEach(ptr, SearchIntQueue, (void*)&searchFor);

	if (forHowMuch == 3)
	{
		result = PASS;
	}	

	QueueDestroy(&ptr, FreeIntQueue);
	return result;
}

test TestForEachGood3()
{
	test result = FAIL;
	Queue *ptr;
	size_t size = FILL_PERSON_SIZE, forHowMuch;
	HowMuchOfThisAge testStruct = {25, 0};

	ptr = QueueCreate(size);
	FillQueuePerson(ptr);
	forHowMuch = QueueForEach(ptr, AgeCount, (void*)&testStruct);

	if (forHowMuch == FILL_PERSON_SIZE && testStruct.m_howMuch == 3)
	{
		result = PASS;
	}	

	QueueDestroy(&ptr, NULL);
	return result;
}

test TestForEachBad1()
{
	test result = FAIL;
	Queue *ptr;
	size_t size = FILL_PERSON_SIZE, forHowMuch;

	ptr = QueueCreate(size);
	FillQueuePerson(ptr);
	forHowMuch = QueueForEach(ptr, NULL, NULL);

	if (forHowMuch == 0)
	{
		result = PASS;
	}	

	QueueDestroy(&ptr, NULL);
	return result;
}

test TestForEachBad2()
{
	test result = FAIL;
	size_t forHowMuch;

	forHowMuch = QueueForEach(NULL, PrintIntQueue, NULL);

	if (forHowMuch == 0)
	{
		result = PASS;
	}	

	return result;
}

void TestQueue(void)
{
	printf("TestCreateQueueGood:");
	PrintTestResult(TestCreateQueueGood());
	
	printf("TestCreateQueueBad1:");
	PrintTestResult(TestCreateQueueBad1());
	
	printf("TestCreateQueueBad2:");
	PrintTestResult(TestCreateQueueBad2());
	
	printf("TestDestroyQueueGood:");
	PrintTestResult(TestDestroyQueueGood());
	
	printf("TestDestroyQueueBad:");
	PrintTestResult(TestDestroyQueueBad());
	
	printf("TestInsertQueueGood:");
	PrintTestResult(TestInsertQueueGood());
	
	printf("TestInsertQueueBad1:");
	PrintTestResult(TestInsertQueueBad1());
	
	printf("TestInsertQueueBad2:");
	PrintTestResult(TestInsertQueueBad2());

	printf("TestInsertQueueBad3:");
	PrintTestResult(TestInsertQueueBad3());
	
	printf("TestRemoveQueueGood:");
	PrintTestResult(TestRemoveQueueGood());
	
	printf("TestRemoveQueueBad1:");
	PrintTestResult(TestRemoveQueueBad1());
	
	printf("TestRemoveQueueBad2:");
	PrintTestResult(TestRemoveQueueBad2());
	
	printf("TestRemoveQueueBad3:");
	PrintTestResult(TestRemoveQueueBad3());
	
	printf("TestEmptyGood1:");
	PrintTestResult(TestEmptyGood1());
	
	printf("TestEmptyGood2:");
	PrintTestResult(TestEmptyGood2());
	
	printf("TestEmptyBad:");
	PrintTestResult(TestEmptyBad());
	
	printf("TestForEachGood1:");
	PrintTestResult(TestForEachGood1());
	
	printf("TestForEachGood2:");
	PrintTestResult(TestForEachGood2());

	printf("TestForEachGood3:");
	PrintTestResult(TestForEachGood3());

	printf("TestForEachBad1:");
	PrintTestResult(TestForEachBad1());
	
	printf("TestForEachBad2:");
	PrintTestResult(TestForEachBad2());
}

/* Static Functions For Tests */

static void FillQueueInt(Queue *_queue)
{
    register int i;
    int *valPtr;
    int arr[FILL_INT_SIZE] = {4, 5, 6, 40, 17, 30, 16, 40, 39, 50};
    for (i = 0; i < FILL_INT_SIZE; ++i)
    {
        valPtr = (int*)malloc(sizeof(int));
        *valPtr = arr[i];
        QueueInsert(_queue, (void*)valPtr);
    }
}

static void FreeIntQueue(void *_item)
{
    free((int*)_item);
}

static int PrintIntQueue(const void *_elem, void * _context)
{
    if (_elem == NULL)
    {
        return 0;
    }
    printf(" %d |", *((int*)_elem));
    return 1;
}

static int SearchIntQueue(const void *_elem, void * _context)
{
    if (_elem == NULL || _context == NULL)
    {
        return 0;
    }
    if (*((int*)_elem) == *((int*)_context))
    {
        return 0;
    }
    return 1;
}

static void FillQueuePerson(Queue *_queue)
{
	register int i;
	for (i = 0; i < FILL_PERSON_SIZE; ++i)
	{
		QueueInsert(_queue, (void*)&personsTest[i]);
	}
}

static int AgeCount(const void *_elem, void * _context)
{
	if (_elem == NULL || _context == NULL)
    {
        return 0;
    }
	if (((Person*)_elem)->m_age == ((HowMuchOfThisAge*)_context)->m_age)
	{
		++((HowMuchOfThisAge*)_context)->m_howMuch;
	}
    return 1;
}