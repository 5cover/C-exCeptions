#include "main.h"
#include <signal.h>

#define THROW_TEST() do { throw(EID_TEST, "test exception"); failThrow(); } while(0)
#define THROW_UNCATCHABLE() do { throw(420, "catch me if you can"); failThrow(); } while(0)

static bool sg_autoTestsCompleted = false;

int main(void)
{
    signal(SIGABRT, signalHandler);

    test(basic, catches,            false, 1,1,0,0,1,0);
    test(basic, catchesFinally,     false, 1,1,0,1,1,0);
    test(basic, doesntCatch,        true,  1,0,0,0,0,0);
    test(basic, doesntCatchFinally, true,  1,0,0,1,0,0);
    test(basic, rethrows,           true,  1,1,0,0,0,1);
    test(basic, rethrowsFinally,    true,  1,1,0,1,0,1);

    test(uncatchable, catches,            false, 1,0,1,0,1,0);
    test(uncatchable, catchesFinally,     false, 1,0,1,1,1,0);
    test(uncatchable, doesntCatch,        true,  1,0,0,0,0,0);
    test(uncatchable, doesntCatchFinally, true,  1,0,0,1,0,0);
    test(uncatchable, rethrows,           true,  1,0,1,0,0,1);
    test(uncatchable, rethrowsFinally,    true,  1,0,1,1,0,1);

    test(multipleThrows, catches,            false, 1,1,0,0,1,0);
    test(multipleThrows, catchesFinally,     false, 1,1,0,1,1,0);
    test(multipleThrows, doesntCatch,        true,  1,0,0,0,0,0);
    test(multipleThrows, doesntCatchFinally, true,  1,0,0,1,0,0);
    test(multipleThrows, rethrows,           true,  1,1,0,0,0,1);
    test(multipleThrows, rethrowsFinally,    true,  1,1,0,1,0,1);

    test(noThrow, catches,            false, 1,0,0,0,1,0);
    test(noThrow, catchesFinally,     false, 1,0,0,1,1,0);
    test(noThrow, doesntCatch,        false, 1,0,0,0,1,0);
    test(noThrow, doesntCatchFinally, false, 1,0,0,1,1,0);
    test(noThrow, rethrows,           false, 1,0,0,0,1,0);
    test(noThrow, rethrowsFinally,    false, 1,0,0,1,1,0);

    sg_autoTestsCompleted = true;

    printf("\nAutomatic tests completed. Running unhandled exception test...\n");

    THROW_TEST();

    return 0;
}

void signalHandler(int sigId)
{
    if(sigId == SIGABRT && sg_autoTestsCompleted)
    {
        assert(askYesNoQuestion("\nIs this correct?"));
        exit(EXIT_SUCCESS);
    }
}

void basic(void)
{
    THROW_TEST();
}

void uncatchable(void)
{
    THROW_UNCATCHABLE();
}

void multipleThrows(void)
{
    THROW_TEST();
    THROW_UNCATCHABLE();
}

void noThrow(void)
{

}

TestResult catchesFinally(t_TestProc const testProc)
{
    TestResult t = {0};

    try
    {
        ++t.tryC;
        testProc();
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

TestResult catches(t_TestProc const testProc)
{
    TestResult t = {0};

    try
    {
        ++t.tryC;
        testProc();
    }
    catch(EID_TEST, e)
    {
        ++t.catchC;
    }
    catchAll(e)
    {
        ++t.catchAllC;
    }

    ++t.afterC;

    return t;
}

TestResult doesntCatchFinally(t_TestProc const testProc)
{
    TestResult t = {0};

    try
    {
        ++t.tryC;
        testProc();
    }
    finally
    {
        ++t.finallyC;
    }

    ++t.afterC;

    return t;
}

TestResult doesntCatch(t_TestProc const testProc)
{
    TestResult t = {0};

    ++t.tryC;
    testProc();

    ++t.afterC;

    return t;
}

TestResult rethrowsFinally(t_TestProc const testProc)
{
    TestResult t = {0};

    try
    {
        ++t.tryC;
        testProc();
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

TestResult rethrows(t_TestProc const testProc)
{
    TestResult t = {0};

    try
    {
        ++t.tryC;
        testProc();
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

    ++t.afterC;

    return t;
}

bool askYesNoQuestion(char const *question)
{
    printf("%s (y/n) ", question);
    char input = '\0';
    do
    {
        input = getchar();
        while(getchar() != '\n');
    } while (input != 'n' && input != 'y' && (printf("Invalid input (y/n) "), true));
    return input == 'y';
}
