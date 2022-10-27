#pragma once

#include "JSystem/JGeometry.h"

class WaterArea;
class OceanBowl;
class OceanRing;
class OceanSphere;

class WaterInfo {
public:
    WaterInfo();

    bool isInWater() const;
    void clear();

    f32 mCamWaterDepth;              // _0
    f32 _4;
    TVec3f _8;
    f32 _14;
    f32 _18;
    f32 _1C;
    f32 _20;
    TVec3f _24;
    f32 _30;
    TVec3f _34;
    const WaterArea* mWaterArea;     // _40
    const OceanBowl* mOceanBowl;     // _44
    const OceanRing* mOceanRing;     // _48
    const OceanSphere* mOceanSphere; // _4C
};
