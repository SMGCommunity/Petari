#include "Game/Enemy/Kameck.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/KameckBeam.hpp"
#include "Game/Enemy/KameckBeamHolder.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/ActiveActorList.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

namespace {
    static const f32 sDefaultActiveDistance = 3000.0f;
    static const f32 sFrontVecBlendRate = 0.2f;
    static const f32 sUpVecBlendRate = 0.04f;
    static const f32 sTurnPlayerLimit = 0.98f;
    // static const s32 sWaitTime =
    // static const s32 mMoveHideTime =
    // static const f32 sMoveSpeed =
    static const s32 sGuardTime = 50;
    // static const f32 sGuardTurnSpeed =
    static const f32 sAttackRange = 2000.0f;
    static const s32 sAttackWaitTime = 60;
    // static const s32 sShootTiming =
    static const f32 sBeamSpeed = 12.0f;
    static const f32 sHitHorizontalPower = 25.0f;
    static const f32 sHitVerticalPower = 25.0f;
    static const f32 sDownGravityAccel = 1.0f;
    static const f32 sDownFreq = 0.99f;
    // static const s32 sPressTime =
};  // namespace

namespace NrvKameck {
    NEW_NERVE(KameckNrvOpeningDemo, Kameck, OpeningDemo);
    NEW_NERVE_ONEND(KameckNrvDemoAppear, Kameck, DemoAppear, DemoAppear);
    NEW_NERVE(KameckNrvNonActive, Kameck, NonActive);
    NEW_NERVE(KameckNrvAppear, Kameck, Appear);
    NEW_NERVE(KameckNrvWait, Kameck, Wait);
    NEW_NERVE(KameckNrvHide, Kameck, Hide);
    NEW_NERVE(KameckNrvAttackWait, Kameck, AttackWait);
    NEW_NERVE(KameckNrvAttack, Kameck, Attack);
    NEW_NERVE(KameckNrvGuard, Kameck, Guard);
    NEW_NERVE(KameckNrvHit, Kameck, Hit);
    NEW_NERVE(KameckNrvMoveHide, Kameck, MoveHide);
    NEW_NERVE(KameckNrvMove, Kameck, Move);
    NEW_NERVE_ONEND(KameckNrvBindStarPointer, Kameck, BindStarPointer, BindStarPointer);
    NEW_NERVE(KameckNrvDown, Kameck, Down);
    NEW_NERVE(KameckNrvPressDown, Kameck, PressDown);
};  // namespace NrvKameck

Kameck::Kameck(const char* pName)
    : LiveActor(pName), mBeam(), mActiveActorList(), mBeamEventListener(), mAnimScaleController(), mStateBindStarPointer(),
      mRotateQuat(0.0f, 0.0f, 0.0f, 1.0f), mFrontVec(0.0f, 0.0f, 1.0f), mBeamType(), mMoveStep(240), mRailCoord(), mRailNextPointCoord(),
      mActiveDistance(::sDefaultActiveDistance) {
    mActiveActorList = new ActiveActorList(8);
    mBeamEventListener = new SmallKameckBeamEventListener(this);
}

void Kameck::init(const JMapInfoIter& rIter) {
    initJMapParam(rIter);
    initModelManagerWithAnm("Kameck", nullptr, false);
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    MR::onCalcGravity(this);
    MR::initShadowVolumeSphere(this, 70.0f);
    MR::makeQuatAndFrontFromRotate(&mRotateQuat, &mFrontVec, this);
    initHitSensor(2);
    MR::addHitSensorEnemy(this, "body", 8, 120.0f, TVec3f(0.0f, 40.0f, 0.0f));
    MR::addHitSensorEnemyAttack(this, "attack", 8, 80.0f, TVec3f(0.0f, 0.0f, 0.0f));
    initBinder(70.0f, 0.0f, 0);
    MR::offBind(this);
    initBeam();
    initEffectKeeper(1, nullptr, false);
    MR::initStarPointerTarget(this, 80.0f, TVec3f(0.0f, 0.0f, 0.0f));

    mAnimScaleController = new AnimScaleController(nullptr);
    mAnimScaleController->setParamTight();

    mStateBindStarPointer = new WalkerStateBindStarPointer(this, mAnimScaleController);

    initSound(4, false);
    MR::addToAttributeGroupSearchTurtle(this);

    if (MR::isConnectedWithRail(rIter)) {
        initRailRider(rIter);
        MR::moveCoordToStartPos(this);
        MR::moveTransToCurrentRailPos(this);
    }

    initNerve(GET_NERVE(Kameck, KameckNrvWait));
    makeActorAppeared();
    MR::declareCoin(this, 1);
    initDemo(rIter);
}

