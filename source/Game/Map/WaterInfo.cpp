#include "Game/Map/WaterInfo.h"

WaterInfo::WaterInfo() {
    clear();
}

bool WaterInfo::isInWater() const {
    if (mWaterArea || mOceanBowl || mOceanRing || mOceanSphere) {
        return true;
    }
    
    return false;
}

void WaterInfo::clear() {
    mCamWaterDepth = 0.0f;
    _4 = 0.0f;
    _8.zero();
    _14 = 0.0f;
    _18 = 1.0f;
    _1C = 0.0f;
    _20 = 0.0f;
    _24.zero();
    _30 = -1.0f;
    _34.zero();
    mWaterArea = NULL;
    mOceanBowl = NULL;
    mOceanRing = NULL;
    mOceanSphere = NULL;
}