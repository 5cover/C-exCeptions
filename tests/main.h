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
} Test;

static inline void failThrow(void)
{
    assert(false && "Throw macro did not leave method.");
}

static inline Test runTest(void (*function)(void))
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

    return t;
}

static inline void showTest(Test t)
{
    printf("Test:\ntryC=%d\ncatchC=%d\ncatchAllC=%d\nfinallyC=%d\n",
           t.tryC,
           t.catchC,
           t.catchAllC,
           t.finallyC);
}
#define assertEquals(a, b) if (a != b) printf("%d != %d\n", a, b); assert(a == b)
#define test(function, tryCV, catchCV, catchAllCV, finallyCV) { \
printf("Testing function %s...\n", #function);                  \
Test t = runTest(function);                                     \
assertEquals(t.tryC, tryCV);                                    \
assertEquals(t.catchC, catchCV);                                \
assertEquals(t.catchAllC, catchAllCV);                          \
assertEquals(t.finallyC, finallyCV); }

#endif // MAIN_H_INCLUDED
