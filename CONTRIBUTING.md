# Contributing

Thank you for your interest in contributing! We welcome pull requests and documentation improvements from the community.

If you have any questions, feel free to join our **[Discord]** server.


# A Reminder

Super Mario Galaxy's codebase was directly built upon for Super Mario Galaxy 2. As such, a **lot** of code will end up being ported over to **[Garigari]**. We would appreciate ensuring Petari code adheres to the **current** contributing guidelines, as they may have changed from when the file was originally submitted. This will help ensure both projects are maintained properly.

---

# Before Submitting

Before opening a pull request, make sure your changes satisfy all of the following:

- Run `clang-format` on every modified file. **Formatting is required.** If using VSCode, please enable auto-formatting on save.
- Run `ninja` and ensure the object you're working on is **1:1 matching**. Non-matching objects cannot be marked as matching and will break the link step.
- Run `objdiff` to verify every function still matches the original binary.
- Once all modified functions are marked as decompiled, run `ninja -t clean` to refresh your build's cache, and then `ninja` again to verify it can successfully link.
- Keep your pull request focused on **a single object whenever possible**. Multiple objects are acceptable only when they are directly related or depend on each other.
- Every non-matching function in your pull request **must** have a `decomp.me` scratch linked in a comment at the top of the function. See **[Non-Matching](#non-matching)** for the proper format.

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
- Pointer values should **explicitly** compare to `nullptr`.
- Use decimal values instead of hex for non-enum numeric values.
- Use enum values instead of magic numbers.
- Use the `f` suffix for floating point literals, and include the trailing `.0`. (eg: `123.0f`)
- Avoid comments that do not provide insightful information. Comments should be clear and concise, and be relevant to the code, or its context within or between Super Mario Galaxy games.

## Headers

Every header should begin with:

```cpp
#pragma once
```

Prefer forward declarations whenever possible instead of adding unnecessary includes.

## Includes

Use quotes (`""`) for project headers:

```cpp
#include "Game/MapObj/HipDropRock.hpp"
```

Use angle brackets (`<>`) for library and SDK headers:

```cpp
#include <revolution/OS.h>
#include <JSystem/JGeometry.hpp>
```

Project includes should always be relative to the `include` directory. Project headers should always be included before library headers. 

## Classes

Do **not** use `this->` unless it is required for compilation.

Member functions should appear in the following order:

1. Constructors
2. Destructor
3. Operators
4. Virtual functions
5. Other member functions
6. Nerve executor functions
7. Inlines

Virtual functions **must** appear in the same order as the original vtable. Preserving the original virtual function order takes precedence over the ordering above. The only exception is for destructors, where unless required for matching, can be omitted in favor of the implicit compiler-generated destructor. Non-virtual functions in header should match appearance of functions in source where applicable.

Class inheritance and class members should be left fully public unless otherwise indicated.

Class members should be documented using the project's standard format:

```cpp
/* 0x10 */ s32 mValue;    ///< Description.
```

Do **not** manually add padding between members unless it is required for matching. The compiler will automatically pack and pad types smaller than 4 bytes. If a member variable goes unused, mark it with a comment indicating so, (eg: `///< Unused`)

Functions should also use the project's Doxygen style:

```cpp
/// @brief Does something.
/// @param pValue Pointer to a value.
/// @return True on success.
bool doThing(int* pValue);
```

If you encounter an older header that has not yet been documented, documentation improvements are always appreciated.

## Constructors

Class constructors must use initializer lists where applicable rather than initializing members directly inside the function scope. Members that are initialized to default values should be included with empty initializers rather than explicitly initializing with a default value.

```cpp
AstroDomeSky::AstroDomeSky(const char* pName) : LiveActor(pName), mBrkFrame(), mColor(0, 0, 0, 255), mProjmap() {
}
```

## Enums

Enums must include a valid `PascalCase` type name, unless otherwise indicated by official names, and enum values must include the type name prefixing the value name, and a comment detailing the raw enum value.

```cpp
enum SwitchType {
    /* 0x0 */ SwitchType_QuarterPlus,
    /* 0x1 */ SwitchType_QuarterMinus,
    /* 0x2 */ SwitchType_HalfPlus,
    /* 0x3 */ SwitchType_HalfMinus,
};
```

Be sure to check relevant cases for common enums and where they are used. Commonly used enums are in `SceneFunction.hpp`, `ActorSensorUtil.hpp`, `AudSoundId.hpp`, and many others, so be sure to become acquainted with where enums are common.

## Namespaces

Functions belonging to a namespace should be explicit when referred to in source files, and individually labelled rather than grouped into a namespace block. 

```cpp
f32 MR::getInterpolateValue(f32 t, f32 start, f32 end) {
    return start + (end - start) * t;
}

f32 MR::getLinerValue(f32 x, f32 start, f32 end, f32 max) {
    return MR::getInterpolateValue(x / max, start, end);
}
```

The only exceptions to this are Nerve definitions the anonymous namespace, which should be grouped in one block unless symbol order indicates otherwise.

```cpp
namespace {
    static const s32 sFreeBloomFlyUpStep = 15;
    static const f32 sSpeedWaitAir = 10.0f;
    ...
    static const s32 sWindRemainTime = 30;
    static const f32 sJumpSpeedY = 18.0f;
};  // namespace
```

References to elements of the anonymous namespace **must** include the `::anonymousScopeResolutionOperator` prefix. Furthermore, namespace blocks must end with a `;` to align the namespace comment. Please include the namespace comment manually if it is not automatically added.

Certain groups of namespaces are potentially static classes. Whether they are a namespace or a class should be decided on a case-by-case basis and should be discussed with the rest of the decomp team if you are creating a new one.

## Nerves

Empty/simple nerve executors should be explicitly written in source rather than header, and never marked as `inline`.

Nerve declarations normally occur in source, not in the headers, and typically stick to a standard layout of being defined in a namespace block.

```cpp
namespace NrvTreasureSpot {
    NEW_NERVE(TreasureSpotNrvWait, TreasureSpot, Wait);
    NEW_NERVE(TreasureSpotNrvEnd, TreasureSpot, End);
    NEW_NERVE(TreasureSpotNrvSpout, TreasureSpot, Spout);
};  // namespace NrvTreasureSpot
```

When getting a nerve, please use the relevant macros.

```cpp
&NrvTreasureSpot::TreasureSpotNrvSpout::sInstance; // No
GET_NERVE(TreasureSpot, TreasureSpotNrvSpout);     // Yes
```

## Naming

Known symbols and enum names must match the original names exactly, including spelling mistakes. Where applicable, the Korean debug symbol consts for `RMGK01` from the Nvidia Shield release for SMG1 should still be used / applied if missing.

Do **not** "fix" typos in symbol names.

Use the following naming conventions:

- Member variables begin with `m`
- Pointer variables begin with `p`
- Reference variables begin with `r`
- Unknown static variables begin with `s`
- Unknown globals begin with `g`
- Unknown functions use `camelCase`

For unknown functions or inlines, follow the known verbiage of the official symbols.

For basis vectors, short directional names are preferred over verbose or axis-based names unless readability is a concern (eg `mFront` over `mFrontVec`, `up` over `upVec`, `side` over `xDir`). Furthermore, the X, Y, and Z directions indicate Side, Up, and Front directions. 

## Types

If an original function uses `int`, then use `int`.

Do **not** replace it with `s32`, even if they are the same size, as they produce different mangled symbols.

Typedefs should be used in place of their raw type counterparts. For example: 
```cpp
void func(const JGeometry::TVec3<float> &, float *, unsigned long)
```
should be written instead as
```cpp
void func(const TVec3&, f32*, u32)
```

Unless logically necessary, or code patterns suggest otherwise, `JGeometry` types are preferred over `RVL` types (eg `TVec3f` over `Vec`, `TQuat4f` over `Quaternion`, `TPos3f` over `MtxPtr`, `TProj3f*` over `Mtx44Ptr`) for member and local variables. Function parameters should follow nearby code standards, and **always** adhere to the types indicated by Petari where applicable.

Because of implicit casting, explicit casts should be avoided unless necessary for `JGeometry` types passed as `RVL` types.

## Function Arguments

Many commonly-used functions have default arguments that will be applied if omitted. Do not include the raw values of these default arguments when calling functions.

```cpp
MR::isSameDirection(front, up, 0.01f); // No, since the default value is 0.01f
MR::isSameDirection(front, up);        // Yes, if default value is used
MR::isSameDirection(front, up, 0.1f);  // Yes, if non-default value is used
```

Many functions relating to the Wiimote will pass an argument indicating which Wiimote (or pad) channel is in use. Please use the relevant `WPAD` defines rather than the raw number for the pad channel (eg: `WPAD_CHAN0` for pad `0`/P1's Wiimote).

## Regional Differences

Regional differences in code should appear in order of regional release, and be contained in preprocessor blocks explicitly indicating the versions that include that code. Please keep revisional difference blocks as small and self-contained as possible.

```cpp
void LuigiMailDirector::lost() {
#if (VERSION == RMGJ01 || VERSION == RMGE01 || VERSION == RMGP01)
    mLostMessageNum++;
#elif (VERSION == RMGK01)
    mLostMessageNum = 1;
    mFoundMessageNum = 0;
#endif
}
```

Please also include a `// REGIONAL` comment nearby to indicate regional differences.

---

# Formatting

Code with directly similar purpose or logical functioning should be grouped together without extra newlines in between. However, an extra newline must be included after the end of closing brace of a scope block and any new code immediately after.

```cpp
if (isNerve(GET_NERVE(Coin, CoinNrvNonActive))) {
    MR::invalidateClipping(this);
    MR::validateHitSensors(this);
    MR::onBind(this);

    MR::calcGravityOrZero(this);
    mVelocity.set(-mGravity * 30.0f);

    setNerve(GET_NERVE(Coin, CoinNrvMove));

    return true;
}

return false;
```

Expressions should favor readability when it does not impact matching. Long single-lines with multiple inlined expressions should be broken into multiple lines, however chained function calls should remain as one line.

---

# Non-Matching, Fakematching, Unclean Code, TODOs

Please mark relevant incomplete or incorrect code that needs to be updated with the relevant comment indicator style.

## Non-Matching

Non-matching code is welcome, but it should be easy for others to continue working on.

For every non-matching function:

- Explain why the function does not currently match, if known, in a comment marked `FIXME`.
- Include a link to the corresponding decomp.me scratch directly above the function.
- Update the comment if the scratch changes or a better reproduction is found.

For example:

```cpp
void someFunction() {
    // FIXME: Stack usage due to inlines not matching
    // https://decomp.me/scratch/XXXXX
    ...
}
```

If you do not know why the function does not match, simply include the decomp.me link and omit the explanation.

## Fakematching 

Code that outputs the same byte-for-byte output when compiling, but is not how the code would have been written are considered "Fakematches". Avoid submitting these wherever possible, and if you *must* submit a fakematch or come across one in the repository, please mark it with a `// FAKEMATCH` comment. 

## Unclean Code

Much of the repository will not be clean due to the nature of a constantly evolving set of standards. Please try to keep code clean as much as possible, and if you come across code that does not meet the contributing standard, please either clean it or mark the code with a `// CLEANME` comment.

## TODOs

If something in the repository requires further investigation that does not fall under the above categories, a `// TODO` comment may also be left, with a **clear and concise** explanation of what needs to be investigated.

---

# Generated Code

Decompiler output can be helpful for understanding code.

However:

- **Do not copy and paste IDA/Ghidra output directly into the repository.**
- **Do not submit AI-generated or otherwise directly machine-generated code into the repository.**

Submissions containing obvious decompiler output or AI-generated code will be rejected.
Contributors are expected to understand the code they submit and ensure it accurately matches the original.

---

# Discord

Need help? Want to discuss with the Petari/Garigari decompilation team?

Join our Discord server via the **[Luma's Workshop]** website, and we'll be happy to answer any questions in our decompilation channel!

<!----------------------------------------------------------------------------->

[Luma's Workshop]: https://www.lumasworkshop.com/
[Discord]: #discord
[Garigari]: https://github.com/SMGCommunity/Garigari
