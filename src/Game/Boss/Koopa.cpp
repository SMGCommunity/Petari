#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/BossAccessor.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/Boss/KoopaParts.hpp"
#include "Game/Boss/KoopaSensorCtrl.hpp"
#include "Game/Boss/KoopaSequencer.hpp"
#include "Game/LiveActor/ActorJointCtrl.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

Koopa::Koopa(const char* pName, KoopaSequencer* pSequencer)
    : LiveActor(pName), mFront(0.0f, 0.0f, 1.0f), mSequencer(pSequencer), mSensorCtrl(), mParts(), mJointCtrl() {
}

void Koopa::kill() {
    MR::onSwitchDead(this);
    LiveActor::kill();
}

void Koopa::control() {
    mSequencer->update();
    mJointCtrl->update();
}

void Koopa::calcAndSetBaseMtx() {
    mSequencer->calcAndSetBaseMtx();
    mJointCtrl->setCallBackFunction();
}

void Koopa::updateHitSensor(HitSensor* pSensor) {
    mSensorCtrl->update(pSensor);
}

void Koopa::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    mSequencer->attackSensor(pSender, pReceiver);
}

bool Koopa::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return mSequencer->receiveMsgPlayerAttack(msg, pSender, pReceiver);
}

bool Koopa::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return mSequencer->receiveMsgEnemyAttack(msg, pSender, pReceiver);
}

bool Koopa::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return mSequencer->receiveOtherMsg(msg, pSender, pReceiver);
}

Koopa::~Koopa() {
}

void Koopa::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("Koopa", nullptr, false);

    MR::calcActorAxisZ(&mFront, this);

    MR::connectToSceneEnemy(this);

    MR::initLightCtrl(this);
    initHitSensor(32);

    mSensorCtrl = new KoopaSensorCtrl(this);
    mSensorCtrl->initSensor();

    initBinder(200.0f, 200.0f, 0);
    initEffectKeeper(0, nullptr, false);
    initSound(16, false);

    mJointCtrl = new ActorJointCtrl(this);
    KoopaFunction::endFaceCtrlDirect(this);

    MR::initShadowFromCSV(this, "Shadow");
    MR::invalidateShadowGroup(this, "デモ用");

    MR::onCalcGravity(this);

    mParts = new KoopaParts(this, rIter);

    mSequencer->init(this, rIter);

    MR::invalidateClipping(this);

    MR::declarePowerStar(this);

    BossAccess::setBossAccessorKoopa(this);

    MR::registerDemoSimpleCastAll(this);

    MR::useStageSwitchReadA(this, rIter);
    MR::useStageSwitchWriteB(this, rIter);
    MR::needStageSwitchWriteDead(this, rIter);

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    } else {
        makeActorAppeared();
        MR::hideModel(this);
    }
}

void KoopaSequencer::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
}

bool KoopaSequencer::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}

bool KoopaSequencer::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}

bool KoopaSequencer::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}
