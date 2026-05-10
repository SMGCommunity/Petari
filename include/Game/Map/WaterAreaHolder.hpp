#pragma once

#include "Game/Map/WaterInfo.hpp"
#include "Game/NameObj/NameObj.hpp"

#include <JSystem/JGeometry/TVec.hpp>

class OceanBowl;
class OceanRing;
class OceanSphere;
class WhirlPool;
class WhirlPoolAccelerator;
class WaterCameraFilter;

class WaterAreaHolder : public NameObj {
public:
    WaterAreaHolder();

    virtual void movement();

    void entryOceanBowl(OceanBowl*);
    void entryOceanRing(OceanRing*);
    void entryOceanSphere(OceanSphere*);
    void entryWhirlPool(WhirlPool*);
    void entryWhirlPoolAccelerator(WhirlPoolAccelerator*);

    /* 0x0C */ s32 mNumOceanBowls;
    /* 0x10 */ OceanBowl** mOceanBowls;
    /* 0x14 */ s32 mNumOceanRings;
    /* 0x18 */ OceanRing** mOceanRings;
    /* 0x1C */ s32 mNumOceanSpheres;
    /* 0x20 */ OceanSphere** mOceanSpheres;
    /* 0x24 */ s32 mNumWhirlPools;
    /* 0x28 */ WhirlPool** mWhirlPools;
    /* 0x2C */ s32 mNumWhirlPoolAccelerators;
    /* 0x30 */ WhirlPoolAccelerator** mWhirlPoolAccelerators;
    /* 0x34 */ bool mCamInWater;
    /* 0x38 */ WaterInfo mWaterInfo;
    /* 0x88 */ WaterCameraFilter* mCameraFilter;
    /* 0x8C */ bool mUseBloom;
};

namespace WaterAreaFunction {
    void entryOceanBowl(OceanBowl*);
    void entryOceanRing(OceanRing*);
    void entryOceanSphere(OceanSphere*);
    void entryWhirlPool(WhirlPool*);
    void entryWhirlPoolAccelerator(WhirlPoolAccelerator*);
    void createWaterAreaHolder();
    bool tryInOceanArea(const TVec3f&, WaterInfo*);
    bool tryInWhirlPoolAccelerator(const TVec3f&, TVec3f*);
    bool isCameraInWaterForCameraUtil();
    f32 getCameraWaterDepth();
    WaterInfo* getCameraWaterInfo();
    void pauseOffWaterArea();
};  // namespace WaterAreaFunction
