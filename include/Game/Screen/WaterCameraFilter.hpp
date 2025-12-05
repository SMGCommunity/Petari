#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JUtility/JUTTexture.hpp"

class WaterCameraFilter : public LiveActor {
public:
    WaterCameraFilter();

    virtual ~WaterCameraFilter();
    virtual void init(const JMapInfoIter&);
    virtual void draw() const;
    virtual void control();

    void exeAirToWater();
    void exeWaterToAir();
    void exeWater();
    void exeAir();
    void loadMaterial() const;

    inline void initScreenTex();

    f32 mWaterAlpha;  // 0x8C
    f32 mWavePhase;   // 0x90
    GXColor _94;
    JUTTexture* mScreenTex;  // 0x98
    JUTTexture* mFilterTex;  // 0x9C
};
