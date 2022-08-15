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

typedef enum
{
    EID_DIVIDE_BY_ZERO = 1,
    EID_ARGUMENT_NULL,
    EID_INVALID_ARGUMENT
} ExceptionID;

enum
{
    _JMP_EXCEPTION = 1,
    _JMP_FINALLY_EXECUTED,
};

typedef struct
{
    char const *message;
    ExceptionID const id;
} Exception;

static Exception *makeException(char const *, ExceptionID const);
inline void setThrownException(char const *, ExceptionID const);
inline void freeThrownException(void);
inline void _abortUnhandledException(void);
inline Exception const *getThrownException(void);

// Internal macros used to avoid code duplication.
#define _AN_EXCEPTION_OCCURED (getThrownException() != NULL)
#define _TOP_TRY_CATCH_ENV (*(jmp_buf*)peekStack(&_g_tryCatchEnvs))

// Used to throw an exception that will bubble up the stack.
#define throw(message, id) do                                                                           \
{                                                                                                       \
    assert(!_AN_EXCEPTION_OCCURED                                                                       \
           && "Cannot throw an exception when another is already being handled. Use rethrow instead."); \
    setThrownException(message, id);                                                                    \
    if(_g_tryCatchEnvs.size == 0)                                                                       \ 
    {                                                                                                   \
        _abortUnhandledException();                                                                     \
    }                                                                                                   \
    longjmp(_TOP_TRY_CATCH_ENV, _JMP_EXCEPTION);                                                        \
}                                                                                                       \
while(0)

// Main block of the construct.
#define try                                                                                             \
bool _handled = false;                                                                                  \
jmp_buf _tryCatchEnv;                                                                                   \
pushStack(&_g_tryCatchEnvs, &_tryCatchEnv);                                                             \
int _setjmpResult = setjmp(_tryCatchEnv);                                                               \
if (_setjmpResult == _JMP_FINALLY_EXECUTED)                                                             \
{                                                                                                       \
    popStack(&_g_tryCatchEnvs);                                                                         \
    if(_AN_EXCEPTION_OCCURED && !_handled)                                                              \
    {                                                                                                   \
        if(_g_tryCatchEnvs.size == 0)                                                                   \
        {                                                                                               \
            _abortUnhandledException();                                                                 \
        }                                                                                               \
        else                                                                                            \
        {                                                                                               \
            longjmp(_TOP_TRY_CATCH_ENV, _JMP_EXCEPTION);                                                \
        }                                                                                               \
    }                                                                                                   \
    else                                                                                                \
    {                                                                                                   \
        freeThrownException();                                                                          \
        goto _finally;                                                                                  \
    }                                                                                                   \
}                                                                                                       \
else if(_setjmpResult == 0)

// Catches an exception that has the specified ID in a local variable limited to the scope of the catch block.
#define catch(targetId, exceptionVarName)                                                               \
for (Exception const *exceptionVarName = getThrownException();                                          \
     !_handled && _AN_EXCEPTION_OCCURED && getThrownException()->id == targetId;                        \
     _handled = true)

// Catches every exception in a local variable limited to the scope of the catchAll block.
#define catchAll(exceptionVarName)                                                                      \
for (Exception const *exceptionVarName = getThrownException();                                          \
     !_handled && _AN_EXCEPTION_OCCURED;                                                                \
     _handled = true)

// Rethrow an exception to the caller.
#define rethrow do                                                                                      \
{                                                                                                       \
    assert(getThrownException() != NULL && "Cannot rethrow an exception when there's no exception.");   \
    assert(!_handled && "Cannot rethrow an exception if it has been already handled.");                 \
    goto _finally;                                                                                      \
} while(0)

// Defines a "finally" block that will get executed regardless if there's an exception or not.
// Often used for cleanup.
#define finally                                                                                         \
bool _finallyExecuted = false;                                                                          \
_finally:                                                                                               \
for(;!_finallyExecuted; _finallyExecuted = true, longjmp(_TOP_TRY_CATCH_ENV, _JMP_FINALLY_EXECUTED))

#endif // EXCEPTIONS_H_INCLUDED
