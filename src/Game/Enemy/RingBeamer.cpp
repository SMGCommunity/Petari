#include "Game/Enemy/RingBeamer.hpp"
#include "Game/Enemy/RingBeam.hpp"

//MR function that has not been defined elsewhere yet
//
namespace MR {
    bool enableGroupAttack(LiveActor*, f32, f32);
};

namespace NrvRingBeamer {
    NEW_NERVE(RingBeamerNrvWait, RingBeamer, Wait);
    NEW_NERVE(RingBeamerNrvAttack, RingBeamer, Attack);
    NEW_NERVE(RingBeamerNrvInter, RingBeamer, Inter);
}; // namespace NrvRingBeamer

RingBeamer::RingBeamer(const char* pName)
    : LiveActor(pName),
      mBeams(nullptr) {
}

void RingBeamer::init(const JMapInfoIter& rIter) {
    initModelManagerWithAnm("RingBeamer", nullptr, false);
    MR::initDefaultPos(this, rIter);
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    initHitSensor(2);
    MR::addHitSensorPush(this, "Body", 8, 160.0f, TVec3f(0.0f, 80.0f, 0.0f));
    MR::addHitSensorMtxEnemyAttack(this, "Head", 8, 130.0f, MR::getJointMtx(this, "Top"), TVec3f(0.0f, 110.0f, 0.0f));
    MR::initShadowVolumeSphere(this, 120.0f);
    initEffectKeeper(1, nullptr, false);
    initSound(8, false);
    initNerve(&NrvRingBeamer::RingBeamerNrvWait::sInstance);
    makeActorAppeared();
    MR::useStageSwitchReadA(this, rIter);
    if (MR::useStageSwitchReadB(this, rIter)) {
        MR::listenStageSwitchOffB(this,
                                  MR::FunctorV0M<RingBeamer*, void (RingBeamer::*)(void)>(this, &RingBeamer::syncSwitchOffB));
    }
    MR::joinToGroupArray(this, rIter, nullptr, 32);
    // initializes to 5 long, but only uses 3?
    mBeams = new RingBeam*[5];
    for (int i = 0; i < 5; i++) {
        mBeams[i] = nullptr;
    }
    f32 arg0 = 20.0f;
    s32 arg1 = 100;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);
    MR::getJMapInfoArg1NoInit(rIter, &arg1);
    for (int i = 0; i < 3; i++) {
        mBeams[i] = new RingBeam("リングビーム", this, false, false);
        mBeams[i]->init(rIter);
        mBeams[i]->setSpeed(arg0);
        mBeams[i]->setLife(arg1);
    }
    MR::startBckWithInterpole(this, "Open", 0);
    MR::setBckFrameAndStop(this, 0.0);
    MR::calcAnimDirect(this);
}

void RingBeamer::syncSwitchOffB() {
    setNerve(&NrvRingBeamer::RingBeamerNrvInter::sInstance);

    for (int i = 0; i < 3; i++) {
        mBeams[i]->kill();
    }
}

void RingBeamer::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
    }

    if (MR::enableGroupAttack(this, 3000.0f, 500.0f)) {
        MR::sendMsgToGroupMember(ACTMES_GROUP_ATTACK, this, getSensor("Body"), "Body");
    }
}

void RingBeamer::exeAttack() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Open", nullptr);
    }

    if (MR::isGreaterEqualStep(this, 240)) {
        setNerve(&NrvRingBeamer::RingBeamerNrvInter::sInstance);
        return;
    }

    if (getNerveStep() % 80 >= 0 && getNerveStep() % 80 < 79) {
        MR::startLevelSound(this, "SE_EM_LV_RINGBEAM_CHARGE", -1, -1, -1);
    }

    if (getNerveStep() % 80 == 0) {
        MR::emitEffect(this, "Charge");
    }

    if (getNerveStep() % 80 == 79) {
        MR::deleteEffect(this, "Charge");

        if (!MR::enableGroupAttack(this, 3200.0f, 500.0f)) {
            MR::sendMsgToGroupMember(ACTMES_GROUP_HIDE, this, getSensor("Body"), "Body");
        } else if (mBeams[getNerveStep() / 80]) {
            mBeams[getNerveStep() / 80]->appear();
        }
    }
}

void RingBeamer::exeInter() {
    if (MR::isFirstStep(this)) {
    }

    if (MR::isGreaterEqualStep(this, 80)) {
        for (int i = 0; i < 3; i++) {
            if (!MR::isDead(mBeams[i])) {
                return;
            }
        }

        setNerve(&NrvRingBeamer::RingBeamerNrvWait::sInstance);
    }
}

void RingBeamer::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayerOrRide(pReceiver) && MR::isSensorEnemyAttack(pSender)) {
        MR::sendMsgEnemyAttackStrong(pReceiver, pSender);
    } else {
        MR::sendMsgPush(pReceiver, pSender);
    }
}

bool RingBeamer::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_GROUP_ATTACK) {
        MR::invalidateClipping(this);
        setNerve(&NrvRingBeamer::RingBeamerNrvAttack::sInstance);

        return true;
    }

    if (msg == ACTMES_GROUP_HIDE) {
        setNerve(&NrvRingBeamer::RingBeamerNrvInter::sInstance);

        return true;
    }

    return false;
}

bool RingBeamer::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgJetTurtleAttack(msg)) {
        return true;
    }

    return MR::isMsgStarPieceReflect(msg);
}
