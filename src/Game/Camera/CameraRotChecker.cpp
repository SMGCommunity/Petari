#include "Game/Camera/CameraRotChecker.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/MathUtil.hpp"

namespace {
    static f32 sThreshold = JGeometry::TUtil< f32 >::PI() / 180.0f;  // FIXME
};                                                                   // namespace

CameraRotChecker::CameraRotChecker() {
    mMtx.identity();
}

void CameraRotChecker::update() {
    TPos3f mtx;
    mtx.concat(MR::getCameraViewMtx(), mMtx);

    TVec3f axis;
    mIsRotatingHard = MR::abs(mtx.getRotate(axis)) > ::sThreshold;

    mMtx.set(MR::getCameraInvViewMtx());
}
