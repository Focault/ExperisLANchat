#include <stdlib.h> /* malloc, free, size_t */
#include <stdio.h>
#include "HashMapAPI.h"
#include "TestDLIB.h"
#define FILL_INT_SIZE 11
#define TRUE 1
#define FALSE 0

size_t GetCapacity(HashMap *_hash);

static size_t HashFunctionInt(const void *_key);
static int EqualityFunctionIntHash(const void* _firstData, const void* _secondData);
static void FillHashInt(HashMap *_ptr);
static void FreeIntHash(void *_item);
static int PrintIntHash(const void* _key, void* _value, void* _context);
static int SearchIntByKeyAndIncreaseByOneHash(const void* _key, void* _value, void* _context);

test HashMap_CreateGood1()
{
    test result = FAIL;
    HashMap *hash;
    size_t size = 10;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);

    if (hash != NULL && GetCapacity(hash) == 11)
    {
        result = PASS;
    }

    HashMap_Destroy(&hash, NULL, NULL);
    return result;
}

test HashMap_CreateGood2()
{
    test result = FAIL;
    HashMap *hash;
    size_t size = 11;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);

    if (hash != NULL && GetCapacity(hash) == 11)
    {
        result = PASS;
    }

    HashMap_Destroy(&hash, NULL, NULL);
    return result;
}

test HashMap_CreateBad1()
{
    test result = FAIL;
    HashMap *hash;
    size_t size = 0;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);

    if (hash == NULL)
    {
        result = PASS;
    }

    return result;
}

test HashMap_CreateBad2()
{
    test result = FAIL;
    HashMap *hash;
    size_t size = 10;
    hash = HashMap_Create(size, NULL, EqualityFunctionIntHash);

    if (hash == NULL)
    {
        result = PASS;
    }

    return result;
}

test HashMap_CreateBad3()
{
    test result = FAIL;
    HashMap *hash;
    size_t size = 10;
    hash = HashMap_Create(size, HashFunctionInt, NULL);

    if (hash == NULL)
    {
        result = PASS;
    }

    return result;
}

test HashMap_CreateBad4()
{
    test result = FAIL;
    HashMap *hash;
    size_t size = 53683888843488;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);

    if (hash == NULL)
    {
        result = PASS;
    }

    return result;
}

test HashMap_CreateBad5()
{
    test result = FAIL;
    HashMap *hash;
    size_t size = -1;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);

    if (hash == NULL)
    {
        result = PASS;
    }

    return result;
}

test HashMap_DestroyGood()
{
    test result = FAIL;
    HashMap *hash;
    size_t size = 10;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);
    FillHashInt(hash);
    HashMap_Destroy(&hash, FreeIntHash, FreeIntHash);
    HashMap_Destroy(&hash, FreeIntHash, FreeIntHash);

    if (hash == NULL)
    {
        result = PASS;
    }

    return result;
}

test HashMap_InsertGood1()
{
    test result = FAIL;
    HashMap *hash;
    Map_Result status;
    size_t size = 10;
    int data = 3, key = 8;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);
    status = HashMap_Insert(hash, (void*)&key, (void*)&data);

    if (status == MAP_SUCCESS && HashMap_Size(hash) == 1)
    {
        result = PASS;
    }

    HashMap_Destroy(&hash, NULL, NULL);
    return result;
}

test HashMap_InsertGood2()
{
    test result = FAIL;
    HashMap *hash;
    Map_Result status;
    size_t size = 10;
    int key = 3;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);
    status = HashMap_Insert(hash, (void*)&key, NULL);

    if (status == MAP_SUCCESS && HashMap_Size(hash) == 1)
    {
        result = PASS;
    }

    HashMap_Destroy(&hash, NULL, NULL);
    return result;
}

test HashMap_InsertGood3()
{
    test result = FAIL;
    HashMap *hash;
    Map_Result status;
    size_t size = 1;
    int key1 = 3, key2 = 4, key3 = 5;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);
    status = HashMap_Insert(hash, (void*)&key1, NULL);
    status = HashMap_Insert(hash, (void*)&key2, NULL);
    status = HashMap_Insert(hash, (void*)&key3, NULL);

    if (status == MAP_SUCCESS && HashMap_Size(hash) == 3)
    {
        result = PASS;
    }

    HashMap_Destroy(&hash, NULL, NULL);
    return result;
}

