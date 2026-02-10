#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class OceanRingPipe;

class OceanRingPipeOutside : public LiveActor {
public:
    OceanRingPipeOutside(const OceanRingPipe*);

    virtual ~OceanRingPipeOutside();
    virtual void init(const JMapInfoIter&);
    virtual void movement();
    virtual void draw() const;

    void initDisplayList();
    void loadMaterial() const;
    void sendGD() const;
    
    const OceanRingPipe* mRingPipe;  // 0x8C
    f32 mTexU;                             // 0x90
    JUTTexture* mWaterPipeIndirectTex;  // 0x94
    JUTTexture* mWaterPipeHighLightTex;     // 0x98
    u32 mDispListLength;                 // 0x9C
    u8* mDispList;                          // 0xA0
};