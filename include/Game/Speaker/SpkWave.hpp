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

    void setResource(void *);
    u32 getWaveSize(long) const;
    u32 getLoopStartPos(long) const;
    u32 getLoopEndPos(long) const;
    u8** getWave(long) const;
    WaveData* getWaveData(long) const;

    void* mResource;    // 0x0, Raw data of AudioRes/SpkRes/SpkRes.arc
};