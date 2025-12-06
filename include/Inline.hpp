#pragma once

// Macro which is put after the function definition (only in the header) to prevent the function from being inlined.
// Example: void someFunction(int someArg) const NO_INLINE;
#define NO_INLINE __attribute__((noinline))

// Macros which should be used when a function is both inlined and not inlined (so it's auto-inlined by the compiler).
// This is used to declare the inline version of the function. If not a constructor, the non-inline version
// should call the inline version.
// Example:
// int someFunction(int SomeArg)
// inline int INLINE_FUNC_DECL(someFunction, int someArg);
// CALL_INLINE_FUNC(someFunction, 0);
#define INLINE_FUNC_DECL(name, ...) name(void*****, __VA_ARGS__)
#define INLINE_FUNC_DECL_NO_ARG(name) name(void*****)

#define CALL_INLINE_FUNC(name, ...) name((void*****)0, __VA_ARGS__)
#define CALL_INLINE_FUNC_NO_ARG(name) name((void*****)0)