void Kameck::initBeam() {
    MR::createKameckBeamHolder();

    switch (mBeamType) {
    case KameckBeam::BeamType_None:
        break;
    case KameckBeam::BeamType_Turtle:
        MR::createKameckBeamTurtleHolder();
        break;
    case KameckBeam::BeamType_FireBall1:
    case KameckBeam::BeamType_FireBall2:
    case KameckBeam::BeamType_FireBall3:
        MR::createKameckFireBallHolder();
        break;
    }
}

void Kameck::initJMapParam(const JMapInfoIter& rIter) {
    if (!MR::isValidInfo(rIter)) {
        return;
    }

    MR::initDefaultPos(this, rIter);
    MR::getJMapInfoArg3NoInit(rIter, &mActiveDistance);

    if (mBeamType != KameckBeam::BeamType_FireBall1) {
        return;
    }

    s32 arg0;
    MR::getJMapInfoArg0WithInit(rIter, &arg0);

    if (arg0 >= 3) {
        mBeamType = KameckBeam::BeamType_FireBall3;
    } else if (arg0 >= 2) {
        mBeamType = KameckBeam::BeamType_FireBall2;
    } else {
        mBeamType = KameckBeam::BeamType_FireBall1;
    }
}

void Kameck::initDemo(const JMapInfoIter& rIter) {
    if (!MR::tryRegisterDemoCast(this, rIter)) {
        return;
    }

    MR::registerDemoActionFunctor(this, MR::Functor(this, &Kameck::startDemoAppear), "ザコカメック登場");
    MR::registerDemoActionFunctor(this, MR::Functor(this, &Kameck::killForce), "ザコカメ");
    makeActorDead();
}

void Kameck::makeActorDead() {
    LiveActor::makeActorDead();
    resetBeam();
}

void Kameck::appear() {
    MR::moveCoordAndTransToRailStartPoint(this);
    LiveActor::appear();
    setNerve(GET_NERVE(Kameck, KameckNrvAppear));
}

void Kameck::kill() {
    LiveActor::kill();
    MR::appearCoinPop(this, mPosition, 1);
    MR::emitEffect(this, "Death");
}

void Kameck::control() {
    mAnimScaleController->updateNerve();
    MR::blendQuatUpFront(&mRotateQuat, -mGravity, mFrontVec, ::sUpVecBlendRate, ::sFrontVecBlendRate);
    mActiveActorList->removeDeadActor();
}

void Kameck::startClipped() {
    LiveActor::startClipped();
    tryNonActive();
}

void Kameck::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, mRotateQuat);
    MR::setBaseScale(this, mAnimScaleController->_C * mScale);
}

void Kameck::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver) && !MR::isPlayerHipDropFalling() && MR::isSensorEnemyAttack(pSender)) {
        requestAttack(pSender, pReceiver);
    }
}

bool Kameck::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorEnemy(pReceiver)) {
        return false;
    }

    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    }

    if (MR::isMsgStarPieceReflect(msg)) {
        return requestGuard(pSender, pReceiver);
    }

    if (MR::isMsgJetTurtleAttack(msg)) {
        return requestDown(pSender, pReceiver);
    }

    if (MR::isMsgPlayerHitAll(msg)) {
        return requestDown(pSender, pReceiver);
    }

    if (MR::isMsgPlayerTrample(msg)) {
        return requestPressDown();
    }

    if (MR::isMsgPlayerHipDrop(msg)) {
        return requestPressDown();
    }

    return false;
}

bool Kameck::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgExplosionAttack(msg)) {
        return requestDown(pSender, pReceiver);
    }

    return false;
}

bool Kameck::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStartPowerStarGet(msg)) {
        makeActorDead();
        return true;
    }

    return false;
}

bool Kameck::requestAttack(HitSensor* pSender, HitSensor* pReceiver) {
    if (isEnableAttack()) {
        if (MR::sendMsgEnemyAttack(pReceiver, pSender)) {
            if (isEnableAttack()) {
                resetBeam();
                setNerve(GET_NERVE(Kameck, KameckNrvHit));
                return true;
            }
        } else {
            MR::sendMsgPush(pReceiver, pSender);
        }
    } else {
        if (isNerve(GET_NERVE(Kameck, KameckNrvBindStarPointer))) {
            MR::sendMsgPush(pReceiver, pSender);
        }
    }

    return false;
}

