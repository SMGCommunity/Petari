#include "Game/Speaker/SpkTable.hpp"

SpkTable::SpkTable() {
    mInitialized = false;
    mResourceCount = 0;
    mParameters = nullptr;
    mNames = nullptr;
}

void SpkTable::setResource(void* pRes) {
    mInitialized = false;

    s32* cursor = (s32*)pRes;

    s32 resourceCount = *cursor++;
    s32 entryOff = *cursor++;
    s32 dataOffsetsStartOff = *cursor++;
    s32* pIsDataOffsetsInitialized = cursor;
    BOOL isDataOffsetsInitialized = *cursor++;

    mResourceCount = resourceCount;

    SpkParameters* entryOffset = (SpkParameters*)((s32)pRes + entryOff);
    mParameters = entryOffset;
    const char** names = (const char**)((s32)pRes + dataOffsetsStartOff);
    if (!isDataOffsetsInitialized) {
        for (s32 i = 0; i < mResourceCount; i++) {
            names[i] += (s32)pRes;
        }
    }

    mNames = names;
    *pIsDataOffsetsInitialized = TRUE;
    mInitialized = true;
}
