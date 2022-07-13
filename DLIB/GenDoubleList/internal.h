#ifndef __INTERNAL_H__
#define __INTERNAL_H__
#include "GenListAPI.h"

typedef struct Node Node;

struct Node
{
	void *m_data;
	Node *m_next;
	Node *m_prev;
};

struct List
{
	Node m_head;
	Node m_tail;
};

void Push(Node *_insert, Node *_before);
void Pop(Node *_popped);

#endif /* __INTERNAL_H__ */