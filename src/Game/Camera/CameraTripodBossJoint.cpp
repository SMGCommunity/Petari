#include "Game/Camera/CameraTripodBossJoint.hpp"
#include "Game/Camera/CamTranslatorTripodBossJoint.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraCalc.hpp"
#include "Game/Boss/TripodBossAccesser.hpp"

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


void CameraTripodBossJoint::calcIdealPose() {
    if (mDist < 300.0f) {
        mDist = 300.0f;
    }

    if (_68) {
        if (_6C > _70) {
            _70 += 0.079999998f;
            if (_70 >= _6C) {
                _70 = _6C;
                _68 = 0;
            }
        } else {
            _70 -= 0.079999998f;
            if (_70 <= _6C) {
                _70 = _6C;
                _68 = 0;
            }
        }
    } else {
        f32 common = 0.78539819f;
        if (CameraLocalUtil::tryCameraReset()) {
            _70 *= 0.69999999f;
            _6C = _70;
        } else if (CameraLocalUtil::testCameraPadTriggerRoundLeft()) {
            f32 f = _70;
            f32 fa = __fabsf(_70);
            s32 dir;
            if (f < 0.0f) {
                dir = -1;
            } else {
                dir = 1;
            }
            f32 p5 = 0.5f;
            f32 muls = common * p5;
            muls += fa;
            muls /= common;
            s32 mulint = (dir * static_cast< s32 >(muls)) - 1;
            if (static_cast< f32 >(mulint) < -4.0f) {
                f32 asd = static_cast< f32 >(mulint) + 8.0f;
                _70 = _70 + 6.2831855f;
                muls = asd;
                mulint = static_cast< s32 >(asd);
            }

            _68 = 1;
            _6C = mulint * common;
        } else if (CameraLocalUtil::testCameraPadTriggerRoundRight()) {
            f32 f = _70;
            f32 fa = __fabsf(_70);
            s32 dir;
            if (f < 0.0f) {
                dir = -1;
            } else {
                dir = 1;
            }
            f32 p5 = 0.5f;
            f32 muls = common * p5;
            muls += fa;
            muls /= common;
            s32 mulint = (dir * static_cast< s32 >(muls)) + 1;
            if (static_cast< f32 >(mulint) > 4.0f) {
                f32 asd = static_cast< f32 >(mulint) - 8.0f;
                _70 = _70 - 6.2831855f;
                muls = asd;
                mulint = static_cast< s32 >(asd);
            }

            _68 = 1;
            _6C = mulint * common;
        }
    }

    TVec3f polecrossdegree;
    MR::polarToCrossDegree(TVec3f(0.0f, 0.0f, 0.0f), &polecrossdegree, mDist, mAngleB, (mAngleA - 90.0f) + _70);

    TVec3f axis;
    if (MR::isCreatedTripodBoss()) {
        TPos3f pos;
        MR::getTripodBossJointMatrix(&pos, _58);
        pos.mult33(polecrossdegree, polecrossdegree);
        pos.getYDirInline(axis);
    } else {
        axis.set< f32 >(0.0f, 1.0f, 0.0f);
    }

    TVec3f watchpoint;
    CameraLocalUtil::makeWatchPoint(&watchpoint, this, CameraLocalUtil::getTarget(this), 0.0066666668f);

    if (MR::isCreatedTripodBoss()) {
        TPos3f pos;
        MR::getTripodBossJointMatrix(&pos, _58);
        TVec3f vec;
        pos.mult33(mAxis, vec);
        watchpoint.add(vec);
    }

    polecrossdegree.add(watchpoint);
    CameraLocalUtil::setPos(this, polecrossdegree);
    CameraLocalUtil::setUpVec(this, axis);
    CameraLocalUtil::setWatchPos(this, watchpoint);
    CameraLocalUtil::setWatchUpVec(this, axis);
}


bool CameraTripodBossJoint::isEnableToReset() const {
    return true;
}
