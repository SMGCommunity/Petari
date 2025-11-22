#include "Game/Enemy/BallBeamer.hpp"
#include "Game/Enemy/RingBeam.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"

namespace NrvBallBeamer {
    NEW_NERVE(BallBeamerNrvWait, BallBeamer, Wait);
    NEW_NERVE(BallBeamerNrvAttack, BallBeamer, Attack);
    NEW_NERVE(BallBeamerNrvInter, BallBeamer, Inter);
};  // namespace NrvBallBeamer

BallBeamer::BallBeamer(const char* pName) : LiveActor(pName), mBeams(nullptr), _90(false) {
    _98.identity();
}

void BallBeamer::makeArchiveList(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter) {
    s32 arg2 = -1;
    MR::getJMapInfoArg2NoInit(rIter, &arg2);
    pCollector->addArchive("JumpBeamerBeam");
    pCollector->addArchive("JumpBeamerBeamBloom");
    if (!arg2) {
        pCollector->addArchive("JumpBeamerBeamShadow");
    }
}

void BallBeamer::init(const JMapInfoIter& rIter) {
    initModelManagerWithAnm("BallBeamer", nullptr, nullptr);
    MR::initDefaultPos(this, rIter);
    mPosition.y = mPosition.y - 50.0f;
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    initHitSensor(1);
    MR::addHitSensorPush(this, "Body", 8, 120.0f, TVec3f(0.0f, -20.0f, 0.0f));
    MR::initShadowVolumeSphere(this, 120.0f);
    initEffectKeeper(3, nullptr, nullptr);
    initSound(2, nullptr);
    initNerve(&NrvBallBeamer::BallBeamerNrvWait::sInstance);
    makeActorAppeared();
    if (MR::useStageSwitchReadA(this, rIter)) {
        MR::listenStageSwitchOnOffA(this, MR::Functor< BallBeamer >(this, &BallBeamer::syncSwitchOffA),
                                    MR::Functor< BallBeamer >(this, &BallBeamer::syncSwitchOnA));
    }

    if (MR::useStageSwitchReadB(this, rIter)) {
        MR::listenStageSwitchOnB(this, MR::Functor< BallBeamer >(this, &BallBeamer::syncSwitchOnB));
    }

    mBeams = new RingBeam*[12];
    for (int i = 0; i < 12; i++) {
        mBeams[i] = nullptr;
    }
    f32 arg0 = 12.0f;
    s32 arg1 = 530;
    s32 arg2 = -1;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);
    MR::getJMapInfoArg1NoInit(rIter, &arg1);
    MR::getJMapInfoArg2NoInit(rIter, &arg2);
    for (int i = 0; i < 12; i++) {
        if (arg2 == 0) {
            mBeams[i] = new RingBeam("リングビーム", this, true, true);
        } else {
            mBeams[i] = new RingBeam("リングビーム", this, true, false);
        }
        mBeams[i]->init(rIter);
        mBeams[i]->setSpeed(arg0);
        mBeams[i]->setLife(arg1);
    }
    _98.setInline(MR::getJointMtx(this, "Head"));
    MR::initCollisionParts(this, "Head", getSensor("Body"), _98.toMtxPtr());
    MR::validateCollisionParts(this);
}

void BallBeamer::syncSwitchOnA() {
    _90 = true;
    BallBeamer::setupAttack();
}

void BallBeamer::syncSwitchOffA() {
    _90 = false;
}

void BallBeamer::syncSwitchOnB() {
    MR::deleteEffect(this, "Charge");
    MR::emitEffect(this, "Vanish");
    kill();
    for (int i = 0; i < 12; i++) {
        if (mBeams[i]) {
            mBeams[i]->kill();
        }
    }
}

void BallBeamer::setupAttack() {
    if (!MR::isValidSwitchA(this) || MR::isOnSwitchA(this)) {
        MR::invalidateClipping(this);
        if (!isNerve(&NrvBallBeamer::BallBeamerNrvAttack::sInstance)) {
            setNerve(&NrvBallBeamer::BallBeamerNrvAttack::sInstance);
        }
    }
}

bool BallBeamer::tryAttack() {
    for (int i = 0; i < 12; i++) {
        if (MR::isDead(mBeams[i])) {
            mBeams[i]->appear();
            return true;
        }
    }
    return false;
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
    } else {
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
    if (MR::isFirstStep(this)) {
    }

    if (MR::isGreaterEqualStep(this, 120)) {
        if (_90) {
            setNerve(&NrvBallBeamer::BallBeamerNrvAttack::sInstance);
        } else {
            setNerve(&NrvBallBeamer::BallBeamerNrvWait::sInstance);
        }
    }
}

void BallBeamer::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    MR::sendMsgPush(pReceiver, pSender);
}

bool BallBeamer::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}

bool BallBeamer::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return MR::isMsgStarPieceReflect(msg);
}
