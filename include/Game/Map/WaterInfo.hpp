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

    f32 mCamWaterDepth;  // 0x0
    f32 _4;
    TVec3f mSurfacePos;               // 0x8
    TVec3f mSurfaceNormal;            // 0x14
    f32 mWaveHeight;                  // 0x20
    TVec3f mStreamVec;                // 0x24
    f32 mEdgeDistance;                // 0x30
    TVec3f mEdgePos;                  // 0x34
    const WaterArea* mWaterArea;      // 0x40
    const OceanBowl* mOceanBowl;      // 0x44
    const OceanRing* mOceanRing;      // 0x48
    const OceanSphere* mOceanSphere;  // 0x4C
};
