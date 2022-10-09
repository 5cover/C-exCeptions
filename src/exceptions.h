/** \brief Simple system for throwing and handling exceptions in C with macros.
 * \author Scover
 */

#ifndef EXCEPTIONS_H_INCLUDED
#define EXCEPTIONS_H_INCLUDED

#include <stdlib.h>
#include <assert.h>
#include <setjmp.h>
#include <stdbool.h>
#include "stack.h"

extern Stack _g_tryCatchEnvs;

// Common exception IDs with their C# counterpart. Don't hesitate to define your own.
typedef enum
{
    // ArgumentException
    EID_ARGUMENT = 1,
    // ArgumentNullException
    EID_ARGUMENT_NULL,
    // ArgumentOutOfRangeException
    EID_ARGUMENT_OUT_OF_RANGE,
    // DivideByZeroException
    EID_DIVIDE_BY_ZERO,
    // FormatException
    EID_FORMAT,
    // IndexOutOfRangeException
    EID_INDEX_OUT_OF_RANGE,
    // InsufficientMemoryException
    EID_INSUFFICIENT_MEMORY,
    // InvalidOperationException
    EID_INVALID_OPERATION,
    // IOException
    EID_IO_EXCEPTION,
    // NotImplementedException
    EID_NOT_IMPLEMENTED,
    // NotSupportedException
    EID_NOT_SUPPORTED,
    // NullReferenceException
    EID_NULL_POINTER,
    // OperationCanceledException
    EID_OPERATION_CANCELED,
    // RankException
    EID_RANK,
} ExceptionID;

typedef struct
{
    char const *message;
    ExceptionID const id;
} Exception;

void _setThrownException(ExceptionID const, char const *);
void _freeThrownException(void);
Exception const *_thrownException(void);
void _abortUnhandledException(void);

// Used to throw an exception that will bubble up the stack.
#define throw(id, message) do                                                                                    \
{                                                                                                                \
    assert(_thrownException() == NULL                                                                            \
           && "Cannot throw an exception when another is already being handled. Use rethrow instead.");          \
    _setThrownException(id, message);                                                                            \
    if(_g_tryCatchEnvs.size == 0)                                                                                \
    {                                                                                                            \
        _abortUnhandledException();                                                                              \
    }                                                                                                            \
    longjmp(*(jmp_buf*)peekStack(&_g_tryCatchEnvs), -1);                                                         \
}                                                                                                                \
while(0)

// Main block of the construct.
#define try                                                                                                      \
bool _handled = false, _finallyPending = true;                                                                   \
{                                                                                                                \
    jmp_buf _localTryCatchEnv;                                                                                   \
    pushStack(&_g_tryCatchEnvs, &_localTryCatchEnv);                                                             \
}                                                                                                                \
int _setjmpResult = setjmp(*(jmp_buf*)peekStack(&_g_tryCatchEnvs));                                              \
if (_setjmpResult == 1)                                                                                          \
{                                                                                                                \
    popStack(&_g_tryCatchEnvs);                                                                                  \
    if(_thrownException() != NULL && !_handled)                                                                  \
    {                                                                                                            \
        if(_g_tryCatchEnvs.size == 0)                                                                            \
        {                                                                                                        \
            _abortUnhandledException();                                                                          \
        }                                                                                                        \
        longjmp(*(jmp_buf*)peekStack(&_g_tryCatchEnvs), -1);                                                     \
    }                                                                                                            \
    else                                                                                                         \
    {                                                                                                            \
        _freeThrownException();                                                                                  \
        goto _finally;                                                                                           \
    }                                                                                                            \
}                                                                                                                \
else if(_setjmpResult == 0)

// Catches an exception that has the specified ID in a local variable limited to the scope of the catch block.
#define catch(targetId, varName)                                                                                 \
for (Exception const *varName = _thrownException();                                                              \
     !_handled && _thrownException() != NULL && varName->id == targetId;                                         \
     _handled = true, varName = varName) // hide "unused variable" warnings

// Catches any exception in a local variable limited to the scope of the catchAll block.
#define catchAll(varName)                                                                                        \
for (Exception const *varName = _thrownException();                                                              \
     !_handled && _thrownException() != NULL;                                                                    \
     _handled = true, varName = varName) // hide "unused variable" warnings

// Rethrow an exception to the caller.
#define rethrow do                                                                                               \
{                                                                                                                \
    assert(_thrownException() != NULL && "Cannot rethrow an exception when there's no exception.");              \
    assert(!_handled && "Cannot rethrow an exception if it has been already handled.");                          \
    goto _finally;                                                                                               \
} while(0)

// Defines a "finally" block that will get executed regardless if there's an exception or not.
// Often used for cleanup. Will only be executed once.
#define finally                                                                                                  \
_finally:                                                                                                        \
/* Jump back to the try block to execute more complex logic here than what could reasonably fit in a for loop. */\
for(; _finallyPending; _finallyPending = false, longjmp(*(jmp_buf*)peekStack(&_g_tryCatchEnvs), 1))

#endif // EXCEPTIONS_H_INCLUDED
