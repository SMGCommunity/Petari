#include "Game/MapObj/HipDropRock.hpp"


HipDropRock::HipDropRock(const char *pName) : LiveActor(pName) {
    mModel = nullptr;
    _C0 = -1;
    _C4 = -1;
    _C8 = false;
    _90.identity();
}

void HipDropRock::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("HipDropRock", nullptr, false);
    initBreakModel();
    MR::connectToSceneMapObjStrongLight(this);
    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);
    MR::initCollisionParts(this, "HipDropRock", getSensor(nullptr), nullptr);
    initEffectKeeper(0, nullptr, false);
    initSound(2, false);
    MR::useStageSwitchWriteDead(this, rIter);
    MR::useStageSwitchWriteB(this, rIter);
    HipDropRock::initItem(rIter);
    MR::getJMapInfoArg7NoInit(rIter, &_C8); 
    if (MR::tryRegisterDemoCast(this, rIter)) {
        MR::tryRegisterDemoCast(mModel, rIter);
    }
    initNerve(&NrvHipDropRock::HipDropRockNrvWait::sInstance);
    makeActorAppeared();
}

bool HipDropRock::receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
    if (isNerve(&NrvHipDropRock::HipDropRockNrvBreak::sInstance)) {
        return false;
    }
    if (MR::isMsgPlayerHipDropFloor(msg)) {
        setNerve(&NrvHipDropRock::HipDropRockNrvBreak::sInstance);
        return true;
    }
    return false;
}

void HipDropRock::initItem(const JMapInfoIter &rIter) {
    MR::getJMapInfoArg0NoInit(rIter, &_C0);
    MR::getJMapInfoArg1NoInit(rIter, &_C4);
    if (_C0 == 1 && _C4 == -1) {
        _C4 = 6;
    }
    if (!_C0) {
        MR::declareCoin(this, 1);
    }
    else if (_C0 == 1){
        MR::declareStarPiece(this, _C4);
    }
}

void HipDropRock::appearItem() {
    if (_C0 != -1) {
        if (!_C0) { 
            MR::appearCoinPop(this, mPosition, 1);
        }
        else if (_C0 == 1){
            MR::appearStarPiece(this, mPosition, _C4, 10.0f, 40.0f, false);
            MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);  
        } 
    }
}

void HipDropRock::exeBreak() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_ROCK_BREAK", -1, -1);
        MR::invalidateCollisionParts(this);
        MR::invalidateClipping(this);
        if (_C8) {
            MR::hideModel(this);
        }
        else {
            MR::startBva(this, "BreakLevel");
        }
        mModel->makeActorAppeared();
        MR::invalidateClipping(mModel);
        MR::startBck(mModel, "HipDropRockBreak", nullptr);
        if (MR::isValidSwitchB(this)) {
            MR::onSwitchB(this);
        }
        HipDropRock::appearItem();
    }    
    if (MR::isStep(this, 30) && MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }
    if (MR::isBckStopped(mModel) && MR::isGreaterStep(this, 30)) {
        mModel->kill();
        if (_C8) {
            kill();
        }
        else {
            setNerve(&NrvHipDropRock::HipDropRockNrvWreck::sInstance);
        }
    }
}

namespace NrvHipDropRock {
    INIT_NERVE(HipDropRockNrvWait);
    INIT_NERVE(HipDropRockNrvBreak);
    INIT_NERVE(HipDropRockNrvWreck);

	void HipDropRockNrvWait::execute(Spine *pSpine) const {
		HipDropRock *pActor = (HipDropRock*)pSpine->mExecutor;
		pActor->ValClip1();
	}    

	void HipDropRockNrvBreak::execute(Spine *pSpine) const {
		HipDropRock *pActor = (HipDropRock*)pSpine->mExecutor;
		pActor->exeBreak();
	}    

	void HipDropRockNrvWreck::execute(Spine *pSpine) const {
		HipDropRock *pActor = (HipDropRock*)pSpine->mExecutor;
		pActor->ValClip2();
	}    
};