test HashMap_InsertGood4()
{
    test result = FAIL;
    HashMap *hash;
    Map_Result status;
    size_t size = 1;
    int data = 4, key1 = 3, key2 = 4, key3 = 5;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);
    status = HashMap_Insert(hash, (void*)&key1, (void*)&data);
    status = HashMap_Insert(hash, (void*)&key2, (void*)&data);
    status = HashMap_Insert(hash, (void*)&key3, (void*)&data);

    if (status == MAP_SUCCESS && HashMap_Size(hash) == 3)
    {
        result = PASS;
    }

    HashMap_Destroy(&hash, NULL, NULL);
    return result;
}

test HashMap_InsertBad1()
{
    test result = FAIL;
    Map_Result status;
    int data = 3;
    status = HashMap_Insert(NULL, (void*)&data, NULL);

    if (status == MAP_UNINITIALIZED_ERROR)
    {
        result = PASS;
    }

    return result;
}

test HashMap_InsertBad2()
{
    test result = FAIL;
    HashMap *hash;
    Map_Result status;
    size_t size = 10;
    int data = 3;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);
    status = HashMap_Insert(hash, (void*)&data, NULL);
    status = HashMap_Insert(hash, (void*)&data, NULL);

    if (status == MAP_KEY_DUPLICATE_ERROR && HashMap_Size(hash) == 1)
    {
        result = PASS;
    }

    HashMap_Destroy(&hash, NULL, NULL);
    return result;
}

test HashMap_InsertBad3()
{
    test result = FAIL;
    HashMap *hash;
    Map_Result status;
    size_t size = 10;
    int data = 3;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);
    status = HashMap_Insert(hash, NULL, (void*)&data);

    if (status == MAP_KEY_NULL_ERROR)
    {
        result = PASS;
    }

    HashMap_Destroy(&hash, NULL, NULL);
    return result;
}

test HashMap_RemoveGood1()
{
    test result = FAIL;
    HashMap *hash;
    Map_Result status;
    size_t size = 10;
    int key = 3, value = 5, *removedKey, *removedValue;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);
    HashMap_Insert(hash, (void*)&key, (void*)&value);
    status = HashMap_Remove(hash, (void*)&key, (void*)&removedKey, (void*)&removedValue);

    if (status == MAP_SUCCESS && HashMap_Size(hash) == 0 && *removedKey == key && *removedValue == value) 
    {
        result = PASS;
    }

    HashMap_Destroy(&hash, NULL, NULL);
    return result;
}

test HashMap_RemoveGood2()
{
    test result = FAIL;
    HashMap *hash;
    Map_Result status;
    size_t size = 10;
    int key = 3, value = 5, *removedKey;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);
    HashMap_Insert(hash, (void*)&key, (void*)&value);
    status = HashMap_Remove(hash, (void*)&key, (void*)&removedKey, NULL);

    if (status == MAP_SUCCESS && HashMap_Size(hash) == 0 && *removedKey == key) 
    {
        result = PASS;
    }

    HashMap_Destroy(&hash, NULL, NULL);
    return result;
}

test HashMap_RemoveBad1()
{
    test result = FAIL;
    HashMap *hash;
    Map_Result status;
    size_t size = 10;
    int data = 3, random = 5, *removedKey, *removedValue;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);
    HashMap_Insert(hash, (void*)&data, NULL);
    status = HashMap_Remove(hash, (void*)&random, (void*)&removedKey, (void*)&removedValue);

    if (status == MAP_KEY_NOT_FOUND_ERROR && HashMap_Size(hash) == 1) 
    {
        result = PASS;
    }

    HashMap_Destroy(&hash, NULL, NULL);
    return result;
}

test HashMap_RemoveBad2()
{
    test result = FAIL;
    HashMap *hash;
    Map_Result status;
    size_t size = 10;
    int *removedKey, *removedValue;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);
    status = HashMap_Remove(hash, NULL, (void*)&removedKey, (void*)&removedValue);

    if (status == MAP_KEY_NULL_ERROR) 
    {
        result = PASS;
    }

    HashMap_Destroy(&hash, NULL, NULL);
    return result;
}

