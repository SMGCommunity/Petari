#include "Game/Enemy/KuriboMini.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/ItemGenerator.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/Enemy/WalkerStateChase.hpp"
#include "Game/Enemy/WalkerStateFindPlayer.hpp"
#include "Game/Enemy/WalkerStateParam.hpp"
#include "Game/Enemy/WalkerStateStagger.hpp"
#include "Game/Enemy/WalkerStateWander.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorStateUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"

void KuriboMini_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)3.0f;
}

namespace {
    class KuriboMiniParam {
    public:
        KuriboMiniParam();

        WalkerStateParam mStateParam;
        WalkerStateStaggerParam mStaggerParam;
        WalkerStateFindPlayerParam mFindPlayerParam;
        WalkerStateChaseParam mChaseParam;
        WalkerStateWanderParam mWanderParam;
    };

    KuriboMiniParam::KuriboMiniParam() {
        mStateParam.mGravityAccel = 1.5f;
        mStateParam.mAirFriction = 0.99f;
        mStateParam.mGroundFriction = 0.93f;
        mStateParam.mPlayerNearDistance = 1000.0f;
        mStateParam.mPlayerSightFanDegreeH = 70.0f;
        mStateParam.mPlayerSightFanDegreeV = 30.0f;
        mWanderParam.mSpeed = 0.1f;
        mWanderParam.mWaitTime = 120;
        mWanderParam.mWalkTime = 120;
        mWanderParam.mTurnMaxRateDegree = 3.0f;
        mChaseParam.mChaseSpeed = 0.2f;
        mFindPlayerParam.mTurnMaxRateDegree = 5.0f;
        mFindPlayerParam.mJumpVelocity = 20.0f;
    }

    static KuriboMiniParam sParam;
};  // namespace

namespace NrvKuriboMini {
    NEW_NERVE(KuriboMiniNrvWander, KuriboMini, Wander);
    NEW_NERVE(KuriboMiniNrvFindPlayer, KuriboMini, FindPlayer);
    NEW_NERVE(KuriboMiniNrvChase, KuriboMini, Chase);
    NEW_NERVE(KuriboMiniNrvStagger, KuriboMini, Stagger);
    NEW_NERVE_ONEND(KuriboMiniNrvBindStarPointer, KuriboMini, BindStarPointer, BindStarPointer);
    NEW_NERVE(KuriboMiniNrvAttackSuccess, KuriboMini, AttackSuccess);
    NEW_NERVE(KuriboMiniNrvHipDropDown, KuriboMini, HipDropDown);
    NEW_NERVE(KuriboMiniNrvPressDown, KuriboMini, PressDown);
    NEW_NERVE(KuriboMiniNrvFlatDown, KuriboMini, FlatDown);
    NEW_NERVE(KuriboMiniNrvBlowDown, KuriboMini, BlowDown);
};  // namespace NrvKuriboMini

KuriboMini::KuriboMini(const char* pName) : LiveActor(pName) {
    mScaleController = nullptr;
    mItemGenerator = nullptr;
    mStateWander = nullptr;
    mStateFindPlayer = nullptr;
    mStateChase = nullptr;
    mStateStagger = nullptr;
    mStateBindStarPointer = nullptr;
    _A8.x = 0.0f;
    _A8.y = 0.0f;
    _A8.z = 0.0f;
    _A8.w = 1.0f;
    _B8.set(0.0f, 0.0f, 1.0f);
}

KuriboMini::~KuriboMini() {
}

void KuriboMini::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("KuriboMini", nullptr, false);
    MR::connectToSceneEnemy(this);
    MR::makeQuatAndFrontFromRotate(&_A8, &_B8, this);
    MR::onCalcGravity(this);
    MR::initLightCtrl(this);
    MR::declareStarPiece(this, 3);
    MR::declareCoin(this, 1);
    mItemGenerator = new ItemGenerator();
    mScaleController = new AnimScaleController(nullptr);
    initSound(4, false);
    initEffectKeeper(1, nullptr, false);
    MR::initStarPointerTarget(this, 40.0f, TVec3f(0.0f, 60.0f, 0.0f));
    MR::initShadowVolumeSphere(this, 40.0f);
    initSensor();
    initBinder(60.0f, 60.0f, 0);
    initNerve(&NrvKuriboMini::KuriboMiniNrvWander::sInstance);
    initState();

    if (MR::isValidInfo(rIter)) {
        MR::setGroupClipping(this, rIter, 32);
    }

    MR::useStageSwitchWriteDead(this, rIter);
    MR::useStageSwitchSleep(this, rIter);
    if (MR::useStageSwitchReadB(this, rIter)) {
        MR::listenStageSwitchOnB(this, MR::Functor_Inline(this, &KuriboMini::calcPassiveMovement));
    }

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    } else {
        makeActorAppeared();
    }
}

