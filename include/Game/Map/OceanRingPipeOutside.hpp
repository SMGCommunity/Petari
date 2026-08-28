#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class OceanRingPipe;
class JUTTexture;

class OceanRingPipeOutside : public LiveActor {
public:
    OceanRingPipeOutside(const OceanRingPipe*);

    /* 0x08 */ virtual ~OceanRingPipeOutside();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x14 */ virtual void movement();
    /* 0x18 */ virtual void draw() const;

    void initDisplayList();
    void loadMaterial() const;
    void sendGD() const;

    const OceanRingPipe* mRingPipe;      // 0x8C
    f32 mTexU;                           // 0x90
    JUTTexture* mWaterPipeIndirectTex;   // 0x94
    JUTTexture* mWaterPipeHighLightTex;  // 0x98
    u32 mDispListLength;                 // 0x9C
    u8* mDispList;                       // 0xA0
};