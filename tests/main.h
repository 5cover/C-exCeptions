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
typedef TestResult (*t_TestFunction)(t_TestProc const);

static inline void failThrow(void)
{
    assert(!"Throw macro did not leave method.");
}
void signalHandler(int sigId);
void basic(void);
void uncatchable(void);
void multipleThrows(void);
void noThrow(void);
bool askYesNoQuestion(char const *);
TestResult catchesFinally(t_TestProc const);
TestResult catches(t_TestProc const);
TestResult doesntCatchFinally(t_TestProc const);
TestResult doesntCatch(t_TestProc const);
TestResult rethrowsFinally(t_TestProc const);
TestResult rethrows(t_TestProc const);

#define assertEquals(a, b) do { if (a != b) printf("%d != %d\n", a, b); assert(a == b); } while(0)

#define test(procName, testName, shouldRethrow, expected_tryC, expected_catchC, expected_catchAllC, expected_finallyC, expected_afterC, expected_rethrowC)            \
do                                                                                                                                                                    \
{                                                                                                                                                                     \
    printf(#procName " test that " #testName "...\n");                                                                                                                \
    TestResult expected = { expected_tryC, expected_catchC, expected_catchAllC, expected_finallyC, expected_afterC, expected_rethrowC };                              \
    _test(&testName, shouldRethrow, &procName, expected);                                                                                                             \
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
