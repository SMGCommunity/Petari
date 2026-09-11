#include "Game/Enemy/NokonokoLand.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/JetTurtle.hpp"
#include "Game/MapObj/MapPartsRailPointPassChecker.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorStateUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/Util/VectorUtil.hpp"

namespace {
    const Vec sSensorOffsetNormal = {0.0f, 0.0f, 30.0f};
    const f32 sSensorRadiusNormal = 120.0f;
    const f32 sSensorRadiusAttack = 60.0f;
    const f32 sSensorRadiusShell = 50.0f;
    const f32 sShadowRadius = 60.0f;
    const f32 sWalkSpeed = 1.6f;
    // sTurnTime
    const f32 sPoseLerpRate = 0.3f;
    const s32 sAppearTime = 60;
    const f32 sAppearValidDistance = 350.0f;
    const s32 sDeadTime = 180;
    const s32 sShellWaitTime = 180;
    const s32 sSceneStopTimePunch = 3;
    const f32 sWalkSpeedFast = 3.2f;
    const s32 sFlyAwayTime = 33;
    const f32 sGravityAccel = 1.5f;
    const f32 sAirFrec = 0.99f;
    const f32 sFlyAwayInitSpeedHoli = 15.0f;
    const f32 sFlyAwayInitSpeedVert = 35.0f;
    const s32 sShellStartPeakPosFrame = 27;
    const f32 sPushPower = 0.0f;

    void startBckBtp(LiveActor* pActor, const char* pBck, const char* pBtp = nullptr) {
        if (pBtp == nullptr) {
            pBtp = pBck;
        }

        MR::startBck(pActor, pBck, nullptr);
        if (MR::isExistBtp(pActor, pBtp)) {
            MR::startBtp(pActor, pBtp);
        }
    }
}  // namespace

namespace NrvNokonokoLand {
    NEW_NERVE(HostTypeAppearReady, NokonokoLand, AppearReady);
    NEW_NERVE_ONEND(HostTypeAppearStart, NokonokoLand, AppearStart, AppearStart);
    NEW_NERVE(HostTypeAppear, NokonokoLand, Appear);
    NEW_NERVE(HostTypeDead, NokonokoLand, Dead);
    NEW_NERVE(HostTypeFlyAway, NokonokoLand, FlyAway);
    NEW_NERVE(HostTypeWalk, NokonokoLand, Walk);
    NEW_NERVE(HostTypeLookAround, NokonokoLand, LookAround);
    NEW_NERVE(HostTypeTurnStart, NokonokoLand, TurnStart);
    NEW_NERVE(HostTypeTurn, NokonokoLand, TurnLoop);
    NEW_NERVE(HostTypeTurnEnd, NokonokoLand, TurnEnd);
    NEW_NERVE(HostTypeAttack, NokonokoLand, Attack);
    NEW_NERVE(HostTypeAttackTurn, NokonokoLand, Attack);
    NEW_NERVE(HostTypeRattle, NokonokoLand, Rattle);
    NEW_NERVE(HostTypeRattleTurn, NokonokoLand, Rattle);
    NEW_NERVE_ONEND(HostTypeBindStarPointer, NokonokoLand, BindStarPointer, BindStarPointer);
    NEW_NERVE_ONEND(HostTypeBindStarPointerTurn, NokonokoLand, BindStarPointer, BindStarPointer);
    NEW_NERVE(HostTypeShellStart, NokonokoLand, ShellStartTrample);
    NEW_NERVE(HostTypeShellWait, NokonokoLand, ShellWait);
    NEW_NERVE(HostTypeShellEnd, NokonokoLand, ShellEnd);
    NEW_NERVE(HostTypeShellStartPunch, NokonokoLand, ShellStartPunch);
    NEW_NERVE(HostTypeBecomeJetTurtle, NokonokoLand, BecomeJetTurtle);
}  // namespace NrvNokonokoLand

NokonokoLand::NokonokoLand(const char* pName)
    : LiveActor(pName), mJetTurtle(new JetTurtle("陸ノコ用ジェット亀")), mRailPointPassChecker(new MapPartsRailPointPassChecker(this)),
      mAnimScaleController(nullptr), mBindStarPointerState(nullptr), mTurtleType(0), mAppearPos(gZeroVec), mTurnAxis(TVec3f(0.0f, 1.0f, 0.0f)) {
    mTargetQuat.set(0.0f, 0.0f, 0.0f, 1.0f);
}

