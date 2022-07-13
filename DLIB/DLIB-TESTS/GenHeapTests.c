#include <stdio.h>
#include <stdlib.h> /* malloc */
#include "GenVector.h"
#include "GenHeap.h"
#include "TestDLIB.h"
#define FATHER(idx) ((idx)-1)/2

size_t GetHeapSize(Heap *_heap);

static void FillVectorForHeap(Vector *_ptr);
static test IsHeapMaxInt(Vector *_ptr);
static void FreeIntHeap(void *_item);
static int HeapMaxInt(const void *_candidate, const void *_context);
static int PrintIntHeap(const void *_elem, void * _context);
static int SearchIntHeap(const void *_elem, void * _context);

test HeapBuildGood1()
{
    test result = FAIL;
    Heap *heapPtr;
    Vector *vecPtr, *returned;
    size_t size;
    vecPtr = VectorCreate(40, 10);
    FillVectorForHeap(vecPtr);
    heapPtr = HeapBuild(vecPtr, HeapMaxInt);
    size = VectorSize(vecPtr);

    if (heapPtr != NULL && IsHeapMaxInt(vecPtr) == PASS && 
    size == GetHeapSize(heapPtr))
    {
        result = PASS;
    }

    returned = HeapDestroy(&heapPtr);
    if (heapPtr != NULL || returned != vecPtr)
    {
        result = FAIL;
    }
    VectorDestroy(&vecPtr, FreeIntHeap);
    return result;
}

test HeapBuildGood2()
{
    test result = FAIL;
    Heap *heapPtr;
    Vector *vecPtr, *returned;
    vecPtr = VectorCreate(40, 10);
    heapPtr = HeapBuild(vecPtr, HeapMaxInt);

    if (heapPtr != NULL && GetHeapSize(heapPtr) == 0)
    {
        result = PASS;
    }

    returned = HeapDestroy(&heapPtr);
    if (heapPtr != NULL || returned != vecPtr)
    {
        result = FAIL;
    }
    VectorDestroy(&vecPtr, FreeIntHeap);
    return result;
}

test HeapBuildBad1()
{
    test result = FAIL;
    Heap *heapPtr;
    Vector *vecPtr = NULL;
    heapPtr = HeapBuild(vecPtr, HeapMaxInt);

    if (heapPtr == NULL)
    {
        result = PASS;
    }

    return result;
}

test HeapBuildBad2()
{
    test result = FAIL;
    Heap *heapPtr;
    Vector *vecPtr;
    vecPtr = VectorCreate(40, 10);
    heapPtr = HeapBuild(vecPtr, NULL);

    if (heapPtr == NULL)
    {
        result = PASS;
    }

    VectorDestroy(&vecPtr, FreeIntHeap);
    return result;
}

test HeapBuildBad3()
{
    test result = FAIL;
    Heap *heapPtr;
    Vector *vecPtr;
    vecPtr = VectorCreate(-1, 10);
    heapPtr = HeapBuild(vecPtr, HeapMaxInt);

    if (heapPtr == NULL)
    {
        result = PASS;
    }

    VectorDestroy(&vecPtr, FreeIntHeap);
    return result;
}

test HeapDestroyBad()
{
    test result = FAIL;
    Heap *heapPtr;
    Vector *vecPtr;
    vecPtr = VectorCreate(40, 10);
    heapPtr = HeapBuild(vecPtr, HeapMaxInt);
    HeapDestroy(&heapPtr);
    HeapDestroy(&heapPtr);
    HeapDestroy(NULL);

    if (heapPtr == NULL)
    {
        result = PASS;
    }

    VectorDestroy(&vecPtr, FreeIntHeap);
    return result;
}

