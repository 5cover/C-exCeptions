#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "..\src\exceptions.h"

int divide(int a, int b);
void doSomething(void *pData);
void runProgram(bool raiseDivideByZero, bool raiseArgumentNull);

int main(void)
{
    // To catch unhandled exceptions, you can write what's known as a "big outer try block" in your main method,
    // with a catchAll block that will catch all thrown exceptions.
    runProgram(false, true);
    system("pause");
    runProgram(true, false);

    return EXIT_SUCCESS;
}

void runProgram(bool raiseDivideByZero, bool raiseArgumentNull)
{
    // Create some fake ad-hoc data to pass into the functions.
    int divisor = raiseDivideByZero ? 0 : 3;
    void *pData = raiseArgumentNull ? NULL : &divisor;

    try
    {
        // rethrow; // causes assertion failure because no exception has been thrown (yet)
        int result = divide(6, divisor);
        printf("Result is %d\n", result);
        doSomething(pData);
        printf("Done something with data successfully.\n");
    }
    // Argument 1 is the ID of the exception to catch
    // Argument 2 is the name of the variable to create (of type Exception const*) local to the scope of the catch block.
    catch(EID_DIVIDE_BY_ZERO, e)
    {
        printf("In catch block for EID_DIVIDE_BY_ZERO.\n");
        printf("Rethrowing exeption...\n");

        // Rethrow the exception and let it bubble up.
        // The equivalent C# syntax would be
        // throw;
        rethrow;
    }
    // The equivalent C# syntax would be
    // catch(ArgumentNullException e)
    catch(EID_ARGUMENT_NULL, e)
    {
        printf("In catch block for EID_ARGUMENT_NULL.\n");
        fprintf(stderr, "An exception occurred\nMessage: %s\nID: %d\n", e->message, e->id);
        printf("Exception swallowed.\n");
    }
    // The finally block will always be executed, regardless if an exception occurred or not.
    // It is optional.
    finally
    {
        printf("In finally block.\n");
    }

    // This line will only be executed if no exception occurred.
    printf("Some additional logic after the try-catch-finally construct that should not be executed if an exception occured...\n");
}

int divide(int a, int b)
{
    if (b == 0)
    {
        // Here an exception is thrown if b equals 0 because dividing by 0 is not a valid operation.
        // In C, dividing by zero is undefined behavior, and on my Windows machine it crashes the program.
        // However, by throwing an exception, we can recover from what would otherwise be a critical failure.

        // The throw macro leaves the method and uses the longjmp() function to jump to the nearest catch block in the stack trace.
        // If there is no catch block defined, it shows a message and aborts the program since the exception is unhandled.
        // See the main() function for instructions on how to handle unhandled exceptions.
        throw(EID_DIVIDE_BY_ZERO, "Can't divide by zero");
    }
    printf("Dividing %d and %d...\n", a, b);
    return a/b;
}
void doSomething(void *pData)
{
    if (pData == NULL)
    {
        // The equivalent C# syntax would be
        // throw new ArgumentNullException(nameof(pData));
        throw(EID_ARGUMENT_NULL, "Argument \"pData\" was NULL");
    }
    printf("Doing something with data at memory address %p...\n", pData);
    // do something with data...
}
