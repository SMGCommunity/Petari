#include "Game/Speaker/SpkTable.hpp"

SpkTable::SpkTable() {
    mInitialized = false;
    mResourceCount = 0;
    mParameters = nullptr;
    mNames = nullptr;
}

void SpkTable::setResource(void* pRes) {
    /* FIXME: regswap */
    mInitialized = false;

    SpkFile* file = reinterpret_cast< SpkFile* >(pRes);
    u32 parameterOffset = file->parametersOffset;

    const char** names = (const char**)((u8*)file + file->namesOffset);

    u32 initialized = file->initialized;

    mResourceCount = file->count;
    mParameters = (SpkParameters*)((u8*)file + parameterOffset);

    if (initialized == 0) {
        for (s32 idx = 0; idx < mResourceCount; idx++) {
            names[idx] += reinterpret_cast< u32 >(file);
        }
    }

    mNames = names;

    file->initialized = 1;
    mInitialized = true;
}
