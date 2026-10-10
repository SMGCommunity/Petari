#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class OceanRingPipe;
class JUTTexture;

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

    /* 0x8C */ const OceanRingPipe* mRingPipe;
    /* 0x90 */ f32 mTexU;
    /* 0x94 */ JUTTexture* mWaterPipeIndirectTex;
    /* 0x98 */ JUTTexture* mWaterPipeHighLightTex;
    /* 0x9C */ u32 mDispListLength;
    /* 0xA0 */ u8* mDispList;
};