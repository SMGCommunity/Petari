#include "Game/Ride/SphereAccelSensorController.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include <JSystem/JMath/JMATrigonometric.hpp>
#include <JSystem/JMath/JMath.hpp>
#include <revolution/wpad.h>

void SphereAccelSensorController_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)MR::epsilon();
    (void)-1.0f;
}

namespace {
    // sMinStableValue
    // sMaxStableValue
    // sLimitAccelValue
    // sIsStableCount
    // sDownAccelRate
    // sAjustAvarageRate
    // sStableRange
    // sVerticalFreq
    // sMinYAcc
    // sMaxYAcc
    static const f32 sCoreBaseDegreeYZ = 10.0f;
    static const f32 sCoreAccelDegreMargine = 5.0f;
    static const f32 sCoreAccelDegreeRange = 25.0f;
    static const f32 sCoreAccelDegreeRangeY = 25.0f;
    static const f32 sSubBaseDegreeYZ = 45.0f;
    static const f32 sSubAccelDegreMargine = 10.0f;
    static const f32 sSubAccelDegreeRange = 40.0f;
    static const f32 sSubAccelDegreeRangeY = 22.5f;
};  // namespace

inline f32 diffAngleAbs(const TVec2f& rV1, const TVec2f& rV2) {
    f32 orientation = rV1.y * rV2.x - rV1.x * rV2.y;
    f32 angle = MR::acos(rV1.dot(rV2));
    if (orientation < 0.0f) {
        angle = -angle;
    }

    return angle;
}

SphereAccelSensorController::SphereAccelSensorController()
    : _58(), _5C(), _74(), _78(), _7C(0.15f), _80(1.0f), _84(1.7f), _88(2.5f), _8C(), _90(), _94(1.0f), _98(), _9C(), _A0(), _A4(-1.0f), _A8(),
      _AC(1.0f), _B0(), _B4(), mPad() {
}

void SphereAccelSensorController::getPadAcceleration(TVec3f* pAccel) const {
    if (mPad == Pad_Core) {
        MR::getCorePadAcceleration(pAccel, WPAD_CHAN0);
    } else {
        MR::getSubPadAcceleration(pAccel, WPAD_CHAN0);
    }
}

bool SphereAccelSensorController::testBrake() const {
    if (mPad == Pad_Core) {
        return MR::testCorePadButtonA(WPAD_CHAN0);
    } else {
        return MR::testSubPadButtonZ(WPAD_CHAN0) || MR::testSubPadButtonC(WPAD_CHAN0);
    }
}

f32 SphereAccelSensorController::calcJumpPower() const {
    return MR::testCorePadTriggerA(WPAD_CHAN0);
}

bool SphereAccelSensorController::doBrake() const {
    return testBrake();
}

void SphereAccelSensorController::update(const TVec3f&) {
}

void SphereAccelSensorController::clacXY(f32* pX, f32* pY) {
    f32 accelMargine = isCorePad() ? MR::toRadian(::sCoreAccelDegreMargine) : MR::toRadian(::sSubAccelDegreMargine);
    f32 accelRange = isCorePad() ? MR::toRadian(::sCoreAccelDegreeRange) : MR::toRadian(::sSubAccelDegreeRange);
    f32 accelRangeY = isCorePad() ? MR::toRadian(::sCoreAccelDegreeRangeY) : MR::toRadian(::sSubAccelDegreeRangeY);
    f32 baseAngleYZ = isCorePad() ? MR::toRadian(::sCoreBaseDegreeYZ) : MR::toRadian(::sSubBaseDegreeYZ);

    TVec3f padAccel;
    getPadAcceleration(&padAccel);

    f32 angleXY = 0.0f;
    TVec2f accelXY(padAccel.x, MR::abs(padAccel.y));
    if (!accelXY.isZero()) {
        MR::normalizeOrZero(&accelXY);
        angleXY = MR::asin(accelXY.x);
    }

    f32 angleYZ = 0.0f;
    TVec2f accelYZ(-padAccel.y, padAccel.z);
    if (!accelYZ.isZero()) {
        MR::normalizeOrZero(&accelYZ);
        angleYZ = diffAngleAbs(accelYZ, TVec2f(MR::cos(baseAngleYZ), MR::sin(baseAngleYZ)));
    }

    if (MR::abs(angleXY) < accelMargine) {
        angleXY = 0.0f;
    } else {
        if (angleXY > 0.0f) {
            angleXY -= accelMargine;
        } else {
            angleXY += accelMargine;
        }

        angleXY /= accelRange - accelMargine;
    }

    if (MR::abs(angleYZ) < accelMargine) {
        angleYZ = 0.0f;
    } else {
        f32 range;
        if (angleYZ > 0.0f) {
            angleYZ -= accelMargine;
            range = accelRange;
        } else {
            angleYZ += accelMargine;
            range = accelRangeY;
        }

        angleYZ /= range - accelMargine;
    }

    *pX = angleXY;
    *pY = angleYZ;
}
