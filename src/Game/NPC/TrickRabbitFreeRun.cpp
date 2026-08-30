#include "Game/NPC/TrickRabbitFreeRun.hpp"
#include "Game/Enemy/WalkerStateBlowDamage.hpp"
#include "Game/Enemy/WalkerStateRunaway.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/SpotMarkLight.hpp"
#include "Game/NPC/RabbitStateCaught.hpp"
#include "Game/NPC/RabbitStateWaitStart.hpp"
#include "Game/NPC/TrickRabbitUtil.hpp"
#include "Game/Util.hpp"

void TrickRabbitSnow_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)3.0f;
}

namespace {
    static MR::ActorMoveParam sStartAwayParam = {1.4f, 1.0f, 0.9f, 4.0f};
    static const f32 sBodyRadius = 70.0f;
    static const f32 sCatchRadius = 30.0f;
    static const f32 sBindRadius = 60.0f;
    static const f32 sBindYOffset = 60.0f;
    static const f32 sUpVecBlendRate = 0.1f;
    static const f32 sFrontVecBlendRate = 0.2f;
    // static const f32 sStarPieceHitAccelH = _;
    // static const f32 sStarPieceHitAccelV = _;
    static const f32 sPressureRange = 600.0f;
    static const f32 sNormalAccel = 0.7f;
    static const f32 sPressureAccel = 1.5f;
    static const f32 sBrakeAccelRate = 0.3f;
};  // namespace

namespace NrvTrickRabbitFreeRun {
    NEW_NERVE(TrickRabbitFreeRunNrvWaitStart, TrickRabbitFreeRun, WaitStart);
    NEW_NERVE(TrickRabbitFreeRunNrvTryDemo, TrickRabbitFreeRun, TryDemo);
    NEW_NERVE(TrickRabbitFreeRunNrvRunawayStart, TrickRabbitFreeRun, RunawayStart);
    NEW_NERVE(TrickRabbitFreeRunNrvRunaway, TrickRabbitFreeRun, Runaway);
    NEW_NERVE(TrickRabbitFreeRunNrvBlowDamage, TrickRabbitFreeRun, BlowDamage);
    NEW_NERVE(TrickRabbitFreeRunNrvCaught, TrickRabbitFreeRun, Caught);
    NEW_NERVE(TrickRabbitFreeRunNrvPowerStarDemo, TrickRabbitFreeRun, PowerStarDemo);
    NEW_NERVE(TrickRabbitFreeRunNrvGiveUp, TrickRabbitFreeRun, GiveUp);
};  // namespace NrvTrickRabbitFreeRun

TrickRabbitFreeRun::TrickRabbitFreeRun(const char* pName)
    : LiveActor(pName), mRotateQuat(0.0f, 0.0f, 0.0f, 1.0f), mFrontVec(0.0f, 0.0f, 1.0f), mStateWaitStart(), mStateRunaway(), mStateBlowDamage(),
      mStateCaught(), mTalkCtrl(), mFootPrint(), mSpotMarkLight() {
}

void TrickRabbitFreeRun::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("TrickRabbit", nullptr, false);
    MR::connectToSceneNpc(this);
    MR::initLightCtrl(this);
    MR::makeQuatAndFrontFromRotate(&mRotateQuat, &mFrontVec, this);

    mSpotMarkLight = new SpotMarkLight(this, 100.0f, 1500.0f, nullptr);
    mSpotMarkLight->initWithoutIter();

    MR::initShadowFromCSV(this, "Shadow");

    mFootPrint = TrickRabbitUtil::createRabbitFootPrint(this);

    initEffectKeeper(0, nullptr, 0);
    MR::onEmitEffectSyncClipping(this, "Light");
    MR::onForceDeleteEffectSyncClipping(this, "Light");
    initSensor();
    initBinder(::sBindRadius, ::sBindYOffset, 0);

    mTalkCtrl = MR::createTalkCtrl(this, rIter, "TrickRabbit", TVec3f(0.0f, 120.0f, 0.0f), nullptr);

    initState();
    initSound(4, false);
    MR::onCalcGravity(this);
    MR::declarePowerStar(this);
    initNerve(&NrvTrickRabbitFreeRun::TrickRabbitFreeRunNrvWaitStart::sInstance);
    makeActorAppeared();
}

