#include "Game/Ride/SphereAccelSensorController.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include <JSystem/JMath/JMATrigonometric.hpp>
#include <JSystem/JMath/JMath.hpp>
#include <revolution/wpad.h>

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
    static const f32 sCoreBaseDegreeYZ = 10.0f * PI_180;
    static const f32 sCoreAccelDegreMargine = 5.0f * PI_180;
    static const f32 sCoreAccelDegreeRange = 25.0f * PI_180;
    static const f32 sCoreAccelDegreeRangeY = 25.0f * PI_180;
    static const f32 sSubBaseDegreeYZ = 45.0f * PI_180;
    static const f32 sSubAccelDegreMargine = 10.0f * PI_180;
    static const f32 sSubAccelDegreeRange = 40.0f * PI_180;
    static const f32 sSubAccelDegreeRangeY = 22.5f * PI_180;
}  // namespace

inline TVec2f getTrig(f32 angle) {
    return TVec2f(JMath::sSinCosTable.cosLapRad(angle), JMath::sSinCosTable.sinLapRad(angle));
}

inline f32 diffAngleAbs(const TVec2f& v1, const TVec2f& v2) {
    f32 orientation = v1.y * v2.x - v1.x * v2.y;
    f32 angle = JMAAcosRadian(v1.dot(v2));
    if (orientation < 0.0f) {
        angle = -angle;
    }
    return angle;
}

SphereAccelSensorController::SphereAccelSensorController()
    : _58(0), _5C(0), _74(0), _78(0), _7C(0.15f), _80(1.0f), _84(1.7f), _88(2.5f), _8C(0), _90(0), _94(1.0f), _98(0.0f), _9C(0.0f), _A0(0.0f),
      _A4(-1.0f), _A8(0.0f), _AC(1.0f), _B0(0), _B4(0.0f), _B8(0) {}

void SphereAccelSensorController::getPadAcceleration(TVec3f* pAccel) {
    if (_B8 == 0) {
        MR::getCorePadAcceleration(pAccel, WPAD_CHAN0);
    } else {
        MR::getSubPadAcceleration(pAccel, WPAD_CHAN0);
    }
}

bool SphereAccelSensorController::testBrake() const {
    if (_B8 == 0) {
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

void SphereAccelSensorController::update(const TVec3f&) {}

void SphereAccelSensorController::clacXY(f32* pX, f32* pY) {
    // FIXME: regswaps and inlines
    // https://decomp.me/scratch/vkqZ8

    // these are probably individual inlines considering the multiple-load of _B8
    f32 accelDegreMargine = _B8 == 0 ? sCoreAccelDegreMargine : sSubAccelDegreMargine;
    f32 accelDegreeRange = _B8 == 0 ? sCoreAccelDegreeRange : sSubAccelDegreeRange;
    f32 accelDegreeRangeY = _B8 == 0 ? sCoreAccelDegreeRangeY : sSubAccelDegreeRangeY;
    f32 baseDegreeYZ = _B8 == 0 ? sCoreBaseDegreeYZ : sSubBaseDegreeYZ;

    TVec3f padAccel;
    getPadAcceleration(&padAccel);

    f32 angleXY = 0.0f;
    TVec2f accelXY(padAccel.x, __fabsf(padAccel.y));
    if (!isDeadZone(accelXY)) {
        MR::normalizeOrZero(&accelXY);
        angleXY = JMAAsinRadian(accelXY.x);
    }

    f32 angleYZ = 0.0f;
    TVec2f accelYZ(-padAccel.y, padAccel.z);
    if (!isDeadZone(accelYZ)) {
        MR::normalizeOrZero(&accelYZ);
        angleYZ = diffAngleAbs(accelYZ, getTrig(baseDegreeYZ));
    }

    f32 x;
    if (__fabsf(angleXY) < accelDegreMargine) {
        x = 0.0f;
    } else {
        if (angleXY > 0.0f) {
            angleXY -= accelDegreMargine;
        } else {
            angleXY += accelDegreMargine;
        }
        x = angleXY / (accelDegreeRange - accelDegreMargine);
    }

    f32 y;
    if (__fabsf(angleYZ) < accelDegreMargine) {
        y = 0.0f;
    } else {
        if (angleYZ > 0.0f) {
            angleYZ -= accelDegreMargine;
        } else {
            angleYZ += accelDegreMargine;
        }
        y = angleYZ / (accelDegreeRangeY - accelDegreMargine);
    }

    *pX = x;
    *pY = y;
}

void SphereController::notifyDeactivate() {}

void SphereController::notifyActivate() {}