bool Kameck::requestDown(HitSensor* pSender, HitSensor* pReceiver) {
    if (isEnableDown()) {
        resetBeam();
        MR::onBind(this);
        MR::invalidateHitSensors(this);
        MR::invalidateClipping(this);
        MR::setVelocityBlowAttack(this, pSender, pReceiver, ::sHitHorizontalPower, ::sHitVerticalPower, 4);
        setNerve(GET_NERVE(Kameck, KameckNrvDown));

        return true;
    }

    return false;
}

bool Kameck::requestGuard(HitSensor* pSender, HitSensor* pReceiver) {
    if (isEnableGurad()) {
        TVec3f vec;
        vec.killElement(-pSender->mHost->mVelocity, mGravity);

        if (!MR::normalizeOrZero(&vec)) {
            mFrontVec.set(vec);
        }

        resetBeam();
        MR::invalidateClipping(this);
        setNerve(GET_NERVE(Kameck, KameckNrvGuard));

        return true;
    }

    return false;
}

bool Kameck::requestPressDown() {
    if (isEnableDown()) {
        resetBeam();
        MR::onBind(this);
        MR::invalidateHitSensors(this);
        MR::invalidateClipping(this);
        setNerve(GET_NERVE(Kameck, KameckNrvPressDown));
        return true;
    }

    return false;
}

void Kameck::startDemoAppear() {
    makeActorAppeared();
    MR::invalidateHitSensors(this);
    MR::invalidateClipping(this);
    setNerve(GET_NERVE(Kameck, KameckNrvDemoAppear));
}

void Kameck::killForce() {
    MR::emitEffect(this, "Death");
    resetBeam();
    mActiveActorList->killAll();
    makeActorDead();
}

void Kameck::makeActorDeadForce() {
    resetBeam();
    mActiveActorList->killAll();
    makeActorDead();
}

void Kameck::hitBeam(s32 beamType) {
    if (MR::isDead(this)) {
        return;
    }

    switch (beamType) {
    case KameckBeam::BeamType_Turtle:
        MR::startSound(this, "SE_EV_KAMECK_ATK_SUCCESS");
        break;
    case KameckBeam::BeamType_FireBall1:
    case KameckBeam::BeamType_FireBall2:
    case KameckBeam::BeamType_FireBall3:
        MR::startSound(this, "SE_EV_KAMECK_ATK_SUCCESS");
        break;
    }
}

bool Kameck::tryOpeningDemo() {
    if (MR::isStageStateScenarioOpeningCamera()) {
        setNerve(GET_NERVE(Kameck, KameckNrvOpeningDemo));
        return true;
    }

    return false;
}

bool Kameck::tryOpeningDemoEnd() {
    if (!MR::isStageStateScenarioOpeningCamera()) {
        setNonActive();

        return true;
    }

    return false;
}

bool Kameck::tryNonActive() {
    if (isNerve(GET_NERVE(Kameck, KameckNrvWait)) && canNonActive()) {
        setNonActive();

        return true;
    }

    return false;
}

void Kameck::setNonActive() {
    MR::hideModel(this);
    MR::invalidateHitSensors(this);
    MR::offCalcGravity(this);
    MR::offCalcShadow(this, nullptr);
    MR::validateClipping(this);
    MR::deleteEffectAll(this);
    setNerve(GET_NERVE(Kameck, KameckNrvNonActive));
}

bool Kameck::tryActive() {
    if (!canNonActive()) {
        setNerve(GET_NERVE(Kameck, KameckNrvAppear));

        return true;
    }

    return false;
}

bool Kameck::tryAppearEnd() {
    if (MR::isBckStopped(this)) {
        if (!tryAttackWait()) {
            setNerve(GET_NERVE(Kameck, KameckNrvWait));
        }

        return true;
    }

    return false;
}

bool Kameck::tryAttackWait() {
    if (MR::isNearPlayer(this, ::sAttackRange) && !mActiveActorList->isFull()) {
        mBeam = MR::startFollowKameckBeam(mBeamType, MR::getJointMtx(this, "Wand"), 0.6f, TVec3f(0.0f, 110.0f, 0.0f), mBeamEventListener);

        if (mBeam != nullptr) {
            setNerve(GET_NERVE(Kameck, KameckNrvAttackWait));

            return true;
        }
    }

    return false;
}

bool Kameck::tryAttack() {
    if (MR::isGreaterStep(this, ::sAttackWaitTime)) {
        setNerve(GET_NERVE(Kameck, KameckNrvAttack));
        return true;
    }

    return false;
}