NokonokoLand::~NokonokoLand() {
}

void NokonokoLand::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    const char* pObjectName = nullptr;
    MR::getObjectName(&pObjectName, rIter);
    initModelManagerWithAnm(pObjectName, nullptr, false);
    MR::getJMapInfoArg0NoInit(rIter, &mTurtleType);
    MR::startBrk(this, "NokonokoLand");
    MR::setBrkFrameAndStop(this, mTurtleType);

    initEffectKeeper(1, nullptr, false);
    MR::addEffectHitNormal(this, nullptr);
    MR::setEffectHostSRT(this, "Appear", &mAppearPos, nullptr, nullptr);
    initSound(4, false);
    initHitSensor(3);
    MR::addHitSensorMtxEnemy(this, "body", 8, sSensorRadiusNormal, MR::getJointMtx(this, "Center"), TVec3f(sSensorOffsetNormal));
    MR::addHitSensorMtxEnemyAttack(this, "attack", 8, sSensorRadiusAttack, MR::getJointMtx(this, "Center"), TVec3f(sSensorOffsetNormal));
    MR::addHitSensorMtx(this, "shell", 0x22, 8, sSensorRadiusShell, MR::getJointMtx(this, "Turtle"), TVec3f(0.0f, 50.0f, 0.0f));
    MR::invalidateHitSensor(this, "shell");

    initRailRider(rIter);
    mRailPointPassChecker->init(rIter);
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    MR::onCalcGravity(this);
    initJetTurtle();
    MR::addToAttributeGroupSearchTurtle(this);
    MR::joinToGroupArray(this, rIter, nullptr, 32);
    MR::initShadowVolumeSphere(this, sShadowRadius);
    MR::onCalcShadow(this, nullptr);
    MR::initStarPointerTarget(this, 100.0f, TVec3f(0.0f));
    initNerve(GET_NERVE(NokonokoLand, HostTypeWalk));
    mAnimScaleController = new AnimScaleController(nullptr);
    mAnimScaleController->setParamTight();
    mBindStarPointerState = new WalkerStateBindStarPointer(this, mAnimScaleController);

    if (MR::tryRegisterDemoCast(this, rIter)) {
        MR::registerDemoActionFunctor(this, MR::FunctorV0M< NokonokoLand*, void (NokonokoLand::*)() >(this, &NokonokoLand::appear), nullptr);
        makeActorDead();
    } else if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    } else {
        makeActorAppeared();
    }
}

void NokonokoLand::initAfterPlacement() {
    MR::moveCoordAndTransToNearestRailPos(this);
    mRailPointPassChecker->start();
    mAppearPos.set(mPosition);
}

void NokonokoLand::appear() {
    LiveActor::appear();
}

void NokonokoLand::kill() {
    MR::emitEffect(this, "Death");
    LiveActor::kill();
}

void NokonokoLand::control() {
    mRailPointPassChecker->movement();
    mAnimScaleController->updateNerve();
    if (isStateWalk()) {
        tryBindStarPointer();
    }
}

void NokonokoLand::calcAndSetBaseMtx() {
    TPos3f mtx;
    mtx.set(getBaseMtx());
    TQuat4f quat;
    mtx.getQuat(quat);
    quat.slerp(mTargetQuat, sPoseLerpRate);
    MR::setBaseTRMtx(this, quat);
    TVec3f scale;
    scale.mul(mAnimScaleController->_C, mScale);
    MR::setBaseScale(this, scale);
}

bool NokonokoLand::isStateShell() const {
    return isNerve(GET_NERVE(NokonokoLand, HostTypeShellStart)) || isNerve(GET_NERVE(NokonokoLand, HostTypeShellWait)) ||
           isNerve(GET_NERVE(NokonokoLand, HostTypeShellEnd));
}

bool NokonokoLand::isStateRattle() const {
    return isNerve(GET_NERVE(NokonokoLand, HostTypeRattle)) || isNerve(GET_NERVE(NokonokoLand, HostTypeRattleTurn)) ||
           isNerve(GET_NERVE(NokonokoLand, HostTypeBindStarPointer)) || isNerve(GET_NERVE(NokonokoLand, HostTypeBindStarPointerTurn));
}