void KuriboMini::initAfterPlacement() {
    MR::trySetMoveLimitCollision(this);
}

void KuriboMini::initSensor() {
    initHitSensor(2);
    MR::addHitSensorEnemy(this, "body", 8, 60.0f, TVec3f(0.0f, 60.0f, 0.0f));
    MR::addHitSensorEnemyAttack(this, "attack", 8, 40.0f, TVec3f(0.0f, 60.0f, 0.0f));
}

void KuriboMini::initState() {
    mStateFindPlayer = new WalkerStateFindPlayer(this, &_B8, &::sParam.mStateParam, &::sParam.mFindPlayerParam);
    mStateWander = new WalkerStateWander(this, &_B8, &::sParam.mStateParam, &::sParam.mWanderParam);
    mStateChase = new WalkerStateChase(this, &_B8, &::sParam.mStateParam, &::sParam.mChaseParam);
    mStateStagger = new WalkerStateStagger(this, &_B8, &::sParam.mStateParam, &::sParam.mStaggerParam);
    mStateBindStarPointer = new WalkerStateBindStarPointer(this, mScaleController);
}

void KuriboMini::makeActorAppeared() {
    LiveActor::makeActorAppeared();
    mItemGenerator->setTypeStarPeace(3);
}

void KuriboMini::kill() {
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }

    MR::emitEffect(this, "Death");
    MR::startSound(this, "SE_EM_EXPLODE_S");
    mItemGenerator->generate(this);
    LiveActor::kill();
}

void KuriboMini::control() {
    MR::blendQuatFromGroundAndFront(&_A8, this, _B8, 0.05f, 0.5f);
    mScaleController->updateNerve();
}

void KuriboMini::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _A8);
    MR::setBaseScale(this, mScaleController->_C * mScale);
}

void KuriboMini::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!isDown() &&
        (MR::isSensorEnemyAttack(pSender) || (isEnableAttack() || !MR::isSensorPlayer(pReceiver)) && !MR::isSensorEnemy(pReceiver) ||
         !MR::sendMsgPushAndKillVelocityToTarget(this, pReceiver, pSender)) &&
        isEnableAttack() && MR::isSensorPlayer(pReceiver) && MR::isSensorEnemyAttack(pSender)) {
        if (!MR::isPlayerHipDropFalling() && MR::sendMsgEnemyAttack(pReceiver, pSender)) {
            requestAttackSuccess();
        } else {
            MR::sendMsgPush(pReceiver, pSender);
        }
    }
}

bool KuriboMini::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isDown()) {
        return false;
    }

    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        return requestStagger(pSender, pReceiver);
    }

    if (MR::isMsgPlayerTrample(msg) && requestFlatDown(pSender, pReceiver)) {
        mItemGenerator->setTypeCoin(1);
        return true;
    }

    if (MR::isMsgInvincibleAttack(msg) && requestBlowDown(pSender, pReceiver)) {
        mItemGenerator->setTypeCoin(1);
        return true;
    }

    if (MR::isMsgPlayerHipDrop(msg) && requestHipDropDown(pSender, pReceiver)) {
        mItemGenerator->setTypeCoin(1);
        return true;
    }

    if (MR::isMsgPlayerHitAll(msg) && requestBlowDown(pSender, pReceiver)) {
        mItemGenerator->setTypeCoin(1);
        return true;
    }

    return false;
}

bool KuriboMini::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isDown()) {
        return false;
    }

    if (MR::isMsgToEnemyAttackTrample(msg) && requestPressDown()) {
        mItemGenerator->setTypeStarPeace(3);
        return true;
    }

    if (MR::isMsgToEnemyAttackShockWave(msg)) {
        return requestStagger(pSender, pReceiver);
    }

    if ((MR::isMsgExplosionAttack(msg) || MR::isMsgToEnemyAttackBlow(msg)) && requestBlowDown(pSender, pReceiver)) {
        mItemGenerator->setTypeStarPeace(3);
        return true;
    }

    return false;
}