test HeapInsertGood1()
{
    test result = FAIL;
    Heap *heapPtr;
    Vector *vecPtr;
    size_t size, oldSize;
    int insertLargest = 60, *newRoot, *largestPtr;
    vecPtr = VectorCreate(40, 10);
    FillVectorForHeap(vecPtr);
    heapPtr = HeapBuild(vecPtr, HeapMaxInt);
    oldSize = VectorSize(vecPtr);
    largestPtr = (int*)malloc(sizeof(int));
    *largestPtr = insertLargest;
    HeapInsert(heapPtr, (void*)largestPtr);
    size = VectorSize(vecPtr);
    VectorGet(vecPtr, 0, (void*)&newRoot);

    #ifdef DEBUG2
        printf("\nsize = %ld\nGetHeapSize = %ld\nnewRoot = %d\ninsertLargest = %d\noldSize = %ld\n", size, GetHeapSize(heapPtr), newRoot, insertLargest, oldSize);
    #endif

    if (IsHeapMaxInt(vecPtr) == PASS && size == GetHeapSize(heapPtr) &&
    *newRoot == insertLargest && size == (oldSize + 1))
    {
        result = PASS;
    }

    VectorDestroy(&vecPtr, FreeIntHeap);
    HeapDestroy(&heapPtr);
    return result;
}

test HeapInsertGood2()
{
    test result = FAIL;
    Heap *heapPtr;
    Vector *vecPtr;
    size_t size;
    int insertSmallest = 1, *newInsert, *smallestPtr;
    vecPtr = VectorCreate(40, 10);
    FillVectorForHeap(vecPtr);
    heapPtr = HeapBuild(vecPtr, HeapMaxInt);
    smallestPtr = (int*)malloc(sizeof(int));
    *smallestPtr = insertSmallest;
    HeapInsert(heapPtr, (void*)smallestPtr);
    size = VectorSize(vecPtr);
    VectorGet(vecPtr, size - 1, (void*)&newInsert);

    if (heapPtr != NULL && IsHeapMaxInt(vecPtr) == PASS && size == GetHeapSize(heapPtr) && *newInsert == insertSmallest)
    {
        result = PASS;
    }

    VectorDestroy(&vecPtr, FreeIntHeap);
    HeapDestroy(&heapPtr);
    return result;
}

test HeapInsertGood3()
{
    test result = FAIL;
    Heap *heapPtr;
    Vector *vecPtr;
    size_t size;
    int insertSmallest = 1, *newInsert, *smallestPtr;
    vecPtr = VectorCreate(40, 10);
    heapPtr = HeapBuild(vecPtr, HeapMaxInt);
    smallestPtr = (int*)malloc(sizeof(int));
    *smallestPtr = insertSmallest;
    HeapInsert(heapPtr, (void*)smallestPtr);
    size = VectorSize(vecPtr);
    VectorGet(vecPtr, size - 1, (void*)&newInsert);

    if (heapPtr != NULL && IsHeapMaxInt(vecPtr) == PASS && size == GetHeapSize(heapPtr) && *newInsert == insertSmallest)
    {
        result = PASS;
    }

    VectorDestroy(&vecPtr, FreeIntHeap);
    HeapDestroy(&heapPtr);
    return result;
}

test HeapInsertBad1()
{
    test result = FAIL;
    Heap *heapPtr;
    Vector *vecPtr;
    HeapResultCode status;
    int itemV = 5;
    void *item = &itemV;
    vecPtr = VectorCreate(10, 0);
    FillVectorForHeap(vecPtr);
    heapPtr = HeapBuild(vecPtr, HeapMaxInt);
    status = HeapInsert(heapPtr, item);

    if (status == HEAP_OVERFLOW)
    {
        result = PASS;
    }

    VectorDestroy(&vecPtr, FreeIntHeap);
    HeapDestroy(&heapPtr);
    return result;
}

test HeapInsertBad2()
{
    test result = FAIL;
    Heap *heapPtr = NULL;
    int itemV = 5;
    void *item = &itemV;
    HeapResultCode status;
    status = HeapInsert(heapPtr, item);

    if (status == HEAP_NOT_INITIALIZED)
    {
        result = PASS;
    }

    return result;
}

test HeapInsertBad3()
{
    test result = FAIL;
    Heap *heapPtr;
    Vector *vecPtr;
    HeapResultCode status;
    vecPtr = VectorCreate(40, 10);
    heapPtr = HeapBuild(vecPtr, HeapMaxInt);
    status = HeapInsert(heapPtr, NULL);

    if (status == HEAP_NOT_INITIALIZED)
    {
        result = PASS;
    }

    VectorDestroy(&vecPtr, FreeIntHeap);
    HeapDestroy(&heapPtr);
    return result;
}

