#include "Game/MapObj/PicketSwitch.hpp"

PicketSwitch::PicketSwitch(const char *pName) : LiveActor(pName) {

}

PicketSwitch::~PicketSwitch() {

}

void PicketSwitch::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    MR::needStageSwitchWriteA(this, rIter);
    const char* pName;
    MR::getObjectName(&pName, rIter);
    initModelManagerWithAnm(pName, nullptr, false);
    MR::connectToSceneMapObjDecorationStrongLight(this);
    initHitSensor(1);
    MR::addBodyMessageSensor(this, 79);
    MR::initCollisionPartsAutoEqualScale(this, pName, getSensor(0), MR::getJointMtx(this, "Picket"));
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    initNerve(&NrvPicketSwitch::PicketSwitchNrvWait::sInstance);
    makeActorAppeared();
}

void PicketSwitch::exeFirstDrop() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "First", nullptr);
        MR::tryRumblePadStrong(this, 0);
        MR::startSound(this, "SE_OJ_PICKET_SWITCH_DROP", -1, -1);
    } 
}

void PicketSwitch::exeLastDrop() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Second", nullptr);
        MR::startBrk(this, "Second");
        MR::tryRumblePadVeryStrong(this, 0);
        MR::startSound(this, "SE_OJ_PICKET_SWITCH_DROP", -1, -1);
        MR::startSound(this, "SE_OJ_PICKET_SWITCH_ON", -1, -1);
    }

    if (MR::isBckStopped(this)) {
        MR::onSwitchA(this);
        MR::invalidateHitSensors(this);
    }
}

bool PicketSwitch::receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
    if (MR::isMsgPlayerHipDropFloor(msg)) {
        if (pReceiver->isType(79)) {
            if (isNerve(&NrvPicketSwitch::PicketSwitchNrvWait::sInstance)) {
                setNerve(&NrvPicketSwitch::PicketSwitchNrvLastDrop::sInstance);
                return true;
            }  
            if ((isNerve(&NrvPicketSwitch::PicketSwitchNrvFirstDrop::sInstance))) {
                if (MR::isBckStopped(this)) {
                    setNerve(&NrvPicketSwitch::PicketSwitchNrvLastDrop::sInstance);
                    return true;
                }
            }
            return false;
        }
    }
    return false;
}

namespace NrvPicketSwitch {
    INIT_NERVE(PicketSwitchNrvWait);
    INIT_NERVE(PicketSwitchNrvFirstDrop);
    INIT_NERVE(PicketSwitchNrvLastDrop);

	void PicketSwitchNrvWait::execute(Spine *pSpine) const {
		PicketSwitch *pActor = (PicketSwitch*)pSpine->mExecutor;
		pActor->exeWait();
	}    

	void PicketSwitchNrvFirstDrop::execute(Spine *pSpine) const {
		PicketSwitch *pActor = (PicketSwitch*)pSpine->mExecutor;
		pActor->exeFirstDrop();
	}    

	void PicketSwitchNrvLastDrop::execute(Spine *pSpine) const {
        PicketSwitch *pActor = (PicketSwitch*)pSpine->mExecutor;
		pActor->exeLastDrop();
	}        
};