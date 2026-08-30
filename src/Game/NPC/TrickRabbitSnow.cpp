#include "Game/NPC/TrickRabbitSnow.hpp"
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
    (void)2.0f;
}

TrickRabbitSnowListener::TrickRabbitSnowListener() {
}

namespace {
    static MR::ActorMoveParam sStartAwayParam = {1.4f, 1.0f, 0.9f, 4.0f};
    static const f32 sBodyRadius = 70.0f;
    static const f32 sCatchRadius = 30.0f;
    static const f32 sBindRadius = 60.0f;
    static const f32 sBindYOffset = 60.0f;
    static const f32 sUpVecBlendRate = 0.1f;
    static const f32 sFrontVecBlendRate = 0.2f;
    static const f32 sStarPieceHitAccelH = 10.0f;
    static const f32 sStarPieceHitAccelV = 40.0f;
    static const f32 sAppearSnowJumpV = 20.0f;
    static const f32 sAppearSnowJumpH = 20.0f;
    static const f32 sPressureRange = 600.0f;
    static const f32 sNormalAccel = 0.7f;
    static const f32 sPressureAccel = 1.2f;
    static const f32 sBrakeAccelRate = 0.99f;
    // static const f32 sAppearSnowRange = _;
};  // namespace

namespace NrvTrickRabbitFreeRun {
    NEW_NERVE(TrickRabbitSnowNrvWaitSnow, TrickRabbitSnow, WaitSnow);
    NEW_NERVE(TrickRabbitSnowNrvStartHideSnow, TrickRabbitSnow, StartHideSnow);
    NEW_NERVE(TrickRabbitSnowNrvVanish, TrickRabbitSnow, Vanish);
    NEW_NERVE(TrickRabbitSnowNrvHideSnow, TrickRabbitSnow, HideSnow);
    NEW_NERVE(TrickRabbitSnowNrvAppearFromSnow, TrickRabbitSnow, AppearFromSnow);
    NEW_NERVE(TrickRabbitSnowNrvRunaway, TrickRabbitSnow, Runaway);
    NEW_NERVE(TrickRabbitSnowNrvBlowDamage, TrickRabbitSnow, BlowDamage);
    NEW_NERVE(TrickRabbitSnowNrvCaught, TrickRabbitSnow, Caught);
    NEW_NERVE(TrickRabbitSnowNrvGiveUp, TrickRabbitSnow, GiveUp);
};  // namespace NrvTrickRabbitFreeRun

TrickRabbitSnow::TrickRabbitSnow(const char* pName)
    : LiveActor(pName), mFinishQuat(0.0f, 0.0f, 0.0f, 1.0f), mFinishPos(0.0f, 0.0f, 0.0f), mRotateQuat(0.0f, 0.0f, 0.0f, 1.0f),
      mFrontVec(0.0f, 0.0f, 1.0f), mStateWaitStart(), mStateRunaway(), mStateBlowDamage(), mStateCaught(), mTalkCtrl(), mFootPrint(),
      mSpotMarkLight(), mListener(), mNotCaughtableTimer(), mIsValidAppearStarPiece(true) {
}

void TrickRabbitSnow::setListener(TrickRabbitSnowListener* pListener) {
    mListener = pListener;
}

void TrickRabbitSnow::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("TrickRabbit", nullptr, false);
    MR::connectToSceneNpc(this);
    MR::initLightCtrl(this);
    MR::makeQuatAndFrontFromRotate(&mRotateQuat, &mFrontVec, this);
    mFinishQuat.set(mRotateQuat);
    mFinishPos.set(mPosition);

    mSpotMarkLight = new SpotMarkLight(this, 100.0f, 1500.0f, nullptr);
    mSpotMarkLight->initWithoutIter();

    MR::initShadowFromCSV(this, "Shadow");

    mFootPrint = TrickRabbitUtil::createRabbitFootPrint(this);

    initEffectKeeper(0, nullptr, false);
    MR::onEmitEffectSyncClipping(this, "Light");
    MR::onForceDeleteEffectSyncClipping(this, "Light");
    initSensor();
    initBinder(::sBindRadius, ::sBindYOffset, 0);

    mTalkCtrl = MR::createTalkCtrl(this, rIter, "SnowRabbit", TVec3f(0.0f, 120.0f, 0.0f), nullptr);

    initState();
    initSound(4, false);
    MR::onCalcGravity(this);
    MR::declareStarPiece(this, 6);
    initNerve(&NrvTrickRabbitFreeRun::TrickRabbitSnowNrvWaitSnow::sInstance);
    MR::tryRegisterNamePosLinkObj(this, rIter);
    MR::useStageSwitchReadA(this, rIter);

    if (MR::useStageSwitchReadB(this, rIter)) {
        MR::listenStageSwitchOnB(this, MR::Functor_Inline(this, &TrickRabbitSnow::clearFootPrint));
    }

    makeActorAppeared();
}

