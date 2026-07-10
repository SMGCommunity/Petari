#include "Game/MapObj/DesertLandMoveSwitch.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjConnector.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/SpringValue.hpp"
#include "Game/Util/StringUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "revolution/mtx.h"
#include "revolution/types.h"

void DesertLandMoveSwitch_FORCE_MATCH_SDATA2() {
    (void)0.0f;
    (void)-1.0f;
}

namespace NrvDesertLandMoveSwitch {
    NEW_NERVE(HostTypeWait, DesertLandMoveSwitch, Wait);
    NEW_NERVE(HostTypeSwitchDown, DesertLandMoveSwitch, SwitchDown);
    NEW_NERVE(HostTypeOn, DesertLandMoveSwitch, On);
    NEW_NERVE(HostTypeReturn, DesertLandMoveSwitch, Return);
};  // namespace NrvDesertLandMoveSwitch

DesertLandMoveSwitch::DesertLandMoveSwitch(const char* pName) : LiveActor(pName) {
    mCollisionParts = 0;
    mSpringValue = new SpringValue();
    mMapObjConnector = new MapObjConnector(this);
    _98 = 0;
    _99 = 0;
    _9A = 0;
    _9C = 0;
    _A0 = 0;
    mSpringValue->setParam(0.0f, 0.0f, 0.35f, 0.75f, 0.0f);
}

void DesertLandMoveSwitch::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::getJMapInfoArg0NoInit(rIter, &_9C);
    s32 stack_C = -1;
    MR::getJMapInfoArg1NoInit(rIter, &stack_C);
    initModelAndCollision(rIter);
    MR::connectToSceneMapObj(this);
    if (stack_C == -1) {
        initNerve(&NrvDesertLandMoveSwitch::HostTypeWait::sInstance);
    } else {
        MR::startBck(this, "On", 0);
        MR::setAllAnimFrameAtEnd(this, "On");
        initNerve(&NrvDesertLandMoveSwitch::HostTypeOn::sInstance);
    }
    f32 stack_8 = -1.0f;
    MR::calcModelBoundingRadius(&stack_8, this);
    MR::setClippingTypeSphere(this, stack_8);
    MR::setGroupClipping(this, rIter, 0x10);
    initSound(4, 0);
    MR::needStageSwitchWriteA(this, rIter);
    MR::tryRegisterDemoCast(this, rIter);
    makeActorAppeared();
}

void DesertLandMoveSwitch::initAfterPlacement() {
    mMapObjConnector->attachToUnder();
}

void DesertLandMoveSwitch::calcAnim() {
    LiveActor::calcAnim();
    MtxPtr mtx = MR::getJointMtx(this, "Move");
    TPos3f mtx2;
    mtx2.setInline(mtx);
    f32 val = mSpringValue->mSpringValue;
    if (!MR::isNearZero(val)) {
        TVec3f pos, up;
        mtx2.getTransInline(pos);
        MR::calcUpVec(&up, this);
        pos += up.multInLine(val);
        mtx2.setTransInline(pos);
        PSMTXCopy(mtx2, mtx);
    }
    mCollisionParts->setMtx(mtx2);
}

void DesertLandMoveSwitch::control() {
    _98 = 0;
    _99 = _9A;
    _9A = 0;
}

void DesertLandMoveSwitch::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();
    mMapObjConnector->connect();
}

bool DesertLandMoveSwitch::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerHipDropFloor(msg) && !_98) {
        _98 = 1;
        return true;
    }
    return false;
}

bool DesertLandMoveSwitch::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == 0xB4) {
        _9A = 1;
        return _99 == 0;
    }
    return false;
}