test HeapInsertBad4()
{
    test result = FAIL;
    Heap *heapPtr;
    Vector *vecPtr;
    HeapResultCode status;
    int itemV = 5;
    void *item = &itemV;
    vecPtr = VectorCreate(10, 2058782939);
    FillVectorForHeap(vecPtr);
    heapPtr = HeapBuild(vecPtr, HeapMaxInt);
    status = HeapInsert(heapPtr, item);

    if (status == HEAP_REALLOCATION_FAILED)
    {
        result = PASS;
    }

    VectorDestroy(&vecPtr, FreeIntHeap);
    HeapDestroy(&heapPtr);
    return result;
}

test HeapInsertBad5()
{
    test result = FAIL;
    Heap *heapPtr;
    Vector *vecPtr;
    HeapResultCode status;
    int itemV = 5;
    void *item = &itemV;
    vecPtr = VectorCreate(1, -1);
    heapPtr = HeapBuild(vecPtr, HeapMaxInt);
    HeapInsert(heapPtr, item);
    status = HeapInsert(heapPtr, item);

    if (status == HEAP_REALLOCATION_FAILED)
    {
        result = PASS;
    }

    VectorDestroy(&vecPtr, NULL);
    HeapDestroy(&heapPtr);
    return result;
}

test HeapPeekGood()
{
    test result = FAIL;
    Heap *heapPtr;
    Vector *vecPtr;
    const void *max;
    vecPtr = VectorCreate(40, 10);
    FillVectorForHeap(vecPtr);
    heapPtr = HeapBuild(vecPtr, HeapMaxInt);
    max = HeapPeek(heapPtr);

    if (*((int*)max) == 50)
    {
        result = PASS;
    }

    VectorDestroy(&vecPtr, FreeIntHeap);
    HeapDestroy(&heapPtr);
    return result;
}

test HeapPeekBad1()
{
    test result = FAIL;
    Heap *heapPtr;
    Vector *vecPtr;
    const void *max;
    vecPtr = VectorCreate(40, 10);
    heapPtr = HeapBuild(vecPtr, HeapMaxInt);
    max = HeapPeek(heapPtr);

    if (max == NULL)
    {
        result = PASS;
    }

    VectorDestroy(&vecPtr, FreeIntHeap);
    HeapDestroy(&heapPtr);
    return result;
}

test HeapPeekBad2()
{
    test result = FAIL;
    const void *max;
    max = HeapPeek(NULL);

    if (max == NULL)
    {
        result = PASS;
    }

    return result;
}

test HeapExtractGood1()
{
    test result = FAIL;
    Heap *heapPtr;
    Vector *vecPtr;
    int *oldMax, *newMax;
    size_t oldSize, newSize;
    vecPtr = VectorCreate(40, 10);
    FillVectorForHeap(vecPtr);
    heapPtr = HeapBuild(vecPtr, HeapMaxInt);
    oldSize = VectorSize(vecPtr);
    oldMax = (int*)HeapExtract(heapPtr);
    newSize = VectorSize(vecPtr);
    newMax = (int*)HeapPeek(heapPtr);

    if (*oldMax == 50 && *newMax == 40 && newSize == (oldSize - 1))
    {
        result = PASS;
    }

    VectorDestroy(&vecPtr, FreeIntHeap);
    HeapDestroy(&heapPtr);
    return result;
}

test HeapExtractGood2()
{
    test result = FAIL;
    Heap *heapPtr;
    Vector *vecPtr;
    int *oldMax, *smallestPtr, insertSmallest = 50;
    size_t oldSize, newSize;
    vecPtr = VectorCreate(40, 10);
    heapPtr = HeapBuild(vecPtr, HeapMaxInt);
    smallestPtr = (int*)malloc(sizeof(int));
    *smallestPtr = insertSmallest;
    HeapInsert(heapPtr, (void*)smallestPtr);
    oldSize = VectorSize(vecPtr);
    oldMax = (int*)HeapExtract(heapPtr);
    newSize = VectorSize(vecPtr);

    if (*oldMax == insertSmallest && newSize == (oldSize - 1))
    {
        result = PASS;
    }

    VectorDestroy(&vecPtr, FreeIntHeap);
    HeapDestroy(&heapPtr);
    return result;
}