test HashMap_RemoveBad3()
{
    test result = FAIL;
    HashMap *hash;
    Map_Result status;
    size_t size = 10;
    int random = 5, *removedValue;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);
    status = HashMap_Remove(hash, (void*)&random, NULL, (void*)&removedValue);

    if (status == MAP_UNINITIALIZED_ERROR) 
    {
        result = PASS;
    }

    HashMap_Destroy(&hash, NULL, NULL);
    return result;
}

test HashMap_RemoveBad4()
{
    test result = FAIL;
    Map_Result status;
    int random = 5, *removedKey, *removedValue;
    status = HashMap_Remove(NULL, (void*)&random, (void*)&removedKey, (void*)&removedValue);

    if (status == MAP_UNINITIALIZED_ERROR) 
    {
        result = PASS;
    }

    return result;
}

test HashMap_FindGood1()
{
    test result = FAIL;
    HashMap *hash;
    Map_Result status;
    size_t size = 10;
    int key = 3, value = 5, *found;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);
    HashMap_Insert(hash, (void*)&key, (void*)&value);
    status = HashMap_Find(hash, (void*)&key, (void*)&found);

    if (*found == value && status == MAP_SUCCESS)
    {
        result = PASS;
    }

    HashMap_Destroy(&hash, NULL, NULL);
    return result;
}

test HashMap_FindGood2()
{
    test result = FAIL;
    HashMap *hash;
    Map_Result status;
    size_t size = 10;
    int key = 3, value = 5, *found = &key;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);
    status = HashMap_Find(hash, (void*)&value, (void*)&found);

    if (*found == key && status == MAP_KEY_NOT_FOUND_ERROR)
    {
        result = PASS;
    }

    HashMap_Destroy(&hash, NULL, NULL);
    return result;
}

test HashMap_FindGood3()
{
    test result = FAIL;
    HashMap *hash;
    Map_Result status;
    size_t size = 10;
    int key = 3, value = 5, *found, *foundAgain;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);
    HashMap_Insert(hash, (void*)&key, (void*)&value);
    HashMap_Find(hash, (void*)&key, (void*)&found);
    ++*found;
    status = HashMap_Find(hash, (void*)&key, (void*)&foundAgain);

    if (*foundAgain == 6 && *foundAgain == value && status == MAP_SUCCESS)
    {
        result = PASS;
    }

    HashMap_Destroy(&hash, NULL, NULL);
    return result;
}

test HashMap_FindBad1()
{
    test result = FAIL;
    HashMap *hash;
    Map_Result status;
    size_t size = 10;
    int key = 3, *found = &key;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);
    status = HashMap_Find(hash, NULL, (void*)&found);

    if (*found == key && status == MAP_KEY_NULL_ERROR)
    {
        result = PASS;
    }

    HashMap_Destroy(&hash, NULL, NULL);
    return result;
}

test HashMap_FindBad2()
{
    test result = FAIL;
    Map_Result status;
    int data = 3, *found;
    status = HashMap_Find(NULL, (void*)&data, (void*)&found);

    if (status == MAP_UNINITIALIZED_ERROR)
    {
        result = PASS;
    }

    return result;
}

test HashMap_FindBad3()
{
    test result = FAIL;
    HashMap *hash;
    Map_Result status;
    size_t size = 10;
    int key = 3;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);
    status = HashMap_Find(hash, (void*)&key, NULL);

    if (status == MAP_UNINITIALIZED_ERROR)
    {
        result = PASS;
    }

    HashMap_Destroy(&hash, NULL, NULL);
    return result;
}

test HashMap_SizeGood1()
{
    test result = FAIL;
    HashMap *hash;
    size_t size = 10, numOfItems;
    int data = 3;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);
    HashMap_Insert(hash, (void*)&data, NULL);
    numOfItems = HashMap_Size(hash);

    if (numOfItems == 1)
    {
        result = PASS;
    }

    HashMap_Destroy(&hash, NULL, NULL);
    return result;
}