bool NokonokoLand::isStateBindStarPointer() const {
    return isNerve(GET_NERVE(NokonokoLand, HostTypeBindStarPointer)) || isNerve(GET_NERVE(NokonokoLand, HostTypeBindStarPointerTurn));
}

bool NokonokoLand::isStateAttack() const {
    return isNerve(GET_NERVE(NokonokoLand, HostTypeAttack)) || isNerve(GET_NERVE(NokonokoLand, HostTypeAttackTurn));
}

void NokonokoLand::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorEnemyAttack(pSender)) {
        if (tryAttack(pSender, pReceiver)) {
            return;
        }
        MR::sendMsgPush(pReceiver, pSender);
        return;
    }
    if (isStateShell() || isStateRattle() || isStateBindStarPointer() || isStateAttack() || MR::isPlayerCarryAny()) {
        MR::sendMsgPush(pReceiver, pSender);
        return;
    }
    if (MR::isSensorPlayerOrRide(pReceiver)) {
        return;
    }

    MR::sendMsgPush(pReceiver, pSender);
}

bool NokonokoLand::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerTrample(msg)) {
        return receiveTrample(pSender, pReceiver);
    }
    if (MR::isMsgPlayerHipDrop(msg)) {
        return receiveHipDrop(pSender, pReceiver);
    }
    if (MR::isMsgJetTurtleAttack(msg)) {
        return receiveTurtleAttack(pSender, pReceiver);
    }
    if (MR::isMsgStarPieceAttack(msg)) {
        return receiveStarPieceAttack(pSender, pReceiver);
    }
    if (MR::isMsgStarPieceReflect(msg) && isStateShell()) {
        return true;
    }
    if (MR::isMsgPlayerHitAll(msg)) {
        return receivePunch(pSender, pReceiver);
    }

    return false;
}

bool NokonokoLand::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgExplosionAttack(msg)) {
        if (isStateShell()) {
            if (pReceiver != getSensor("shell")) {
                return false;
            }

            TVec3f velocity(pSender->mHost->mVelocity);
            if (!MR::isNearZero(velocity)) {
                mVelocity.set(velocity);
            } else {
                mVelocity.set(pReceiver->mPosition - pSender->mPosition);
            }
            setNerve(GET_NERVE(NokonokoLand, HostTypeFlyAway));
            return true;
        }
        if ((isStateWalk() || isStateRattle()) && receiveTurtleAttack(pSender, pReceiver)) {
            return true;
        }
    }

    return false;
}

bool NokonokoLand::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isPlayerElementModeBee()) {
        return false;
    }
    if (MR::isMsgSpinStormRange(msg) && receiveStorm(pSender, pReceiver)) {
        return true;
    }
    if (MR::isMsgItemGet(msg)) {
        if (MR::isPlayerCarryAny()) {
            return false;
        }
        if (tryJetTurtleTouch()) {
            return true;
        }
    }

    return false;
}

void NokonokoLand::initJetTurtle() {
    mJetTurtle->initWithoutIter();
    if (mTurtleType == 1) {
        mJetTurtle->becomeFastType();
    } else {
        mJetTurtle->becomeSlowType();
    }
    mJetTurtle->makeActorDead();
}

bool NokonokoLand::isEnableAttack() const {
    return isNerve(GET_NERVE(NokonokoLand, HostTypeWalk)) || isNerve(GET_NERVE(NokonokoLand, HostTypeLookAround)) ||
           isNerve(GET_NERVE(NokonokoLand, HostTypeTurnStart)) || isNerve(GET_NERVE(NokonokoLand, HostTypeTurn)) ||
           isNerve(GET_NERVE(NokonokoLand, HostTypeTurnEnd)) || isNerve(GET_NERVE(NokonokoLand, HostTypeAppear)) ||
           isNerve(GET_NERVE(NokonokoLand, HostTypeRattle)) || isNerve(GET_NERVE(NokonokoLand, HostTypeRattleTurn));
}

