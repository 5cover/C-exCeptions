#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include "..\src\exceptions.h"

#define EID_TEST 999

// A test result. Each member corresponds to the number of times the specified structure was executed.
typedef struct
{
    int tryC;
    int catchC;
    int catchAllC;
    int finallyC;
    int afterC;
    int rethrowC;
} TestResult;

typedef void (*t_TestProc)(void);
typedef TestResult (*t_TestFunction)(t_TestProc);

static inline void failThrow(void)
{
    assert(!"Throw macro did not leave method.");
}

static inline TestResult catches(t_TestProc function)
{
    TestResult t = {0};

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

static inline TestResult doesntCatch(t_TestProc function)
{
    TestResult t = {0};

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

static inline TestResult rethrows(t_TestProc function)
{
    TestResult t = {0};

    try
    {
        ++t.tryC;
        function();
    }
    catch(EID_TEST, e)
    {
        ++t.catchC;
        ++t.rethrowC;
        rethrow;
    }
    catchAll(e)
    {
        ++t.catchAllC;
        ++t.rethrowC;
        rethrow;
    }
    finally
    {
        ++t.finallyC;
    }

    ++t.afterC;

    return t;
}

#define assertEquals(a, b) if (a != b) printf("%d != %d\n", a, b); assert(a == b)

#define test(testFunctionName, shouldRethrow, functionName, expected_tryC, expected_catchC, expected_catchAllC, expected_finallyC, expected_afterC, expected_rethrowC)\
do                                                                                                                                                                    \
{                                                                                                                                                                     \
    printf("%s test that %s...\n", #functionName, #testFunctionName);                                                                                                 \
    TestResult expected = { expected_tryC, expected_catchC, expected_catchAllC, expected_finallyC, expected_afterC, expected_rethrowC };                              \
    _test(&testFunctionName, shouldRethrow, &functionName, expected);                                                                                                 \
}                                                                                                                                                                     \
while(0)

static inline void _test(t_TestFunction testFunction, bool shouldRethrow, t_TestProc function, TestResult expected)
{
    TestResult t = expected;
    try
    {
        t = testFunction(function);
        assert(!shouldRethrow);
    }
    catchAll(e)
    {
        assert(shouldRethrow);
    }
    finally
    {
        assertEquals(t.tryC, expected.tryC);
        assertEquals(t.catchC, expected.catchC);
        assertEquals(t.catchAllC, expected.catchAllC);
        assertEquals(t.finallyC, expected.finallyC);
        assertEquals(t.afterC, expected.afterC);
        assertEquals(t.rethrowC, expected.rethrowC);
    }
}

#endif // MAIN_H_INCLUDED
