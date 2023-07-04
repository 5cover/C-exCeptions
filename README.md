# C-exCeptions

Static library containing macros for throwing and handling exceptions in C based on the C# syntax.

Unhandled exceptions result in a call to ``abort()``.
To respond to unhandled exceptions, the ``SIGABRT`` signal can be handled or a *try/catchAll* block can be defined in the main function.

## Usage

Interface reference:

- ``throw``: throw an exception
    1. *id* - ID of the exception. Can be an ``ExceptionID`` value or an integer.
    2. *message* - Message of the exception. Must be a string (``char *``).
- ``try``: define a try block
- ``catch``: catch an exception with a specific ID
    1. *targetId* - ID of the exception to catch. Can be an ``ExceptionID`` value or an integer.
    2. *varName* - Name of the variable (of type ``Exception const *const``) that points the catched exception.
- ``catchWhen``: catch an exception with a specific ID only when the provided boolean expression evaluates to true
    1. *targetId* - ID of the exception to catch. Can be an ``ExceptionID`` value or an integer.
    2. *varName* - Name of the variable (of type ``Exception const *const``) that points the catched exception.
- ``catchAll``: catch any exception
    1. *varName* - Name of the variable (of type ``Exception const *const``) that points the catched exception.
- ``catchAllWhen``: catch any exception when the provided boolean expression evaluates to true
    1. *varName* - Name of the variable (of type ``Exception const *const``) that points the catched exception.
- ``rethrow``: rethrow an exception from a catch block.
- ``finally``: defines a block to execute regardless of whether an exception occured or not.

See the comments in [example](https://github.com/5cover/C-exCeptions/blob/main/example/main.c) for a walk-through of the code.

## Known issues
- The ``try`` macro cannot be used in a single-statement scope (*i.e.* ``if`` statement without braces). This is not a problem since it is useless without *catch* or *finally* blocks.
- An exception variable name has to be specified when defining a *catch* block. You can use ``_`` as the identifier if you're not planning to use it. Unused variable warnings will be hidden if GNU C is supported (with the ``unused`` attribute).
- The try-catch stack is defined as thread-local, so exceptions will not bubble up outside of their original thread.

## Reserved identifiers

All reserved identifiers are prefixed by ``_ex_``.

## Contributing

This is mostly an experiment with advanced macro features and the ``setjmp``/``longjmp``.