void DesertLandMoveSwitch::initModelAndCollision(const JMapInfoIter& rIter) {
    const char* pObjName = "";
    MR::getObjectName(&pObjName, rIter);
    _A0 = pObjName;
    initModelManagerWithAnm(pObjName, 0, 0);
    initHitSensor(2);
    HitSensor* sensorBody = MR::addHitSensorMapObj(this, "body", 8, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
    HitSensor* sensorMove = MR::addHitSensorMapObj(this, "move", 8, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::initCollisionParts(this, pObjName, sensorBody, 0);
    mCollisionParts = MR::createCollisionPartsFromLiveActor(this, "Move", sensorMove, MR::CollisionScaleType_Unk2);
    MR::validateCollisionParts(mCollisionParts);
}

bool DesertLandMoveSwitch::tryOn() {
    if (MR::isGreaterStep(this, 6) && MR::isBckStopped(this)) {
        MR::startSound(this, "SE_OJ_HIPDROP_SWITCH_ON");
        MR::shakeCameraNormal();
        setNerve(&NrvDesertLandMoveSwitch::HostTypeOn::sInstance);
        return true;
    }
    return false;
}

bool DesertLandMoveSwitch::trySwitchDown() {
    if (!_98)
        return false;

    setNerve(&NrvDesertLandMoveSwitch::HostTypeSwitchDown::sInstance);
    return true;
}

bool DesertLandMoveSwitch::tryConnect() {
    if (mMapObjConnector->_4 != 0)
        return true;

    return mMapObjConnector->attachToUnder();
}

void DesertLandMoveSwitch::updateTimerSE() {
    if (!MR::isPlayerDead() && !MR::isPowerStarGetDemoActive()) {
        s32 l = _9C % 60;
        s32 step = getNerveStep();
        s32 lf = step - l;
        if (l != 0) {
            lf += 60;
        }
        s32 step2 = getNerveStep();
        if (MR::isEqualString(_A0, "DesertLandDownSwitch") && MR::isEqualStageName("SandClockGalaxy")) {
            lf += 180;
            step2 += 180;
        }
        if (lf >= 60) {
            s32 s = _9C;
            if (lf <= s) {
                if (step2 == s) {
                    MR::startSystemSE("SE_SY_TIMER_A_0");
                } else {
                    if (!(lf % 60)) {
                        if (lf < s - 600)
                            MR::startSystemSE("SE_SY_TIMER_A_2");
                        else if (lf < s - 120)
                            MR::startSystemSE("SE_SY_TIMER_A_1");
                        else if (lf < s)
                            MR::startSystemSE("SE_SY_TIMER_A_QUASI_0");
                    }
                }
            }
        }
    }
}

void DesertLandMoveSwitch::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", 0);
        _98 = 0;
        mSpringValue->reset();
    }

    tryConnect();
    if (!_99 && _9A) {
        mSpringValue->mVelocity += -10.0f;
    }

    mSpringValue->update();
    if (trySwitchDown()) {
        mSpringValue->reset();
    }
}

void DesertLandMoveSwitch::exeSwitchDown() {
    if (MR::isFirstStep(this))
        MR::invalidateClipping(this);

    if (MR::isStep(this, 6))
        MR::startBck(this, "On", 0);

    if (tryOn())
        return;
}

void DesertLandMoveSwitch::exeOn() {
    if (MR::isFirstStep(this))
        MR::onSwitchA(this);
    tryConnect();
    if (_9C > 0) {
        updateTimerSE();
        if (MR::isStep(this, _9C))
            setNerve(&NrvDesertLandMoveSwitch::HostTypeReturn::sInstance);
    } else if (!MR::isOnSwitchA(this))
        setNerve(&NrvDesertLandMoveSwitch::HostTypeReturn::sInstance);
}

void DesertLandMoveSwitch::exeReturn() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Return", 0);
        MR::offSwitchA(this);
        _98 = 0;
        mSpringValue->reset();
    }

    if (!_99 && _9A)
        mSpringValue->mVelocity += -10.0f;

    mSpringValue->update();

    if (trySwitchDown())
        mSpringValue->reset();
    else if (MR::isBckStopped(this)) {
        setNerve(&NrvDesertLandMoveSwitch::HostTypeWait::sInstance);
    }
}

void SpringValue::reset() {
    mSpringValue = mRestValue;
    mVelocity = 0.0f;
}
