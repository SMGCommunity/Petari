#include "Game/Enemy/Jiraira.hpp"
#include "Game/Map/CollisionParts.hpp"

Jiraira::Jiraira(const char *pName) : LiveActor(pName) {
    _8C = nullptr;
    _90 = 500.0f;
}

Jiraira::~Jiraira() {

}

void Jiraira::appear() {
    LiveActor::appear();
    getSensor("explode")->invalidate();
}

void Jiraira::kill() {
    LiveActor::kill();
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }
}

void Jiraira::control() {
    _8C->setMtx();
}

void Jiraira::exeWait() {
    if (MR::isFirstStep(this)){
        MR::startBck(this, "Wait", 0);
        MR::startBrk(this, "Wait");
    }

    if (MR::isOnPlayer(getSensor("Body"))){
        setNerve(&NrvJiraira::HostTypeNrvStepped::sInstance);
    }
}

void Jiraira::exeStepped() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Stepped", 0);
        MR::startBrk(this, "Stepped");
        MR::tryRumblePadMiddle(this, 0);
        MR::startSound(this, "SE_OJ_JIRAIRA_STEPPED", -1, -1);
    }   
    MR::startLevelSound(this, "SE_OJ_LV_JIRAIRA_CHARGE", -1, -1, -1);
    if (MR::isGreaterStep(this, 30)) {
        setNerve(&NrvJiraira::HostTypeNrvSteppedExplode::sInstance);
    }
}

void Jiraira::exeExplode() {
    /*
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "explosion");
        MR::startSound(this, "SE_OJ_JIRAIRA_EXPLODE", -1, -1);
        MR::startBck(this, "Down", 0);
        MR::startBrk(this, "Down");
        getSensor("explode")->validate();
        //MR::tryRumblePadAndCameraDistanceStrong(this, 800.0, 1200.0, 2000.0); UNDOCUMENTED FUNCTION
    }
    if (MR::isStep(this, 8)) {
        getSensor("explode")->invalidate();
    }
    else if (MR::isLessStep(this, 8)) {

    }
    if (MR::isGreaterStep(this, 188)) {
        setNerve(&NrvJiraira::HostTypeNrvPreRecover::sInstance);
    }
    */
}

void Jiraira::exePreRecover() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Down", 0);
        MR::startBrk(this, "RecoveryLoop");
    }
    MR::startLevelSound(this, "SE_OJ_LV_JIRAIRA_RECOVERING", -1, -1, -1);
    if (MR::isGreaterStep(this, 120)) {
        setNerve(&NrvJiraira::HostTypeNrvRecover::sInstance);
    }
}

void Jiraira::exeRecover() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Recovery", 0);
        MR::startBrk(this, "Recovery");
        MR::startSound(this, "SE_OJ_JIRAIRA_RECOVER", -1, -1);        
    }
    if (MR::isBckStopped(this)) {
        setNerve(&NrvJiraira::HostTypeNrvWait::sInstance);
    }
}

bool Jiraira::receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
    if (isNerve(&NrvJiraira::HostTypeNrvExplode::sInstance) || isNerve(&NrvJiraira::HostTypeNrvSteppedExplode::sInstance) || isNerve(&NrvJiraira::HostTypeNrvPreRecover::sInstance)) {
        return false;
    }
    if (MR::isMsgStarPieceAttack(msg)) {
        if (!isNerve(&NrvJiraira::HostTypeNrvStepped::sInstance)) {
            setNerve(&NrvJiraira::HostTypeNrvStepped::sInstance);
        }
        return true;
    }
    return false;
}

bool Jiraira::receiveMsgEnemyAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
    if (isNerve(&NrvJiraira::HostTypeNrvExplode::sInstance) || isNerve(&NrvJiraira::HostTypeNrvSteppedExplode::sInstance)) {
        return false;
    }
    if (MR::isMsgExplosionAttack(msg)) {
        setNerve(&NrvJiraira::HostTypeNrvExplode::sInstance);
        return true;
    }
    return false;
}

namespace NrvJiraira {
    INIT_NERVE(HostTypeNrvWait);
    INIT_NERVE(HostTypeNrvStepped);
    INIT_NERVE(HostTypeNrvPreRecover);
    INIT_NERVE(HostTypeNrvRecover);
    INIT_NERVE(HostTypeNrvSteppedExplode);
    INIT_NERVE(HostTypeNrvExplode);

	void HostTypeNrvWait::execute(Spine *pSpine) const {
		Jiraira *pActor = (Jiraira*)pSpine->mExecutor;
		pActor->exeWait();
	}    

	void HostTypeNrvStepped::execute(Spine *pSpine) const {
		Jiraira *pActor = (Jiraira*)pSpine->mExecutor;
		pActor->exeStepped();
	}    

	void HostTypeNrvPreRecover::execute(Spine *pSpine) const {
		Jiraira *pActor = (Jiraira*)pSpine->mExecutor;
		pActor->exePreRecover();
	}    

	void HostTypeNrvRecover::execute(Spine *pSpine) const {
		Jiraira *pActor = (Jiraira*)pSpine->mExecutor;
		pActor->exeRecover();
	}    

	void HostTypeNrvSteppedExplode::execute(Spine *pSpine) const {
		Jiraira *pActor = (Jiraira*)pSpine->mExecutor;
		pActor->exeExplode();
	}    

	void HostTypeNrvExplode::execute(Spine *pSpine) const {
		Jiraira *pActor = (Jiraira*)pSpine->mExecutor;
		pActor->exeExplode();
	}    
};