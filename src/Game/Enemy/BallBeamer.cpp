#include "Game/Enemy/BallBeamer.hpp"

BallBeamer::BallBeamer(const char *pName) : LiveActor(pName) {
    mBeams = nullptr;
    _90 = false;
    _98.identity();
}

void BallBeamer::makeArchiveList(NameObjArchiveListCollector *pCollector, const JMapInfoIter &rIter) {
    s32 arg2 = -1;
    MR::getJMapInfoArg2NoInit(rIter, &arg2);
    pCollector->addArchive("JumpBeamerBeam");
    pCollector->addArchive("JumpBeamerBeamBloom");
    if (!arg2) {
        pCollector->addArchive("JumpBeamerBeamShadow");
    }
}

#ifdef NON_MATCHING
void BallBeamer::init(const JMapInfoIter &rIter) {
    initModelManagerWithAnm("BallBeamer", nullptr, nullptr);
    MR::initDefaultPos(this, rIter);
    mPosition.y = mPosition.y - 50.0f;
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    initHitSensor(1);
    TVec3f vec;
    vec.x = 0.0f;
    vec.y = -20.0f;
    vec.z = 0.0f;
    MR::addHitSensorPush(this, "Body", 8, 120.0f, vec);
    MR::initShadowVolumeSphere(this, 120.0f);
    initEffectKeeper(3, nullptr, nullptr);
    initSound(2, nullptr);
    initNerve(&NrvBallBeamer::BallBeamerNrvWait::sInstance);
    makeActorAppeared();
    if (MR::useStageSwitchReadA(this, rIter)) {
        MR::FunctorV0M<BallBeamer *, void (BallBeamer::*)()> functor1 = MR::Functor<BallBeamer>(this, &BallBeamer::syncSwitchOffA);
        MR::FunctorV0M<BallBeamer *, void (BallBeamer::*)()> functor2 = MR::Functor<BallBeamer>(this, &BallBeamer::syncSwitchOnA);
        MR::listenStageSwitchOnOffA(this, functor1, functor2);
    }

    if (MR::useStageSwitchReadB(this, rIter)) {
        MR::FunctorV0M<BallBeamer *, void (BallBeamer::*)()> functor3 = MR::Functor<BallBeamer>(this, &BallBeamer::syncSwitchOnB);
        MR::listenStageSwitchOnB(this, functor3);
    }





    MR::initCollisionParts(this, "Head", getSensor("Body"), _98.toMtxPtr());
    MR::validateCollisionParts(this);
}
#endif

void BallBeamer::syncSwitchOnA() {
    _90 = true;
    BallBeamer::setupAttack();
}

void BallBeamer::syncSwitchOffA() {
    _90 = false;
}

void BallBeamer::syncSwitchOnB() {
    
}

void BallBeamer::setupAttack() {
    if (!MR::isValidSwitchA(this) || MR::isOnSwitchA(this)) {
        MR::invalidateClipping(this);
        if (!isNerve(&NrvBallBeamer::BallBeamerNrvAttack::sInstance)) {
            setNerve(&NrvBallBeamer::BallBeamerNrvAttack::sInstance);
        }
    }
}

void BallBeamer::tryAttack() {

}

void BallBeamer::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
        MR::startAction(this, "Wait");
    }

    if (MR::isNearPlayer(this, 3000.0f)) {
        BallBeamer::setupAttack();
    }
}

void BallBeamer::exeAttack() {
    if (MR::isGreaterEqualStep(this, 360)) {
        setNerve(&NrvBallBeamer::BallBeamerNrvInter::sInstance);
    }
    else {
        if (getNerveStep() % 120 >= 75 && getNerveStep() % 120 < 119) {
            MR::startLevelSound(this, "SE_EM_LV_RINGBEAM_CHARGE", -1, -1, -1);
        }

        if (getNerveStep() % 120 == 75) {
            MR::emitEffect(this, "Charge");
        }

        if (getNerveStep() % 120 == 119) {
            MR::startAction(this, "Sign");
            MR::startSound(this, "SE_EM_BALLBEAMER_ROT_HEAD", -1, -1);
            BallBeamer::tryAttack();
        }
    }
}

void BallBeamer::exeInter() {
    MR::isFirstStep(this);
    if (MR::isGreaterEqualStep(this, 120)) {
        if (_90) {
            setNerve(&NrvBallBeamer::BallBeamerNrvAttack::sInstance);
        }
        else {
            setNerve(&NrvBallBeamer::BallBeamerNrvWait::sInstance);
        }
    }    
}

void BallBeamer::attackSensor(HitSensor *pSender, HitSensor *pReceiver) {
    MR::sendMsgPush(pReceiver, pSender);
}

bool BallBeamer::receiveOtherMsg(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
    return false;
}

bool BallBeamer::receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
    return MR::isMsgStarPieceReflect(msg);
}

namespace NrvBallBeamer {
    INIT_NERVE(BallBeamerNrvWait);
    INIT_NERVE(BallBeamerNrvAttack);
    INIT_NERVE(BallBeamerNrvInter)

	void BallBeamerNrvWait::execute(Spine *pSpine) const {
		BallBeamer *pActor = (BallBeamer*)pSpine->mExecutor;
		pActor->exeWait();
	}    

	void BallBeamerNrvAttack::execute(Spine *pSpine) const {
		BallBeamer *pActor = (BallBeamer*)pSpine->mExecutor;
		pActor->exeAttack();
	}    

	void BallBeamerNrvInter::execute(Spine *pSpine) const {
		BallBeamer *pActor = (BallBeamer*)pSpine->mExecutor;
		pActor->exeInter();
	}    
};