test HashMap_SizeGood2()
{
    test result = FAIL;
    HashMap *hash;
    size_t size = 10, numOfItems;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);
    FillHashInt(hash);
    numOfItems = HashMap_Size(hash);

    if (numOfItems == FILL_INT_SIZE)
    {
        result = PASS;
    }

    HashMap_Destroy(&hash, FreeIntHash, FreeIntHash);
    return result;
}

test HashMap_SizeGood3()
{
    test result = FAIL;
    HashMap *hash;
    size_t size = 10, numOfItems;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);
    numOfItems = HashMap_Size(hash);

    if (numOfItems == 0)
    {
        result = PASS;
    }

    HashMap_Destroy(&hash, NULL, NULL);
    return result;
}

test HashMap_SizeBad()
{
    test result = FAIL;
    size_t numOfItems;
    numOfItems = HashMap_Size(NULL);

    if (numOfItems == 0)
    {
        result = PASS;
    }

    return result;
}

test HashMap_ForEachGood1()
{
    test result = FAIL;
    HashMap *hash;
    size_t size = 10, numOfTimes;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);
    FillHashInt(hash);
    printf("\n\nIf Prints the Hash HashPrintGood1 Passed:\n\n");
    numOfTimes = HashMap_ForEach(hash, PrintIntHash, NULL);
    printf("\n\n");

    if (numOfTimes == FILL_INT_SIZE)
    {
        result = PASS;
    }

    HashMap_Destroy(&hash, FreeIntHash, FreeIntHash);
    return result;
}

test HashMap_ForEachGood2()
{
    test result = FAIL;
    HashMap *hash;
    size_t size = 10, numOfTimes;
    int context = 0, *found;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);
    FillHashInt(hash);
    numOfTimes = HashMap_ForEach(hash, SearchIntByKeyAndIncreaseByOneHash, (void*)&context);
    HashMap_Find(hash, (void*)&context, (void*)&found);

    if (numOfTimes < FILL_INT_SIZE && *found == 5)
    {
        result = PASS;
    }

    HashMap_Destroy(&hash, FreeIntHash, FreeIntHash);
    return result;
}

test HashMap_ForEachBad1()
{
    test result = FAIL;
    HashMap *hash;
    size_t size = 10, numOfTimes;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);
    numOfTimes = HashMap_ForEach(hash, NULL, NULL);

    if (numOfTimes == 0)
    {
        result = PASS;
    }

    HashMap_Destroy(&hash, NULL, NULL);
    return result;
}

test HashMap_ForEachBad2()
{
    test result = FAIL;
    size_t numOfTimes;
    numOfTimes = HashMap_ForEach(NULL, PrintIntHash, NULL);

    if (numOfTimes == 0)
    {
        result = PASS;
    }

    return result;
}

test HashMap_GetStatisticsGood1()
{
    test result = FAIL;
    HashMap *hash;
    Map_Stats stats;
    size_t size = 4;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);
    FillHashInt(hash);
    stats = HashMap_GetStatistics(hash);

    if (stats.maxChainLength == 3 && 
        stats.numberOfBuckets == 5 &&
        stats.numberOfChains == 11 &&
        stats.averageChainLength == 2.2)
    {
        result = PASS;
    }

    HashMap_Destroy(&hash, FreeIntHash, FreeIntHash);
    return result;
}

test HashMap_GetStatisticsGood2()
{
    test result = FAIL;
    HashMap *hash;
    Map_Stats stats;
    size_t size = 4;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);
    stats = HashMap_GetStatistics(hash);

    if (stats.maxChainLength == 0 && 
        stats.numberOfBuckets == 0 &&
        stats.numberOfChains == 0 &&
        stats.averageChainLength == 0)
    {
        result = PASS;
    }

    HashMap_Destroy(&hash, FreeIntHash, FreeIntHash);
    return result;
}

test HashMap_GetStatisticsBad()
{
    test result = FAIL;
    Map_Stats stats;
    stats = HashMap_GetStatistics(NULL);

    if (stats.maxChainLength == 0 && 
        stats.numberOfBuckets == 0 &&
        stats.numberOfChains == 0 &&
        stats.averageChainLength == 0)
    {
        result = PASS;
    }

    return result;
}

