# Contributing

Thank you for your interest in contributing! We welcome pull requests and documentation improvements from the community.

If you have any questions, feel free to join our **[Discord]** server.

---

# Before Submitting

Before opening a pull request, make sure your changes satisfy all of the following:

- Run `clang-format` on every modified file. **Formatting is required.**
- Run `ninja` and ensure the object you're working on is **1:1 matching**. Non-matching objects cannot be marked as matching and will break the link step.
- Run `objdiff` to verify every function still matches the original binary.
- Once all modified functions are marked as decompiled, run `ninja` again to verify it can successfully link.
- Keep your pull request focused on **a single object whenever possible**. Multiple objects are acceptable only when they are directly related or depend on each other.
- Every non-matching function in your pull request **must** have a decomp.me scratch linked in a comment directly above the function.

---

# Requirements

## Tools

Recommended tools:

- **IDA Pro**, **Ghidra**, or another PowerPC disassembler
  - A decompiler (such as Hex-Rays) can help understand code, but its output should never be copied directly.
- **Python 3.7+**
- A code editor (Visual Studio Code is recommended)

When using Visual Studio Code, `dtk` automatically generates a `compile_commands.json` file for `clangd`.

## Knowledge

Contributors should be familiar with:

- C and C++
- PowerPC assembly
- PowerPC reverse engineering

---

# Coding Guidelines

## General

- Run `clang-format` before submitting any changes.
- Use `nullptr` instead of `0` for pointers in C++.
- Use `NULL` for pointers in C.
- Use `0` only for numeric values.
- Avoid humorous comments, jokes, or commentary about typos, compiler behavior, or reverse engineering discoveries. Comments should remain professional and describe the code, not the contributor's thought process.

## Headers

Every header should begin with:

```cpp
#pragma once
```

Prefer forward declarations whenever possible instead of adding unnecessary includes.

## Includes

Use angle brackets (`<>`) for library and SDK headers:

```cpp
#include <revolution/OS.h>
#include <JSystem/JGeometry.hpp>
```

Use quotes (`""`) for project headers:

```cpp
#include "Game/MapObj/HipDropRock.hpp"
```

Project includes should always be relative to the `include` directory.

## Classes

Do not use `this->` unless it is required for compilation.

Member functions should appear in the following order:

1. Constructors
2. Destructor
3. Operators
4. Virtual functions
5. Other member functions

Virtual functions **must** appear in the same order as the original vtable. Preserving the original virtual function order takes precedence over the ordering above.

Class members should be documented using the project's standard format:

```cpp
/* 0x10 */ s32 mValue;    ///< Description.
```

Functions should also use the project's Doxygen style:

```cpp
/// @brief Does something.
/// @param pValue Pointer to a value.
/// @return True on success.
bool doThing(int* pValue);
```

If you encounter an older header that has not yet been documented, documentation improvements are always appreciated.

## Naming

Known symbols must match the original names exactly, including spelling mistakes.

Do **not** "fix" typos in symbol names.

Use the following naming conventions:

- Member variables begin with `m`
- Pointer parameters begin with `p`
- Reference parameters begin with `r`
- Unknown static variables begin with `s`
- Unknown globals begin with `g`
- Unknown functions use `camelCase`

## Types

If an original function uses `int`, then use `int`.

Do **not** replace it with `s32`, even if they are the same size, as they produce different mangled symbols.

---

# Non-Matching Code

Non-matching code is welcome, but it should be easy for others to continue working on.

For every non-matching function:

- Explain why the function does not currently match, if known.
- Include a link to the corresponding decomp.me scratch directly above the function.
- Update the comment if the scratch changes or a better reproduction is found.

For example:

```cpp
// NON_MATCHING
// decomp.me: https://decomp.me/scratch/XXXXX
// Stack usage due to inlines not matching

void someFunction() {
    ...
}
```

If you do not know why the function does not match, simply include the decomp.me link and omit the explanation.

---

# Generated Code

Decompiler output and AI tools can be helpful for understanding code, suggesting variable names, or improving the readability of existing C++ code.

However:

- **Do not copy and paste IDA output directly into the repository.**
- **Do not copy and paste AI-generated code directly into the repository.**

Submissions containing obvious decompiler output or AI-generated code will be rejected.

AI is perfectly acceptable for tasks such as:

- Suggesting better variable or function names.
- Cleaning up existing C++ code.
- Explaining PowerPC assembly.
- Answering reverse engineering questions.

Contributors are expected to understand the code they submit and ensure it accurately matches the original.

---

# Questions

Need help?

Join our **[Discord]** server and we'll be happy to answer any questions in our decompilation channel!

<!----------------------------------------------------------------------------->

[Discord]: https://discord.gg/ZxEqyYeZbf
