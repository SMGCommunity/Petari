#include "Game/Speaker/SpkTable.hpp"

SpkTable::SpkTable() {
    mInitialized = false;
    mResourceCount = 0;
    mParameterOffsets = nullptr;
    mNameOffsets = nullptr;
}

void SpkTable::setResource(void* pRes) {
    mInitialized = false;

    SpkFile* file = reinterpret_cast< SpkFile* >(pRes);
    u32 parameterOffset = file->parametersOffset;

    u32* nameOffsets = (u32*)((u8*)pRes + file->namesOffset);

    u32 initialized = file->initialized;

    mResourceCount = file->count;
    mParameterOffsets = (u32*)((u8*)pRes + parameterOffset);

    if (initialized == 0) {
        for (s32 idx = 0; idx < mResourceCount; idx++) {
            nameOffsets[idx] += (u32)pRes;
        }
    }

    mNameOffsets = nameOffsets;

    file->initialized = 1;
    mInitialized = true;
}