void TrickRabbitSnow::initAfterPlacement() {
    MR::trySetMoveLimitCollision(this);
}

void TrickRabbitSnow::initSensor() {
    initHitSensor(2);
    MR::addHitSensorAtJointEnemy(this, "Body", "Spine", 8, ::sBodyRadius, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJointEnemy(this, "Catch", "Spine", 8, ::sCatchRadius, TVec3f(0.0f, 0.0f, 0.0f));
    MR::initStarPointerTargetAtJoint(this, "Spine", ::sBodyRadius, TVec3f(0.0f));
}

void TrickRabbitSnow::initState() {
    mStateCaught = new RabbitStateCaught(this, mTalkCtrl);
    mStateCaught->unusePowerStarModel();
    mStateCaught->init();

    mStateWaitStart = new RabbitStateWaitStart(this, &mFrontVec, mTalkCtrl);
    mStateWaitStart->setTalkActionName("GiveUpTalk");
    mStateWaitStart->_1C = false;
    mStateWaitStart->_1D = false;
    mStateWaitStart->init();

    mStateRunaway = new WalkerStateRunaway(this, &mFrontVec, nullptr);
    mStateRunaway->init();

    mStateBlowDamage = new WalkerStateBlowDamage(this, &mFrontVec, nullptr);
    mStateBlowDamage->init();
}

void TrickRabbitSnow::clearFootPrint() {
    mFootPrint->clearForce();
}

void TrickRabbitSnow::control() {
    if (mNotCaughtableTimer > 0) {
        mNotCaughtableTimer--;
    }

    MR::blendQuatUpFront(&mRotateQuat, -mGravity, mFrontVec, ::sUpVecBlendRate, ::sFrontVecBlendRate);
}

void TrickRabbitSnow::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, mRotateQuat);
}

void TrickRabbitSnow::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvTrickRabbitFreeRun::TrickRabbitSnowNrvHideSnow::sInstance)) {
        return;
    }

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

bool TrickRabbitSnow::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvTrickRabbitFreeRun::TrickRabbitSnowNrvHideSnow::sInstance)) {
        return false;
    }

    if (MR::isSensorPlayer(pSender)) {
        return !isEnableCaught();
    }

    return false;
}

bool TrickRabbitSnow::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgToEnemyAttackBlow(msg)) {
        return receiveMsgBlowDamage(pSender, pReceiver);
    }

    return false;
}

bool TrickRabbitSnow::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvTrickRabbitFreeRun::TrickRabbitSnowNrvGiveUp::sInstance)) {
        return mStateWaitStart->receiveMsgPlayerAttack(msg, pSender, pReceiver);
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        MR::limitedStarPieceHitSound();
        MR::startSound(this, "SE_SM_RABBIT_STAR_PIECE_HIT");
        MR::startSound(this, "SE_SV_RABBIT_STAR_PIECE_HIT");

        if (receiveMsgBlowDamage(pSender, pReceiver)) {
            return true;
        }
    }

    if (MR::isMsgPlayerSpinAttack(msg)) {
        return requestCaught();
    }

    if (MR::isMsgPlayerTrample(msg)) {
    }

    return false;
}

bool TrickRabbitSnow::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvTrickRabbitFreeRun::TrickRabbitSnowNrvGiveUp::sInstance)) {
        return mStateWaitStart->receiveOtherMsg(msg, pSender, pReceiver);
    }

    if (!MR::isValidSwitchA(this)) {
        if (msg == ACTMES_ASK_HIDDEN_BY_SNOW) {
            return isNerve(&NrvTrickRabbitFreeRun::TrickRabbitSnowNrvHideSnow::sInstance);
        }

        if (msg == ACTMES_NOTIFY_DISCOVER_SNOW) {
            if (isNerve(&NrvTrickRabbitFreeRun::TrickRabbitSnowNrvHideSnow::sInstance)) {
                MR::showModel(this);
                setNerve(&NrvTrickRabbitFreeRun::TrickRabbitSnowNrvAppearFromSnow::sInstance);

                return true;
            }
        }
    }

    return false;
}

