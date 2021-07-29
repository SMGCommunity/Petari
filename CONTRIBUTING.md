# Contributing

To contribute, you will need the following tools:

* A disassembler, IDA Pro or Ghidra work very well (you can also use a decompiler, it can make some things easier)
* devKitPro for Wii, as it includes `powerpc-eabi-as` and `powerpc-eabi-objcopy` for building
* CodeWarrior, we specifically use version 4.3 build 213 (subject to change)
* make

It is recommended that you have knowledge of the following:

* C / C++ (however, C++ is recommended)
* PowerPC Assembly
* Reverse engineering instructions from PowerPC to C / C++

Decompilers such as Hex-Rays (included in IDA Pro) are useful as they can make the decompilation easier to write.

Be sure that all of your submitted code follows the guidelines that are listed below.

If you have any questions or concerns, please join [our Discord server](https://discord.gg/k7ZKzSDsVq)!

## Guidelines

### General

* Lines should not exceed 100 characters, these can be split into multiple lines
* Use `NULL` when assigning or comparing a pointer, not `0` -- Only use `0` when assigning or comparing a value

### Headers

* Use `#pragma once` at the top of each header file
* Use forward-declared types when possible


### Includes

* Use `#include<>` for system library includes
* Use `#include "..."` for including game headers, and they must be relative to `include`

### Names

* Names for known symbols should match exactly, even including typoes in the symbol
* Member variables must be prefixed with `m`
* Arguments for functions must be prefixed with `p` for pointers or `r` for passed-by-reference
* Static variables with no known symbol must be prefixed with `s`, with globals prefixed with `g`
* Functions with no symbols (such as inlined functions), must use camelCase

### Classes

* Functions for classes must be put in this order: constructor, destructor, operators, virtual functions, member functions
    * If the virtual functions are not in the order that they are in the vtable, then the rule above can be ignored as these functions must be placed in order
* When referencing a class member, do not use `this->`, unless it is required for compilation


### Nonmatching Code
If your code does NOT match, use the `NONMATCHING` macro and include the assembly file and explain in a comment why it does not match.

### Types
If the function arguments in the symbol use `int`, do NOT use `s32`, you have to use `int`, as they do not mangle to the same symbol.