bool KuriboMini::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    if (isDown()) {
        return false;
    }

    if (MR::isSensorEnemyAttack(pReceiver)) {
        return false;
    }

    if ((MR::isSensorEnemy(pSender) || MR::isSensorRide(pSender) || !isEnableAttack() && MR::isSensorPlayer(pSender))) {
        bool isDwn = isDown() == false;
        if (isDwn) {
            MR::addVelocityFromPush(this, 1.5f, pSender, pReceiver);
            return true;
        }
    }

    return false;
}

bool KuriboMini::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isDown()) {
        return false;
    }

    if (MR::isMsgInhaleBlackHole(msg)) {
        mItemGenerator->setTypeNone();
        kill();
        return true;
    }

    if (MR::isMsgPlayerKick(msg) && isEnableKick() && requestBlowDown(pSender, pReceiver)) {
        mItemGenerator->setTypeStarPeace(3);
        return true;
    }

    return false;
}

bool KuriboMini::requestHipDropDown(HitSensor* pSender, HitSensor* pReceiver) {
    if (isDown()) {
        return false;
    }

    if (MR::isSensorEnemyAttack(pReceiver)) {
        return false;
    }

    MR::startSound(this, "SE_EM_STOMPED_S");
    MR::startAction(this, "FlatDown");
    setNerve(&NrvKuriboMini::KuriboMiniNrvHipDropDown::sInstance);
    MR::offBind(this);
    return true;
}

bool KuriboMini::requestFlatDown(HitSensor* pSender, HitSensor* pReceiver) {
    if (isDown()) {
        return false;
    }

    MR::startSound(this, "SE_EM_STOMPED_S");
    MR::startAction(this, "FlatDown");
    setNerve(&NrvKuriboMini::KuriboMiniNrvFlatDown::sInstance);
    MR::offBind(this);
    return true;
}

bool KuriboMini::requestPressDown() {
    if (isDown()) {
        return false;
    }

    MR::startSound(this, "SE_EM_STOMPED_S");
    MR::startAction(this, "FlatDown");
    setNerve(&NrvKuriboMini::KuriboMiniNrvPressDown::sInstance);
    MR::offBind(this);
    return true;
}

bool KuriboMini::requestBlowDown(HitSensor* pSender, HitSensor* pReceiver) {
    if (isDown()) {
        return false;
    }

    bool isDwn = isDown() == false;
    if (!isDwn) {
        return false;
    }

    MR::setVelocityBlowAttack(this, pSender, pReceiver, 22.0f, 25.0f, 4);
    setNerve(&NrvKuriboMini::KuriboMiniNrvBlowDown::sInstance);
    return true;
}

bool KuriboMini::requestStagger(HitSensor* pSender, HitSensor* pReceiver) {
    if (isDown()) {
        return false;
    }

    bool isDwn = isDown() == false;
    if (isDwn) {
        mStateStagger->setPunchDirection(pSender, pReceiver);
        setNerve(&NrvKuriboMini::KuriboMiniNrvStagger::sInstance);
        return true;
    }

    return false;
}

bool KuriboMini::requestAttackSuccess() {
    if (isDown()) {
        return false;
    }

    if (isEnableAttack()) {
        setNerve(&NrvKuriboMini::KuriboMiniNrvAttackSuccess::sInstance);
        return true;
    }

    return false;
}

bool KuriboMini::tryFind() {
    if (mStateFindPlayer->isInSightPlayer()) {
        setNerve(&NrvKuriboMini::KuriboMiniNrvFindPlayer::sInstance);
        return true;
    }

    return false;
}

bool KuriboMini::tryPointBind() {
    if (mStateBindStarPointer->tryStartPointBind()) {
        setNerve(&NrvKuriboMini::KuriboMiniNrvBindStarPointer::sInstance);
        return true;
    }

    return false;
}

bool KuriboMini::tryDeadMap() {
    if (MR::isInDeath(this, TVec3f(0.0f, 0.0f, 0.0f)) || MR::isBindedGroundDamageFire(this) || MR::isInWater(mPosition)) {
        mItemGenerator->setTypeNone();
        kill();
        return true;
    }

    return false;
}

void KuriboMini::exeWander() {
    MR::updateActorState(this, mStateWander);
    if (!tryFind() && !tryDeadMap() && tryPointBind()) {
        return;
    }
}

void KuriboMini::exeFindPlayer() {
    if (!MR::updateActorStateAndNextNerve(this, mStateFindPlayer, &NrvKuriboMini::KuriboMiniNrvChase::sInstance)) {
        if (mStateFindPlayer->isFindJumpBegin()) {
            MR::startSound(this, "SE_EM_KURIBOMINI_FIND");
        }

        if (!tryDeadMap() && tryPointBind()) {
            return;
        }
    }
}