bool NokonokoLand::isStateWalk() const {
    return isNerve(GET_NERVE(NokonokoLand, HostTypeWalk)) || isNerve(GET_NERVE(NokonokoLand, HostTypeLookAround)) ||
           isNerve(GET_NERVE(NokonokoLand, HostTypeTurnStart)) || isNerve(GET_NERVE(NokonokoLand, HostTypeTurn)) ||
           isNerve(GET_NERVE(NokonokoLand, HostTypeTurnEnd)) || isNerve(GET_NERVE(NokonokoLand, HostTypeAppear));
}

bool NokonokoLand::isLookDirRailDirection() const {
    TVec3f front;
    mTargetQuat.getZDir(front);
    TVec3f railDirection(MR::getRailDirection(this));
    if (!MR::isSameDirection(front, railDirection, 0.01f)) {
        return false;
    }

    return 0.0f <= front.dot(railDirection);
}

bool NokonokoLand::tryAttack(HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorEnemyAttack(pSender)) {
        return false;
    }
    if (!MR::isSensorPlayer(pReceiver)) {
        return false;
    }
    if (!isEnableAttack()) {
        return false;
    }
    if (!MR::sendMsgEnemyAttack(pReceiver, pSender)) {
        return false;
    }

    if (isNerve(GET_NERVE(NokonokoLand, HostTypeTurn))) {
        setNerve(GET_NERVE(NokonokoLand, HostTypeAttackTurn));
        return true;
    } else {
        setNerve(GET_NERVE(NokonokoLand, HostTypeAttack));
    }
    return true;
}

bool NokonokoLand::isStateShellStart() const {
    return isNerve(GET_NERVE(NokonokoLand, HostTypeShellStart)) || isNerve(GET_NERVE(NokonokoLand, HostTypeShellStartPunch));
}

bool NokonokoLand::isStateShellWait() const {
    return isNerve(GET_NERVE(NokonokoLand, HostTypeShellWait)) || isNerve(GET_NERVE(NokonokoLand, HostTypeShellEnd));
}

bool NokonokoLand::tryJetTurtleTouch() {
    if (isNerve(GET_NERVE(NokonokoLand, HostTypeAppearStart))) {
        return false;
    }

    bool canTake = false;
    if (isStateShellStart() && MR::isGreaterStep(this, sShellStartPeakPosFrame)) {
        canTake = true;
    }
    if (!isStateShellWait() && !canTake) {
        return false;
    }

    setNerve(GET_NERVE(NokonokoLand, HostTypeBecomeJetTurtle));
    return true;
}

bool NokonokoLand::tryRattle() {
    if (!isStateWalk()) {
        return false;
    }
    if (isNerve(GET_NERVE(NokonokoLand, HostTypeTurn))) {
        setNerve(GET_NERVE(NokonokoLand, HostTypeRattleTurn));
        return true;
    } else {
        setNerve(GET_NERVE(NokonokoLand, HostTypeRattle));
    }
    return true;
}

bool NokonokoLand::tryBindStarPointer() {
    if (!MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        return false;
    }
    if (!isStateWalk()) {
        return false;
    }
    if (isNerve(GET_NERVE(NokonokoLand, HostTypeTurn))) {
        setNerve(GET_NERVE(NokonokoLand, HostTypeBindStarPointerTurn));
        return true;
    } else {
        setNerve(GET_NERVE(NokonokoLand, HostTypeBindStarPointer));
    }
    return true;
}

bool NokonokoLand::receiveTrample(HitSensor* pSender, HitSensor* pReceiver) {
    if (pReceiver == getSensor("body") && (isStateWalk() || isStateRattle())) {
        setNerve(GET_NERVE(NokonokoLand, HostTypeShellStart));
        return true;
    }
    if (pReceiver == getSensor("shell") && isNerve(GET_NERVE(NokonokoLand, HostTypeShellWait)) && MR::isPlayerCarryAny()) {
        setNerve(GET_NERVE(NokonokoLand, HostTypeShellStart));
        return true;
    }

    MR::startSound(this, "SE_EM_TURTLE_SHELL_TRAMPLE");
    return true;
}

bool NokonokoLand::receivePunch(HitSensor* pSender, HitSensor* pReceiver) {
    if (pReceiver != getSensor("body")) {
        return false;
    }
    if (isStateWalk() || isStateRattle()) {
        TVec3f hitPos;
        MR::calcPosBetweenSensors(&hitPos, pSender, pReceiver, 0.0f);
        MR::emitEffectHit(this, hitPos, nullptr);
        setNerve(GET_NERVE(NokonokoLand, HostTypeShellStartPunch));
        return true;
    }

    return false;
}

