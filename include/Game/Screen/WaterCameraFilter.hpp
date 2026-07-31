#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class JUTTexture;

class WaterCameraFilter : public LiveActor {
public:
    WaterCameraFilter();

    virtual void init(const JMapInfoIter&);
    virtual void draw() const;
    virtual void control();

    void exeAirToWater();
    void exeWaterToAir();
    void exeWater();
    void exeAir();
    void loadMaterial() const;

    inline void initScreenTex();

    /* 0x8C */ f32 mWaterAlpha;
    /* 0x90 */ f32 mWavePhase;
    /* 0x94 */ GXColor _94;
    /* 0x98 */ JUTTexture* mScreenTex;
    /* 0x9C */ JUTTexture* mFilterTex;
};