void KuriboMini::exeChase() {
    if (MR::updateActorStateAndNextNerve(this, mStateChase, &NrvKuriboMini::KuriboMiniNrvWander::sInstance)) {
        mStateWander->setWanderCenter(mPosition);
    }

    if (mStateChase->isRunning()) {
        MR::startLevelSound(this, "SE_EM_LV_KURIBOMINI_DASH");
    }

    if (!tryDeadMap() && tryPointBind()) {
        return;
    }
}

void KuriboMini::exeStagger() {
    if (!MR::updateActorStateAndNextNerve(this, mStateStagger, &NrvKuriboMini::KuriboMiniNrvWander::sInstance)) {
        if (mStateStagger->isStaggerStart()) {
            MR::startSound(this, "SE_EM_CRASH_S");
            MR::startBlowHitSound(this);
        }

        if (mStateStagger->isSwooning(15)) {
            MR::startLevelSound(this, "SE_EM_LV_SWOON_S");
        }

        if (mStateStagger->isRecoverStart()) {
            MR::startSound(this, "SE_EM_KURIBO_SWOON_RECOVER");
        }

        if (tryDeadMap()) {
            return;
        }
    }
}

void KuriboMini::exeBindStarPointer() {
    if (!MR::updateActorStateAndNextNerve(this, mStateBindStarPointer, &NrvKuriboMini::KuriboMiniNrvWander::sInstance)) {
        tryDeadMap();
    }
}

void KuriboMini::endBindStarPointer() {
    mStateBindStarPointer->kill();
}

void KuriboMini::exeAttackSuccess() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Hit");
    }
    MR::turnDirectionToPlayerDegree(this, &_B8, 5.0f);
    calcPassiveMovement();

    if (tryDeadMap() || tryPointBind()) {
        return;
    }

    if (MR::isGreaterStep(this, 60)) {
        setNerve(&NrvKuriboMini::KuriboMiniNrvWander::sInstance);
    }
}

void KuriboMini::exeHipDropDown() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_EM_CRASH_S");
        MR::zeroVelocity(this);
    }

    if (MR::isGreaterStep(this, 40)) {
        kill();
    }
}

void KuriboMini::exeFlatDown() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_EM_CRASH_S");
        MR::zeroVelocity(this);
    }

    if (MR::isGreaterStep(this, 20)) {
        kill();
    }
}

void KuriboMini::exePressDown() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_EM_CRASH_S");
        MR::zeroVelocity(this);
    }

    if (MR::isGreaterStep(this, 180)) {
        kill();
    }
}

void KuriboMini::exeBlowDown() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "BlowDown");
        MR::startSound(this, "SE_EM_CRASH_S");
        MR::startBlowHitSound(this);
    }

    calcPassiveMovement();
    MR::turnDirectionDegree(this, &_B8, -mVelocity, 30.0f);

    if (MR::isGreaterStep(this, 30)) {
        kill();
    }
}

void KuriboMini::calcPassiveMovement() {
    if (!MR::isOnGround(this)) {
        MR::addVelocityToGravity(this, 1.5f);
    }

    f32 velocity;
    if (MR::isOnGround(this)) {
        velocity = 0.93f;
    } else {
        velocity = 0.99f;
    }

    MR::attenuateVelocity(this, velocity);
    if (MR::isBindedWall(this)) {
        MR::calcReboundVelocity(&mVelocity, *MR::getWallNormal(this), 0.2f, 0.7f);
    }
}

bool KuriboMini::isEnableAttack() const {
    if (isNerve(&NrvKuriboMini::KuriboMiniNrvWander::sInstance) || isNerve(&NrvKuriboMini::KuriboMiniNrvFindPlayer::sInstance) ||
        isNerve(&NrvKuriboMini::KuriboMiniNrvChase::sInstance)) {
        return true;
    }

    return false;
}

bool KuriboMini::isEnableKick() const {
    return isNerve(&NrvKuriboMini::KuriboMiniNrvStagger::sInstance);
}

bool KuriboMini::isDown() const {
    if (isNerve(&NrvKuriboMini::KuriboMiniNrvFlatDown::sInstance) || isNerve(&NrvKuriboMini::KuriboMiniNrvHipDropDown::sInstance) ||
        isNerve(&NrvKuriboMini::KuriboMiniNrvPressDown::sInstance) || isNerve(&NrvKuriboMini::KuriboMiniNrvBlowDown::sInstance)) {
        return true;
    }
    return false;
}