bool NokonokoLand::receiveHipDrop(HitSensor* pSender, HitSensor* pReceiver) {
    if (pReceiver == getSensor("body") && (isStateWalk() || isStateRattle())) {
        MR::sendMsgAwayJump(pSender, pReceiver);
        setNerve(GET_NERVE(NokonokoLand, HostTypeShellStart));
        return true;
    }
    if (pReceiver == getSensor("shell") && isStateShell()) {
        setNerve(GET_NERVE(NokonokoLand, HostTypeDead));
        return true;
    }

    return false;
}

bool NokonokoLand::receiveTurtleAttack(HitSensor* pSender, HitSensor* pReceiver) {
    if (pReceiver == getSensor("body") && isStateWalk()) {
        TVec3f velocity(pSender->mHost->mVelocity);
        if (!MR::isNearZero(velocity)) {
            mVelocity.set(velocity);
        } else {
            mVelocity.set(pReceiver->mPosition - pSender->mPosition);
        }
        setNerve(GET_NERVE(NokonokoLand, HostTypeFlyAway));
        return true;
    }
    if (pReceiver == getSensor("shell") && isStateShell()) {
        setNerve(GET_NERVE(NokonokoLand, HostTypeDead));
        return true;
    }

    return false;
}

bool NokonokoLand::receiveStorm(HitSensor* pSender, HitSensor* pReceiver) {
    if (pReceiver == getSensor("shell") && isStateShell() && !(isNerve(GET_NERVE(NokonokoLand, HostTypeShellStart)) && MR::isFirstStep(this))) {
        setNerve(GET_NERVE(NokonokoLand, HostTypeBecomeJetTurtle));
        return true;
    }

    if (pReceiver == getSensor("body") && tryRattle()) {
        return true;
    }

    return false;
}

bool NokonokoLand::receiveStarPieceAttack(HitSensor* pSender, HitSensor* pReceiver) {
    if (pReceiver == getSensor("body") && tryRattle()) {
        return true;
    }

    return false;
}

void NokonokoLand::exeAppearReady() {
    if (!(MR::calcDistanceToPlayer(this) < sAppearValidDistance)) {
        setNerve(GET_NERVE(NokonokoLand, HostTypeAppearStart));
    }
}

void NokonokoLand::exeAppearStart() {
    if (MR::isFirstStep(this)) {
        MR::moveCoordAndTransToNearestRailPos(this, mAppearPos);
        if (!MR::isRailGoingToEnd(this)) {
            MR::reverseRailDirection(this);
        }
        MR::makeQuatFromVec(&mTargetQuat, MR::getRailDirection(this), -mGravity);
        if (!MR::isHiddenModel(this)) {
            MR::hideModel(this);
        }
        MR::emitEffect(this, "Appear");
        MR::invalidateClipping(this);
    }

    MR::startLevelSound(this, "SE_EM_LV_NOKOLAND_REVIVE_EFFECT");
    if (MR::isStep(this, sAppearTime)) {
        MR::showModel(this);
        MR::startSound(this, "SE_EM_NOKOLAND_REVIVE_APPEAR");
        MR::startSound(this, "SE_EV_NOKONOKO_APPEAR");
        setNerve(GET_NERVE(NokonokoLand, HostTypeAppear));
    }
}

void NokonokoLand::endAppearStart() {
    tearAppearStart();
}

void NokonokoLand::tearAppearStart() {
    MR::validateHitSensor(this, "body");
    MR::validateHitSensor(this, "attack");
    MR::invalidateHitSensor(this, "shell");
    calcAnim();
    MR::validateClipping(this);
}

void NokonokoLand::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Appear", nullptr);
    }
    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(NokonokoLand, HostTypeWalk));
    }
}

