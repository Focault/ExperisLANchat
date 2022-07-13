#ifndef __BSTREE_H__
#define __BSTREE_H__
#include <stddef.h>  /* size_t */

/** 
 *  @file BSTree.h
 *  @brief Generic Binary Search Tree
 *	
 *  @details  Implement a binary search tree ADT.
 *  The tree is implemented using a sentinel. 
 *  The first node (the root) in the tree is the sentinel left subtree.
 *
 */

typedef struct BSTree BSTree;
typedef void*  BSTreeItr;

/*  A comparison function that returns:
                        true (positive value) if _compared < _with
                        false (negative value) if _compared > _with
                        and 0 if duplicate. */
typedef int (*LessComparator)(void* _compared, void* _with);

/** 
 * @brief Predicate function returns a none zero value if predicate holds for element
 *
 * @param _element : element to test
 * @param _context : context to be used
 * @return none zero if predicate holds
 */
typedef int (*PredicateFunction)(void * _element, void* _context);

/** 
 * @brief Action function to operate on an element
 *
 * @param _element : element to test
 * @param _context : context to be used
 * @return zero if action fails
 */
typedef int (*ActionFunction)(void* _element, void* _context);

typedef enum
{
    BSTREE_TRAVERSAL_PREORDER =0,
    BSTREE_TRAVERSAL_INORDER,
    BSTREE_TRAVERSAL_POSTORDER
} BSTreeTraversalMode;

/** 
 * @brief Create a binary search tree
 * Creates an empty binary search tree with sentinel
 * 
 * @param _less: A comparison function that returns:
                        true (positive value) if _compared < _with
                        false (negative value) if _compared > _with
                        and 0 if duplicate.
 * @return a pointer to the newly created tree.
 * @retval NULL on failure due to allocation failure or NULL function pointer given
 */
BSTree* BSTreeCreate(LessComparator _less);

/** 
 * @brief Destroy tree
 * Destroys the tree, freeing the memory.
 * If supplied with non-NULL destroyer function, frees items in tree.
 * Average time complexity: O(n) 
 *
 * @param _tree : A previously created Tree ADT returned via BSTreeCreate
 * @param _destroyer : A function to destroy the data in the tree (may be NULL if unnecessary)
 */
void BSTreeDestroy(BSTree** _tree, void (*_destroyer)(void*));

/** 
 * @brief Add an element to tree if it'snot already there
 * Insert element to binary tree, using the tree's comparison function
 * Average time complexity O(log n).
 * Worst: O(n)
 *
 * @param _tree : A previously created Tree ADT returned via BSTreeCreate
 * @param _item: An item to add to the tree
 * @return iterator pointing to the item added or iterator to end on failure (duplicate or NULL as one of the parameters)
 * @warning don't allow duplicates
 * @warning If _tree is NULL returns NULL
 */
BSTreeItr BSTreeInsert(BSTree* _tree, void* _item);

/** 
 * @brief Search the first element for which the given predicate returns 0
 * iterating using in-order manner over elements
 * O(k * n) where O(k) is the time complexity of the predicate
 *
 * @param _tree : A previously created Tree ADT returned via BSTreeCreate
 * @param _predicate : Predicate function
 * @param _context : Predicate parameters (optional)
 * @return an iterator pointing to the first data found, to end of tree if not found or NULL on NULL input
 */
BSTreeItr BSTreeFindFirst(const BSTree* _tree, PredicateFunction _predicate, void* _context);

/** 
 * @brief Get an in-order itertator to the tree's begin 
 *
 * @param _tree : tree to create iterator from
 * @return an iterator pointing at the tree's begin or end if tree is empty 
 * @warning returns NULL if input is NULL
 */
BSTreeItr BSTreeItrBegin(const BSTree* _tree);

/** 
 * @brief Get itertator to the tree's end (in order)
 *
 * @param _tree : tree to create iterator from
 * @return an iterator pointing at the tree's end (the sentinel)
 * @warning returns NULL if input is NULL
 */
BSTreeItr BSTreeItrEnd(const BSTree* _tree);

/** 
 * @brief Compare two iterators
 *
 * @param _a, _b : Iterators to compare
 * @return : 0 if are not the same (or if input NULL)
 *           any noze zero value if iterators are the same
 */
int BSTreeItrEquals(BSTreeItr _a, BSTreeItr _b);

/** 
 * @brief Get itertator to the next element from current iterator
 *
 * @params _it : A tree iterator
 * @returns an iterator pointing at the next element after _it or end iterator.
 * @warning returns NULL if input is NULL
 */
BSTreeItr BSTreeItrNext(BSTreeItr _it);

/** 
 * @brief Get itertator to the previous element from current iterator
 *
 * @param _it : A tree iterator
 * @return an iterator pointing at the previous element
 * or to the beginning if _it points to the beginning
 * @warning returns NULL if input is NULL
 */
BSTreeItr BSTreeItrPrev(BSTreeItr _it);

/** 
 * @brief Removes element from tree
 * @details Remove element pointed to by _it and rearranges the tree so that it maintains binary search tree arrangement
 * Average/Worst time complexity O(1).
 *
 * @param _it : A tree iterator
 * @return removed item or null on failure
 */
void* BSTreeItrRemove(BSTreeItr _it);

/** 
 * @brief Get element pointed to by iterator from tree
 * Average/Worst time complexity O(1).
 *
 * @param _it : A tree iterator
 * @return the item the iterator points at or null if _it is at end
 */
void* BSTreeItrGet(BSTreeItr _it);

/** 
 * @brief Performs an action function on every element in tree, by given traversal mode
 * @details iteration will end on the first element for which action returns a zero
 * or on reaching end of the conrainer
 *
 * @param _tree : tree to iterate over
 * @param _mode : Traversal mode - TREE_TRAVERSAL_PREORDER, TREE_TRAVERSAL_INORDER or TREE_TRAVERSAL_POSTORDER
 * @param _action : Action function to call for each element
 * @param _context : Parameters for the function
 * @return Iterator to end or an iterator to the specific element where action returned none zero value
 * @warning if _tree or _action are NULL or if _mode is illigal returns NULL
 */
BSTreeItr BSTreeForEach(const BSTree* _tree, BSTreeTraversalMode _mode, ActionFunction _action, void* _context);

#endif /* __BSTREE_H__ */