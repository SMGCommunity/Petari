#include "Game/Util/LingcodPatch.h"
#include <revolution/os.h>

const char* lingcod_getIniKeyValue(const char* pSection, const char* pKey) {
    OSReport("lingcod_getIniKeyValue(%s,%s)\n", pSection, pKey);
    return NULL;
}

s16 lingcod_getIniKeyValueInt16(const char* pSection, const char* pKey, s16 defaultValue) {
    return defaultValue;
}

int lingcod_fireGameEventEx() {
    return 0;
}

void lingcod_revalidateNVSI() {
}

f32 lingcod_getBloomScale() {
    return 1.0f;
}

f32 lingcod_getBloomIntensity() {
    return 1.0f;
}