bool TrickRabbitSnow::receiveMsgBlowDamage(HitSensor* pSender, HitSensor* pReceiver) {
    if (isEnableBlowDamage()) {
        MR::setVelocitySeparateHV(this, pSender, pReceiver, ::sAppearSnowJumpH, sAppearSnowJumpV);
        setNerve(&NrvTrickRabbitFreeRun::TrickRabbitSnowNrvBlowDamage::sInstance);

        return true;
    }

    return false;
}

bool TrickRabbitSnow::requestCaught() {
    if (isEnableCaught()) {
        if (mListener != nullptr) {
            mListener->noticeCaught(this);
        }

        MR::tryPlayerDropTakingActor();
        MR::invalidateClipping(this);
        setNerve(&NrvTrickRabbitFreeRun::TrickRabbitSnowNrvCaught::sInstance);

        return true;
    }

    return false;
}

bool TrickRabbitSnow::requestStartHideSnow() {
    if (isNerve(&NrvTrickRabbitFreeRun::TrickRabbitSnowNrvWaitSnow::sInstance)) {
        MR::invalidateClipping(this);
        MR::requestMovementOn(this);
        setNerve(&NrvTrickRabbitFreeRun::TrickRabbitSnowNrvStartHideSnow::sInstance);

        return true;
    }

    return false;
}

void TrickRabbitSnow::setFinishPosition() {
    mRotateQuat.set(mFinishQuat);
    mRotateQuat.getZDir(mFrontVec);
    mPosition.set(mFinishPos);
    MR::showModel(this);
    MR::resetPosition(this);
    MR::zeroVelocity(this);
    MR::invalidateClipping(this);
    MR::requestMovementOn(this);
    MR::forwardNode(mTalkCtrl);
    mStateWaitStart->_1D = false;
    setNerve(&NrvTrickRabbitFreeRun::TrickRabbitSnowNrvGiveUp::sInstance);
}

void TrickRabbitSnow::setNotCaughtable() {
    mNotCaughtableTimer = 5;
}

void TrickRabbitSnow::exeWaitSnow() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "StartEvent");
    }

    MR::turnDirectionToPlayerDegree(this, &mFrontVec, 15.0f);
    MR::zeroVelocity(this);
}

void TrickRabbitSnow::exeStartHideSnow() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Run");
    }

    MR::escapeFromPlayer(this, &mFrontVec, ::sStartAwayParam._0, ::sStartAwayParam._4, ::sStartAwayParam._8, ::sStartAwayParam._C);

    if (MR::checkPassBckFrame(this, 3.0f)) {
        startJumpSound();
    }

    if (MR::isGreaterStep(this, 120)) {
        setNerve(&NrvTrickRabbitFreeRun::TrickRabbitSnowNrvVanish::sInstance);
    }
}

void TrickRabbitSnow::exeVanish() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Change");
        MR::startSound(this, "SE_SM_RABBIT_HIDE");
        MR::zeroVelocity(this);
    }

    if (!MR::isBckStopped(this)) {
        return;
    }

    MR::startSound(this, "SE_SM_METAMORPHOSE_SMOKE");

    TVec3f trans;
    TVec3f rotate;

    if (MR::tryFindLinkNamePos(this, "隠れ位置", &trans, &rotate)) {
        mPosition.set(trans);
        mRotation.set(rotate);
        MR::makeQuatAndFrontFromRotate(&mRotateQuat, &mFrontVec, this);
        MR::resetPosition(this);
    }

    MR::stopBck(this);
    MR::zeroVelocity(this);
    MR::hideModelAndOnCalcAnim(this);
    setNerve(&NrvTrickRabbitFreeRun::TrickRabbitSnowNrvHideSnow::sInstance);
}

void TrickRabbitSnow::exeHideSnow() {
    if (MR::isFirstStep(this)) {
        MR::deleteEffectAll(this);
    }

    MR::zeroVelocity(this);
    MR::turnDirectionFromPlayerDegree(this, &mFrontVec, 15.0f);

    if (MR::isValidSwitchA(this) && MR::isOnSwitchA(this)) {
        MR::showModel(this);
        setNerve(&NrvTrickRabbitFreeRun::TrickRabbitSnowNrvAppearFromSnow::sInstance);
    }
}

