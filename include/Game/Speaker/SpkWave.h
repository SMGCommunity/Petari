#pragma once

#include "revolution.h"

class SpkWave {
public:
    SpkWave();

    void setResource(void *resource);
    u32 getWaveSize(long wave) const;
    u32 getLoopStartPos(s64 wave) const;
    u32 getLoopEndPos(s64 wave) const;
    u32 getWave(s64 wave) const;
    u32* getWaveData(long wave) const;

    void *resource;
};