#pragma once

#include "revolution.h"

struct WaveData {
    u32 mSize;
    u32 mLoopStartPos;
    u32 mLoopEndPos;
    u8* mWave;
};

class SpkWave {
public:
    SpkWave();

    void setResource(void * pResource);
    u32 getWaveSize(long wave) const;
    u32 getLoopStartPos(long wave) const;
    u32 getLoopEndPos(long wave) const;
    u8** getWave(long wave) const;
    WaveData* getWaveData(long wave) const;

    void* mResource; // Is this some specific type of resource?
};