test HeapExtractBad1()
{
    test result = FAIL;
    Heap *heapPtr;
    Vector *vecPtr;
    void *max;
    vecPtr = VectorCreate(40, 10);
    heapPtr = HeapBuild(vecPtr, HeapMaxInt);
    max = HeapExtract(heapPtr);

    if (max == NULL)
    {
        result = PASS;
    }

    VectorDestroy(&vecPtr, FreeIntHeap);
    HeapDestroy(&heapPtr);
    return result;
}

test HeapExtractBad2()
{
    test result = FAIL;
    Heap *heapPtr = NULL;
    void *max;
    max = HeapExtract(heapPtr);

    if (max == NULL)
    {
        result = PASS;
    }

    return result;
}

test HeapSizeGood1()
{
    test result = FAIL;
    Heap *heapPtr;
    Vector *vecPtr;
    vecPtr = VectorCreate(40, 10);
    FillVectorForHeap(vecPtr);
    heapPtr = HeapBuild(vecPtr, HeapMaxInt);

    if (VectorSize(vecPtr) == HeapSize(heapPtr))
    {
        result = PASS;
    }

    VectorDestroy(&vecPtr, FreeIntHeap);
    HeapDestroy(&heapPtr);
    return result;
}

test HeapSizeGood2()
{
    test result = FAIL;
    Heap *heapPtr;
    Vector *vecPtr;
    vecPtr = VectorCreate(40, 10);
    heapPtr = HeapBuild(vecPtr, HeapMaxInt);

    if (HeapSize(heapPtr) == 0)
    {
        result = PASS;
    }

    VectorDestroy(&vecPtr, FreeIntHeap);
    HeapDestroy(&heapPtr);
    return result;
}

test HeapSizeBad()
{
    test result = FAIL;
    Heap *heapPtr = NULL;

    if (HeapSize(heapPtr) == 0)
    {
        result = PASS;
    }

    return result;
}

test HeapForEachGood1()
{
    test result = FAIL;
    Heap *heapPtr;
    Vector *vecPtr;
    size_t forHowMuch;
    vecPtr = VectorCreate(40, 10);
    FillVectorForHeap(vecPtr);
    heapPtr = HeapBuild(vecPtr, HeapMaxInt);
    printf("If prints Heap HeapForEachGood1 Passed!\n\n");
    forHowMuch = HeapForEach(heapPtr, PrintIntHeap, NULL);
    printf("\n\n");

    if (forHowMuch == HeapSize(heapPtr))
    {
        result = PASS;
    }

    VectorDestroy(&vecPtr, FreeIntHeap);
    HeapDestroy(&heapPtr);
    return result;
}

test HeapForEachGood2()
{
    test result = FAIL;
    Heap *heapPtr;
    Vector *vecPtr;
    size_t forHowMuch;
    int root = 50;
    vecPtr = VectorCreate(40, 10);
    FillVectorForHeap(vecPtr);
    heapPtr = HeapBuild(vecPtr, HeapMaxInt);
    forHowMuch = HeapForEach(heapPtr, SearchIntHeap, (void*)&root);

    if (forHowMuch == 0)
    {
        result = PASS;
    }

    VectorDestroy(&vecPtr, FreeIntHeap);
    HeapDestroy(&heapPtr);
    return result;
}

test HeapForEachBad1()
{
    test result = FAIL;
    Heap *heapPtr;
    Vector *vecPtr;
    size_t forHowMuch;
    vecPtr = VectorCreate(40, 10);
    FillVectorForHeap(vecPtr);
    heapPtr = HeapBuild(vecPtr, HeapMaxInt);
    forHowMuch = HeapForEach(heapPtr, NULL, NULL);

    if (forHowMuch == 0)
    {
        result = PASS;
    }

    VectorDestroy(&vecPtr, FreeIntHeap);
    HeapDestroy(&heapPtr);
    return result;
}

test HeapForEachBad2()
{
    test result = FAIL;
    size_t forHowMuch;
    int root = 50;
    forHowMuch = HeapForEach(NULL, SearchIntHeap, (void*)&root);

    if (forHowMuch == 0)
    {
        result = PASS;
    }

    return result;
}