void TrickRabbitSnow::exeAppearFromSnow() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "AppearSmoke");
        MR::startAction(this, "Jump");
        MR::setVelocityJump(this, 25.0f);
        MR::addVelocityMoveToDirection(this, mFrontVec, 20.0f);
        MR::startSound(this, "SE_SM_RABBIT_APPEAR");
        MR::startSystemSE("SE_SM_RUNAWAY_RABBIT_APP_ME");
    }

    MR::addVelocityToGravity(this, 1.0f);
    MR::attenuateVelocity(this, ::sBrakeAccelRate);

    if (MR::isGreaterStep(this, 5)) {
        if (MR::isBindedGround(this)) {
            setNerve(&NrvTrickRabbitFreeRun::TrickRabbitSnowNrvRunaway::sInstance);
        }
    }
}

void TrickRabbitSnow::exeRunaway() {
    MR::updateActorState(this, mStateRunaway);

    if (mStateRunaway->isRunning() && MR::checkPassBckFrame(this, 3.0f)) {
        startJumpSound();
    }

    f32 speed = ::sNormalAccel;

    if (MR::isNearPlayerHorizontal(this, ::sPressureRange)) {
        speed = ::sPressureAccel;
    }

    mStateRunaway->mRunawaySpeed = speed;

    if (MR::isBindedGround(this)) {
        mFootPrint->addPrint(mPosition, mFrontVec, *MR::getGroundNormal(this), false);
    }
}

void TrickRabbitSnow::exeBlowDamage() {
    MR::updateActorStateAndNextNerve(this, mStateBlowDamage, &NrvTrickRabbitFreeRun::TrickRabbitSnowNrvRunaway::sInstance);
}

void TrickRabbitSnow::exeCaught() {
    if (MR::isFirstStep(this) && MR::isDemoActive()) {
        setNerve(&NrvTrickRabbitFreeRun::TrickRabbitSnowNrvCaught::sInstance);
    } else if (MR::updateActorState(this, mStateCaught)) {
        MR::endDemo(this, "捕まり");
        MR::forwardNode(mTalkCtrl);
        mSpotMarkLight->kill();
        mFootPrint->clear();
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST");
        mStateWaitStart->_1D = true;
        setNerve(&NrvTrickRabbitFreeRun::TrickRabbitSnowNrvGiveUp::sInstance);

        if (mListener != nullptr) {
            mListener->noticeGiveUp(this);
        }

        if (mIsValidAppearStarPiece) {
            MR::appearStarPiece(this, mPosition, 6, ::sStarPieceHitAccelH, ::sStarPieceHitAccelV, false);
        }
    }
}

void TrickRabbitSnow::exeGiveUp() {
    MR::updateActorState(this, mStateWaitStart);
}

bool TrickRabbitSnow::isGiveUp() const {
    return isNerve(&NrvTrickRabbitFreeRun::TrickRabbitSnowNrvGiveUp::sInstance);
}

bool TrickRabbitSnow::isHideSnow() const {
    return isNerve(&NrvTrickRabbitFreeRun::TrickRabbitSnowNrvHideSnow::sInstance);
}

bool TrickRabbitSnow::isEnableCaught() const {
    if (mNotCaughtableTimer > 0) {
        return false;
    }

    if (isNerve(&NrvTrickRabbitFreeRun::TrickRabbitSnowNrvRunaway::sInstance) ||
        isNerve(&NrvTrickRabbitFreeRun::TrickRabbitSnowNrvBlowDamage::sInstance)) {
        return true;
    }

    return false;
}

bool TrickRabbitSnow::isEnableBlowDamage() const {
    return isNerve(&NrvTrickRabbitFreeRun::TrickRabbitSnowNrvRunaway::sInstance);
}

void TrickRabbitSnow::startJumpSound() {
    if (MR::isBindedGroundWater(this)) {
        MR::startSound(this, "SE_SM_RABBIT_JUMP_WATER");
    } else if (MR::isBindedGroundBrake(this)) {
        MR::startSound(this, "SE_SM_RABBIT_JUMP_WATER");
    } else {
        MR::startSound(this, "SE_SM_RABBIT_JUMP");
    }
}