void TrickRabbitFreeRun::initAfterPlacement() {
    MR::trySetMoveLimitCollision(this);
}

void TrickRabbitFreeRun::initSensor() {
    initHitSensor(2);
    MR::addHitSensorAtJointEnemy(this, "Body", "Spine", 8, ::sBodyRadius, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJointEnemy(this, "Catch", "Spine", 8, ::sCatchRadius, TVec3f(0.0f, 0.0f, 0.0f));
    MR::initStarPointerTargetAtJoint(this, "Spine", ::sBodyRadius, TVec3f(0.0f));
}

void TrickRabbitFreeRun::initState() {
    mStateCaught = new RabbitStateCaught(this, mTalkCtrl);
    mStateCaught->init();

    mStateWaitStart = new RabbitStateWaitStart(this, &mFrontVec, mTalkCtrl);
    mStateWaitStart->init();

    mStateRunaway = new WalkerStateRunaway(this, &mFrontVec, nullptr);
    mStateRunaway->init();

    mStateBlowDamage = new WalkerStateBlowDamage(this, &mFrontVec, nullptr);
    mStateBlowDamage->init();
}

void TrickRabbitFreeRun::control() {
    MR::blendQuatUpFront(&mRotateQuat, -mGravity, mFrontVec, ::sUpVecBlendRate, ::sFrontVecBlendRate);
}

void TrickRabbitFreeRun::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, mRotateQuat);
}

void TrickRabbitFreeRun::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorPlayer(pReceiver)) {
        return;
    }

    if (pSender == getSensor("Catch") && requestCaught()) {
        return;
    }

    if (isEnableCaught()) {
        return;
    }

    MR::sendMsgPush(pReceiver, pSender);
}

bool TrickRabbitFreeRun::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pSender)) {
        return !isEnableCaught();
    }

    return false;
}

bool TrickRabbitFreeRun::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgToEnemyAttackBlow(msg)) {
        return receiveMsgBlowDamage(pSender, pReceiver);
    }

    return false;
}

bool TrickRabbitFreeRun::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvTrickRabbitFreeRun::TrickRabbitFreeRunNrvWaitStart::sInstance) ||
        isNerve(&NrvTrickRabbitFreeRun::TrickRabbitFreeRunNrvGiveUp::sInstance)) {
        return mStateWaitStart->receiveMsgPlayerAttack(msg, pSender, pReceiver);
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        if (receiveMsgBlowDamage(pSender, pReceiver)) {
            return true;
        }
    }

    if (MR::isMsgPlayerSpinAttack(msg)) {
        return requestCaught();
    }

    return false;
}

bool TrickRabbitFreeRun::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvTrickRabbitFreeRun::TrickRabbitFreeRunNrvWaitStart::sInstance) ||
        isNerve(&NrvTrickRabbitFreeRun::TrickRabbitFreeRunNrvGiveUp::sInstance)) {
        return mStateWaitStart->receiveOtherMsg(msg, pSender, pReceiver);
    }

    return false;
}

bool TrickRabbitFreeRun::receiveMsgBlowDamage(HitSensor* pSender, HitSensor* pReceiver) {
    if (isEnableBlowDamage()) {
        MR::setVelocitySeparateHV(this, pSender, pReceiver, 20.0f, 20.0f);
        setNerve(&NrvTrickRabbitFreeRun::TrickRabbitFreeRunNrvBlowDamage::sInstance);

        return true;
    }

    return false;
}

bool TrickRabbitFreeRun::requestCaught() {
    if (isEnableCaught()) {
        MR::invalidateClipping(this);
        MR::forwardNode(mTalkCtrl);
        setNerve(&NrvTrickRabbitFreeRun::TrickRabbitFreeRunNrvCaught::sInstance);

        return true;
    }

    return false;
}

void TrickRabbitFreeRun::exeWaitStart() {
    if (MR::isFirstStep(this)) {
        mStateWaitStart->setTalkActionName("StartEvent");
        mStateWaitStart->_1C = true;
    }

    if (MR::updateActorState(this, mStateWaitStart)) {
        MR::requestStartDemoMarioPuppetable(this, "逃走", &NrvTrickRabbitFreeRun::TrickRabbitFreeRunNrvRunawayStart::sInstance,
                                            &NrvTrickRabbitFreeRun::TrickRabbitFreeRunNrvTryDemo::sInstance);
    }
}