bool Kameck::tryAttackEnd() {
    if (MR::isGreaterStep(this, 30)) {
        setNerve(GET_NERVE(Kameck, KameckNrvMoveHide));
        return true;
    }

    return false;
}

bool Kameck::tryHitEnd() {
    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(Kameck, KameckNrvMoveHide));
        return true;
    }

    return false;
}

bool Kameck::tryMove() {
    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(Kameck, KameckNrvMove));
        return true;
    }

    return false;
}

bool Kameck::tryMoveEnd() {
    if (MR::isGreaterStep(this, mMoveStep)) {
        setNerve(GET_NERVE(Kameck, KameckNrvAppear));
        return true;
    }

    return false;
}

bool Kameck::tryPointBind() {
    if (mStateBindStarPointer->tryStartPointBind()) {
        resetBeam();
        setNerve(GET_NERVE(Kameck, KameckNrvBindStarPointer));

        return true;
    }

    return false;
}

void Kameck::exeOpeningDemo() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait");
        MR::showModel(this);
        MR::validateHitSensors(this);
        MR::validateClipping(this);
    }

    if (tryOpeningDemoEnd()) {
        return;
    }
}

void Kameck::exeDemoAppear() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Appear");
    }

    MR::startSound(this, "SE_EM_KAMECK_SMOKE");
    MR::startSound(this, "SE_EM_KAMECK_APPEAR");
    MR::turnDirectionToTarget(this, &mFrontVec, *MR::getPlayerPos(), ::sTurnPlayerLimit);

    if (MR::isBckOneTimeAndStopped(this)) {
        MR::startBck(this, "Wait");
    }
}

void Kameck::endDemoAppear() {
    if (isNerve(GET_NERVE(Kameck, KameckNrvDemoAppear))) {
        setNerve(GET_NERVE(Kameck, KameckNrvWait));
    }
}

void Kameck::exeNonActive() {
    if (tryOpeningDemo() || tryActive()) {
        MR::onCalcShadow(this, nullptr);
        MR::onCalcGravity(this);
    }
}

void Kameck::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Appear");
        MR::startSound(this, "SE_EM_KAMECK_SMOKE");
        MR::startSound(this, "SE_EM_KAMECK_APPEAR");
        MR::showModel(this);
        MR::validateHitSensors(this);
        MR::invalidateClipping(this);
    }

    if (MR::isNearPlayer(this, ::sAttackRange)) {
        MR::turnDirectionToTarget(this, &mFrontVec, *MR::getPlayerPos(), ::sTurnPlayerLimit);
    }

    if (!tryPointBind() && tryAppearEnd()) {
        return;
    }
}

void Kameck::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait");
        MR::validateClipping(this);
        MR::validateHitSensors(this);
    }

    if (!tryPointBind() && !tryOpeningDemo() && tryAttackWait()) {
        return;
    }
}

void Kameck::exeHide() {
}

void Kameck::exeAttackWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "AttackWait");
        MR::invalidateClipping(this);
    }

    MR::startLevelSound(this, "SE_EM_LV_KAMECK_STAFF_TURN");
    MR::turnDirectionToTarget(this, &mFrontVec, *MR::getPlayerPos(), ::sTurnPlayerLimit);

    if (!tryPointBind() && tryAttack()) {
        return;
    }
}

void Kameck::exeGuard() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Guard");
    }

    if (!tryPointBind() && MR::isGreaterStep(this, ::sGuardTime)) {
        setNerve(GET_NERVE(Kameck, KameckNrvMoveHide));
    }
}

void Kameck::exeAttack() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Attack");
        MR::startSound(this, "SE_EM_KAMECK_STAFF_SWING");
        MR::startSound(this, "SE_EV_KAMECK_STAFF_SWING");
    }

    if (MR::isStep(this, 9)) {
        mBeam->requestShootToPlayerCenter(::sBeamSpeed);
        mActiveActorList->addActor(mBeam);
        mBeam = nullptr;
    }

    if (!tryPointBind() && tryAttackEnd()) {
        return;
    }
}

void Kameck::exeHit() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Hit");
    }

    if (!tryPointBind() && tryHitEnd()) {
        return;
    }
}

void Kameck::exeMoveHide() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Hide");
        MR::startSound(this, "SE_EM_KAMECK_HIDE");
    }

    MR::turnDirectionToTarget(this, &mFrontVec, *MR::getPlayerPos(), ::sTurnPlayerLimit);

    if (!tryPointBind() && !tryMove()) {
        MR::startSound(this, "SE_EM_KAMECK_SMOKE");
    }
}

