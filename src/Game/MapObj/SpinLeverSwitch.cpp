#include "Game/MapObj/SpinLeverSwitch.hpp"

SpinLeverSwitch::SpinLeverSwitch(const char *pName) : LiveActor(pName) {
    mConnector = nullptr;
    mConnector = new MapObjConnector(this);
}   

SpinLeverSwitch::~SpinLeverSwitch(){

}

void SpinLeverSwitch::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("SpinLeverSwitch", nullptr, false);
    MR::connectToSceneMapObj(this);
    initHitSensor(2);
    MR::addBodyMessageSensorMapObj(this);
    MR::addHitSensorAtJointMapObj(this, "spin", "Spin", 4, 50.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::initCollisionParts(this, "SpinLeverSwitch", getSensor("body"), nullptr);
    initEffectKeeper(0, 0, false);
    initSound(4, false);
    bool returnedValue = MR::useStageSwitchWriteA(this, rIter);
    MR::setGroupClipping(this, rIter, 16);
    if (returnedValue) {
        initNerve(&NrvSpinLeverSwitch::SpinLeverSwitchNrvWait::sInstance);
    }
    else {
        MR::startBck(this, "On", 0);
        MR::setBckFrameAndStop(this, MR::getBckFrameMax(this));
        MR::startBrk(this, "On");
        MR::setBrkFrameAndStop(this, MR::getBrkFrameMax(this));
        initNerve(&NrvSpinLeverSwitch::SpinLeverSwitchNrvEnd::sInstance);
    }
    makeActorAppeared();
}

void SpinLeverSwitch::initAfterPlacement() {
    mConnector->attachToUnder();
}

void SpinLeverSwitch::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();
    mConnector->connect();
}

void SpinLeverSwitch::attackSensor(HitSensor *pSender, HitSensor *pReceiver) {
    if (MR::isSensorPlayer(pReceiver)){
        if (pSender == (getSensor("spin"))) {
            if (MR::sendMsgPush(pReceiver, pSender)) {
                return;
            }
        }  
    }
}

bool SpinLeverSwitch::receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
    if (!isNerve(&NrvSpinLeverSwitch::SpinLeverSwitchNrvWait::sInstance)) {
        return false;
    }
    if (pReceiver != getSensor("spin")) {
        return false;
    }
    if (MR::isMsgPlayerHitAll(msg)) {
        setNerve(&NrvSpinLeverSwitch::SpinLeverSwitchNrvSwitchOn::sInstance);
        MR::stopSceneForDefaultHit(8);
        return true;
    }
    return false;
}

void SpinLeverSwitch::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", 0);
        MR::startBrk(this, "On");
        MR::setBrkFrameAndStop(this, 0.0f);
    }
}

void SpinLeverSwitch::exeSwitchOn() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "On", 0);
        MR::startBrk(this, "On");
        if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
            MR::startSound(this, "SE_OJ_SPIN_LEVER_SW_HIT_W", -1, -1);
        }
        else {
            MR::startSound(this, "SE_OJ_SPIN_LEVER_SW_HIT", -1, -1);
        }
        MR::tryRumblePadStrong(this, 0);
        MR::shakeCameraNormal();
    }
    if (MR::isStep(this, 8)) {
        if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
            MR::startSound(this, "SE_OJ_SPIN_LEVER_SW_ON_W", -1, -1);
        }
        else {
            MR::startSound(this, "SE_OJ_SPIN_LEVER_SW_ON", -1, -1);
        }        
    }
    if (MR::isStep(this, 15)) {
        MR::onSwitchA(this);
    }
    if (MR::isBckStopped(this)) {
        setNerve(&NrvSpinLeverSwitch::SpinLeverSwitchNrvEnd::sInstance);
    }
}

namespace NrvSpinLeverSwitch {
    INIT_NERVE(SpinLeverSwitchNrvWait);
    INIT_NERVE(SpinLeverSwitchNrvSwitchOn);
    INIT_NERVE(SpinLeverSwitchNrvEnd);

	void SpinLeverSwitchNrvWait::execute(Spine *pSpine) const {
		SpinLeverSwitch *pActor = (SpinLeverSwitch*)pSpine->mExecutor;
		pActor->exeWait();
	}    

	void SpinLeverSwitchNrvSwitchOn::execute(Spine *pSpine) const {
		SpinLeverSwitch *pActor = (SpinLeverSwitch*)pSpine->mExecutor;
		pActor->exeSwitchOn();
	}    

	void SpinLeverSwitchNrvEnd::execute(Spine *pSpine) const {
	}        
};