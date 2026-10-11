#!/usr/bin/env bash
# Syntax-only ARM sweep of Petari sources with clang (no devkitARM needed).
# usage: 3ds/check.sh src/JSystem/JMath src/JSystem/JGadget ...   (dirs or files, relative to repo root)
# Prints one line per failing file plus a summary; full diagnostics go to $OUT (default /tmp/petari_check).
set -u
cd "$(dirname "$0")/.."
OUT=${OUT:-/tmp/petari_check}
mkdir -p "$OUT"
FLAGS=(--target=arm-none-eabi -mcpu=mpcore -std=gnu++14 -fsyntax-only -ferror-limit=0
       -Wno-unknown-pragmas -Wno-return-type -Wno-register -Wno-c++11-narrowing -D__fabs=__builtin_fabs
       -DVERSION=${VERSION_NUM:-1} -DRMGJ01=0 -DRMGE01=1 -DRMGP01=2 -DRMGK01=3 -DNDEBUG=1 -DREVOLUTION
       -I include -I libs/JSystem/include -I libs/RVL_SDK/include -I libs/MSL_C/include
       -I libs/MSL_C++/include -I libs/Runtime/include -I libs/nw4r/include -I libs/RVLFaceLib/include -I 3ds/compat -include 3ds/compat/petari_compat.h)
pass=0; fail=0
while IFS= read -r f; do
  log="$OUT/$(echo "$f" | tr '/' '_').log"
  if clang "${FLAGS[@]}" "$f" >"$log" 2>&1; then pass=$((pass+1)); rm -f "$log"
  else fail=$((fail+1)); echo "FAIL $f ($(grep -c 'error:' "$log") errors)"; fi
done < <(find "$@" -name '*.cpp' | sort)
echo "pass=$pass fail=$fail (logs in $OUT)"
