#include <stdlib.h> /* size_t, NULL, malloc, calloc, free */
#include "GenQueue.h"
#define CYCLIC(idx, size) ((idx) + 1) % (size)

struct Queue
{
	void **m_que;
	size_t m_size;
	size_t m_head;
	size_t m_tail;
	size_t m_nItems;
};

Queue *QueueCreate(size_t _size)
{
	Queue *ptr;
	if (!_size)
	{
		return NULL;
	}
	if ((ptr = (Queue*)calloc(1, sizeof(Queue))) == NULL)
	{
		return NULL;
	}
	if ((ptr->m_que = (void**)malloc(sizeof(void*) * _size)) == NULL)
	{
		free(ptr);
		return NULL;
	}
	ptr->m_size = _size;
	return ptr;
}

void QueueDestroy(Queue** _queue, DestroyItem _itemDestroy)
{
	register size_t i, j;
	if (_queue != NULL && *_queue != NULL)
	{
		if (_itemDestroy != NULL)
		{
			for (i = (*_queue)->m_head, j = 0; j < (*_queue)->m_nItems; ++j, i = CYCLIC(i, (*_queue)->m_size))
			{
				_itemDestroy((*_queue)->m_que[i]);
			}
		}
		free((*_queue)->m_que);
		free(*_queue);
		*_queue = NULL;
	}
}

QueueResult QueueInsert(Queue* _queue,void* _item)
{
	if (_queue == NULL)
	{
		return QUEUE_UNINITIALIZED_ERROR;
	}
	if (_item == NULL)
	{
		return QUEUE_DATA_UNINITIALIZED_ERROR;
	}
	if (_queue->m_nItems == _queue->m_size)
	{
		return QUEUE_OVERFLOW_ERROR;
	}
	
	_queue->m_que[_queue->m_tail] = _item;
	_queue->m_tail = CYCLIC(_queue->m_tail, _queue->m_size);	
	++_queue->m_nItems;
	return QUEUE_SUCCESS;
}

QueueResult QueueRemove(Queue* _queue,void** _item)
{
	if (_queue == NULL)
	{
		return QUEUE_UNINITIALIZED_ERROR;
	}
	if (_item == NULL)
	{
		return QUEUE_DATA_UNINITIALIZED_ERROR;
	}
	if (_queue->m_nItems == 0)
	{
		return QUEUE_DATA_NOT_FOUND_ERROR;
	}
	
	*_item = _queue->m_que[_queue->m_head];
	_queue->m_head = CYCLIC(_queue->m_head, _queue->m_size);
	--_queue->m_nItems;
	return QUEUE_SUCCESS;
}

size_t QueueIsEmpty(Queue* _queue)
{
	if (_queue == NULL)
	{
		return PTR_NULL;
	}
	if (_queue->m_nItems == 0)
	{
		return EMPTY;
	}
	return NOT_EMPTY;
}

size_t QueueForEach(Queue* _queue, ActionFunction _action, void* _context)
{
	register size_t i, j;
	if (_queue == NULL || _action == NULL)
	{
		return 0;
	}

	for(i = 0, j = _queue->m_head; i < _queue->m_nItems; ++i, j = CYCLIC(j, _queue->m_nItems))
	{
		if(_action(_queue->m_que[j], _context) == 0)
		{
			break;
		}	
	}
	return i;
}

/* Get Functions For Tests */

size_t GetQueueSize(Queue *_queue)
{
	return _queue->m_size;
}

size_t GetHead(Queue *_queue)
{
	return _queue->m_head;
}

size_t GetTail(Queue *_queue)
{
	return _queue->m_tail;
}

size_t GetNItems(Queue *_queue)
{
	return _queue->m_nItems;
}