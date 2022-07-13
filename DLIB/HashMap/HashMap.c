#include <stdlib.h> /* malloc, calloc, free, size_t */
#include <math.h> /* sqrt */
#include "HashMapAPI.h"
#include "GenListAPI.h"
#include "ListFunctions.h"
#include "ListItr.h"
#define TRUE 1
#define FALSE 0

struct HashMap
{
    List **m_hash;
    HashFunction m_hashFunction;
    EqualityFunction m_equalityFunction;
    size_t m_capacity;
    size_t m_numOfItems;
    size_t m_maxChainLength;
    size_t m_numberOfBuckets;
};

typedef struct Pair
{
    void *m_key;
    void *m_value;
} Pair;

typedef struct Context
{
    const void *m_key;
    EqualityFunction m_equalityFunction;
    KeyValueActionFunction m_action;
    KeyDestroy m_keyDestroy;
    ValDestroy m_valDestroy;
    void *m_actionFunctionContext;
    void *m_context;
} Context;

static int IsPrime(size_t _n);
static size_t CalcCapacity(size_t _userSize);
static int IsKeyFound(const List *_list, const void* _key, EqualityFunction _equalFunc, ListItr *_found);
static Map_Result Insert(HashMap *_hash, List *_list, const void* _key, const void* _value);
static int SearchFunc(void * _element, void* _context);
static Map_Result Remove(HashMap *_hash, ListItr _remove, void** _pKey, void** _pValue);
static Map_Result CreateListIfNeededAndInsert(HashMap* _map, const void* _key, const void* _value, int _rehash);
static Map_Result TransferFromListToNewHash(HashMap* _map, List *_list);
static int BlackHashForEach(void* _element, void* _context);
static int BlackDestroy(void* _element, void* _context);
static void MiniStroy(HashMap* _map, void *_context);

HashMap* HashMap_Create(size_t _capacity, HashFunction _hashFunc, EqualityFunction _keysEqualFunc)
{
    HashMap *hash;
    if (!_capacity || _hashFunc == NULL || _keysEqualFunc == NULL)
    {
        return NULL;
    }
    if ((hash = (HashMap*)calloc(1, sizeof(HashMap))) != NULL)
    {
        hash->m_equalityFunction = _keysEqualFunc;
        hash->m_hashFunction = _hashFunc;
        hash->m_capacity = CalcCapacity(_capacity);
        if (!hash->m_capacity || (hash->m_hash = (List**)calloc(hash->m_capacity, sizeof(List*))) == NULL)
        {
            free(hash);
            return NULL;
        }
    }
    return hash;
}

void HashMap_Destroy(HashMap** _map, KeyDestroy _keyDestroy, ValDestroy _valDestroy)
{
    Context context;
    if (_map != NULL && *_map != NULL)
    {
        context.m_valDestroy = _valDestroy;
        context.m_keyDestroy = _keyDestroy;
        MiniStroy(*_map, (void*)&context);
        free(*_map);
        *_map = NULL;
    }
}

Map_Result HashMap_Rehash(HashMap *_map, size_t _newCapacity)
{
    register size_t i;
    HashMap coverMap;
    List **newHash;
    if (_map == NULL)
    {
        return MAP_UNINITIALIZED_ERROR;
    }
    if (!_newCapacity)
    {
        return MAP_SIZE_ILLIGAL_ERROR;
    }
    _newCapacity = CalcCapacity(_newCapacity);
    if (!_newCapacity || (newHash = (List**)calloc(_newCapacity, sizeof(List*))) == NULL)
    {
        return MAP_ALLOCATION_ERROR;
    }
    coverMap.m_hash = newHash;
    coverMap.m_hashFunction = _map->m_hashFunction;
    coverMap.m_capacity = _newCapacity;
    coverMap.m_numberOfBuckets = 0;
    coverMap.m_maxChainLength = 0;
    for (i = 0; i < _map->m_capacity; ++i)
    {
        if (_map->m_hash[i] != NULL)
        {
            if (TransferFromListToNewHash(&coverMap, _map->m_hash[i]) != MAP_SUCCESS)
            {
                free(newHash);
                return MAP_ALLOCATION_ERROR;
            }
        }
    }
    MiniStroy(_map, NULL);
    _map->m_hash = newHash;
    _map->m_capacity = _newCapacity;
    _map->m_numberOfBuckets = coverMap.m_numberOfBuckets;
    _map->m_maxChainLength = coverMap.m_maxChainLength;
    return MAP_SUCCESS;
}