void NokonokoLand::exeFlyAway() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Rotate", nullptr);
        MR::startSound(this, "SE_EM_TURTLE_HIT");
        MR::startSound(this, "SE_EV_NOKONOKO_DAMAGE");

        TVec3f direction;
        MR::vecKillElement(mVelocity, -mGravity, &direction);
        MR::normalize(&direction);
        TVec3f up(-mGravity);
        mVelocity.set(direction * 15.0f + up.multInLine(35.0f));
        MR::makeQuatFromVec(&mTargetQuat, -direction, -mGravity);
    }

    mVelocity.add(mGravity * 1.5f);
    mVelocity.mult(0.99f);
    if (MR::isStep(this, 33)) {
        mVelocity.zero();
        setNerve(GET_NERVE(NokonokoLand, HostTypeDead));
    }
}

void NokonokoLand::exeDead() {
    if (MR::isFirstStep(this)) {
        MR::hideModel(this);
        MR::emitEffect(this, "Death");
        MR::startSound(this, "SE_EM_EXPLODE_S");
        MR::invalidateHitSensors(this);
    }
    if (MR::isStep(this, sDeadTime)) {
        setNerve(GET_NERVE(NokonokoLand, HostTypeAppearReady));
    }
}

void NokonokoLand::exeWalk() {
    if (MR::isFirstStep(this)) {
        if (mTurtleType == 1) {
            startBckBtp(this, "WalkFastWait", "WalkWait");
            MR::setRailCoordSpeed(this, sWalkSpeedFast);
        } else {
            startBckBtp(this, "WalkWait");
            MR::setRailCoordSpeed(this, sWalkSpeed);
        }
    }

    MR::moveCoordAndFollowTrans(this);
    MR::makeQuatFromVec(&mTargetQuat, MR::getRailDirection(this), -mGravity);
    if (mRailPointPassChecker->isPassed() && !MR::isRailReachedGoal(this)) {
        s32 pointArg = -1;
        MR::getCurrentRailPointArg0NoInit(this, &pointArg);
        if (pointArg == 0) {
            setNerve(GET_NERVE(NokonokoLand, HostTypeLookAround));
            return;
        }
    }
    if (MR::isRailReachedGoal(this)) {
        setNerve(GET_NERVE(NokonokoLand, HostTypeTurnStart));
    }
}

void NokonokoLand::exeLookAround() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "LookAround", nullptr);
    }
    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(NokonokoLand, HostTypeWalk));
    }
}

void NokonokoLand::exeTurnStart() {
    if (MR::isFirstStep(this)) {
        startBckBtp(this, "TurnStart", "WalkWait");
    }
    if (MR::isBckStopped(this)) {
        MR::reverseRailDirection(this);
        setNerve(GET_NERVE(NokonokoLand, HostTypeTurn));
    }
}

void NokonokoLand::exeTurnLoop() {
    if (MR::isFirstStep(this)) {
        startBckBtp(this, "TurnLoopStart", "WalkWait");
        TPos3f mtx;
        mtx.set(getBaseMtx());
        mtx.getYDir(mTurnAxis);
        MR::normalize(&mTurnAxis);
    }
    if (MR::isBckOneTimeAndStopped(this)) {
        startBckBtp(this, "TurnLoop", "WalkWait");
    }

    TQuat4f rotation;
    rotation.setRotate(mTurnAxis, -PI / 139.0f);
    mTargetQuat.mult(rotation);
    if (isLookDirRailDirection()) {
        setNerve(GET_NERVE(NokonokoLand, HostTypeTurnEnd));
    }
}

void NokonokoLand::exeTurnEnd() {
    if (MR::isFirstStep(this)) {
        startBckBtp(this, "TurnEnd", "WalkWait");
    }
    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(NokonokoLand, HostTypeWalk));
    }
}

void NokonokoLand::exeAttack() {
    if (MR::isFirstStep(this)) {
        startBckBtp(this, "Attack");
    }
    if (MR::isBckStopped(this)) {
        if (isNerve(GET_NERVE(NokonokoLand, HostTypeAttackTurn))) {
            setNerve(GET_NERVE(NokonokoLand, HostTypeTurn));
        } else {
            setNerve(GET_NERVE(NokonokoLand, HostTypeWalk));
        }
    }
}

void NokonokoLand::exeRattle() {
    if (MR::isFirstStep(this)) {
        startBckBtp(this, "Rattle");
    }

    MR::startLevelSound(this, "SE_EM_LV_NOKOLAND_RATTLE");
    if (MR::isBckStopped(this)) {
        if (isNerve(GET_NERVE(NokonokoLand, HostTypeRattleTurn))) {
            setNerve(GET_NERVE(NokonokoLand, HostTypeTurn));
        } else {
            setNerve(GET_NERVE(NokonokoLand, HostTypeWalk));
        }
    }
}

