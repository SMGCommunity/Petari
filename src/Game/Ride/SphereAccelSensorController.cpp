#include "Game/Ride/SphereAccelSensorController.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "revolution/wpad.h"

SphereAccelSensorController::SphereAccelSensorController()
    : _58(0), _5C(0), _74(0), _78(0),
      _7C(0.15f), _80(1.0f), _84(1.7f), _88(2.5f),
      _8C(0), _90(0),
      _94(1.0f), _98(0.0f), _9C(0.0f), _A0(0.0f),
      _A4(-1.0f), _A8(0.0f), _AC(1.0f),
      _B0(0), _B4(0.0f), _B8(0) {
}

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
        bool brake = false;
        if (MR::testSubPadButtonZ(WPAD_CHAN0) || MR::testSubPadButtonC(WPAD_CHAN0)) {
            brake = true;
        }
        return brake;
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
    // this cannot be properly completed until some specific
    // table is made in J3DMtxBuffer.cpp
    // https://decomp.me/scratch/L8lBc

    f32 d8;
    if (_B8 == 0) {
        d8 = 0.08726646f; // 0.5f * sin(5.0f)
    } else {
        d8 = 0.1745329f; // 1.0f * sin(1.0f)
    }

    f32 d5;
    if (_B8 == 0) {
        d5 = 0.4363323f; // 2.5f * sin(1.0f)
    } else {
        d5 = 0.6981317f; // 4.0f * sin(1.0f)
    }

    f32 d7;
    if (_B8 == 0) {
        d7 = 0.4363323f; // 2.5f * sin(1.0f)
    } else {
        d7 = 0.3926991f; // 2.25f * sin(1.0f)
    }

    f32 d4;
    if (_B8 == 0) {
        d4 = 0.1745329f; // 1.0f * sin(1.0f)
    } else {
        d4 = 0.7853982f; // 4.5f * sin(1.0f)
    }

    TVec3f padAccel;
    getPadAcceleration(&padAccel);

    f32    d6 = 0.0f;
    TVec2f accelXY(padAccel.x, __fabsf(padAccel.y));

    if (accelXY.dot(accelXY) > 0.0000038146973f) {
        // if mag of accelXY > 0.002f
        MR::normalizeOrZero(&accelXY);
        d6 = JMAAsinRadian(accelXY.x);
    }

    f32    d2 = 0.0f;
    TVec2f accelYZ(-padAccel.y, padAccel.z);
    if (accelYZ.dot(accelYZ) > 0.0000038146973f) {
        // if mag of accelYZ > 0.002f
        MR::normalizeOrZero(&accelYZ);
        d2 = d4;
        if (d4 < 0.0f) { // ... nice
            d2 = -d4;
        }

        // this is the table in J3DMtxBuffer.cpp that
        // needs to be completed to match this.

        // "tableScaleValue" = 2607.5945; // 2048 / (4.5f * sin(1.0f))
        TVec2f v2;
        //v2.x = SOME_TABLE[(u32)(d2 * tableScaleValue)][1];
        if (d4 >= 0.0f) {
            //v2.y = SOME_TABLE[(u32)(d4 * tableScaleValue)][0];
            v2.y = 1.0f; // temp for now to get other code to match
        } else {
            //v2.y = -SOME_TABLE[(u32)(d4 * -tableScaleValue)][0];
            v2.y = -1.0f; // temp for now to get other code to match
        }

        // careful, accelYZ.y is normalized accel.z
        // likewise accelYZ.x is normalized accel.y
        d4 = accelYZ.y * v2.x - accelYZ.x * v2.y;
        d2 = JMAAcosRadian(accelYZ.dot(v2));
        if (d4 < 0.0f) {
            d2 = -d2;
        }
    }

    f32 x;
    if (d8 > __fabsf(d6)) {
        x = 0.0f;
    } else {
        if (d6 > 0.0f) {
            d6 -= d8;
        } else {
            d6 += d8;
        }
        x = d6 / (d5 - d8);
    }

    f32 y;
    if (d8 > __fabsf(d2)) {
        y = 0.0f;
    } else {
        if (d2 > 0.0f) {
            d2 -= d8;
        } else {
            d2 += d8;
        }
        y = d2 / (d7 - d8);
    }

    *pX = x;
    *pY = y;
}

void SphereController::notifyDeactivate() {
}

void SphereController::notifyActivate() {
}
