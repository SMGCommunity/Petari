#pragma once

#include <JSystem/JGeometry.hpp>

class WaterArea;
class OceanBowl;
class OceanRing;
class OceanSphere;

class WaterInfo {
public:
    WaterInfo();

    bool isInWater() const;
    void clear();

    const TVec3f& getSurfacePos() const {
        return mSurfacePos;
    }

    /* 0x0 */ f32 mCamWaterDepth;
    /* 0x4 */ f32 _4;
    /* 0x8 */ TVec3f mSurfacePos;
    /* 0x14 */ TVec3f mSurfaceNormal;
    /* 0x20 */ f32 mWaveHeight;
    /* 0x24 */ TVec3f mStreamVec;
    /* 0x30 */ f32 mEdgeDistance;
    /* 0x34 */ TVec3f mEdgePos;
    /* 0x40 */ const WaterArea* mWaterArea;
    /* 0x44 */ const OceanBowl* mOceanBowl;
    /* 0x48 */ const OceanRing* mOceanRing;
    /* 0x4C */ const OceanSphere* mOceanSphere;
};
