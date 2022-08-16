# C-exCeptions

Static library containing macros for throwing and handling exceptions in C, in a ``try``/``catch``/``finally``-like fashion, along with a basic implementation of a stack.
I tried my best to reproduce the C# exception handling syntax, but it cannot be perfect due to the limitations of macros.

## How to use

See the comments in [example](https://github.com/5cover/C-exCeptions/blob/main/example/main.c).

## Reserved identifiers
Reserved identifiers used in macros. 
> **Warning** The reserved identifiers may change at any time. Client code should not use or rely on them. Manually calling a function or modifying a variable of this list results to undefined behavior and is not supported.

Name | Type | Scope | Defined in
| - | - | - | -
``_g_tryCatchEnvs`` | Variable | Global | [exceptions.h](https://github.com/5cover/C-exCeptions/blob/main/src/exceptions.h)
``_setThrownException`` | Function | Global | [exceptions.h](https://github.com/5cover/C-exCeptions/blob/main/src/exceptions.h)
``_freeThrownException`` | Function | Global | [exceptions.h](https://github.com/5cover/C-exCeptions/blob/main/src/exceptions.h)
``_thrownException`` | Function | Global | [exceptions.h](https://github.com/5cover/C-exCeptions/blob/main/src/exceptions.h)
``_abortUnhandledException`` | Function | Global | [exceptions.h](https://github.com/5cover/C-exCeptions/blob/main/src/exceptions.h)
``_handled`` | Variable | Local | [try](https://github.com/5cover/C-exCeptions/blob/main/src/exceptions.h#41)
``_finallyPending`` | Variable | Local | [try](https://github.com/5cover/C-exCeptions/blob/main/src/exceptions.h#41)
``_setjmpResult`` | Variable | Local | [try](https://github.com/5cover/C-exCeptions/blob/main/src/exceptions.h#41)
``_finally`` | Label | Local | [finally](https://github.com/5cover/C-exCeptions/blob/main/src/exceptions.h#L103)


## Contributing

I'm not a very experienced C programmer. There's probably a million ways to break this. Any feedback will be greatly appreciated.