void Kameck::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Move");
        MR::hideModelAndOnCalcAnim(this);
        MR::invalidateHitSensors(this);
        MR::invalidateClipping(this);

        if (MR::isExistRail(this)) {
            if (MR::isRailReachedGoal(this)) {
                MR::reverseRailDirection(this);
            }

            mRailCoord = MR::getRailCoord(this);
            mRailNextPointCoord = MR::getRailPointCoord(this, MR::getNextRailPointNo(this));
            mMoveStep = MR::abs(mRailNextPointCoord - mRailCoord) / 20.0f;
        }
    }

    MR::startLevelSound(this, "SE_EM_LV_KAMECK_WARP");

    if (MR::isExistRail(this)) {
        MR::setRailCoord(this, MR::calcNerveEaseInOutValue(this, mMoveStep, mRailCoord, mRailNextPointCoord));
        MR::moveTransToCurrentRailPos(this);
    }

    MR::turnDirectionToTarget(this, &mFrontVec, *MR::getPlayerPos(), ::sTurnPlayerLimit);

    if (tryMoveEnd()) {
        return;
    }
}

void Kameck::exeBindStarPointer() {
    MR::updateActorStateAndNextNerve(this, mStateBindStarPointer, GET_NERVE(Kameck, KameckNrvWait));
}

void Kameck::endBindStarPointer() {
    mStateBindStarPointer->kill();
}

void Kameck::exeDown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Down");
        MR::startBlowHitSound(this);
        MR::startSound(this, "SE_EV_KAMECK_DEAD_BLOW");
    }

    MR::addVelocityToGravity(this, ::sDownGravityAccel);
    MR::attenuateVelocity(this, ::sDownFreq);

    if (MR::isBckStopped(this) || MR::isBinded(this)) {
        MR::startSound(this, "SE_EM_EXPLODE_S");
        kill();
    }
}

void Kameck::exePressDown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Press");
        MR::startSound(this, "SE_EM_STOMPED_S");
        MR::startSound(this, "SE_EV_KAMECK_DEAD_STOMPED");
        MR::zeroVelocity(this);
    }

    if (MR::isBckStopped(this)) {
        MR::startSound(this, "SE_EM_EXPLODE_S");
        kill();
    }
}

void Kameck::resetBeam() {
    if (mBeam != nullptr) {
        mBeam->kill();
        mBeam = nullptr;
    }
}

void Kameck::setBeamType(s32 type) {
    mBeamType = type;
}

bool Kameck::canNonActive() const {
    return !MR::isNearPlayerAnyTime(this, mActiveDistance);
}

bool Kameck::isEnableAttack() const {
    if (!(!isNerve(GET_NERVE(Kameck, KameckNrvWait)) && !isNerve(GET_NERVE(Kameck, KameckNrvAppear)) &&
          !isNerve(GET_NERVE(Kameck, KameckNrvAttackWait)) && !isNerve(GET_NERVE(Kameck, KameckNrvAttack)) &&
          !isNerve(GET_NERVE(Kameck, KameckNrvMoveHide)))) {
        return true;
    }

    return false;
}

bool Kameck::isEnableGurad() const {
    if (!(!isNerve(GET_NERVE(Kameck, KameckNrvWait)) && !isNerve(GET_NERVE(Kameck, KameckNrvAppear)) &&
          !isNerve(GET_NERVE(Kameck, KameckNrvAttackWait)) && !isNerve(GET_NERVE(Kameck, KameckNrvAttack)) &&
          !isNerve(GET_NERVE(Kameck, KameckNrvGuard)) && !isNerve(GET_NERVE(Kameck, KameckNrvMoveHide)))) {
        return true;
    }

    return false;
}

bool Kameck::isEnableDown() const {
    if (MR::isDead(this)) {
        return false;
    }

    if (!(!isNerve(GET_NERVE(Kameck, KameckNrvNonActive)) && !isNerve(GET_NERVE(Kameck, KameckNrvDown)) &&
          !isNerve(GET_NERVE(Kameck, KameckNrvPressDown)))) {
        return false;
    }

    return true;
}

namespace MR {
    NameObj* createFireBallBeamKameck(const char* pName) {
        Kameck* pKameck;

        pKameck = new Kameck(pName);
        pKameck->setBeamType(KameckBeam::BeamType_FireBall1);

        return pKameck;
    }

    NameObj* createTurtleBeamKameck(const char* pName) {
        Kameck* pKameck;

        pKameck = new Kameck(pName);
        pKameck->setBeamType(KameckBeam::BeamType_Turtle);

        return pKameck;
    }
};  // namespace MR
