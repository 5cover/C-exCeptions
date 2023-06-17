# C-exCeptions

Static library containing macros for throwing and handling exceptions in C, in a ``try``/``catch``/``finally``-like fashion, along with a basic implementation of a stack.
I tried my best to reproduce the C# exception handling syntax, but it cannot be perfect due to the limitations of macros.

## How to use

See the comments in [example](https://github.com/5cover/C-exCeptions/blob/main/example/main.c).

## Reserved identifiers

All reserved identifiers are prefixed by ``_ex_``.

## Contributing

This is mostly an experiment with advanced macro features, ``setjmp`` and ``longjmp`` interfaces.
