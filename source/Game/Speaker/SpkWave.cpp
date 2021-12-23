#include "Game/Speaker/SpkWave.h"

SpkWave::SpkWave() {
    mResource = NULL;
}

void SpkWave::setResource(void * pResource) {
    OSDisableInterrupts();
    mResource = pResource;
    OSEnableInterrupts();
}

u32 SpkWave::getWaveSize(long wave) const {
    if (mResource == NULL) {
        return 0;
    } else {
        return getWaveData(wave)->mSize;
    }
}

u32 SpkWave::getLoopStartPos(long wave) const {
    if (mResource == NULL) {
        return 0;
    } else {
        return getWaveData(wave)->mLoopStartPos;
    }
}

u32 SpkWave::getLoopEndPos(long wave) const {
    if (mResource == NULL) {
        return 0;
    } else {
        return getWaveData(wave)->mLoopEndPos;
    }
}

u8** SpkWave::getWave(long wave) const {
    if (mResource == NULL) {
        return 0;
    } else {
        return &getWaveData(wave)->mWave;
    }
}

WaveData* SpkWave::getWaveData(long wave) const {
    return (WaveData*)((u32)mResource + *(u32*)((u32)mResource + wave * 4 + 8));
}