void TestHeap(void)
{
    printf("HeapBuildGood1:");
    PrintTestResult(HeapBuildGood1());

    printf("HeapBuildGood2:");
    PrintTestResult(HeapBuildGood2());

    printf("HeapBuildBad1:");
    PrintTestResult(HeapBuildBad1());

    printf("HeapBuildBad2:");
    PrintTestResult(HeapBuildBad2());

    printf("HeapBuildBad3:");
    PrintTestResult(HeapBuildBad3());

    printf("HeapDestroyBad:");
    PrintTestResult(HeapDestroyBad());

    printf("HeapInsertGood1:");
    PrintTestResult(HeapInsertGood1());

    printf("HeapInsertGood2:");
    PrintTestResult(HeapInsertGood2());

    printf("HeapInsertGood3:");
    PrintTestResult(HeapInsertGood3());

    printf("HeapInsertBad1:");
    PrintTestResult(HeapInsertBad1());

    printf("HeapInsertBad2:");
    PrintTestResult(HeapInsertBad2());

    printf("HeapInsertBad3:");
    PrintTestResult(HeapInsertBad3());

    printf("HeapInsertBad4:");
    PrintTestResult(HeapInsertBad4());

    printf("HeapInsertBad5:");
    PrintTestResult(HeapInsertBad5());

    printf("HeapPeekGood:");
    PrintTestResult(HeapPeekGood());

    printf("HeapPeekBad1:");
    PrintTestResult(HeapPeekBad1());

    printf("HeapPeekBad2:");
    PrintTestResult(HeapPeekBad2());

    printf("HeapExtractGood1:");
    PrintTestResult(HeapExtractGood1());

    printf("HeapExtractGood2:");
    PrintTestResult(HeapExtractGood2());

    printf("HeapExtractBad1:");
    PrintTestResult(HeapExtractBad1());

    printf("HeapExtractBad2:");
    PrintTestResult(HeapExtractBad2());

    printf("HeapSizeGood1:");
    PrintTestResult(HeapSizeGood1());

    printf("HeapSizeGood2:");
    PrintTestResult(HeapSizeGood2());

    printf("HeapSizeBad:");
    PrintTestResult(HeapSizeBad());

    printf("HeapForEachGood1:");
    PrintTestResult(HeapForEachGood1());

    printf("HeapForEachGood2:");
    PrintTestResult(HeapForEachGood2());

    printf("HeapForEachBad1:");
    PrintTestResult(HeapForEachBad1());

    printf("HeapForEachBad2:");
    PrintTestResult(HeapForEachBad2());
}

/* Static Test Functions */

static void FillVectorForHeap(Vector *_ptr)
{
    register int i;
    int *valPtr;
    int arr[10] = {4, 5, 6, 40, 17, 30, 16, 40, 39, 50};
    for (i = 0; i < 10; ++i)
    {
        valPtr = (int*)malloc(sizeof(int));
        *valPtr = arr[i];
        VectorAppend(_ptr, (void*)valPtr);
    }
}

static void FreeIntHeap(void *_item)
{
    free((int*)_item);
}

static int PrintIntHeap(const void *_elem, void * _context)
{
    if (_elem == NULL)
    {
        return 0;
    }
    printf(" %d |", *((int*)_elem));
    return 1;
}

static int SearchIntHeap(const void *_elem, void * _context)
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

static int HeapMaxInt(const void *_candidate, const void *_context)
{
    if (*((int*)_candidate) > *((int*)_context))
    {
        return TRUE;
    }
    return FALSE;
}

static test IsHeapMaxInt(Vector *_ptr)
{
    test result = PASS;
    size_t i;
    void *item, *father;
    i = VectorSize(_ptr);
    #ifdef DEBUG
            printf("\ni = %d\nresult = %d\n", i, result);
    #endif
    if (i > 0)
    {
        --i;
    }
    for (; i > 0; --i)
    {
        #ifdef DEBUG
            printf("\ni = %d\nresult = %d\n", i, result);
        #endif 
        VectorGet(_ptr, i, &item);
        VectorGet(_ptr, FATHER(i), &father);
        if (*((int*)item) > *((int*)father))
        {
            result = FAIL;
            break;
        }
    }
    return result;
}