Map_Result HashMap_Insert(HashMap* _map, const void* _key, const void* _value)
{
    if (_map == NULL)
    {
        return MAP_UNINITIALIZED_ERROR;
    }
    if (_key == NULL)
    {
        return MAP_KEY_NULL_ERROR;
    }
    return CreateListIfNeededAndInsert(_map, _key, _value, FALSE);
}

Map_Result HashMap_Remove(HashMap* _map, const void* _searchKey, void** _pKey, void** _pValue)
{
    size_t index;
    ListItr remove;
    if (_map == NULL || _pKey == NULL)
    {
        return MAP_UNINITIALIZED_ERROR;
    }
    if (_searchKey == NULL)
    {
        return MAP_KEY_NULL_ERROR;
    }
    index = _map->m_hashFunction(_searchKey) % _map->m_capacity;
    if (_map->m_hash[index] == NULL || !IsKeyFound(_map->m_hash[index], _searchKey, _map->m_equalityFunction, &remove))
    {
        return MAP_KEY_NOT_FOUND_ERROR;
    }
    return Remove(_map, remove, _pKey, _pValue);
}

Map_Result HashMap_Find(const HashMap* _map, const void* _key, void** _pValue)
{
    size_t index;
    ListItr found;
    Pair *pair;
    if (_map == NULL || _pValue == NULL)
    {
        return MAP_UNINITIALIZED_ERROR;
    }
    if (_key == NULL)
    {
        return MAP_KEY_NULL_ERROR;
    }
    index = _map->m_hashFunction(_key) % _map->m_capacity;
    if (_map->m_hash[index] == NULL || !IsKeyFound(_map->m_hash[index], _key, _map->m_equalityFunction, &found))
    {
        return MAP_KEY_NOT_FOUND_ERROR;
    }
    pair = (Pair*)ListItrGet(found);
    *_pValue = pair->m_value;
    return MAP_SUCCESS;
}

size_t HashMap_Size(const HashMap* _map)
{
    if (_map == NULL)
    {
        return 0;
    }
    return _map->m_numOfItems;
}

size_t HashMap_ForEach(const HashMap* _map, KeyValueActionFunction _action, void* _context)
{
    register size_t i;
    size_t count = 0;
    ListItr end, begin, stoppedAt;
    Context context;
    if (_map != NULL && _action != NULL)
    {
        context.m_action = _action;
        context.m_actionFunctionContext = _context;
        context.m_context = (void*)&count;
        for (i = 0; i < _map->m_capacity; ++i)
        {
            if (_map->m_hash[i] != NULL)
            {
                end = ListItrEnd(_map->m_hash[i]);
                begin = ListItrBegin(_map->m_hash[i]);
                if (ListItrEquals(begin, end))
                {
                    continue;
                }
                stoppedAt = ListItrForEach(begin, end, BlackHashForEach, (void*)&context);
                if (!ListItrEquals(stoppedAt, end))
                {
                    break;
                }
            }
        }
    }
    return count;
}

Map_Stats HashMap_GetStatistics(const HashMap* _map)
{
    Map_Stats stats = {0};
    if (_map != NULL)
    {
        stats.maxChainLength = _map->m_maxChainLength;
        stats.numberOfChains = _map->m_numOfItems;
        if ((stats.numberOfBuckets = _map->m_numberOfBuckets))
        {
            stats.averageChainLength = (double)_map->m_numOfItems / _map->m_numberOfBuckets;
        }
    }
    return stats;
}

/* Static Functions */

static void MiniStroy(HashMap* _map, void *_context)
{
    register size_t i;
    ListItr begin, end;
    for (i = 0; i < _map->m_capacity; ++i)
    {
        if (_map->m_hash[i] != NULL)
        {
            end = ListItrEnd(_map->m_hash[i]);
            begin = ListItrBegin(_map->m_hash[i]);
            if (!ListItrEquals(begin, end))
            {
                ListItrForEach(begin, end, BlackDestroy, _context);
            }
            ListDestroy(&_map->m_hash[i], NULL);
        }
    }
    free(_map->m_hash);
}

static int BlackHashForEach(void* _element, void* _context)
{
    ++*(size_t*)(((Context*)_context)->m_context);
    return ((Context*)_context)->m_action(((Pair*)_element)->m_key, ((Pair*)_element)->m_value, ((Context*)_context)->m_actionFunctionContext);
}

