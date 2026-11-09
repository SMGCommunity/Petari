#pragma once

#include <revolution/types.h>

// Hooks for Lingcod, NVIDIA's Wii emulator for the Shield TV release
// The game calls these stubs and the emulator replaces them with native code

#ifdef __cplusplus
extern "C" {
#endif

const char* lingcod_getIniKeyValue(const char* pSection, const char* pKey);
s16 lingcod_getIniKeyValueInt16(const char* pSection, const char* pKey, s16 defaultValue);
int lingcod_fireGameEventEx();
void lingcod_revalidateNVSI();
f32 lingcod_getBloomScale();
f32 lingcod_getBloomIntensity();

#ifdef __cplusplus
}
#endif
