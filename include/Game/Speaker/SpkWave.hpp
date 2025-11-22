#pragma once

#include <revolution/types.h>

struct WaveData {
    u32 mSize;
    u32 mLoopStartPos;
    u32 mLoopEndPos;
    u8* mWave;
};

class SpkWave {
public:
    SpkWave();

    void setResource(void*);
    u32 getWaveSize(s32) const;
    u32 getLoopStartPos(s32) const;
    u32 getLoopEndPos(s32) const;
    u8** getWave(s32) const;
    WaveData* getWaveData(s32) const;

    /* 0x0 */ void* mResource;  // Raw data of AudioRes/SpkRes/SpkRes.arc
};
