/** \brief Simple system for throwing and handling exceptions in C with macros.
 * \author Scover
 */

#ifndef EXCEPTIONS_H_INCLUDED
#define EXCEPTIONS_H_INCLUDED

#include <stdlib.h>
#include <assert.h>
#include <setjmp.h>
#include <stdbool.h>

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

// Internal functions
bool _ex_noTryCatchEnvsDefined(void);
Exception const *_ex_thrownException(void);
jmp_buf *_ex_peekTryCatchEnvs(void);
void _ex_abortDueToUnhandledException(void);
void _ex_freeThrownException(void);
void _ex_popTryCatchEnvs(void);
void _ex_pushTryCatchEnvs(jmp_buf*);
void _ex_setThrownException(ExceptionID const, char const *);

// Used to throw an exception that will bubble up the stack.
#define throw(id, message) do                                                                         \
{                                                                                                     \
    assert(_ex_thrownException() == NULL                                                              \
        && "Cannot throw an exception when another is already being handled. Use rethrow instead.");  \
    _ex_setThrownException(id, message); printf("Set throw exception\n");                             \
    if(_ex_noTryCatchEnvsDefined())                                                                   \
    {                                                                                                 \
        _ex_abortDueToUnhandledException();                                                           \
    }                                                                                                 \
    longjmp(*_ex_peekTryCatchEnvs(), -1);                                                             \
}                                                                                                     \
while(0)

// Try something and execute catch blocks if an exception is thrown.
#define try                                                                                           \
/* Using static to prevent jmp from changing values */                                                \
static bool _ex_finallyPending, _ex_handled;                                                          \
_ex_finallyPending = true;                                                                            \
_ex_handled = false;                                                                                  \
int _ex_setjmpResult;                                                                                 \
{                                                                                                     \
    jmp_buf env;                                                                                      \
    _ex_pushTryCatchEnvs(&env);                                                                       \
    _ex_setjmpResult = setjmp(env);\
}                                                                                                     \
if (_ex_setjmpResult == 1)                                                                            \
{                                                                                                     \
    _ex_finallyPending = false;                                                                       \
    _ex_popTryCatchEnvs();                                                                            \
    if(_ex_thrownException() != NULL && !_ex_handled)                                                 \
    {                                                                                                 \
        if(_ex_noTryCatchEnvsDefined())                                                               \
        {                                                                                             \
            _ex_abortDueToUnhandledException();                                                       \
        }                                                                                             \
        longjmp(*_ex_peekTryCatchEnvs(), -1);                                                         \
    }                                                                                                 \
    else                                                                                              \
    {                                                                                                 \
        _ex_freeThrownException();                                                                    \
        goto _ex_finally;                                                                             \
    }                                                                                                 \
}                                                                                                     \
else if(_ex_setjmpResult == 0)

// Catches an exception that has the specified ID in a local variable with the scope of the catch block.
#define catch(targetId, varName)                                                                      \
for (Exception const *varName = _ex_thrownException();                                                \
     !_ex_handled && _ex_thrownException() != NULL && varName->id == targetId;                        \
     _ex_handled = true, varName = varName) // hide "unused variable" warnings

// Catches any exception in a local variable limited to the scope of the catchAll block.
#define catchAll(varName)                                                                             \
for (Exception const *varName = _ex_thrownException();                                                \
     !_ex_handled && _ex_thrownException() != NULL;                                                   \
     _ex_handled = true, varName = varName) // hide "unused variable" warnings

// Rethrow an exception to the caller.
#define rethrow do                                                                                    \
{                                                                                                     \
    assert(_ex_thrownException() != NULL                                                              \
        && "Cannot rethrow an exception when there's no exception.");\
    assert(!_ex_handled                                                                               \
        && "Cannot rethrow an exception if it has already been handled.");                            \
    goto _ex_finally;                                                                                 \
} while(0)

// Defines a "finally" block that will get executed regardless if there's an exception or not.
// Often used for cleanup. Will only be executed once.
#define finally                                                                                       \
_ex_finally:                                                                                          \
/* Jump back to the try block to execute more complex logic here than what could fit in a for loop. */\
for(; _ex_finallyPending; longjmp(*_ex_peekTryCatchEnvs(), 1))

#endif // EXCEPTIONS_H_INCLUDED
