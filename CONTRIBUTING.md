
# Contributing

We will be glad to answer any questions <br>
for people who wish to contribute.

All pull requests and issues are welcome.

<br>

### Before Submission

Be sure that all of your submitted code <br>
follows the guidelines that are listed below.

When running `ninja`, it is **REQUIRED** that the output is `1:1` for a file to be marked MATCHING (otherwise the linking sequence will fail).

After the code matches, be sure to run `objdiff` <br>
to check the functions to ensure that they are matching.

If it matches, it will automatically be marked as decompiled.

When each function you've matched has been marked as decompiled, <br>
run `ninja` to see the current percentages.

<br>

### Questions

If you have any questions or concerns, <br>
please join our **[Discord]** server.

<br>

---

<br>

## Requirements

<br>

### Tools

- A **Disassembler** / **IDA Pro** / **Ghidra**

    *You can also use a decompiler, it can make some things easier*

- **CodeWarrior**

    *We specifically use version `3.0a3`*

- **Python**

    *Version `3.7+`*

- **Any Code IDE (Visual Studio Code recommended)**
    *If you are using Visual Studio Code, `dtk` automatically creates a `compile_commands.json` file that will help with `clangd`.

<br>

### Knowledge

- **C** / **C++**

    *However **C++** is recommended*

- **PowerPC Assembly**

- **PowerPC** ➝ **C** / **C++** reverse engineering instructions

<br>

*Decompilers such as Hex-Rays (included in IDA Pro) are* <br>
*useful as they can make the decompilation easier to write.*

<br>

---

<br>

## Guidelines

<br>

### General

<br>

- `clang-format` will do a lot of the work for you. Just be sure that it is enabled to run on save.

- Use `nullptr` instead of `0` when assigning / comparing a pointer in C++ code, use `NULL` in C.

  *Only use `0` when assigning or comparing a value.*

<br>

### Headers

<br>

- Use `forward-declared` types when possible

- At the top of every header place:

    ```c++
    #pragma once
    ```


<br>

### Classes

<br>

- Follow the proper syntax for documenting a class's functions and variables:

    ```c++
    /* member-offset */     s32 varName;    ///< Description of varName.
    ```

    ```c++
    /// @brief Function does a thing.
    /// @param argumentName Is an argument to a function that does a thing.
    /// @return If the function did a thing.
    /// @note Might have not done a thing...
    bool func(s32 argumentName);
    ```

    If there is a function that does not have this implemented (ie in headers before the new documentation revamp), please take the time to help reimplement them to document this repository better!

- At the top of every header place:

    ```c++
    #pragma once
    ```


<br>

### Includes

<br>

- For system library includes use:

    ```c++
    #include <...>
    ```

- For game header includes use:

    ```c++
    #include "..."
    ```

    *These includes must be relative to the `include` folder.*

<br>

### Names

<br>

- Names for known symbols should match **exactly**, <br>
  even including typos in the symbol.

- Member variables must be prefixed with `m`.

- Arguments for functions must be prefixed with:

    - `p` for pointers

    - `r` for passed-by-reference

- Static variables with:

    - No known symbol must be prefixed with `s`

    - Global scope must be prefixed with `g`

- Functions with no symbols must use **camelCase**.

    *Such as **inlined** functions.*

<br>

### Classes

<br>

- When referencing a class member, do **not** use <br>
  `this->`, unless it is required for compilation.

- Functions for classes must be put in the following order:

    - Constructor

    - Destructor

    - Operators

    - Virtual Functions

    - Member Functions

    *If the virtual functions are not in the order that* <br>
    *they are in the **vtable**, then the rule above can be* <br>
    *ignored as these functions must be placed in order.*


<br>

### Nonmatching Code

If your code does **NOT** match, explain in a comment why it does not match.<br>
Be sure to also include a decomp.me scratch in a comment as well.

<br>

### Types

If the function arguments in the symbol use `int`, <br>
do **NOT** use `s32`, you have to use `int`, as they <br>
do not mangle to the same symbol.

<!----------------------------------------------------------------------------->

[Discord]: https://discord.gg/QnZ4cKkZm3
