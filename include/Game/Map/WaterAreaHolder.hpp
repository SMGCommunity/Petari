#pragma once

#include <revolution.h>

class OceanRing;

class WaterAreaFunction {
public:
    static void createWaterAreaHolder();

    static f32 getCameraWaterDepth();

    static bool isCameraInWaterForCameraUtil();

    static void entryOceanRing(OceanRing *);
};