void NokonokoLand::endBindStarPointer() {
    mBindStarPointerState->kill();
}

void NokonokoLand::exeBindStarPointer() {
    if (MR::isFirstStep(this)) {
        startBckBtp(this, "Rattle");
    }

    if (MR::updateActorStateAndNextNerve(this, mBindStarPointerState,
                                         isNerve(GET_NERVE(NokonokoLand, HostTypeBindStarPointerTurn)) ?
                                             static_cast< const Nerve* >(GET_NERVE(NokonokoLand, HostTypeTurn)) :
                                             GET_NERVE(NokonokoLand, HostTypeWalk))) {
        return;
    }
}

void NokonokoLand::exeShellStartTrample() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "ShellStart", nullptr);
        MR::startSound(this, "SE_EM_TURTLE_SHELL_TRAMPLE");
        MR::startSound(this, "SE_EV_NOKONOKO_TRAMPLE");
        MR::emitEffect(this, "ShellStart");
        MR::emitEffect(this, "ShellStartFace");
        MR::invalidateHitSensor(this, "body");
        MR::invalidateHitSensor(this, "attack");
        MR::validateHitSensor(this, "shell");
        calcAnim();
    }
    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(NokonokoLand, HostTypeShellWait));
    }
}

void NokonokoLand::exeShellStartPunch() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "ShellStartPunch", nullptr);
        MR::startSound(this, "SE_EM_NOKOLAND_SHELL_PUNCH");
        MR::startSound(this, "SE_EV_NOKONOKO_PUNCH_HIT");
        MR::emitEffect(this, "ShellStart");
        MR::emitEffect(this, "ShellStartFace");
        MR::invalidateHitSensor(this, "body");
        MR::invalidateHitSensor(this, "attack");
        MR::validateHitSensor(this, "shell");
        calcAnim();
        MR::stopScene(sSceneStopTimePunch);

        if (!MR::isNearZero(sPushPower)) {
            TVec3f direction(*MR::getPlayerCenterPos() - getSensor("shell")->mPosition);
            MR::vecKillElement(direction, mGravity, &direction);
            MR::normalize(&direction);
            TVec3f pushVelocity(direction);
            pushVelocity.scale(sPushPower);
            MR::pushPlayer(pushVelocity);
        }
    }
    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(NokonokoLand, HostTypeShellWait));
    }
}

void NokonokoLand::exeShellWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "ShellWait", nullptr);
    }
    if (MR::isStep(this, sShellWaitTime)) {
        setNerve(GET_NERVE(NokonokoLand, HostTypeShellEnd));
    }
}

void NokonokoLand::exeShellEnd() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "ShellEnd", nullptr);
        MR::startSound(this, "SE_EM_NOKOLAND_RECVER_SHELL");
        MR::startSound(this, "SE_EV_NOKONOKO_RECOVER");
    }
    if (MR::isBckStopped(this)) {
        MR::validateHitSensor(this, "body");
        MR::validateHitSensor(this, "attack");
        MR::invalidateHitSensor(this, "shell");
        calcAnim();
        if (!isLookDirRailDirection()) {
            setNerve(GET_NERVE(NokonokoLand, HostTypeTurn));
        } else {
            setNerve(GET_NERVE(NokonokoLand, HostTypeWalk));
        }
    }
}

void NokonokoLand::exeBecomeJetTurtle() {
    if (MR::isFirstStep(this)) {
        MR::hideModel(this);
        MR::deleteEffectAll(this);
        TPos3f mtx;
        mtx.set(MR::getJointMtx(this, "Center"));
        TVec3f position;
        mtx.getTrans(position);
        mPosition.set(position);
        mJetTurtle->mPosition.set(position);
        mJetTurtle->appearAndTryTaken();
        MR::invalidateHitSensors(this);
        MR::invalidateClipping(this);
    }
    if (mJetTurtle->isRestart()) {
        MR::validateClipping(this);
        mJetTurtle->kill();
        setNerve(GET_NERVE(NokonokoLand, HostTypeAppearReady));
    }
}
