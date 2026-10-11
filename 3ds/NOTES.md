# Petari -> New 3DS: bring-up notes

Target: New 3DS / New 2DS XL (ARM11 MPCore, armv6k, ~124 MB app RAM). Old 3DS is not realistic.
Status date: 2026-10-10. Decomp base: SMGCommunity/Petari (shallow clone).

## What exists
- `3ds/Makefile`   devkitARM + libctru + citro3d build. `SRCS` lists files known to build; grow it.
                   `GAME_VERSION_NUM` picks the region (0 JPN, 1 USA, 2 PAL, 3 KOR).
- `3ds/source/main.cpp`  on-device sanity test for the ported JMath routines (compare with the expected values it prints).
- `3ds/check.sh`   syntax-only ARM sweep with clang (works without devkitARM). `3ds/check.sh src/Game` etc.
- `3ds/compat/`    force-included compat header (`__declspec`, INFINITY, NAN) and a case-fix shim for
                   `JASDSPInterface.hpp` (the real file is `JASDspInterface.hpp`; only matters on Linux).

## Syntax-check results (ARM, clang -fsyntax-only, USA version, MSL headers)
| Tree          | Before | After |
|---------------|--------|-------|
| src/Game      | 83 / 1606  | 1576 / 1606 |
| src/JSystem   | 41 / 169   | 166 / 169 |
| src/nw4r      | 10 / 29    | 28 / 29 |
This only proves the code *parses* for ARM. Nothing here has been linked or run, and the 3DS Makefile has not been
run against a real devkitARM yet (gcc may report things clang did not).

## Changes made to upstream files (all guarded so PPC/MWCC output is unchanged)
- Heap `operator new` overloads (JKRHeap.hpp/.cpp) were declared only under `__MWERKS__`; now always declared, with `size_t`.
  Reason: decomp `u32` is `unsigned long`, but ARM `size_t` is `unsigned int`. Same for JASHeapCtrl.hpp, MSL `new`,
  and `MSL_C/__internal/__size_t.h`.
- Two-phase template lookup (MWCC accepts, GCC/clang do not): TMatrix.hpp (`this->get`), TVec.hpp (`this->template set`),
  JASGadget.hpp (`JASPtrTable< T >(...)`), JMATrigonometric.hpp (declare `atan2_` specialization before first use).
- Empty `#ifdef __MWERKS__` bodies replaced with real C fallbacks: JMath.hpp/.cpp (FastSqrt, Hermite, QuatLerp,
  VECLerp, VECScaleAdd), nw4r arithmetic.h (FAbs, FSelect), OSFastCast.h (u16<->f32), MathUtil.cpp (JMASqrt).
  **These were hand-decoded from the PPC asm and are unverified on hardware** - run `main.cpp` and check.
- PPC-only varargs: MSL `va_list.h`, `stdarg.h`, `cstdarg` use compiler built-ins off MWCC; `ReplaceTagProcessor.cpp` uses `va_copy`.
- Misc: J3DTevs.hpp (`J3DTevsDefault` is a matching hack with C-linkage globals; aliased instead), LinkList.h (`friend Self;`),
  `0f` float literals in MainLoopFramework.cpp, switch-case scoping in JKRMemArchive.cpp, runtime.h asm decls guarded.
- Flags: `-std=gnu++14`, `-Wno-register`, `-Wno-narrowing` (CodeWarrior allows `-1` into `u32` tables), `-DVERSION=...`.

## Remaining syntax blockers (~35 files)
- Top-level PPC `asm` functions that need real C ports: J3DGraphBase/J3DTransform.cpp (4 funcs), J3DGraphAnimator/J3DMtxBuffer.cpp.
  The paired-single (`psq_*`/`ps_*`) code is the bulk of the remaining asm: ~1,400 lines in ~51 files, mostly RVL_SDK/mtx
  (replace with libctru `mtx_*`/plain C) and J3D.
- `switch` jumping over initialisations: Game/System/GameEventFlagChecker.cpp, Game/Util/JMapInfo.cpp (add braces).
- `Game/Util/DemoUtil.cpp:602`, `ResourceHolderManager.cpp:89/94`, `GalaxyMapController.cpp:107` (Functor / member-pointer misuse
  that MWCC tolerates), `GameSystem.cpp:48` (`main` signature), `ActorMovementUtil.cpp` (missing `Game/Animation/Xanimecore.hpp`).
- nw4r/db/db_assert.cpp, JUTException.cpp:191, src/Runtime/*: CodeWarrior runtime/debug code. Exclude from the 3DS build.

## What "working" still requires (none of this is started)
1. **Real build**: get devkitARM/gcc compiling the SRCS list (clang's pass is not a guarantee). Then grow SRCS tree by tree.
2. **Endianness**: every game data file (BCSV, BMD/J3D, BCK, ARC/RARC, SZS, layouts) is big-endian. Byte-swap at load.
3. **RVL_SDK shims** (src/RVL_SDK is the PPC SDK itself - do not compile it; replace it):
   - DVD/ARC file I/O -> stdio on the SD card (player's own game dump; ship no Nintendo assets)
   - OS (alloc, threads, time) -> libctru; keep a fixed heap that fits in ~120 MB
   - PAD/KPAD/WPAD -> libctru HID. Wii Remote pointer (star bits) -> touch screen; Z/C -> shoulder buttons
   - AI/AX/DSP audio -> ndsp (Galaxy's JAudio2 is large; expect to stub audio first)
4. **GX -> citro3d**: ~81 files call GX directly. The Wii TEV pipeline does not map 1:1 onto PICA200 combiners;
   expect reduced materials/effects. Start with a model viewer + one test galaxy before the full game.
5. **Performance/memory**: New 3DS ARM11 is far weaker than the Wii's Broadway + GPU. Plan for frame-rate and RAM cuts.

Also: Petari's own AI policy bans AI-generated *decompilation* contributions. This port work is separate from decomp,
but if you upstream anything, say how it was written.
