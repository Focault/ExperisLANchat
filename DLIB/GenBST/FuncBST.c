#include <stdlib.h> /* malloc, free */
#include "BSTree.h"
#define AM_I_LEFT_SON(node) ((node)->m_father->m_left == (node))
#define AM_I_RIGHT_SON(node) ((node)->m_father->m_right == (node))
#define AM_I_SENTINEL(node) ((node)->m_father == (node))
#define IS_TWO_SONS(node) ((node)->m_left != NULL && (node)->m_right != NULL)

typedef struct BSTNode BSTNode;

struct BSTNode
{
	void *m_data;
	BSTNode *m_left;
	BSTNode *m_right;
	BSTNode *m_father;
};

struct BSTree
{
	BSTNode m_sentinel;
	LessComparator m_compare;
};

static BSTreeItr Insert(BSTNode **_place, void *_data, BSTNode *_father);
static void DestroyTree(BSTNode *_root, void (*_destroyer)(void*));
static BSTreeItr InsertRec(BSTree *_tree, BSTNode *_read, void *_item);
static void RemoveItem(BSTNode *_it);
static BSTreeItr ActInOrder(BSTNode *_root, ActionFunction _action, void* _context);

BSTree* BSTreeCreate(LessComparator _less)
{
	BSTree *treeP = NULL;
	if (_less != NULL && ((treeP = (BSTree*)malloc(sizeof(BSTree))) != NULL))
	{
		treeP->m_sentinel.m_data = NULL;
		treeP->m_sentinel.m_father = &treeP->m_sentinel;
		treeP->m_sentinel.m_left = NULL;
		treeP->m_sentinel.m_right = NULL;
		treeP->m_compare = _less;
	}
	return treeP;
}

void BSTreeDestroy(BSTree** _tree, void (*_destroyer)(void*))
{
	if (_tree != NULL && *_tree != NULL)
	{
		DestroyTree((*_tree)->m_sentinel.m_left, _destroyer);
		free(*_tree);
		*_tree = NULL;
	}
}

BSTreeItr BSTreeInsert(BSTree* _tree, void* _item)
{
	BSTreeItr inserted;
	if (_tree == NULL)
	{
		return NULL;
	}
	if (_item == NULL)
	{
		return (BSTreeItr*)&_tree->m_sentinel;
	}
	if (_tree->m_sentinel.m_left == NULL)
	{
		inserted = Insert(&(_tree->m_sentinel.m_left), _item, &_tree->m_sentinel);
		return inserted == NULL ? (BSTreeItr*)&_tree->m_sentinel : inserted;
	}
	inserted = InsertRec(_tree, _tree->m_sentinel.m_left, _item);
	return inserted == NULL ? (BSTreeItr*)&_tree->m_sentinel : inserted;
}

BSTreeItr BSTreeItrBegin(const BSTree* _tree)
{
	BSTreeItr *begin;
	if (_tree == NULL)
	{
		return NULL;
	}
	for (begin = (BSTreeItr*)&_tree->m_sentinel; ((BSTNode*)begin)->m_left != NULL; begin = (BSTreeItr*)((BSTNode*)begin)->m_left);
	return begin;
}

BSTreeItr BSTreeItrEnd(const BSTree* _tree)
{
	if (_tree == NULL)
	{
		return NULL;
	}
	return (BSTreeItr*)&_tree->m_sentinel;
}

int BSTreeItrEquals(BSTreeItr _a, BSTreeItr _b)
{
	if (_a == NULL || _b == NULL)
	{
		return 0;
	}
	return ((BSTNode*)_a == (BSTNode*)_b);
}

BSTreeItr BSTreeItrNext(BSTreeItr _it)
{
	BSTNode *next;
	if (_it == NULL)
	{
		return NULL;
	}
	if (AM_I_SENTINEL((BSTNode*)_it))
	{
		return _it;
	}
	if ((next = ((BSTNode*)_it)->m_right) != NULL)
	{
		for (; next->m_left != NULL; next = next->m_left);
		return (BSTreeItr*)next;
	}
	next = (BSTNode*)_it;
	while (1)
	{
		if (AM_I_LEFT_SON(next))
		{
			return (BSTreeItr*)next->m_father;
		}
		next = next->m_father;
	}
}

BSTreeItr BSTreeItrPrev(BSTreeItr _it)
{
	BSTNode *prev;
	if (_it == NULL)
	{
		return NULL;
	}
	if ((prev = ((BSTNode*)_it)->m_left) != NULL)
	{
		if (AM_I_SENTINEL((BSTNode*)_it))
		{
			for (; prev->m_right != NULL; prev = prev->m_right);
		}
		return (BSTreeItr*)prev;
	}
	prev = (BSTNode*)_it;
	while (1)
	{
		if (AM_I_RIGHT_SON(prev))
		{
			return (BSTreeItr*)prev->m_father;
		}
		if (AM_I_SENTINEL(prev))
		{
			return _it;
		}
		prev = prev->m_father;
	}
}

