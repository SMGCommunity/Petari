#pragma once

#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/types.h>

class OceanBowl;
class OceanRing;
class OceanSphere;
class WhirlPool;
class WhirlPoolAccelerator;
class WaterInfo;

class WaterAreaFunction {
public:
    static void entryOceanBowl(OceanBowl*);
    static void entryOceanRing(OceanRing*);
    static void entryOceanSphere(OceanSphere*);
    static void entryWhirlPool(WhirlPool*);
    static void entryWhirlPoolAccelerator(WhirlPoolAccelerator*);
    static void createWaterAreaHolder();
    static bool tryInOceanArea(const TVec3f&, WaterInfo*);
    static bool tryInWhirlPoolAccelerator(const TVec3f&, TVec3f*);
    static bool isCameraInWaterForCameraUtil();
    static f32 getCameraWaterDepth();
    static WaterInfo* getCameraWaterInfo();
    static void pauseOffWaterArea();
};
