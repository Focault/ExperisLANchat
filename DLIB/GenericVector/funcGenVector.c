#include <stdlib.h> /* size_t, malloc, realloc, free, NULL */
#include "GenVector.h"

struct Vector
{
	void **m_items;
	size_t m_originalSize;
	size_t m_size;
	size_t m_nItems;
	size_t m_blockSize;
};

Vector *VectorCreate(size_t _initialCapacity, size_t _blockSize)
{
	Vector *ptr;
	if (_initialCapacity == 0 && _blockSize == 0)
	{
		return NULL;
	}
	
	if ((ptr = (Vector*)malloc(sizeof(Vector))) == NULL)
	{
		return NULL;
	}
	if ((ptr->m_items = malloc(sizeof(void*)*_initialCapacity)) == NULL)
	{
		free(ptr);
		return NULL;
	}

	ptr->m_originalSize = _initialCapacity;
	ptr->m_size = _initialCapacity;
	ptr->m_nItems = 0;
	ptr->m_blockSize = _blockSize;	
	return ptr;
}

void VectorDestroy(Vector** _vector, void (*_elementDestroy)(void* _item))
{
	register int i;
	if (_vector != NULL && *_vector != NULL)
	{
		if (_elementDestroy != NULL)
		{
			for (i = 0; i < (*_vector)->m_nItems; ++i)
			{
				_elementDestroy((*_vector)->m_items[i]);
			}
		}
		free((*_vector)->m_items);
		free(*_vector);
		*_vector = NULL;
	}
}

VectorResult VectorAppend(Vector* _vector, void* _item)
{
	void **ptr;
	if (_vector == NULL || _item == NULL)
	{
		return VECTOR_UNITIALIZED_ERROR;
	}
	
	if (_vector->m_nItems == _vector->m_size)
	{
		if (_vector->m_blockSize == 0)
		{
			return VECTOR_OVERFLOW;
		}
		if (!(_vector->m_size + _vector->m_blockSize) || 
			(ptr = realloc(_vector->m_items, sizeof(void*)*(_vector->m_size + _vector->m_blockSize))) == NULL)
		{
			return VECTOR_ALLOCATION_ERROR;
		}
		_vector->m_items = ptr;
		_vector->m_size += _vector->m_blockSize;
	}
	_vector->m_items[_vector->m_nItems] = _item;
	++_vector->m_nItems;
	return VECTOR_SUCCESS;
}

VectorResult VectorRemove(Vector* _vector, void** _pValue)
{
	void **ptr;
	if (_vector == NULL || _pValue == NULL)
	{
		return VECTOR_UNITIALIZED_ERROR;
	}
	if (_vector->m_nItems == 0)
	{
		return VECTOR_UNDERFLOW;
	}
	
	--_vector->m_nItems;
	*_pValue = _vector->m_items[_vector->m_nItems];
	/* realloc if needed */
	if (_vector->m_size > _vector->m_originalSize && (_vector->m_size - _vector->m_nItems) >= 2*(_vector->m_blockSize))
	{
		ptr = realloc(_vector->m_items, sizeof(void*)*(_vector->m_size - _vector->m_blockSize));
		if (ptr != NULL)
		{
			_vector->m_items = ptr;
			_vector->m_size -= _vector->m_blockSize;
		}
	}
	return VECTOR_SUCCESS;
}

VectorResult VectorGet(const Vector* _vector, size_t _index, void** _pValue)
{
	if (_vector == NULL || _pValue == NULL)
	{
		return VECTOR_UNITIALIZED_ERROR;
	}
	if (_index >= _vector->m_nItems)
	{
		return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;
	}
	
	*_pValue = _vector->m_items[_index];
	return VECTOR_SUCCESS;
}

VectorResult VectorSet(Vector* _vector, size_t _index, void*  _value)
{
	if (_vector == NULL || _value == NULL)
	{
		return VECTOR_UNITIALIZED_ERROR;
	}
	if (_index >= _vector->m_nItems)
	{
		return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;
	}
	
	_vector->m_items[_index] = _value;
	return VECTOR_SUCCESS;
}

size_t VectorSize(const Vector* _vector)
{
	if (_vector == NULL)
	{
		return 0;
	}
	
	return _vector->m_nItems;
}

size_t VectorCapacity(const Vector* _vector)
{
	if (_vector == NULL)
	{
		return 0;
	}
	
	return _vector->m_size;
}

size_t VectorForEach(const Vector* _vector, VectorElementAction _action, void* _context)
{
	register size_t i;
	if (_vector == NULL || _action == NULL)
	{
		return 0;
	}

	for(i = 0; i < _vector->m_nItems; ++i)
	{
		if(_action(_vector->m_items[i], i, _context) == 0)
		{
			break;
		}	
	}
	return i;
}

/* functions for tests */

size_t GetNumOfElements(Vector *_ptr)
{
	return _ptr->m_nItems;
}

size_t GetSize(Vector *_ptr)
{
	return _ptr->m_size;
}

size_t GetOriginalSize(Vector *_ptr)
{
	return _ptr->m_originalSize;
}

size_t GetBlockSize(Vector *_ptr)
{
	return _ptr->m_blockSize;
}

void **GetArrayPtr(Vector *_ptr)
{
	return _ptr->m_items;
}