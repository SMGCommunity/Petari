#include "Game/MapObj/HipDropSwitch.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/MapObj/MapObjConnector.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/SpringValue.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>
#include <JSystem/JGeometry/TVec.hpp>

namespace {
    static const s32 sOnSomethingAcc = 0;
    // sSwitchOnDelayTime
};  // namespace

namespace NrvHipDropSwitch {
    NEW_NERVE(HipDropSwitchNrvOff, HipDropSwitch, Off);
    NEW_NERVE(HipDropSwitchNrvSwitchDown, HipDropSwitch, SwitchDown);
    NEW_NERVE(HipDropSwitchNrvOn, HipDropSwitch, On);
};  // namespace NrvHipDropSwitch

HipDropSwitch::HipDropSwitch(const char* pName) : LiveActor(pName), _8C(), mConnector(), mSpringModel(), _CC(), _CD(), _CE() {
    mSpringValue = new SpringValue();
    mConnector = new MapObjConnector(this);
    mSpringValue->setParam(0.0f, 0.0f, 0.2f, 0.90f, 0.0f);
    _9C.identity();
}

inline void HipDropSwitch::initStarPointerTarget() {
    TVec3f targetOffs;
    targetOffs.set(0.0f);
    MR::initStarPointerTarget(this, 150.0f, targetOffs);
}

void HipDropSwitch::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("HipDropSwitch", nullptr, false);
    MR::connectToSceneMapObjDecorationStrongLight(this);
    MR::initLightCtrl(this);
    initSound(4, false);
    initHitSensor(2);
    MR::addHitSensorMapObj(this, "body", 16, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorMapObj(this, "hit", 16, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
    initStarPointerTarget();
    MR::initCollisionParts(this, "HipDropSwitch", getSensor("body"), nullptr);
    mSpringModel = MR::createCollisionPartsFromLiveActor(this, "Move", getSensor("hit"), MR::CollisionScaleType_Unk2);
    MR::validateCollisionParts(mSpringModel);
    initNerve(GET_NERVE(HipDropSwitch, HipDropSwitchNrvOff));
    MR::needStageSwitchWriteA(this, rIter);

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    } else {
        makeActorAppeared();
    }
}

void HipDropSwitch::initAfterPlacement() {
    mConnector->attachToUnder();
}

void HipDropSwitch::control() {
    _CC = false;
    _CD = _CE;
    _CE = false;
}

void HipDropSwitch::calcAnim() {
    LiveActor::calcAnim();
    TPos3f mtx;
    MtxPtr jointMtx = MR::getJointMtx(this, "Move");
    mtx.set(jointMtx);
    f32 springValue = mSpringValue->mSpringValue;

    if (!MR::isNearZero(springValue)) {
        TVec3f trans, upVec;
        mtx.getTrans(trans);
        MR::calcUpVec(&upVec, this);
        trans += upVec * springValue;
        mtx.setTrans(trans);
        PSMTXCopy(mtx, jointMtx);
    }

    mSpringModel->setMtx(mtx);
}

void HipDropSwitch::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();
    mConnector->connect();
}

bool HipDropSwitch::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceAttack(msg)) {
        _CE = true;
        return true;
    } else if (MR::isMsgPlayerHipDropFloor(msg) && mSpringModel->mHitSensor == pReceiver && !_CC) {
        _CC = true;
        return true;
    }

    return false;
}

bool HipDropSwitch::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgFloorTouch(msg) && mSpringModel->mHitSensor == pReceiver) {
        _CE = true;
        return _CD == false;
    }

    return false;
}

bool HipDropSwitch::trySwitchDown() {
    if (_CC) {
        setNerve(GET_NERVE(HipDropSwitch, HipDropSwitchNrvSwitchDown));
        return true;
    }

    return false;
}

bool HipDropSwitch::tryOn() {
    if (MR::isGreaterStep(this, ::sOnSomethingAcc) && MR::isBckStopped(this)) {
        setNerve(GET_NERVE(HipDropSwitch, HipDropSwitchNrvOn));
        return true;
    }

    return false;
}

void HipDropSwitch::exeOff() {
    if (MR::isFirstStep(this)) {
        _CC = false;
        MR::validateClipping(this);
        MR::offSwitchA(this);
        MR::startBck(this, "Wait");
        MR::startBrk(this, "Off");
        mSpringValue->reset();
    }

    if (MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        _CE = true;
    }

    if (!_CD && _CE) {
        mSpringValue->mVelocity += -10.0f;
        MR::startSound(this, "SE_OJ_PNC_KINOKO_BOUND");
    }

    mSpringValue->update();

    if (trySwitchDown()) {
        mSpringValue->reset();
    }
}

void HipDropSwitch::exeSwitchDown() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(this);
    }

    if (MR::isStep(this, ::sOnSomethingAcc)) {
        MR::startBck(this, "On");
        MR::startBrk(this, "On");
    }

    if (tryOn()) {
        return;
    }
}

void HipDropSwitch::exeOn() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
        MR::onSwitchA(this);
        MR::startSound(this, "SE_OJ_HIPDROP_SWITCH_ON");
        MR::shakeCameraNormal();
    }
}
