#include "ListFunctions.h"
#include "ListItr.h"
#define RESUME 1
#define EXIT 0

typedef struct Parameters Parameters;

typedef int (*GenericFunction)(void* _element, void* _context);
typedef int (*ResponsiveFunction)(void* _reContext, int _response);

struct Parameters
{
    ListItr m_begin;
    ListItr m_end;
    GenericFunction m_func;
    ResponsiveFunction m_response;
    void *m_ReContext;
    void *m_context;
};

static ListItr GenForEach(Parameters *_param);
static int ResponseFindFirst(void* _reContext, int _response);
static int ResponseCountIf(void* _reContext, int _response);
static int ResponseForEach(void* _reContext, int _response);

ListItr ListItrFindFirst(ListItr _begin, ListItr _end, PredicateFunction _predicate, void* _context)
{
    Parameters params;
    ListItr stoppedAt;
    if (_begin == NULL || _end == NULL || _predicate == NULL)
    {
        return NULL;
    }
    params.m_begin = _begin;
    params.m_end = _end;
    params.m_func = _predicate;
    params.m_context = _context;
    params.m_response = ResponseFindFirst;
    params.m_ReContext = NULL;
    stoppedAt = GenForEach(&params);
    return stoppedAt;
}

size_t ListItrCountIf(ListItr _begin, ListItr _end, PredicateFunction _predicate, void* _context)
{
    Parameters params;
    size_t count = 0;
    if (_begin == NULL || _end == NULL || _predicate == NULL)
    {
        return 0;
    }
    params.m_begin = _begin;
    params.m_end = _end;
    params.m_func = _predicate;
    params.m_context = _context;
    params.m_response = ResponseCountIf;
    params.m_ReContext = (void*)&count;
    GenForEach(&params);
    return count;
}

ListItr ListItrForEach(ListItr _begin, ListItr _end, ListActionFunction _action, void* _context)
{
    Parameters params;
    ListItr stoppedAt;
    if (_begin == NULL || _end == NULL || _action == NULL)
    {
        return NULL;
    }
    params.m_begin = _begin;
    params.m_end = _end;
    params.m_func = _action;
    params.m_context = _context;
    params.m_response = ResponseForEach;
    params.m_ReContext = NULL;
    stoppedAt = GenForEach(&params);
    return stoppedAt;
}

/* Static Functions */

static ListItr GenForEach(Parameters *_param)
{
    int response;
    ListItr stoppedAt = _param->m_begin;
    for (; stoppedAt != _param->m_end; stoppedAt = ListItrNext(stoppedAt))
    {
        response = _param->m_func(ListItrGet(stoppedAt), _param->m_context);
        if ((_param->m_response(_param->m_ReContext, response)) == EXIT)
        {
            break;
        }
    }
    return stoppedAt;
}

static int ResponseFindFirst(void* _reContext, int _response)
{
    (void)_reContext;
    return !_response;
}

static int ResponseCountIf(void* _reContext, int _response)
{
    if (_response)
    {
        ++(*(size_t*)_reContext);
    }
    return RESUME;
}

static int ResponseForEach(void* _reContext, int _response)
{
    (void)_reContext;
    return _response;
}