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

    /* 0x8C */ const OceanRingPipe* mRingPipe;
    /* 0x90 */ f32 mTexU0;
    /* 0x94 */ f32 mTexV0;
    /* 0x98 */ f32 mTexU1;
    /* 0x9C */ f32 mTexV1;
    /* 0xA0 */ JUTTexture* mWaterPipeInsideTex;
    /* 0xA4 */ u32 mDispListLength;
    /* 0xA8 */ u8* mDispList;
};
