#include "Game/Ride/SpherePadController.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TQuat.hpp"
#include "JSystem/JGeometry/TUtil.hpp"
#include "revolution/wpad.h"

template <>
void TRot3f::setRotate(const TVec3f& v1, const TVec3f& v2) {
    // warning, does not match because of the quaternion rotation inline
    // though logic is correct.
    TQuat4f q;
    q.setRotate(v1, v2);
    this->setRotateQuaternionInline(q);
}

SpherePadController::SpherePadController()
    : SphereController() {
}

f32 SpherePadController::calcMoveVector(TVec3f* v1, const TVec3f& v2) {
    return calcDirSphereMove(v1, v2, 0);
}

f32 SpherePadController::calcJumpPower() const {
    if (MR::testSystemTriggerA()) {
        return 1.0f;
    }
    return 0.0f;
}

void SpherePadController::update(const TVec3f&) {
}

void SpherePadController::clacXY(f32* x, f32* y) {
    *x = MR::getSubPadStickX(WPAD_CHAN0);
    *y = MR::getSubPadStickY(WPAD_CHAN0);
}

f32 SpherePadController::calcDirSphereMove(TVec3f* v1, const TVec3f& v2, u32 u1) {
    f32 x = 0.0f;
    f32 y = 0.0f;
    clacXY(&x, &y);

    f32 mag = x * x + y * y;
    mag = JGeometry::TUtil<f32>::sqrt(mag);

    if (mag >= 0.0001f) {
        TRot3f invMtx = *MR::getCameraInvViewMtx();

        TVec3f dirX;
        TVec3f dirY;
        TVec3f dirZ;

        invMtx.getXDirInline(dirX);
        invMtx.getYDirInline(dirY);
        invMtx.getZDirInline(dirZ);

        MR::normalizeOrZero(&dirX);
        MR::normalizeOrZero(&dirY);
        MR::normalizeOrZero(&dirZ);

        if ((u1 != 0 && dirZ.dot(v2) < 0.0f) || (u1 == 0 && dirY.dot(-v2) < 0.0f)) {
            dirX = -dirX;
            dirY = -dirY;
            dirZ = -dirZ;
        }

        TRot3f rotMtx;
        if (u1 != 0) {
            rotMtx.setRotate(dirZ, v2);
            rotMtx.mult33(dirX);
            rotMtx.mult33(dirY);
            v1->set(-dirX * x - dirY * y);
        } else {
            rotMtx.setRotate(dirY, -v2);
            rotMtx.mult33(dirX);
            rotMtx.mult33(dirZ);
            v1->set(dirX * x - dirZ * y);
        }
        MR::separateScalarAndDirection(&mag, v1, *v1);
    } else {
        v1->z = 0.0f;
        v1->y = 0.0f;
        v1->x = 0.0f;
    }

    if (mag > 1.0f) {
        mag = 1.0f;
    }

    return mag;
}
