#include "Game/MapObj/HipDropSwitch.hpp"
#include "Game/MapObj/MapObjActor.hpp"
#include "Game/MapObj/MapObjConnector.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SpringValue.hpp"
#include "Game/Util.hpp"
#include "Game/Util/StarPointerUtil.hpp"

namespace NrvHipDropSwitch {
    NEW_NERVE(HipDropSwitchNrvOff, HipDropSwitch, Off);
    NEW_NERVE(HipDropSwitchNrvSwitchDown, HipDropSwitch, SwitchDown);
    NEW_NERVE(HipDropSwitchNrvOn, HipDropSwitch, On);
};

HipDropSwitch::HipDropSwitch(const char *pName) : LiveActor(pName) {
    _8C = 0;
    mConnector = nullptr;
    _98 = nullptr;
    _CC = 0;
    _CD = 0;
    _CE = 0;
    mSpringValue = new SpringValue();
    mConnector = new MapObjConnector(this);
    mSpringValue->setParam(0.0f, 0.0f, 0.2f, 0.89f, 0.0f);
    _9C.identity();
}

void HipDropSwitch::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("HipDropSwitch", nullptr, false);
    MR::connectToSceneMapObjDecorationStrongLight(this);
    MR::initLightCtrl(this);
    initSound(4, false);
    initHitSensor(2);
    MR::addHitSensorMapObj(this, "body", 0x10, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorMapObj(this, "hit", 0x10, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
    TVec3f targetOffs;
    targetOffs.x = 0.0f;
    targetOffs.y = 0.0f;
    targetOffs.z = 0.0f;
    MR::initStarPointerTarget(this, 150.0f, targetOffs);
    MR::initCollisionParts(this, "HipDropSwitch", getSensor("body"), nullptr);
    _98 = MR::createCollisionPartsFromLiveActor(this, "Move", getSensor("hit"), (MR::CollisionScaleType)2);
    MR::validateCollisionParts(_98);
    initNerve(&NrvHipDropSwitch::HipDropSwitchNrvOff::sInstance);
    MR::needStageSwitchWriteA(this, rIter);
    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    }
    else {
        makeActorAppeared();
    }
}

void HipDropSwitch::initAfterPlacement() {
    mConnector->attachToUnder();
}

void HipDropSwitch::control() {
    _CC = 0;
    _CD = _CE;
    _CE = 0;
}

// HipDropSwitch::calcAnim

void HipDropSwitch::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();
    mConnector->connect();
}

bool HipDropSwitch::receiveMsgPlayerAttack(u32 msg, HitSensor *a2, HitSensor *a3) {
    if (MR::isMsgStarPieceAttack(msg)) {
        _CE = 1;
        return true;
    }
    else if (MR::isMsgPlayerHipDropFloor(msg) && _98->mHitSensor == a3 && !_CC) {
        _CC = 1;
        return true;
    }
    
    return false;
}

bool HipDropSwitch::receiveOtherMsg(u32 msg, HitSensor *a2, HitSensor *a3) {
    if (MR::isMsgFloorTouch(msg) && _98->mHitSensor == a3) {
        _CE = 1;
        return _CD == 0;
    }

    return false;
}

bool HipDropSwitch::trySwitchDown() {
    if (_CC) {
        setNerve(&NrvHipDropSwitch::HipDropSwitchNrvSwitchDown::sInstance);
        return true;
    }

    return false;
}

bool HipDropSwitch::tryOn() {
    if (MR::isGreaterStep(this, 0) && MR::isBckStopped(this)) {
        setNerve(&NrvHipDropSwitch::HipDropSwitchNrvOn::sInstance);
        return true;
    }

    return false;
}

void HipDropSwitch::exeOff() {
    if (MR::isFirstStep(this)) {
        _CC = 0;
        MR::validateClipping(this);
        MR::offSwitchA(this);
        MR::startBck(this, "Wait", nullptr);
        MR::startBrk(this, "Off");
        mSpringValue->reset();
    }

    if (MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        _CE = 1;
    }

    if (!_CD && _CE) {
        mSpringValue->_10 += -10.0f;
        MR::startSound(this, "SE_OJ_PNC_KINOKO_BOUND", -1, -1);
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

    if (MR::isStep(this, 0)) {
        MR::startBck(this, "On", nullptr);
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
        MR::startSound(this, "SE_OJ_HIPDROP_SWITCH_ON", -1, -1);
        MR::shakeCameraNormal();
    }
}

HipDropSwitch::~HipDropSwitch() {

}
