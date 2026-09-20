#include "Game/MapObj/HitWallTimerSwitch.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/MapObj/MapObjConnector.hpp"
#include "Game/Util.hpp"
#include "Game/Util/LiveActorUtil.hpp"

namespace NrvHitWallTimerSwitch {
    NEW_NERVE(HitWallTimerSwitchNrvOff, HitWallTimerSwitch, Off);
    NEW_NERVE(HitWallTimerSwitchNrvSwitchDown, HitWallTimerSwitch, SwitchDown);
    NEW_NERVE(HitWallTimerSwitchNrvOn, HitWallTimerSwitch, On);
    NEW_NERVE(HitWallTimerSwitchNrvSwitchUp, HitWallTimerSwitch, SwitchUp);
};  // namespace NrvHitWallTimerSwitch

HitWallTimerSwitch::HitWallTimerSwitch(const char* pName) : LiveActor(pName) {
    _8C = nullptr;
    mConnector = nullptr;
    mTimer = 300;
    _98 = 0;
}

void HitWallTimerSwitch::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("HitWallTimerSwitch", nullptr, false);
    MR::connectToSceneMapObjDecoration(this);
    initHitSensor(2);
    MR::addHitSensorMapObj(this, "body", 0x10, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensor(this, "hit", ATYPE_SWITCH_BIND, 0x10, 50.f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::initCollisionParts(this, "HitWallTimerSwitch", getSensor("body"), nullptr);
    _8C = MR::createCollisionPartsFromLiveActor(this, "Move", getSensor("hit"), MR::CollisionScaleType_Unk2);
    mConnector = new MapObjConnector(this);
    initSound(4, false);
    MR::needStageSwitchWriteA(this, rIter);
    MR::getJMapInfoArg0NoInit(rIter, &mTimer);
    initNerve(GET_NERVE(HitWallTimerSwitch, HitWallTimerSwitchNrvOff));
    MR::startBck(this, "Wait");
    appear();
}

void HitWallTimerSwitch::initAfterPlacement() {
    mConnector->attachToBack();
}

void HitWallTimerSwitch::calcAnim() {
    LiveActor::calcAnim();
    TPos3f mtx;
    mtx.setInline(MR::getJointMtx(this, "Move"));
    _8C->setMtx(mtx);
}

void HitWallTimerSwitch::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();
    mConnector->connect();
}

void HitWallTimerSwitch::control() {
    _98 = 0;
}

bool HitWallTimerSwitch::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(HitWallTimerSwitch, HitWallTimerSwitchNrvOff)) && pReceiver->isType(ATYPE_SWITCH_BIND)) {
        _98 = 1;
        return true;
    }

    return false;
}

bool HitWallTimerSwitch::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!isNerve(GET_NERVE(HitWallTimerSwitch, HitWallTimerSwitchNrvOff))) {
        return false;
    }

    if (msg == ACTMES_IS_RUSH_ENABLE) {
        if (pReceiver->isType(ATYPE_SWITCH_BIND)) {
            return true;
        }
    }

    return false;
}

bool HitWallTimerSwitch::trySwitchDown() {
    if (_98) {
        setNerve(GET_NERVE(HitWallTimerSwitch, HitWallTimerSwitchNrvSwitchDown));
        return true;
    }

    return false;
}

bool HitWallTimerSwitch::tryOn() {
    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(HitWallTimerSwitch, HitWallTimerSwitchNrvOn));
        return true;
    }

    return false;
}

bool HitWallTimerSwitch::trySwitchUp() {
    if (getNerveStep() > mTimer) {
        setNerve(GET_NERVE(HitWallTimerSwitch, HitWallTimerSwitchNrvSwitchUp));
        return true;
    } else {
        if (!MR::isPlayerDead() && !MR::isPowerStarGetDemoActive()) {
            if (mTimer == getNerveStep()) {
                MR::startSystemSE("SE_SY_TIMER_A_0");
            } else if (!(getNerveStep() % 60)) {
                if (getNerveStep() < mTimer - 120) {
                    MR::startSystemSE("SE_SY_TIMER_A_2");
                } else {
                    MR::startSystemSE("SE_SY_TIMER_A_1");
                }
            }
        }

        return false;
    }
}

bool HitWallTimerSwitch::tryOff() {
    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(HitWallTimerSwitch, HitWallTimerSwitchNrvOff));
        return true;
    }

    return false;
}

void HitWallTimerSwitch::exeOff() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait");
        MR::validateClipping(this);
        MR::offSwitchA(this);
        MR::validateCollisionParts(_8C);
    }

    if (trySwitchDown()) {
        return;
    }
}

void HitWallTimerSwitch::exeSwitchDown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "On");
        MR::startBtp(this, "On");
        MR::invalidateClipping(this);
    }

    if (tryOn()) {
        return;
    }
}

void HitWallTimerSwitch::exeOn() {
    if (MR::isFirstStep(this)) {
        MR::shakeCameraNormal();
        MR::onSwitchA(this);
        MR::startSound(this, "SE_OJ_SWITCH_1");
        MR::invalidateCollisionParts(_8C);
    }

    if (trySwitchUp()) {
        return;
    }
}

void HitWallTimerSwitch::exeSwitchUp() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait");
        MR::startBtp(this, "Off");
    }

    if (tryOff()) {
        return;
    }
}

HitWallTimerSwitch::~HitWallTimerSwitch() {
}
