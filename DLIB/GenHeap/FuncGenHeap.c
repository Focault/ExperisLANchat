#include <stdlib.h> /* size_t, malloc, free */
#include "GenVector.h"
#include "GenHeap.h"
#define FATHER(idx) ((idx)-1)/2
#define LEFT_SON(idx) ((idx)*2)+1
#define RIGHT_SON(idx) ((idx)*2)+2

#define SWAP_VECTOR(heap, idx1, idx2) \
{ \
	void *temp1, *temp2; \
    VectorGet(heap->m_vec, idx1, &temp1); \
    VectorGet(heap->m_vec, idx2, &temp2); \
    VectorSet(heap->m_vec, idx1, temp2); \
    VectorSet(heap->m_vec, idx2, temp1); \
}

struct Heap 
{
    Vector *m_vec;
    size_t m_heapSize;
    Qriteria m_qriteria;
};

static void Heapify(Heap *_heap, size_t _index);
static void WithdrawValues(Heap *_heap, size_t _index, void **_leftSon, void **_rightSon);
static void HeapifyAll(Heap *_heap);
static void MiniFy(Heap *_heap, void *_father, size_t _fatherIdx, size_t _sonIdx);
static void BubbleUp(Heap *_heap, void *_inserted);

Heap* HeapBuild(Vector* _vector, Qriteria _pfQriteria)
{
    Heap *ptr;
    if (_vector == NULL || _pfQriteria == NULL)
    {
        return NULL;
    }
    if ((ptr = (Heap*)malloc(sizeof(Heap))) == NULL)
    {
        return NULL;
    } 
    ptr->m_heapSize = VectorSize(_vector);
    ptr->m_vec = _vector;
    ptr->m_qriteria = _pfQriteria;
    if (ptr->m_heapSize > 1)
    {
        HeapifyAll(ptr);
    }
    return ptr;
}

Vector* HeapDestroy(Heap** _heap)
{
    Vector *vec = NULL;
    if (_heap != NULL && *_heap != NULL)
    {
        vec = (*_heap)->m_vec;
        free(*_heap);
        *_heap = NULL;
    }
    return vec;
}

HeapResultCode HeapInsert(Heap* _heap, void* _element)
{
    VectorResult status;
    if (_heap == NULL || _element == NULL)
    {
        return HEAP_NOT_INITIALIZED;
    }
    if ((status = VectorAppend(_heap->m_vec, _element)) != VECTOR_SUCCESS)
    {
        switch (status)
        {
        case VECTOR_OVERFLOW:
            return HEAP_OVERFLOW;
        case VECTOR_ALLOCATION_ERROR:
            return HEAP_REALLOCATION_FAILED;
        default:
            return HEAP_NOT_INITIALIZED;
        }
    }
    ++_heap->m_heapSize;
    BubbleUp(_heap, _element);
    return HEAP_SUCCESS;
}

const void* HeapPeek(const Heap* _heap)
{
    void *peek;
    if (_heap == NULL || _heap->m_heapSize == 0)
    {
        return NULL;
    }
    VectorGet(_heap->m_vec, 0, &peek);
    return peek;
}

void* HeapExtract(Heap* _heap)
{
    void *extract;
    if (_heap == NULL || _heap->m_heapSize == 0)
    {
        return NULL;
    }
    if (_heap->m_heapSize > 1)
    {
        SWAP_VECTOR(_heap, 0, (_heap->m_heapSize-1));
    }
    VectorRemove(_heap->m_vec, &extract);
    --_heap->m_heapSize;
    if (_heap->m_heapSize > 1)
    {
	    Heapify(_heap, 0);
    }
    return extract;
}

size_t HeapSize(const Heap* _heap)
{
    if (_heap == NULL)
    {
        return 0;
    }
    return _heap->m_heapSize;
}

size_t HeapForEach(const Heap* _heap, ActionFunction _act, void* _context)
{
    register size_t i;
	void *elem;
	if (_heap == NULL || _act == NULL)
	{
		return 0;
	}

	for(i = 0; i < _heap->m_heapSize; ++i)
	{
		VectorGet(_heap->m_vec, i, &elem);
		if(_act(elem, _context) == 0)
		{
			break;
		}	
	}
	return i;
}

/* Static Functions */

static void WithdrawValues(Heap *_heap, size_t _index, void **_leftSon, void **_rightSon)
{
    if (LEFT_SON(_index) >= _heap->m_heapSize || VectorGet(_heap->m_vec, LEFT_SON(_index), _leftSon) == VECTOR_INDEX_OUT_OF_BOUNDS_ERROR)
    {
        *_leftSon = NULL;
    }
    if (RIGHT_SON(_index) >= _heap->m_heapSize ||VectorGet(_heap->m_vec, RIGHT_SON(_index), _rightSon) == VECTOR_INDEX_OUT_OF_BOUNDS_ERROR)
    {
        *_rightSon = NULL;
    }
}

static void Heapify(Heap *_heap, size_t _index)
{
    void *leftSon, *rightSon, *father;
    if (LEFT_SON(_index) < _heap->m_heapSize)
    {
        VectorGet(_heap->m_vec, _index, &father);
        WithdrawValues(_heap, _index, &leftSon, &rightSon);
        if ((rightSon == NULL || _heap->m_qriteria(leftSon, rightSon)) && _heap->m_qriteria(leftSon, father))
        {
            MiniFy(_heap, father, _index, LEFT_SON(_index));
            return;
        }
        if (rightSon != NULL && _heap->m_qriteria(rightSon, father))
        {
            MiniFy(_heap, father, _index, RIGHT_SON(_index));
        }
    }
}

static void MiniFy(Heap *_heap, void *_father, size_t _fatherIdx, size_t _sonIdx)
{
    void *sonLeftSon, *sonRightSon;
    SWAP_VECTOR(_heap, _fatherIdx, _sonIdx);
    WithdrawValues(_heap, _sonIdx, &sonLeftSon, &sonRightSon);
    if ((sonLeftSon != NULL && _heap->m_qriteria(sonLeftSon, _father)) || (sonRightSon != NULL && _heap->m_qriteria(sonRightSon, _father)))
    {
        Heapify(_heap, _sonIdx);
    }
}

static void HeapifyAll(Heap *_heap)
{
    register size_t i;
    for (i = FATHER(_heap->m_heapSize - 1); i > 0; --i)
    {
        Heapify(_heap, i);
    }
    Heapify(_heap, 0);
}

static void BubbleUp(Heap *_heap, void *_inserted)
{
    size_t idxFather;
    void *father;
    idxFather = FATHER(_heap->m_heapSize-1);
    VectorGet(_heap->m_vec, idxFather, &father);
    while (_heap->m_qriteria(_inserted, father))
    {
        Heapify(_heap, idxFather);
        if (idxFather == 0)
        {
            return;
        }
        VectorGet(_heap->m_vec, idxFather, &_inserted);
        idxFather = FATHER(idxFather);
        VectorGet(_heap->m_vec, idxFather, &father);    
    }
}

/* Test Functions */

size_t GetHeapSize(Heap *_heap)
{
    return _heap->m_heapSize;
}