void TrickRabbitFreeRun::exeTryDemo() {
}

void TrickRabbitFreeRun::exeRunawayStart() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("BattleWait", static_cast< const char* >(nullptr));
        MR::startAction(this, "Run");
        MR::startEventBGM(MR::EventBgmID_Chase);
    }

    MR::escapeFromPlayer(this, &mFrontVec, ::sStartAwayParam._0, ::sStartAwayParam._4, ::sStartAwayParam._8, ::sStartAwayParam._C);

    if (MR::checkPassBckFrame(this, 3.0f)) {
        startJumpSound();
    }

    if (MR::isGreaterStep(this, 120)) {
        MR::endDemo(this, "逃走");
        setNerve(&NrvTrickRabbitFreeRun::TrickRabbitFreeRunNrvRunaway::sInstance);
    }
}

void TrickRabbitFreeRun::exeRunaway() {
    MR::updateActorState(this, mStateRunaway);

    if (mStateRunaway->isRunning() && MR::checkPassBckFrame(this, 3.0f)) {
        startJumpSound();
    }

    f32 speed = ::sNormalAccel;

    if (MR::isNearPlayerHorizontal(this, ::sPressureRange)) {
        speed = ::sPressureAccel;
    }

    if (MR::isBindedGroundWater(this) || MR::isBindedGroundBrake(this)) {
        speed *= ::sBrakeAccelRate;
    }

    mStateRunaway->mRunawaySpeed = speed;

    if (MR::isBindedGround(this)) {
        mFootPrint->addPrint(mPosition, mFrontVec, *MR::getGroundNormal(this), false);
    }
}

void TrickRabbitFreeRun::exeBlowDamage() {
    MR::updateActorStateAndNextNerve(this, mStateBlowDamage, &NrvTrickRabbitFreeRun::TrickRabbitFreeRunNrvRunaway::sInstance);
}

void TrickRabbitFreeRun::exeCaught() {
    if (MR::isFirstStep(this)) {
        MR::stopStageBGM(60);
    }

    if (MR::updateActorState(this, mStateCaught)) {
        setNerve(&NrvTrickRabbitFreeRun::TrickRabbitFreeRunNrvPowerStarDemo::sInstance);
    }
}

void TrickRabbitFreeRun::exePowerStarDemo() {
    if (MR::isFirstStep(this)) {
        MR::endDemo(this, "捕まり");
        MR::startAfterBossBGM();
        MR::requestAppearPowerStar(this, mPosition - mGravity * 200.0f);
        MR::startAction(this, "Wait");
        mFootPrint->clear();
        mSpotMarkLight->kill();
    }

    MR::zeroVelocity(this);

    if (MR::isEndPowerStarAppearDemo(this)) {
        MR::forwardNode(mTalkCtrl);
        setNerve(&NrvTrickRabbitFreeRun::TrickRabbitFreeRunNrvGiveUp::sInstance);
    }
}

void TrickRabbitFreeRun::exeGiveUp() {
    if (MR::isFirstStep(this)) {
        mStateWaitStart->setTalkActionName("GiveUpTalk");
        mStateWaitStart->_1C = false;
    }

    MR::updateActorState(this, mStateWaitStart);
}

bool TrickRabbitFreeRun::isEnableCaught() const {
    if (isNerve(&NrvTrickRabbitFreeRun::TrickRabbitFreeRunNrvRunaway::sInstance) ||
        isNerve(&NrvTrickRabbitFreeRun::TrickRabbitFreeRunNrvBlowDamage::sInstance)) {
        return true;
    }

    return false;
}

bool TrickRabbitFreeRun::isEnableBlowDamage() const {
    return isNerve(&NrvTrickRabbitFreeRun::TrickRabbitFreeRunNrvRunaway::sInstance);
}

void TrickRabbitFreeRun::startJumpSound() {
    if (MR::isBindedGroundWater(this)) {
        MR::startSound(this, "SE_SM_RABBIT_JUMP_WATER");
    } else if (MR::isBindedGroundBrake(this)) {
        MR::startSound(this, "SE_SM_RABBIT_JUMP_WATER");
    } else {
        MR::startSound(this, "SE_SM_RABBIT_JUMP");
    }
}