static Map_Result TransferFromListToNewHash(HashMap* _map, List *_list)
{
    ListItr read, end;
    Pair *pair;
    read = ListItrBegin(_list);
    end = ListItrEnd(_list);
    while (!ListItrEquals(read, end))
    {
        pair = (Pair*)ListItrGet(read);
        if (CreateListIfNeededAndInsert(_map, pair->m_key, pair->m_value, TRUE) != MAP_SUCCESS)
        {
            return MAP_ALLOCATION_ERROR;
        }
        read = ListItrNext(read);
    }
    return MAP_SUCCESS;
}

static Map_Result CreateListIfNeededAndInsert(HashMap* _map, const void* _key, const void* _value, int _rehash)
{
    size_t index = _map->m_hashFunction(_key) % _map->m_capacity;
    if (_map->m_hash[index] == NULL)
    {
        if ((_map->m_hash[index] = ListCreate()) == NULL)
        {
            return MAP_ALLOCATION_ERROR;
        }
        _map->m_numberOfBuckets++;
    } else {
        if (!_rehash && IsKeyFound(_map->m_hash[index], _key, _map->m_equalityFunction, NULL))
        {
            return MAP_KEY_DUPLICATE_ERROR;
        }
    }
    return Insert(_map, _map->m_hash[index], _key, _value);
}

static Map_Result Remove(HashMap *_hash, ListItr _remove, void** _pKey, void** _pValue)
{
    Pair *removed;
    if ((removed = ListItrRemove(_remove)) == NULL)
    {
        return MAP_UNINITIALIZED_ERROR;
    }
    *_pKey = removed->m_key;
    if (_pValue != NULL)
    {
        *_pValue = removed->m_value;
    }
    _hash->m_numOfItems--;
    free(removed);
    return MAP_SUCCESS;
}

static Map_Result Insert(HashMap *_hash, List *_list, const void* _key, const void* _value)
{
    Pair *pair;
    size_t listSize = 1;
    if ((pair = (Pair*)malloc(sizeof(Pair))) == NULL || 
        (ListPushHead(_list, (void*)pair) != LIST_SUCCESS))
    {
        return MAP_ALLOCATION_ERROR;
    }
    pair->m_key = (void*)_key;
    pair->m_value = (void*)_value;
    _hash->m_numOfItems++;
    if (!_hash->m_maxChainLength || _hash->m_maxChainLength < (listSize = ListSize(_list)))
    {
        _hash->m_maxChainLength = listSize;
    }
    return MAP_SUCCESS;
}

static int SearchFunc(void * _element, void* _context)
{
    return ((Context*)_context)->m_equalityFunction(((Pair*)_element)->m_key, ((Context*)_context)->m_key);
}

static int IsKeyFound(const List *_list, const void* _key, EqualityFunction _equalFunc, ListItr *_found)
{
    ListItr begin, end, found;
    Context context;
    context.m_equalityFunction = _equalFunc;
    context.m_key = _key;
    begin = ListItrBegin(_list);
    end = ListItrEnd(_list);
    found = ListItrFindFirst(begin, end, SearchFunc, (void*)&context);
    if (ListItrEquals(found, end))
    {
        return FALSE;
    }
    if (_found != NULL)
    {
        *_found = found;
    }
    return TRUE;
}

static size_t CalcCapacity(size_t _userSize)
{
    while (!IsPrime(_userSize))
    {
        ++_userSize;
    }
    return _userSize;
}

static int IsPrime(size_t _n)
{
    register int i;
    size_t _nSquare = (size_t)sqrt(_n) + 1;
    for (i = 2; i <= _nSquare; ++i)
    {
        if (_n % i == 0)
        {
            return FALSE;
        }
    }
    return TRUE;
}

static int BlackDestroy(void* _element, void* _context)
{
    if (_context != NULL)
    {
        if (((Context*)_context)->m_keyDestroy != NULL)
        {
            ((Context*)_context)->m_keyDestroy(((Pair*)_element)->m_key);
        }
        if (((Context*)_context)->m_valDestroy != NULL)
        {
            ((Context*)_context)->m_valDestroy(((Pair*)_element)->m_value);
        }
    }
    free((Pair*)_element);
    return 1;
}

/* Get functions for Tests */

size_t GetCapacity(HashMap *_hash)
{
    return _hash->m_capacity;
}