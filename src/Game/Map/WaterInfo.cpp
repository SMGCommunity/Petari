#include "Game/Map/WaterInfo.hpp"

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
    mSurfacePos.zero();
    mSurfaceNormal.x = 0.0f;
    mSurfaceNormal.y = 1.0f;
    mSurfaceNormal.z = 0.0f;
    mWaveHeight = 0.0f;
    mStreamVec.zero();
    mEdgeDistance = -1.0f;
    mEdgePos.zero();
    mWaterArea = nullptr;
    mOceanBowl = nullptr;
    mOceanRing = nullptr;
    mOceanSphere = nullptr;
}