test HashMap_RehashGood()
{
    test result = FAIL;
    HashMap *hash;
    Map_Stats stats;
    Map_Result status;
    size_t size = 4;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);
    FillHashInt(hash);
    status = HashMap_Rehash(hash, 6);
    stats = HashMap_GetStatistics(hash);

    if (status == MAP_SUCCESS &&
        GetCapacity(hash) == 7 &&
        stats.maxChainLength == 2 && 
        stats.numberOfBuckets == 7 &&
        stats.numberOfChains == 11 &&
        stats.averageChainLength == 11 / 7.0)
    {
        result = PASS;
    }

    HashMap_Destroy(&hash, FreeIntHash, FreeIntHash);
    return result;
}

test HashMap_RehashBad1()
{
    test result = FAIL;
    HashMap *hash;
    Map_Result status;
    size_t size = 4;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);
    status = HashMap_Rehash(hash, 0);

    if (status == MAP_SIZE_ILLIGAL_ERROR)
    {
        result = PASS;
    }

    HashMap_Destroy(&hash, FreeIntHash, FreeIntHash);
    return result;
}

test HashMap_RehashBad2()
{
    test result = FAIL;
    Map_Result status;
    status = HashMap_Rehash(NULL, 6);

    if (status == MAP_UNINITIALIZED_ERROR)
    {
        result = PASS;
    }

    return result;
}

test HashMap_RehashBad3()
{
    test result = FAIL;
    HashMap *hash;
    Map_Result status;
    size_t size = 4;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);
    status = HashMap_Rehash(hash, -1);

    if (status == MAP_ALLOCATION_ERROR)
    {
        result = PASS;
    }

    HashMap_Destroy(&hash, FreeIntHash, FreeIntHash);
    return result;
}

test HashMap_RehashBad4()
{
    test result = FAIL;
    HashMap *hash;
    Map_Result status;
    size_t size = 4;
    hash = HashMap_Create(size, HashFunctionInt, EqualityFunctionIntHash);
    status = HashMap_Rehash(hash, 4378597903);

    if (status == MAP_ALLOCATION_ERROR)
    {
        result = PASS;
    }

    HashMap_Destroy(&hash, FreeIntHash, FreeIntHash);
    return result;
}

