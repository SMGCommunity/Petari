#include "Game/MapObj/ArrowSwitch.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

namespace {
    static const f32 sRotYTargetList[] = {0.0f, 90.0f, 180.0f, -90.0f};
};  // namespace

namespace NrvArrowSwitch {
    NEW_NERVE(ArrowSwitchNrvWait, ArrowSwitch, Wait);
    NEW_NERVE(ArrowSwitchNrvRotate, ArrowSwitch, Rotate);
    NEW_NERVE(ArrowSwitchNrvLock, ArrowSwitch, Lock);
};  // namespace NrvArrowSwitch

ArrowSwitch::ArrowSwitch(const char* pName) : LiveActor(pName), _8C(), mRotYTargetIndex(), _94(), mObjArg0(-1), mObjArg1(), _9D(true), _9E(), _9F() {
}

void ArrowSwitch::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    mScale.x = 1.0f;
    mScale.y = 1.0f;
    mScale.z = 1.0f;
    initModelManagerWithAnm("ArrowSwitch", nullptr, false);
    MR::connectToSceneNoSilhouettedMapObjStrongLight(this);

    TVec3f up;
    MR::calcUpVec(&up, this);
    mGravity = -up;

    MR::initShadowFromCSV(this, "Shadow");
    MR::onCalcShadow(this, nullptr);
    MR::onCalcGravity(this);

    if (MR::isInAreaObj("PlaneModeCube", mPosition)) {
        initHitSensor(1);
        MR::addHitSensorMapObj(this, "body", 8, 100.0f, TVec3f(0.0f, 50.0f, 250.0f));
    } else {
        initHitSensor(2);
        MR::addHitSensorMapObj(this, "body", 8, 100.0f, TVec3f(0.0f, 50.0f, 250.0f));
        MR::addHitSensorMapObjMoveCollision(this, "collision", 8, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
        MR::initCollisionParts(this, "ArrowSwitch", getSensor("collision"), nullptr);
    }

    initSound(4, false);
    initNerve(&NrvArrowSwitch::ArrowSwitchNrvWait::sInstance);
    MR::getJMapInfoArg0WithInit(rIter, &mObjArg0);
    MR::getJMapInfoArg1WithInit(rIter, &mObjArg1);

    s32 arg2 = -1;
    MR::getJMapInfoArg2WithInit(rIter, &arg2);

    if (arg2 >= 0) {
        MR::setShadowDropLength(this, nullptr, arg2);
    }

    MR::needStageSwitchWriteA(this, rIter);

    if (!mObjArg1) {
        MR::listenStageSwitchOnOffA(this, MR::Functor(this, &ArrowSwitch::listenOffSwitch), MR::Functor(this, &ArrowSwitch::listenOnSwitch));
    }

    makeActorAppeared();
}

void ArrowSwitch::control() {
}

void ArrowSwitch::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();
    MtxPtr baseMtx = getBaseMtx();
    TPos3f pos;
    pos.setInline(baseMtx);
    TMtx34f tr_mtx;
    tr_mtx.identity();
    f32 v11 = _8C;
    v11 = v11 * PI_180;
    f32 v12 = sin(v11);
    f32 v13 = cos(v11);
    tr_mtx.mMtx[0][2] = v12;
    tr_mtx.mMtx[1][1] = 1.0f;
    tr_mtx.mMtx[0][0] = v13;
    tr_mtx.mMtx[2][0] = -v12;
    tr_mtx.mMtx[2][2] = v13;
    tr_mtx.mMtx[2][1] = 0.0f;
    tr_mtx.mMtx[1][2] = 0.0f;
    tr_mtx.mMtx[1][0] = 0.0f;
    tr_mtx.mMtx[0][1] = 0.0f;
    pos.concat(pos, tr_mtx);
    MR::setBaseTRMtx(this, pos);
}

void ArrowSwitch::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        MR::sendMsgPush(pReceiver, pSender);
    }
}

bool ArrowSwitch::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerHitAll(msg)) {
        return requestPunch(pSender, pReceiver);
    }

    return false;
}

bool ArrowSwitch::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}

bool ArrowSwitch::requestPunch(HitSensor* pSender, HitSensor* pReceiver) {
    if (getSensor("body") != pReceiver) {
        return false;
    }

    if (!isNerve(&NrvArrowSwitch::ArrowSwitchNrvWait::sInstance)) {
        return false;
    }

    TVec3f side;
    MR::calcSideVec(&side, this);

    TVec3f sensorDir;
    MR::calcSensorDirection(&sensorDir, pSender, pReceiver);

    if (isPlusLimit()) {
        _9D = false;
    } else if (isMinusLimit()) {
        _9D = true;
    } else if (side.dot(sensorDir) > 0.0f) {
        _9D = true;
    } else {
        _9D = false;
    }

    mRotYTargetIndex += _9D ? getOneStep() : -getOneStep();
    mRotYTargetIndex = (mRotYTargetIndex + ARRAY_SIZE(::sRotYTargetList)) % ARRAY_SIZE(::sRotYTargetList);
    _94 = _9D ? -6.0f : 6.0f;
    _9F = true;

    MR::invalidateClipping(this);
    setNerve(&NrvArrowSwitch::ArrowSwitchNrvRotate::sInstance);

    return true;
}

