#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <assert.h>
#include <stdio.h>
#include "..\src\exceptions.h"

#define EID_TEST 99

// A test. Each member corresponds to the number of times the specified structure was executed..
typedef struct
{
    int tryC;
    int catchC;
    int catchAllC;
    int finallyC;
    int afterC;
} Test;

static inline void failThrow(void)
{
    assert(false && "Throw macro did not leave method.");
}

static inline Test catches(void (*function)(void))
{
    Test t = {0};

    try
    {
        ++t.tryC;
        function();
    }
    catch(EID_TEST, e)
    {
        ++t.catchC;
    }
    catchAll(e)
    {
        ++t.catchAllC;
    }
    finally
    {
        ++t.finallyC;
    }
    ++t.afterC;
    return t;
}

static inline Test doesntCatch(void (*function)(void))
{
    Test t = {0};

    try
    {
        ++t.tryC;
        function();
    }
    finally
    {
        ++t.finallyC;
    }
    ++t.afterC;
    return t;
}

#define assertEquals(a, b) if (a != b) printf("%d != %d\n", a, b); assert(a == b)

#define test(testFunction,function, expected_tryC, expected_catchC, expected_catchAllC, expected_finallyC, expected_afterC) \
{                                                                      \
    printf("Testing \"%s\" with function \"%s\"...\n", #function, #testFunction); \
    Test t = (&testFunction)(&function);                                 \
    assertEquals(t.tryC, expected_tryC);                               \
    assertEquals(t.catchC, expected_catchC);                           \
    assertEquals(t.catchAllC, expected_catchAllC);                     \
    assertEquals(t.finallyC, expected_finallyC);                       \
    assertEquals(t.afterC, expected_afterC);                           \
}

#endif // MAIN_H_INCLUDED