void TestHash(void)
{
	printf("HashMap_CreateGood1:");
	PrintTestResult(HashMap_CreateGood1());

    printf("HashMap_CreateGood2:");
	PrintTestResult(HashMap_CreateGood2());
    
    printf("HashMap_CreateBad1:");
	PrintTestResult(HashMap_CreateBad1());

    printf("HashMap_CreateBad2:");
	PrintTestResult(HashMap_CreateBad2());

    printf("HashMap_CreateBad3:");
	PrintTestResult(HashMap_CreateBad3());

    printf("HashMap_CreateBad4:");
	PrintTestResult(HashMap_CreateBad4());

    printf("HashMap_CreateBad5:");
	PrintTestResult(HashMap_CreateBad5());

    printf("HashMap_DestroyGood:");
	PrintTestResult(HashMap_DestroyGood());

    printf("HashMap_InsertGood1:");
	PrintTestResult(HashMap_InsertGood1());

    printf("HashMap_InsertGood2:");
	PrintTestResult(HashMap_InsertGood2());

    printf("HashMap_InsertGood3:");
	PrintTestResult(HashMap_InsertGood3());

    printf("HashMap_InsertGood4:");
	PrintTestResult(HashMap_InsertGood4());
    
    printf("HashMap_InsertBad1:");
	PrintTestResult(HashMap_InsertBad1());

    printf("HashMap_InsertBad2:");
	PrintTestResult(HashMap_InsertBad2());

    printf("HashMap_InsertBad3:");
	PrintTestResult(HashMap_InsertBad3());

    printf("HashMap_RemoveGood1:");
	PrintTestResult(HashMap_RemoveGood1());

    printf("HashMap_RemoveGood2:");
	PrintTestResult(HashMap_RemoveGood2());

    printf("HashMap_RemoveBad1:");
	PrintTestResult(HashMap_RemoveBad1());

    printf("HashMap_RemoveBad2:");
	PrintTestResult(HashMap_RemoveBad2());

    printf("HashMap_RemoveBad3:");
	PrintTestResult(HashMap_RemoveBad3());

    printf("HashMap_RemoveBad4:");
	PrintTestResult(HashMap_RemoveBad4());

    printf("HashMap_FindGood1:");
	PrintTestResult(HashMap_FindGood1());

    printf("HashMap_FindGood2:");
	PrintTestResult(HashMap_FindGood2());

    printf("HashMap_FindGood3:");
	PrintTestResult(HashMap_FindGood3());
    
    printf("HashMap_FindBad1:");
	PrintTestResult(HashMap_FindBad1());

    printf("HashMap_FindBad2:");
	PrintTestResult(HashMap_FindBad2());

    printf("HashMap_FindBad3:");
	PrintTestResult(HashMap_FindBad3());

    printf("HashMap_SizeGood1:");
	PrintTestResult(HashMap_SizeGood1());

    printf("HashMap_SizeGood2:");
	PrintTestResult(HashMap_SizeGood2());

    printf("HashMap_SizeGood3:");
	PrintTestResult(HashMap_SizeGood3());

    printf("HashMap_SizeBad:");
	PrintTestResult(HashMap_SizeBad());

    printf("HashMap_ForEachGood1:");
	PrintTestResult(HashMap_ForEachGood1());

    printf("HashMap_ForEachGood2:");
	PrintTestResult(HashMap_ForEachGood2());
    
    printf("HashMap_ForEachBad1:");
	PrintTestResult(HashMap_ForEachBad1());

    printf("HashMap_ForEachBad2:");
	PrintTestResult(HashMap_ForEachBad2());

    printf("HashMap_GetStatisticsGood1:");
	PrintTestResult(HashMap_GetStatisticsGood1());

    printf("HashMap_GetStatisticsGood2:");
	PrintTestResult(HashMap_GetStatisticsGood2());

    printf("HashMap_GetStatisticsBad:");
	PrintTestResult(HashMap_GetStatisticsBad());

    printf("HashMap_RehashGood:");
	PrintTestResult(HashMap_RehashGood());

    printf("HashMap_RehashBad1:");
	PrintTestResult(HashMap_RehashBad1());

    printf("HashMap_RehashBad2:");
	PrintTestResult(HashMap_RehashBad2());

    printf("HashMap_RehashBad3:");
	PrintTestResult(HashMap_RehashBad3());

    printf("HashMap_RehashBad4:");
	PrintTestResult(HashMap_RehashBad4());
}

/* Static test Functions */

static size_t HashFunctionInt(const void *_key)
{
    return *(int*)_key * 2;
}

static int EqualityFunctionIntHash(const void* _firstData, const void* _secondData)
{
    if (*(int*)_firstData == *(int*)_secondData)
    {
        return TRUE;
    }
    return FALSE;
}

static void FillHashInt(HashMap *_ptr)
{
    register int i;
    int *valPtr, *keyPtr;
    int valArr[FILL_INT_SIZE] = {4, 5, 6, 40, 17, 30, 16, 41, 39, 50, 328};
    int keyArr[FILL_INT_SIZE] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (i = 0; i < FILL_INT_SIZE; ++i)
    {
        valPtr = (int*)malloc(sizeof(int));
        *valPtr = valArr[i];
        keyPtr = (int*)malloc(sizeof(int));
        *keyPtr = keyArr[i];
        HashMap_Insert(_ptr, (void*)keyPtr, (void*)valPtr);
    }
}

static void FreeIntHash(void *_item)
{
    free((int*)_item);
}

static int PrintIntHash(const void* _key, void* _value, void* _context)
{
    printf("Value of key %d: %d\n", *(int*)_key, *(int*)_value);
    return 1;
}

static int SearchIntByKeyAndIncreaseByOneHash(const void* _key, void* _value, void* _context)
{
    if (*(int*)_key == *(int*)_context)
    {
        ++*(int*)_value;
        return 0;
    }
    return 1;
}
