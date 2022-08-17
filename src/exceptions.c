/** \brief exCeptions.h implementation
 * \author Scover
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "exceptions.h"

Stack _g_tryCatchEnvs = EMPTY_STACK_INITIALIZER;

static Exception *sgp_thrownException = NULL;

static Exception *makeException(ExceptionID const id, char const *message);

void _setThrownException(ExceptionID const id, char const *message)
{
    sgp_thrownException = makeException(id, message);
}

Exception const *_thrownException(void)
{
    return sgp_thrownException;
}

void _freeThrownException(void)
{
    free(sgp_thrownException);
    sgp_thrownException = NULL;
}

Exception *makeException(ExceptionID const id, char const *message)
{
    // Duplicate the string to ensure it's allocated on the heap.
    char const *HEAP_MESSAGE = strdup(message);
    if (HEAP_MESSAGE == NULL)
    {
        fprintf(stderr, "\nString duplication failed: strdup(\"%s\") returned NULL\nAborting program...\n", message);
        abort();
    }

    Exception *pNew = malloc(sizeof *pNew);
    if (pNew == NULL)
    {
        fprintf(stderr, "\nException heap allocation failed: malloc(%u) returned NULL\nAborting program...\n", (unsigned)sizeof *pNew);
        abort();
    }

    // Dynamic allocation of immutable struct

    // Declare a stack-allocated variable for initializing the const members of Exception
    Exception init =
    {
        .message = HEAP_MESSAGE,
        .id = id
    };
    // Copy the binary data to the heap-allocated exception.
    memcpy(pNew, &init, sizeof *pNew);

    // Return the heap-allocated exception and automatically discard init.
    return pNew;

}

void _abortUnhandledException(void)
{
    assert(_thrownException() != NULL && "Attempted to abort for an unhandled exception, but no exception occurred.");
    fprintf(stderr, "\nUnhandled exception!\nMessage: %s\nID: %d\n", _thrownException()->message, _thrownException()->id);

    abort();
}
