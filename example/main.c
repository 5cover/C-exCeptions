#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "..\src\exceptions.h"

int divide(int a, int b);
void doSomething(void *pData);
void runProgram(bool raiseDivideByZero, bool raiseArgumentNull);

int main(void)
{
    runProgram(false, true);
    system("pause");
    runProgram(false, false);
    return EXIT_SUCCESS;
}

void runProgram(bool raiseDivideByZero, bool raiseArgumentNull)
{
    // Create some fake ad-hoc data to pass into the functions.
    int divisor = raiseDivideByZero ? 0 : 3;
    void *pData = raiseArgumentNull ? NULL : &divisor;

    /* Identifiers reserved by the try/catch/finally structure
     * _handled
     * _tryCatchEnv
     * _finally */
    try
    {
        // rethrow; // causes assertion failure
        int result = divide(6, divisor);
        printf("Result is %d\n", result);
        doSomething(pData);
        printf("Done something with data successfully.\n");
    }
    catch(EID_DIVIDE_BY_ZERO, e)
    {
        printf("In catch block for EID_DIVIDE_BY_ZERO.\n");
        printf("Rethrowing exeption...");
        rethrow;
    }
    catch(EID_ARGUMENT_NULL, e)
    {
        printf("In catch block for EID_ARGUMENT_NULL.\n");
        fprintf(stderr, "An exception occurred\nMessage: %s\nID: %d\n", getThrownException()->message, getThrownException()->id);
        printf("Exception discarded.\n");
    }
    finally
    {
        printf("In finally block.\n");
    }

    printf("\nSome additional logic after the try-catch-finally construct that should not be executed if an exception occured...\n\n");
}

int divide(int a, int b)
{
    if (b == 0)
    {
        throw("Can't divide by zero", EID_DIVIDE_BY_ZERO);
    }
    printf("Dividing %d and %d...\n", a, b);
    return a/b;
}
void doSomething(void *pData)
{
    if (pData == NULL)
    {
        throw("Argument \"data\" is NULL", EID_ARGUMENT_NULL);
    }
    printf("Doing something with data at memory address %p...\n", pData);
    // do something with data...
}
