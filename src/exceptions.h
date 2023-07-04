/** \brief Simple system for throwing and handling exceptions in C with macros.
 * \author Scover
 */

#ifndef EXCEPTIONS_H_INCLUDED
#define EXCEPTIONS_H_INCLUDED

#include <stdlib.h>
#include <assert.h>
#include <setjmp.h>
#include <stdbool.h>

#ifdef __GNUC__
#define unused __attribute__((unused))
#else
#define unused
#endif

// Common exception IDs with their C# counterpart.
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
    char const *const message;
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
void _ex_cleanup(bool*, bool);
void _ex_bubbleUp(void);


// Used to throw an exception that will bubble up the stack.
#define throw(id, message) do                                                                         \
{                                                                                                     \
    _ex_setThrownException(id, message);                                                              \
    _ex_bubbleUp();                                                                                   \
}                                                                                                     \
while(0)

// Try something and execute catch blocks if an exception is thrown.
#define try                                                                                           \
jmp_buf _ex_env;                                                                                      \
_ex_pushTryCatchEnvs(&_ex_env);                                                                       \
int const _ex_setjmpResult = setjmp(_ex_env);                                                         \
bool _ex_handled = false, _ex_cleanedUp = false;                                                      \
if (_ex_setjmpResult == 1)                                                                            \
{                                                                                                     \
    _ex_popTryCatchEnvs(); /* Remove this environment as it can no longer receive an exception */     \
}                                                                                                     \
for (bool _ex_tryExecuted = false; _ex_setjmpResult == 0 && !_ex_tryExecuted; _ex_tryExecuted = true, _ex_popTryCatchEnvs())

// Catches an exception that has the specified ID in a local variable with the scope of the catch block.
#define catch(targetId, varName)                                                                      \
for (Exception const unused *const varName = _ex_thrownException();                                   \
     !_ex_handled && _ex_thrownException() != NULL && varName->id == targetId;                        \
     _ex_cleanup(&_ex_cleanedUp, _ex_handled = true))

#define catchWhen(targetId, varName, condition)                                                       \
for (Exception const unused *const varName = _ex_thrownException();                                   \
     !_ex_handled && _ex_thrownException() != NULL && varName->id == targetId && (condition);         \
     _ex_cleanup(&_ex_cleanedUp, _ex_handled = true))

// Catches any exception in a local variable limited to the scope of the catchAll block.
#define catchAll(varName)                                                                             \
for (Exception const unused *const varName = _ex_thrownException();                                   \
     !_ex_handled && _ex_thrownException() != NULL;                                                   \
     _ex_cleanup(&_ex_cleanedUp, _ex_handled = true))

#define catchAllWhen(varName, condition)                                                              \
for (Exception const unused *const varName = _ex_thrownException();                                   \
     !_ex_handled && _ex_thrownException() != NULL && (condition);                                    \
     _ex_cleanup(&_ex_cleanedUp, _ex_handled = true))

// Rethrow an exception to the caller.
#define rethrow do                                                                                    \
{                                                                                                     \
    assert(_ex_thrownException() != NULL                                                              \
        && "Cannot rethrow an exception when there's no exception.");                                 \
    assert(!_ex_handled                                                                               \
        && "Cannot rethrow an exception if it has already been handled.");                            \
    _ex_bubbleUp();                                                                                   \
} while(0)

// Defines a "finally" block that will get executed regardless if there's an exception or not.
// Often used for cleanup. Will only be executed once.
// It is optional.
#define finally                                                                                       \
for (bool _ex_finallyExecuted = false; !_ex_finallyExecuted; _ex_finallyExecuted = true, _ex_cleanup(&_ex_cleanedUp, _ex_handled))

#endif // EXCEPTIONS_H_INCLUDED
