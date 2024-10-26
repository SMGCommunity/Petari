#include "Game/Map/OceanRing.hpp"

void OceanRing::initAfterPlacement() {
    for (s32 i = 0; i < mWaterPointNum; i++) {
        mWaterPoints[i]->initAfterPlacement();
    }
}

bool OceanRing::isInWater(const TVec3f &rVec) const {
    if (!mClippingBox.intersectsPoint(rVec)) {
        return false;
    } 

    TVec3f nearPos;
    f32 pos = calcNearestPos(rVec, &nearPos, nullptr, nullptr);
    f32 w = (mWidthMax * calcCurrentWidthRate(pos));

    if (PSVECDistance(nearPos.toCVec(), rVec.toCVec()) > w) {
        return false;
    }

    TVec3f stack_14(rVec);
    stack_14.subtract(nearPos);
    TVec3f gravVec;
    MR::calcGravityVector(this, rVec, &gravVec, nullptr, 0);
    return !(stack_14.dot(gravVec) < 0.0f);
}