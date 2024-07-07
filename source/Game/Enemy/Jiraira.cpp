#include "Game/Enemy/Jiraira.hpp"
#include "Game/Map/CollisionParts.hpp"

Jiraira::Jiraira(const char *pName) : LiveActor(pName) {
    _8C = nullptr;
    _90 = 500.0f;
}

Jiraira::~Jiraira() {

}

void Jiraira::init(const JMapInfoIter &rIter) {
    if (MR::isValidInfo(rIter)) {
        MR::initDefaultPos(this, rIter);
        s32 arg1 = _90;
        MR::getJMapInfoArg1NoInit(rIter, &arg1);
        _90 = arg1;
        MR::useStageSwitchWriteDead(this, rIter);
    }
    initModelManagerWithAnm("Jiraira", nullptr, false);
    MR::connectToSceneCollisionEnemy(this);
    initHitSensor(2);
    f32 scaleY = mScale.y;
    TVec3f offset;
    offset.x = 0.0f;
    offset.y = 30.0f;
    offset.z = 0.0f;
    MR::addHitSensorMapObjMoveCollision(this, "body", 0x10, (100.0f * scaleY), offset);
    TVec3f offset2;
    offset2.x = 0.0f;
    offset2.y = 30.0f * scaleY;
    offset2.z = 0.0f;
    MR::addHitSensorEnemyAttack(this, "explode", 16, (_90 * scaleY), offset2);
    MR::initCollisionPartsAutoEqualScaleOne(this, "Jiraira", getSensor("body"), MR::getJointMtx(this, "Jiraira"));
    CollisionParts *colPart = MR::createCollisionPartsFromLiveActor(this, "Button", getSensor("body"), MR::getJointMtx(this, "Button"), (MR::CollisionScaleType)1);
    _8C = colPart;
    MR::validateCollisionParts(colPart);
    initSound(6, false);
    initEffectKeeper(0, nullptr, false);
    initNerve(&NrvJiraira::HostTypeNrvWait::sInstance);
    MR::invalidateClipping(this);
    appear();
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
        MR::startBck(this, "Wait", nullptr);
        MR::startBrk(this, "Wait");
    }

    if (MR::isOnPlayer(getSensor("Body"))){
        setNerve(&NrvJiraira::HostTypeNrvStepped::sInstance);
    }
}

void Jiraira::exeStepped() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Stepped", nullptr);
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
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "explosion");
        MR::startSound(this, "SE_OJ_JIRAIRA_EXPLODE", -1, -1);
        MR::startBck(this, "Down", nullptr);
        MR::startBrk(this, "Down");
        getSensor("explode")->validate();
        MR::tryRumblePadAndCameraDistanceStrong(this, 800.0f, 1200.0f, 2000.0f);
    }

    if (MR::isStep(this, 8)) {
        getSensor("explode")->invalidate();
    }
    else if (MR::isLessStep(this, 8)) {
        getSensor("explode")->mRadius = _90 * getNerveStep() * 0.125f;
    }

    if (MR::isGreaterStep(this, 188)) {
        setNerve(&NrvJiraira::HostTypeNrvPreRecover::sInstance);
    }
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

#ifdef NON_MATCHING
void Jiraira::attackSensor(HitSensor *pSender, HitSensor *pReceiver) {
    if (pSender == getSensor("explode")) {
        TVec3f tes = mPosition;
        LiveActor* sensorActor;
        TVec3f thing;
        HitSensor* sensor = 0;
        sensorActor = sensor->mActor;
        thing.subInline2(mPosition, sensorActor->mPosition);
        if (!MR::isExistMapCollisionExceptActor(thing, thing, this)) {
            if (MR::isSensorPlayer(pReceiver)) {
                MR::sendMsgEnemyAttackExplosion(pReceiver, pSender);
            }
            else if (MR::isSensorEnemy(pReceiver)) {
                if (isNerve(&NrvJiraira::HostTypeNrvSteppedExplode::sInstance)) {
                    MR::sendMsgEnemyAttackExplosion(pReceiver, pSender);
                }
            }    
            else if (MR::isSensorMapObj(pReceiver)) {
                if (isNerve(&NrvJiraira::HostTypeNrvSteppedExplode::sInstance)) {
                    MR::sendMsgEnemyAttackExplosion(pReceiver, pSender);
                }
            }
            
        }    
	}
}
#endif

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