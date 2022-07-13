#include <stdlib.h> /* free */
#include "internal.h"

void Push(Node *_insert, Node *_before)
{
	_insert->m_prev = _before->m_prev;
	_insert->m_next = _before;
	_before->m_prev->m_next = _insert;
	_before->m_prev = _insert;
}

void Pop(Node *_popped)
{
	_popped->m_next->m_prev = _popped->m_prev;
	_popped->m_prev->m_next = _popped->m_next;
	free(_popped);
}