void* BSTreeItrRemove(BSTreeItr _it)
{
	void* data;
	BSTNode *remove = (BSTNode*)_it;
	if (_it == NULL || AM_I_SENTINEL((BSTNode*)_it))
	{
		return NULL;
	}
	data = remove->m_data;
	if (IS_TWO_SONS(remove))
	{
		remove = (BSTNode*)BSTreeItrNext(_it);
		((BSTNode*)_it)->m_data = remove->m_data;
	}
	RemoveItem(remove);
	free(remove);
	return data;
}

void* BSTreeItrGet(BSTreeItr _it)
{
	if (_it == NULL || AM_I_SENTINEL((BSTNode*)_it))
	{
		return NULL;
	}
	return ((BSTNode*)_it)->m_data;
}

BSTreeItr BSTreeFindFirst(const BSTree* _tree, PredicateFunction _predicate, void* _context)
{
	BSTreeItr *read;
	if (_tree == NULL || _predicate == NULL)
	{
		return NULL;
	}
	for (read = BSTreeItrBegin(_tree); (BSTNode*)read != &_tree->m_sentinel; read = BSTreeItrNext(read))
	{
		if (_predicate(((BSTNode*)read)->m_data, _context))
		{
			break;
		}
	}
	return read;
}

BSTreeItr BSTreeForEach(const BSTree* _tree, BSTreeTraversalMode _mode, ActionFunction _action, void* _context)
{
	BSTreeItr *result;
	if (_tree == NULL || _action == NULL)
	{
		return NULL;
	}
	if ((result = (BSTreeItr*)_tree->m_sentinel.m_left) == NULL)
	{
		return (BSTreeItr*)&_tree->m_sentinel;
	}
	switch (_mode)
	{
	case BSTREE_TRAVERSAL_PREORDER:
		if ((!_action(((BSTNode*)result)->m_data, _context)) ||
		   ((result = ActInOrder(((BSTNode*)result)->m_left, _action, _context)) != NULL) ||
		   ((result = ActInOrder(_tree->m_sentinel.m_left->m_right, _action, _context)) != NULL))
		{
			return result;
		}
		return (BSTreeItr*)&_tree->m_sentinel;
	case BSTREE_TRAVERSAL_INORDER:
		if ((result = ActInOrder(_tree->m_sentinel.m_left, _action, _context)) != NULL)
		{
			return result;
		}
		return (BSTreeItr*)&_tree->m_sentinel;
	case BSTREE_TRAVERSAL_POSTORDER:
		if (((result = ActInOrder(((BSTNode*)result)->m_left, _action, _context)) != NULL) ||
		    ((result = ActInOrder(_tree->m_sentinel.m_left->m_right, _action, _context)) != NULL) ||
		    (!_action(_tree->m_sentinel.m_left->m_data, _context)))
		{
			return result;
		}
		return (BSTreeItr*)&_tree->m_sentinel;
	}
	return NULL;
}

/* Main Static Functions */ 

static BSTreeItr ActInOrder(BSTNode *_root, ActionFunction _action, void* _context)
{
	BSTreeItr *result = NULL;
	if (_root == NULL)
	{
		return NULL;
	}
	if ((result = ActInOrder(_root->m_left, _action, _context)) != NULL)
	{
		return result;
	}
	if (!_action(_root->m_data, _context))
	{
		return _root;
	}
	result = ActInOrder(_root->m_right, _action, _context);
	return result;
}

static void RemoveItem(BSTNode *_it)
{
	BSTNode *son;
	if ((son = _it->m_left) == NULL)
	{
		son = _it->m_right;
	}
	if (son != NULL)
	{
		son->m_father = _it->m_father;
	}
	if (AM_I_LEFT_SON(_it))
	{
		_it->m_father->m_left = son;
		return;
	}
	_it->m_father->m_right = son;
}

static BSTreeItr Insert(BSTNode **_place, void *_data, BSTNode *_father)
{
	BSTNode *ptr;
	if ((ptr = (BSTNode*)malloc(sizeof(BSTNode))) != NULL)
	{
		ptr->m_data = _data;
		ptr->m_left = NULL;
		ptr->m_right = NULL;
		ptr->m_father = _father;
		*_place = ptr;
	}
	return (BSTreeItr*)ptr;
}

static void DestroyTree(BSTNode *_root, void (*_destroyer)(void*))
{
	if (_root == NULL)
	{
		return;
	}
	DestroyTree(_root->m_left, _destroyer);
	DestroyTree(_root->m_right, _destroyer);
	if (_destroyer != NULL)
	{
		_destroyer(_root->m_data);
	}
	free(_root);
}

static BSTreeItr InsertRec(BSTree *_tree, BSTNode *_read, void *_item)
{
	int result = _tree->m_compare(_item, _read->m_data);
	if (result > 0)
	{
		if (_read->m_left != NULL)
		{
			return InsertRec(_tree, _read->m_left, _item);
		}
		return Insert(&(_read->m_left), _item, _read);	
	}
	if (result < 0)
	{
		if (_read->m_right != NULL)
		{
			return InsertRec(_tree, _read->m_right, _item);
		}
		return Insert(&(_read->m_right), _item, _read);	
	}
	return NULL;
}