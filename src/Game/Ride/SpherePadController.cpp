#include "Game/Ride/SpherePadController.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>
#include <JSystem/JGeometry/TQuat.hpp>
#include <JSystem/JGeometry/TUtil.hpp>
#include <revolution/wpad.h>

SpherePadController::SpherePadController() : SphereController() {
}

f32 SpherePadController::calcMoveVector(TVec3f* pMoveDir, const TVec3f& rGravity) {
    return calcDirSphereMove(pMoveDir, rGravity, 0);
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

f32 SpherePadController::calcDirSphereMove(TVec3f* pMoveDir, const TVec3f& rBaseVec, u32 isFrontVec) {
    f32 x = 0.0f;
    f32 y = 0.0f;
    clacXY(&x, &y);

    f32 mag = x * x + y * y;
    mag = JGeometry::TUtil< f32 >::sqrt(mag);

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

        if ((isFrontVec != 0 && dirZ.dot(rBaseVec) < 0.0f) || (isFrontVec == 0 && dirY.dot(-rBaseVec) < 0.0f)) {
            dirX = -dirX;
            dirY = -dirY;
            dirZ = -dirZ;
        }

        TRot3f rotMtx;
        if (isFrontVec != 0) {
            rotMtx.setRotate(dirZ, rBaseVec);
            rotMtx.mult33(dirX);
            rotMtx.mult33(dirY);
            pMoveDir->set(-dirX * x - dirY * y);
        } else {
            rotMtx.setRotate(dirY, -rBaseVec);
            rotMtx.mult33(dirX);
            rotMtx.mult33(dirZ);
            pMoveDir->set(dirX * x - dirZ * y);
        }
        MR::separateScalarAndDirection(&mag, pMoveDir, *pMoveDir);
    } else {
        pMoveDir->z = 0.0f;
        pMoveDir->y = 0.0f;
        pMoveDir->x = 0.0f;
    }

    if (mag > 1.0f) {
        mag = 1.0f;
    }

    return mag;
}
