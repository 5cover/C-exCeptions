# C-exCeption

Static library containing macros for throwing and handling exceptions in C, in a ``try``/``catch``/``finally``-like fashion, along with a basic implementation of a stack.
I tried my best to reproduce the C# exception handling syntax, but it cannot be perfect due to the limitations of macros.

## How to use

See the [example](https://github.com/5cover/C-exCeptions/blob/main/example/main.c).

## Reserved identifiers
> **Warning** The reserved identifiers are implementation-defined. Client code should not use them.

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
