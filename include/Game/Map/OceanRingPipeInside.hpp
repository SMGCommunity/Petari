#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class OceanRingPipe;
class JUTTexture;

class OceanRingPipeInside : public LiveActor {
public:
    OceanRingPipeInside(const OceanRingPipe*);

    virtual ~OceanRingPipeInside();
    virtual void init(const JMapInfoIter&);
    virtual void movement();
    virtual void draw() const;

    void initDisplayList();
    void loadMaterial() const;
    void sendGD() const;
    
    const OceanRingPipe* mRingPipe;  // 0x8C
    f32 mTexU0; // 0x90
    f32 mTexV0; // 0x94
    f32 mTexU1; // 0x98
    f32 mTexV1; // 0x9C
    JUTTexture* mWaterPipeInsideTex; // 0xA0
    u32 mDispListLength; // 0xA4
    u8* mDispList; // 0xA8
};