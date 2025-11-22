#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class OceanRing;

class OceanRingPipe : public LiveActor {
public:
    OceanRingPipe(const OceanRing*, f32, f32);

    virtual ~OceanRingPipe();
    virtual void init(const JMapInfoIter&);
    virtual void movement();

    const OceanRing* mOceanRing;  // 0x8C
    u8 _90;
    u32 _94;
    u32 _98;
    u32 _9C;
    u32 _A0;
    u32 _A4;
    f32 _A8;
    f32 _AC;
    u32 _B0;
    u32 _B4;
};
