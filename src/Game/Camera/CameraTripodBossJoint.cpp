#include "Game/Camera/CameraTripodBossJoint.hpp"
#include "Game/Camera/CamTranslatorTripodBossJoint.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"

CameraTripodBossJoint::CameraTripodBossJoint(const char* pName) : Camera(pName) {
    mAngleB = 0.0f;
    mAngleA = 0.0f;
    mDist = 3000.0f;
    _68 = false;
    _6C = 0.0f;
    _70 = 0.0f;
}

void CameraTripodBossJoint::reset() {
    TVec3f watchPoint;
    CameraLocalUtil::makeWatchPoint(&watchPoint, this, CameraLocalUtil::getTarget(this), 0.0066666668f);
    CameraLocalUtil::setWatchPos(this, watchPoint);
    calcIdealPose();
    _68 = 0;
    _6C = 0.0f;
    _70 = 0.0f;
}

CameraTargetObj* CameraTripodBossJoint::calc() {
    calcIdealPose();
    return CameraLocalUtil::getTarget(this);
}

CameraTripodBossJoint::~CameraTripodBossJoint() {}

CamTranslatorBase* CameraTripodBossJoint::createTranslator() {
    return new CamTranslatorTripodBossJoint(this);
}

/*
void CameraTripodBossJoint::calcIdealPose() {
    if (mDist < 300.0f) {
        mDist = 300.0f;
    }

    if (_68) {
        if (_6C <= _70) {
            f32 v5 = (_70 - 0.08f);
            _70 -= 0.08f;
            if (v5 <= _6C) {
                _70 = _6C;
                _68 = 0;
            }
        }
        else {
            f32 v4 = _70 + 0.08f;
            _70 += 0.08f;
            if (v4 >= _6C) {
                _70 = _6C;
                _68 = 0;
            }
        }
    }
    else if (CameraLocalUtil::tryCameraReset()) {
        f32 v6 = _70 * 0.7f;
        _70 *= 0.7f;
        _6C = v6;
    }
    else if (CameraLocalUtil::testCameraPadTriggerRoundLeft()) {
        f32 v8 = __fabs(_70);
        s32 v9;
        if (_70 >= 0.0f) {
            v9 = 1;
        }
        else {
            v9 = -1;
        }

        f32 v10 = v9 * (((0.78539819 * 0.5) + v8) / 0.78539819) - 1;

        if (v10 < 4.0f) {
            f32 v11 = v10 + 8.0f;
            _70 += 6.2831855f;
        }
    }
}*/

bool CameraTripodBossJoint::isEnableToReset() const {
    return true;
}
