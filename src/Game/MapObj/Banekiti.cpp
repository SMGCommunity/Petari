#include "Game/MapObj/Banekiti.hpp"

Banekiti::Banekiti(const char *pName) : LiveActor(pName) {
    mAnimScaleCtrl = nullptr;
    mBindStarPointer = nullptr;
    mRailMover = nullptr;
}

void Banekiti::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("Banekiti", nullptr, false);
    MR::connectToSceneMapObjStrongLight(this);
    MR::initLightCtrl(this);
    initHitSensor(3);
    MR::addHitSensorMapObjSimple(this, "body", 4, 80.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorMapObjSimple(this, "right", 4, 80.0f, TVec3f(-100.0f, 0.0f, 0.0f));
    MR::addHitSensorMapObjSimple(this, "left", 4, 80.0f, TVec3f(100.0f, 0.0f, 0.0f));
    initEffectKeeper(1, nullptr, false);
    initSound(4, false);
    initRailRider(rIter);
    mRailMover = new MapPartsRailMover(this);
    mRailMover->init(rIter);
    mRailMover->start();
    TVec3f offset;
    MR::initStarPointerTarget(this, 120.0f, TVec3f(0.0f));
    mAnimScaleCtrl = new AnimScaleController(nullptr);
    mBindStarPointer = new WalkerStateBindStarPointer(this, mAnimScaleCtrl);
    initNerve(&NrvBanekiti::BanekitiNrvWait::sInstance);
    makeActorAppeared();
}

void Banekiti::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
    }
    MR::startLevelSound(this, "SE_OJ_LV_BANEKITI_MOVE", -1, -1, -1);
}

void Banekiti::exeRepel() {
    if (MR::isFirstStep(this)) {
        MR::tryRumblePadWeak(this, 0);
        MR::shakeCameraWeak();
        MR::startBck(this, "Repel", nullptr);
        MR::startSound(this, "SE_OJ_BANEKITI_REPEL", -1, -1);
    }
    if (MR::isBckStopped(this)) {
        setNerve(&NrvBanekiti::BanekitiNrvWait::sInstance);
    }
}

void Banekiti::exeDPDSwoon() {
    if (MR::isFirstStep(this)) {
        mRailMover->_14 = false;
    }
    MR::updateActorStateAndNextNerve(this, mBindStarPointer, &NrvBanekiti::BanekitiNrvWait::sInstance);
}

void Banekiti::endDPDSwoon() {
    mRailMover->_14 = true;
    mBindStarPointer->kill();
}

void Banekiti::calcAndSetBaseMtx() {
    TVec3f mtx;
    LiveActor::calcAndSetBaseMtx();
    mtx.multPS(mScale, mAnimScaleCtrl->_C);
    MR::setBaseScale(this, mtx);
}

void Banekiti::control() {
    mAnimScaleCtrl->updateNerve();
    mRailMover->movement();
    if (mRailMover->isWorking()) {
        mPosition.x = mRailMover->_28.x;
        mPosition.y = mRailMover->_28.y;
        mPosition.z = mRailMover->_28.z;
    }
    if (!isNerve(&NrvBanekiti::BanekitiNrvDPDSwoon::sInstance)) {
        if (mBindStarPointer->tryStartPointBind()) {
            setNerve(&NrvBanekiti::BanekitiNrvDPDSwoon::sInstance);
        }
    }
}

bool Banekiti::receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
    if (MR::isMsgStarPieceAttack(msg)) {
        mAnimScaleCtrl->startHitReaction();
        return true;
    }
    return false;
}

bool Banekiti::receiveOtherMsg(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
    if (!MR::isSensorPlayer(pSender)) {
        return false;
    }
    if (isNerve(&NrvBanekiti::BanekitiNrvRepel::sInstance)) {
        return false;
    }
    if (msg == 68) {
        setNerve(&NrvBanekiti::BanekitiNrvRepel::sInstance);
        return true;
    }   
    return false;
}

namespace NrvBanekiti {
    INIT_NERVE(BanekitiNrvWait);
    INIT_NERVE(BanekitiNrvRepel);
    INIT_NERVE(BanekitiNrvDPDSwoon);

	void BanekitiNrvWait::execute(Spine *pSpine) const {
		Banekiti *pActor = (Banekiti*)pSpine->mExecutor;
		pActor->exeWait();
	}    

	void BanekitiNrvRepel::execute(Spine *pSpine) const {
		Banekiti *pActor = (Banekiti*)pSpine->mExecutor;
		pActor->exeRepel();
	}    

	void BanekitiNrvDPDSwoon::execute(Spine *pSpine) const {
		Banekiti *pActor = (Banekiti*)pSpine->mExecutor;
		pActor->exeDPDSwoon();
	}

    void BanekitiNrvDPDSwoon::executeOnEnd(Spine *pSpine) const {
		Banekiti *pActor = (Banekiti*)pSpine->mExecutor;
		pActor->endDPDSwoon();
	}        
};