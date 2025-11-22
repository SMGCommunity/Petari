#include "Game/Speaker/SpkWave.hpp"
#include <revolution/os.h>

SpkWave::SpkWave()
    : mResource(nullptr) {
}

void SpkWave::setResource(void* pResource) {
    BOOL status = OSDisableInterrupts();
    mResource = pResource;
    OSRestoreInterrupts(status);
}

u32 SpkWave::getWaveSize(s32 wave) const {
    if (mResource == nullptr) {
        return 0;
    }

    return getWaveData(wave)->mSize;
}

u32 SpkWave::getLoopStartPos(s32 wave) const {
    if (mResource == nullptr) {
        return 0;
    }

    return getWaveData(wave)->mLoopStartPos;
}

u32 SpkWave::getLoopEndPos(s32 wave) const {
    if (mResource == nullptr) {
        return 0;
    }

    return getWaveData(wave)->mLoopEndPos;
}

u8** SpkWave::getWave(s32 wave) const {
    if (mResource == nullptr) {
        return nullptr;
    }

    return &getWaveData(wave)->mWave;
}

WaveData* SpkWave::getWaveData(s32 wave) const {
    return (WaveData*)((u32)mResource + *(u32*)((u32)mResource + wave * 4 + 8));
}