void ArrowSwitch::listenOnSwitch() {
    if (mRotYTargetIndex != 0) {
        return;
    }

    switch (mObjArg0) {
    case 2:
    case -1:
        _9D = true;
        _94 = 6.0f;
        mRotYTargetIndex = 2;
        break;
    case 3:
        _9D = false;
        _94 = 6.0f;
        mRotYTargetIndex = 2;
        break;
    case 0:
        _9D = true;
        _94 = -6.0f;
        mRotYTargetIndex = 1;
        break;
    case 1:
        _9D = false;
        _94 = -6.0f;
        mRotYTargetIndex = 3;
        break;
    default:
        return;
    }

    MR::invalidateClipping(this);
    setNerve(&NrvArrowSwitch::ArrowSwitchNrvRotate::sInstance);
}

void ArrowSwitch::listenOffSwitch() {
    if (mRotYTargetIndex == 0) {
        return;
    }

    mRotYTargetIndex = 0;

    switch (mObjArg0) {
    case 2:
    case 0:
    case -1:
        _9D = false;
        _94 = -6.0f;
        break;
    case 1:
    case 3:
        _9D = true;
        _94 = 6.0f;
        break;
    default:
        return;
    }

    MR::invalidateClipping(this);
    setNerve(&NrvArrowSwitch::ArrowSwitchNrvRotate::sInstance);
}

void ArrowSwitch::exeWait() {
    if (MR::isFirstStep(this)) {
        if (mRotYTargetIndex == 0) {
            MR::startBtk(this, "Off");
        } else {
            MR::startBtk(this, "On");
        }

        MR::validateClipping(this);
        MR::onCalcShadow(this, nullptr);
    }
}

void ArrowSwitch::exeRotate() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_ARROW_SWITCH_ON");
        MR::onCalcShadow(this, nullptr);

        if (mObjArg1) {
            MR::invalidateHitSensors(this);

            if (MR::isExistCollisionParts(this)) {
                MR::invalidateCollisionParts(this);
            }
        }
    }

    MR::startLevelSound(this, "SE_OJ_LV_ARROW_SWITCH_MOVE");
    f32 v2 = (_8C + _94) - -180.0f;
    _8C += _94;
    f32 v3 = fmod(360.0 + v2, 360.0);
    f32 v4 = (-180.0f + v3);
    f32 v5 = (::sRotYTargetList[mRotYTargetIndex] - (-180.0f + v3));
    _8C = v4;
    f32 v6 = (-180.0 + fmod((360.0 + (v5 - -180.0)), 360.0));

    if (_9D && v6 < 0.0f || !_9D && v6 > 0.0f) {
        f32 rot = ::sRotYTargetList[mRotYTargetIndex];
        _94 = 0.0f;
        _8C = rot;

        if (MR::isValidSwitchA(this) && _9F) {
            if (mRotYTargetIndex != 0) {
                MR::onSwitchA(this);
            } else {
                MR::offSwitchA(this);
            }

            MR::startSystemSE("SE_SY_GRAVITY_SWITCHED");
        }

        MR::startSound(this, "SE_OJ_ARROW_SWITCH_STOP");

        _9F = false;

        if (mObjArg1) {
            setNerve(&NrvArrowSwitch::ArrowSwitchNrvLock::sInstance);
        } else {
            setNerve(&NrvArrowSwitch::ArrowSwitchNrvWait::sInstance);
        }
    }
}

void ArrowSwitch::exeLock() {
    if (MR::isFirstStep(this)) {
        if (mRotYTargetIndex == 0) {
            MR::startBtk(this, "Off");
        } else {
            MR::startBtk(this, "On");
        }

        MR::validateClipping(this);
        MR::onCalcShadow(this, nullptr);
    }
}

bool ArrowSwitch::isPlusLimit() const {
    switch (mObjArg0) {
    case 2:
        return mRotYTargetIndex == 2;
    case 0:
        return mRotYTargetIndex == 1;
    case 1:
        return mRotYTargetIndex == 0;
    case 3:
        return mRotYTargetIndex == 0;
    default:
        return false;
    }
}

bool ArrowSwitch::isMinusLimit() const {
    switch (mObjArg0) {
    case 2:
        return mRotYTargetIndex == 0;
    case 0:
        return mRotYTargetIndex == 0;
    case 1:
        return mRotYTargetIndex == 3;
    case 3:
        return mRotYTargetIndex == 2;
    default:
        return false;
    }
}

s32 ArrowSwitch::getOneStep() const {
    switch (mObjArg0) {
    case -1:
    case 2:
    case 3:
        return 2;
    case 0:
    case 1:
        return 1;
    default:
        return 0;
    }
}
