#pragma once

#include <JSystem/JGeometry.h>

class WaterArea;
class OceanBowl;
class OceanRing;
class OceanSphere;

class WaterInfo {
public:
    WaterInfo();

    bool isInWater() const;
    void clear();

    f32 mCamWaterDepth;                 // _0
    f32 _4;
    TVec3f mSurfacePos;                 // _8
    TVec3f mSurfaceNormal;              // _14
    f32 mWaveHeight;                    // _20
    TVec3f mStreamVec;                  // _24
    f32 mEdgeDistance;                  // _30
    TVec3f mEdgePos;                    // _34
    const WaterArea* mWaterArea;        // _40
    const OceanBowl* mOceanBowl;        // _44
    const OceanRing* mOceanRing;        // _48
    const OceanSphere* mOceanSphere;    // _4C
};
