#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class OceanRingPipe;

class OceanRingPipeOutside : public LiveActor {
public:
    OceanRingPipeOutside(const OceanRingPipe*);

    
    const OceanRing* mOceanRing;  // 0x8C
    f32 _90;
    u32 _94;
    u32 _98;
    u32 _9C;
    u32 _A0;
};