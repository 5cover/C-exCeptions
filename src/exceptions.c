/** \brief exCeptions.h implementation
 * \author Scover
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "exceptions.h"

Stack _g_tryCatchEnvs = EMPTY_STACK_INITIALIZER;

Exception *makeException(char const *message, ExceptionID const id);

static Exception *g_pThrownException = NULL;

Exception *makeException(char const *message, ExceptionID const id)
{
    // Duplicate the string to ensure it's allocated on the heap.
    char const *HEAP_MESSAGE = strdup(message);
    if (HEAP_MESSAGE == NULL)
    {
        fprintf(stderr, "\nString duplication failed: strdup(\"%s\") returned NULL\n", message);
        abort();
    }

    Exception *pNew = malloc(sizeof(Exception));
    if (pNew == NULL)
    {
        fprintf(stderr, "\nException heap allocation failed: malloc(%u) returned NULL\n", (unsigned)sizeof(Exception));
        abort();
    }

    // Declare a stack-allocated variable for initializing the const members of Exception
    Exception init =
    {
        .message = HEAP_MESSAGE,
        .id = id
    };
    // Copy the binary data it to the heap-allocated exception.
    memcpy(pNew, &init, sizeof(Exception));

    // Return the heap-allocated exception and automatically discard init.
    return pNew;

}

void setThrownException(char const *message, ExceptionID const id)
{
    // make sure its freed. freeThrownException() nullifies the pointer so it's safe to call it multiple times.
    freeThrownException();
    g_pThrownException = makeException(message, id);
}

void freeThrownException(void)
{
    free(g_pThrownException);
    g_pThrownException = NULL;
}

Exception const *getThrownException(void)
{
    return g_pThrownException;
}

void _abortUnhandledException(void)
{
    assert(g_pThrownException != NULL && "Attempted to abort for an unhandled exception, but there's not exception");
    fprintf(stderr, "\nUnhandled exception!\nMessage: %s\nID: %d\nAborting application...\n", g_pThrownException->message, g_pThrownException->id);
    abort();
}
