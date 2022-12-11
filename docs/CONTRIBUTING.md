
# Contributing

We will be glad to answer any questions <br>
for people who wish to contribute.

All pull requests and issues are welcome.

<br>

### Before Submission

Be sure that all of your submitted code <br>
follows the guidelines that are listed below.

When running `build.py`, it is **REQUIRED** that the output is `1:1`.

After the code matches, be sure to run `scripts/check.py` <br>
to check the function that you want to mark.

If it matches, it will automatically be marked as decompiled.

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

    *If you are using Visual Studio Code, add this to* `c_cpp_properties.json`:

     ```json
    {
        "configurations": [
            {
                "name": "Win32",
                "includePath": [
                    "${workspaceFolder}/**",
                    "${workspaceFolder}/libs/JSystem/include",
                    "${workspaceFolder}/libs/MetroTRK/include",
                    "${workspaceFolder}/libs/MSL_C/include",
                    "${workspaceFolder}/libs/nw4r/include",
                    "${workspaceFolder}/libs/Runtime/include",
                    "${workspaceFolder}/libs/RVL_SDK/include",
                    "${workspaceFolder}/libs/RVLFaceLib/include"
                ],
                "defines": [
                    "_DEBUG",
                    "UNICODE",
                    "_UNICODE",
                    "MTX_USE_PS",
                    "GEKKO",
                ],
                "cStandard": "c99",
                "cppStandard": "c++98",
                "intelliSenseMode": "windows-msvc-x64"
            }
        ],
        "version": 4
    }
    ```

    *This setup will help you include each library without compiler error issues from Intellisense.*

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

- Lines should not exceed `100` characters, <br>
  these can be split into multiple lines.
  
- Use `NULL` instead of `0` when assigning / comparing a pointer.
  
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
    Note that the documentation website only updates every 15 minutes, so wait up to the nearest 15 minutes after your pull request has been merged. However, you can preview your changes before merging by running `doxygen`.

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
  `this ->`, unless it is required for compilation.

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

If your code does **NOT** match, use the <br>
`NONMATCHING` macro, and explain in a <br>
comment why it does not match.

<br>

### Types

If the function arguments in the symbol use `int`, <br>
do **NOT** use `s32`, you have to use `int`, as they <br>
do not mangle to the same symbol.

<!----------------------------------------------------------------------------->

[Discord]: https://discord.gg/k7ZKzSDsVq
