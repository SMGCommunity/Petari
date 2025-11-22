#pragma once

#include "Game/LiveActor/RailRider.hpp"
#include "Game/Util/ParabolicPath.hpp"
#include <revolution.h>

class SpinDriverShootPath {
public:
    SpinDriverShootPath();

    void init(const JMapInfoIter&);
    void initUsingParabolic(const JMapInfoIter&, const TVec3f&);
    void setStartPosition(const TVec3f&);
    void calcPosition(TVec3f*, f32) const;
    void calcDirection(TVec3f*, f32, f32) const;
    void calcInitPose(TVec3f*, TVec3f*, TVec3f*, f32) const;
    f32 getTotalLength() const;
    void calcClippingInfo(TVec3f*, f32*, f32, f32);

    RailRider* mRailRider;  // 0x0
    ParabolicPath* mPath;   // 0x4
    TVec3f mStartPosition;  // 0x8
    bool mUsesParabolic;    // 0x14
};