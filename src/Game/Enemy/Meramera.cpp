#include "Game/Enemy/Meramera.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorStateUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/ParabolicPath.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

void Meramera_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)3.0f;
    (void)-1.0f;
    (void)2.0f;
    (void)900.0f;
    (void)55.0f;
    (void)10.0f;
    (void)70.0f;
    (void)45.0f;
    (void)50.0f;
    (void)150.0f;
    (void)300.0f;
    (void)5.0f;
    (void)35.0f;
    (void)0.001f;
    (void)200.0f;
    (void)100.0f;
    (void)3000.0f;
    (void)1500.0f;
    (void)130.0f;
    (void)0.1f;
    (void)15.0f;
    (void)0.98f;
    (void)0.05f;
    (void)0.995f;
    (void)0.2f;
    (void)0.99f;
    (void)0.97f;
    (void)190.0f;
    (void)20.0f;
    (void)0.175f;
    (void)0.3f;
    (void)140.0f;
    (void)1.2f;
    (void)0.9f;
    (void)12.0f;
    (void)80.0f;
    (void)250.0f;
    (void)0.8f;
    (void)(1 / 0.35f);
    (void)1.4f;
    (void)40.0f;
    (void)180.0f;
    (void)0.94f;
    (void)25.0f;
    (void)0.02f;
    (void)500.0f;
    (void)1000.0f;
    (void)0.25f;
    (void)600.0f;
}

namespace {
    const char* sElementArcName[] = {"Meramera", "IceMeramera", "ChildMeramera", "ChildIceMeramera"};
    s32 sElementList[] = {Meramera_FIRE, Meramera_ICE, Meramera_FIRE, Meramera_ICE};

    inline s32 getElementList(const JMapInfoIter& rIter) {
        for (s32 idx = 0; idx < ARRAY_SIZE(::sElementArcName); idx++) {
            if (MR::isEqualObjectName(rIter, ::sElementArcName[idx])) {
                return ::sElementList[idx];
            }
        }

        return 0;
    }

    const TVec3f sAttackHitOffset = TVec3f(50.0f, 0.0f, 0.0f);
};  // namespace

namespace NrvMeramera {
    NEW_NERVE(MerameraNrvWait, Meramera, Wait);
    NEW_NERVE(MerameraNrvWalk, Meramera, Walk);
    NEW_NERVE(MerameraNrvChaseStart, Meramera, ChaseStart);
    NEW_NERVE(MerameraNrvChaseDash, Meramera, ChaseDash);
    NEW_NERVE(MerameraNrvChaseTurn, Meramera, ChaseTurn);
    NEW_NERVE(MerameraNrvDamage, Meramera, Damage);
    NEW_NERVE(MerameraNrvShrink, Meramera, Shrink);
    NEW_NERVE_ONEND(MerameraNrvRunaway, Meramera, Runaway, Runaway);
    NEW_NERVE_ONEND(MerameraNrvStartDiving, Meramera, StartDiving, StartDiving);
    NEW_NERVE(MerameraNrvDiving, Meramera, Diving);
    NEW_NERVE(MerameraNrvSink, Meramera, Sink);
    NEW_NERVE(MerameraNrvFloat, Meramera, Float);
    NEW_NERVE(MerameraNrvIgnition, Meramera, Ignition);
    NEW_NERVE(MerameraNrvIgnitionForce, Meramera, IgnitionForce);
    NEW_NERVE(MerameraNrvAttackSuccess, Meramera, AttackSuccess);
    NEW_NERVE_ONEND(MerameraNrvBindStarPointer, Meramera, BindStarPointer, BindStarPointer);
    NEW_NERVE_ONEND(MerameraNrvBindStarPointerNoFire, Meramera, BindStarPointerNoFire, BindStarPointerNoFire);
    NEW_NERVE(MerameraNrvDown, Meramera, Down);
    NEW_NERVE(MerameraNrvFlatDown, Meramera, FlatDown);
    NEW_NERVE(MerameraNrvReadyRestart, Meramera, ReadyRestart);
}  // namespace NrvMeramera

inline f32 scaleAdd(f32 val, f32 t) {
    return t + (1.0f - t) * val;
}

inline bool Meramera::isSensor(const HitSensor* pSensor, const char* pName) const {
    return getSensor(pName) == pSensor;
}

inline bool Meramera::isActionDisabled() const {
    return isNerve(&NrvMeramera::MerameraNrvRunaway::sInstance) || isNerve(&NrvMeramera::MerameraNrvStartDiving::sInstance) ||
           isNerve(&NrvMeramera::MerameraNrvDiving::sInstance) || isNerve(&NrvMeramera::MerameraNrvBindStarPointerNoFire::sInstance);
}

inline TVec3f Meramera::getDistanceToPlayer() const {
    TVec3f playerPos(*MR::getPlayerPos());
    playerPos -= mPosition;
    return playerPos;
}

inline TVec3f Meramera::getParabolicPos(f32 a2) const {
    TVec3f pos;
    mParabolicPath->calcPosition(&pos, a2);
    return pos;
}

Meramera::Meramera(const char* pName)
    : LiveActor(pName), mAnimScaleController(), mWalkerStateBindStarPointer(), _128(0.0f, 1.0f), _138(0, 0, 1), _144(0, 1, 0), mHomePosition(0, 0, 0),
      _15C(0, 0, 1), _174(0, 0, 0), _180(0, 0, 0), _18C(0, 0, 1), _198(0, 1, 0), _1A4(-1.0f), mChaseDistance(1.0f), mInitialBehaviour(-1),
      mElementType(), mEffectType(), mBodyEffect(), mRunawayTimer(), mCanDive(), mRespawnEnable() {
    _98.identity();
    mExtinguishMtx.identity();
    _F8.identity();

    mParabolicPath = new ParabolicPath();
}

void Meramera::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initElementType(rIter);

    mScale.mult(1.0f);

    initModelManagerWithAnm(::sElementArcName[mElementType], nullptr, false);

    MR::connectToSceneEnemy(this);

    initSound(4, false);
    initSensor();
    initBind();
    initEffect();
    initShadow();

    MR::calcGravity(this);

    mHomePosition = mPosition;

    initNerve(&NrvMeramera::MerameraNrvWait::sInstance);

    MR::useStageSwitchWriteDead(this, rIter);

    mAnimScaleController = new AnimScaleController(nullptr);
    mAnimScaleController->setParamTight();

    mWalkerStateBindStarPointer = new WalkerStateBindStarPointer(this, mAnimScaleController);

    MR::declareCoin(this, 1);

    initAppearState(rIter);
}

void Meramera::initElementType(const JMapInfoIter& rIter) {
    mElementType = ::getElementList(rIter);
}

void Meramera::initEffect() {
    initEffectKeeper(1, nullptr, false);

    MR::setEffectHostMtx(this, "ExtinguishSide", mExtinguishMtx);
    MR::setEffectHostMtx(this, "ExtinguishSideLight", mExtinguishMtx);

    MR::setEffectHostMtx(this, "Wait", _98);
    MR::setEffectHostMtx(this, "Escape", _98);
    MR::setEffectHostMtx(this, "ExtinguishSmoke", _98);
    MR::setEffectHostMtx(this, "ChaseStart", _98);
    MR::setEffectHostMtx(this, "Chase", _98);
    MR::setEffectHostMtx(this, "Attack", _98);

    MR::setEffectHostMtx(this, "Fall", _F8);
}

void Meramera::initSensor() {
    f32 scale = mScale.x;

    LiveActor::initHitSensor(3);

    MR::addHitSensorEnemy(this, "body", 8, scale * 55.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorEnemy(this, "break", 8, scale * 10.0f, TVec3f(0.0f, 0.0f, 0.0f));

    MR::addHitSensorAtJointEnemy(this, "attack", "JointRoot", 8, scale * 70.0f, ::sAttackHitOffset * scale);

    MR::invalidateHitSensor(this, "break");

    MR::initStarPointerTarget(this, scale * 55.0f, TVec3f(0.0f, 0.0f, 0.0f));
}

void Meramera::initBind() {
    f32 scale = mScale.x;
    initBinder(scale * 45.0f, 0.0f, 0);

    MR::onCalcGravity(this);
}

void Meramera::initShadow() {
    f32 scale = mScale.x;

    MR::initShadowController(this, 2);

    MR::addShadowSurfaceCircle(this, "水面用", scale * 50.0f);
    MR::setShadowDropStartOffset(this, "水面用", 150.0f);
    MR::addShadowVolumeSphere(this, "地面用", scale * 50.0f);
}

void Meramera::initAppearState(const JMapInfoIter& rIter) {
    MR::getJMapInfoArg0WithInit(rIter, &mInitialBehaviour);

    resetAppear();

    MR::getJMapInfoArg2NoInit(rIter, &mRespawnEnable);

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    } else {
        makeActorAppeared();
    }
}

void Meramera::makeActorAppeared() {
    LiveActor::makeActorAppeared();
}

void Meramera::kill() {
    MR::emitEffect(this, "Break");

    MR::appearCoinPop(this, mPosition, 1);

    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }

    if (mRespawnEnable) {
        MR::invalidateClipping(this);
        MR::hideModel(this);
        MR::deleteEffectAll(this);

        setNerve(&NrvMeramera::MerameraNrvReadyRestart::sInstance);
    } else {
        LiveActor::kill();
    }
}

void Meramera::startClipped() {
    LiveActor::startClipped();

    MR::deleteEffectAll(this);

    mEffectType = Effect_NULL;
}

void Meramera::endClipped() {
    LiveActor::endClipped();

    setNerve(&NrvMeramera::MerameraNrvWait::sInstance);
}

void Meramera::control() {
    mAnimScaleController->updateNerve();
    updatePose();

    MR::turnVecToVecDegree(&_144, _144, -mGravity, 1.0f, _138);

    MR::makeMtxUpNoSupportPos(&_98, _144, MR::getJointPos(this, "Sin"));

    tryPointBind();
}

void Meramera::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _128);
    MR::setBaseScale(this, mAnimScaleController->_C.mult(mScale));
}

void Meramera::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isSensor(pSender, "body") && (MR::isSensorEnemy(pReceiver) || MR::isSensorPlayer(pReceiver))) {
        MR::sendMsgPush(pReceiver, pSender);
    }

    requestAttack(pSender, pReceiver);
}

bool Meramera::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    if (!isSensor(pReceiver, "body")) {
        return false;
    }

    if (MR::isSensorPlayer(pSender) && requestDown(pReceiver, pSender)) {
        MR::sendMsgKick(pSender, pReceiver);
        return true;
    }

    if (MR::isSensorEnemy(pSender) || MR::isSensorPlayer(pSender)) {
        if (isPushMoved()) {
            TVec3f dir;
            MR::calcSensorDirectionNormalize(&dir, pSender, pReceiver);
            MR::vecScaleAdd(&mVelocity, &dir, 0.5f);
        }

        return true;
    }

    return false;
}

bool Meramera::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return isAffectedSpin();
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        return requestForceRunaway();
    }

    if (MR::isMsgInvincibleAttack(msg)) {
        return requestDownForce(pReceiver, pSender);
    }

    if (MR::isMsgFireBallAttack(msg)) {
        return requestFire(pReceiver, pSender);
    }

    if (MR::isMsgPlayerTrample(msg) || MR::isMsgPlayerHipDrop(msg)) {
        return requestFlatDown(pReceiver, pSender);
    }

    if (MR::isMsgPlayerHitAll(msg)) {
        return requestDown(pReceiver, pSender);
    }

    return false;
}

bool Meramera::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgExplosionAttack(msg)) {
        return requestDownForce(pReceiver, pSender);
    }

    return false;
}

bool Meramera::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgSpinStormRange(msg)) {
        return requestDamage(pReceiver, pSender);
    }

    if (MR::isMsgStartPowerStarGet(msg)) {
        makeActorDead();

        return true;
    }

    return false;
}

bool Meramera::requestDamage(HitSensor* pSender, HitSensor* pReceiver) {
    if (!isSensor(pSender, "body")) {
        return false;
    }

    if (!MR::isSensorPlayer(pReceiver)) {
        return false;
    }

    if (!isAffectedSpin()) {
        return false;
    }

    bool isNear = MR::isNear(pSender, pReceiver, 300.0f) == 0;
    if (isNear) {
        emitEffectHead(1);
        turnFireDirectionToSpin(5.0f);

        return false;
    }

    if (!MR::isPlayerSwingAction()) {
        return false;
    }

    MR::setVelocitySeparateHV(this, pReceiver, pSender, 35.0f, 0.0f);

    setNerve(&NrvMeramera::MerameraNrvDamage::sInstance);

    return true;
}

bool Meramera::requestFire(HitSensor* pSender, HitSensor* pReceiver) {
    switch (mElementType) {
    case Meramera_FIRE:
        if (isEnableFireball()) {
            setNerve(&NrvMeramera::MerameraNrvIgnitionForce::sInstance);
            return true;
        }
        break;

    case Meramera_ICE:
        if (requestForceRunaway()) {
            return true;
        }

        if (requestDownForce(pSender, pReceiver)) {
            return true;
        }
        break;
    }

    return false;
}

bool Meramera::requestForceRunaway() {
    if (!isAffectedSpin()) {
        return false;
    }

    MR::startBck(this, "Damage", nullptr);

    setNerve(&NrvMeramera::MerameraNrvRunaway::sInstance);

    return true;
}

bool Meramera::requestDownForce(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvMeramera::MerameraNrvDown::sInstance)) {
        return false;
    }

    MR::emitEffectHitBetweenSensors(this, pSender, pReceiver, 0.0f, "HitMark");

    TVec3f horizon;
    MR::calcSensorHorizonNormalize(&horizon, mGravity, pSender, pReceiver);

    if (!MR::isNearZero(horizon)) {
        _138.set(horizon);
    }

    MR::zeroVelocity(this);

    setNerve(&NrvMeramera::MerameraNrvDown::sInstance);

    return true;
}

bool Meramera::requestDown(HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorPlayer(pReceiver)) {
        return false;
    }

    if (isActionDisabled()) {
        return requestDownForce(pSender, pReceiver);
    }

    return false;
}

bool Meramera::requestAttack(HitSensor* pSender, HitSensor* pReceiver) {
    if (!isSensor(pSender, "attack") || !isEnableAttack()) {
        return false;
    }

    if (MR::isSensorPlayer(pReceiver)) {
        if (sendMsgElementAttack(pReceiver, pSender)) {
            setNerve(&NrvMeramera::MerameraNrvAttackSuccess::sInstance);
            return true;
        }
    } else {
        sendMsgElementAttack(pReceiver, pSender);
    }
    return false;
}

bool Meramera::requestFlatDown(HitSensor* pSender, HitSensor* pReceiver) {
    if (!isSensor(pSender, "body")) {
        return false;
    }

    if (isActionDisabled()) {
        MR::emitEffectHitBetweenSensors(this, pSender, pReceiver, 0.0f, "HitMark");

        setNerve(&NrvMeramera::MerameraNrvFlatDown::sInstance);

        return true;
    }

    return false;
}

bool Meramera::sendMsgElementAttack(HitSensor* pSender, HitSensor* pReceiver) {
    switch (mElementType) {
    case Meramera_FIRE:
        return MR::sendMsgEnemyAttackFire(pSender, pReceiver);

    case Meramera_ICE:
        return MR::sendMsgEnemyAttackFreeze(pSender, pReceiver);

    default:
        return false;
    }
}

bool Meramera::tryWalk() {
    if (getNerveStep() > 120) {
        TVec3f randomVec;
        MR::getRandomVector(&randomVec, 1.0f);
        MR::normalizeOrZero(&randomVec);
        randomVec.killElement(mGravity);
        randomVec *= 200.0f;
        _174 = randomVec + mHomePosition;

        setNerve(&NrvMeramera::MerameraNrvWalk::sInstance);

        return true;
    }

    return false;
}

bool Meramera::tryWalkEnd() {
    if (getNerveStep() > 300 || (0.0f <= _1A4 && _1A4 <= 100.0f)) {
        setNerve(&NrvMeramera::MerameraNrvWait::sInstance);

        return true;
    }

    return false;
}

bool Meramera::tryEndShrink() {
    if (MR::isGreaterStep(this, 20)) {
        if (isEnableChase()) {
            setNerve(&NrvMeramera::MerameraNrvChaseStart::sInstance);
        } else {
            setNerve(&NrvMeramera::MerameraNrvWait::sInstance);
        }

        return true;
    }

    return false;
}

bool Meramera::tryChase() {
    if (isEnableChase()) {
        setNerve(&NrvMeramera::MerameraNrvChaseStart::sInstance);
        return true;
    }

    return false;
}

bool Meramera::tryChaseStartToDash() {
    if (MR::isGreaterStep(this, 60)) {
        setNerve(&NrvMeramera::MerameraNrvChaseDash::sInstance);
        return true;
    }

    return false;
}

bool Meramera::tryChaseDashToTurn() {
    if (MR::isGreaterStep(this, 45)) {
        setNerve(&NrvMeramera::MerameraNrvChaseTurn::sInstance);
        return true;
    }

    return false;
}

bool Meramera::tryChaseTurnToDash() {
    if (MR::isGreaterStep(this, 50)) {
        setNerve(&NrvMeramera::MerameraNrvChaseDash::sInstance);
        return true;
    }

    return false;
}

bool Meramera::tryEndChase() {
    bool isNearPlayer = MR::isNearPlayer(this, mChaseDistance) == 0;
    if (isNearPlayer || MR::isGreaterStep(this, 240) || mHomePosition.distance(mPosition) > 3000.0f) {
        setNerve(&NrvMeramera::MerameraNrvWait::sInstance);
        return true;
    }

    return false;
}

bool Meramera::tryRunaway() {
    if (MR::isGreaterStep(this, MR::getRandom(0L, 0L) + 30)) {
        setNerve(&NrvMeramera::MerameraNrvRunaway::sInstance);
        return true;
    }

    return false;
}

bool Meramera::tryStartDiving() {
    if (mCanDive && MR::isBindedGround(this)) {
        MR::zeroVelocity(this);

        setNerve(&NrvMeramera::MerameraNrvStartDiving::sInstance);

        return true;
    }

    return false;
}

bool Meramera::tryDiving() {
    if (MR::isGreaterStep(this, 35)) {
        setNerve(&NrvMeramera::MerameraNrvDiving::sInstance);
        return true;
    }

    return false;
}

bool Meramera::tryEndDiving() {
    if (MR::isGreaterStep(this, 40)) {
        setNerve(&NrvMeramera::MerameraNrvRunaway::sInstance);
        return true;
    }

    return false;
}

bool Meramera::tryFloat() {
    if (MR::isGreaterStep(this, 50)) {
        setNerve(&NrvMeramera::MerameraNrvFloat::sInstance);
        return true;
    }

    return false;
}

bool Meramera::tryEndFloat() {
    if (MR::isGreaterStep(this, 30)) {
        setNerve(&NrvMeramera::MerameraNrvIgnition::sInstance);
        return true;
    }

    return false;
}

bool Meramera::tryEndIgnition() {
    if (MR::isGreaterStep(this, 20)) {
        if (isEnableChase()) {
            setNerve(&NrvMeramera::MerameraNrvChaseDash::sInstance);
        } else {
            setNerve(&NrvMeramera::MerameraNrvWait::sInstance);
        }
        return true;
    }
    return false;
}

bool Meramera::tryForceSink() {
    if (isHitRecover()) {
        _180.set(mPosition);
        _198.set(-mGravity);

        MR::zeroVelocity(this);

        setNerve(&NrvMeramera::MerameraNrvSink::sInstance);
        return true;
    }

    return false;
}

bool Meramera::tryRecovery() {
    f32 scale = mScale.x;
    bool isThisNearPlayer = MR::isNearPlayer(this, 1500.0f) == 0;
    if (isThisNearPlayer) {
        bool is150NearPlayer = MR::isNearPlayer(mHomePosition, 1500.0f) == 0;
        if (is150NearPlayer && MR::isJudgedToClipFrustum(mPosition, scale * 100.0f) && MR::isJudgedToClipFrustum(mHomePosition, scale * 100.0f)) {
            resetAppear();

            return true;
        }
    }

    return false;
}

bool Meramera::tryEndAttackSuccess() {
    if (MR::isGreaterStep(this, 180)) {
        setNerve(&NrvMeramera::MerameraNrvWait::sInstance);
        return true;
    }

    return false;
}

bool Meramera::tryPointBind() {
    if ((isNerve(&NrvMeramera::MerameraNrvWait::sInstance) || isNerve(&NrvMeramera::MerameraNrvWalk::sInstance) ||
         isNerve(&NrvMeramera::MerameraNrvChaseStart::sInstance) || isNerve(&NrvMeramera::MerameraNrvChaseDash::sInstance) ||
         isNerve(&NrvMeramera::MerameraNrvChaseTurn::sInstance) || isNerve(&NrvMeramera::MerameraNrvIgnition::sInstance) ||
         isNerve(&NrvMeramera::MerameraNrvIgnitionForce::sInstance) || isNerve(&NrvMeramera::MerameraNrvAttackSuccess::sInstance)) &&
        mWalkerStateBindStarPointer->tryStartPointBind()) {
        setNerve(&NrvMeramera::MerameraNrvBindStarPointer::sInstance);
        return true;
    }

    if ((isNerve(&NrvMeramera::MerameraNrvRunaway::sInstance) || isNerve(&NrvMeramera::MerameraNrvStartDiving::sInstance)) &&
        mWalkerStateBindStarPointer->tryStartPointBind()) {
        setNerve(&NrvMeramera::MerameraNrvBindStarPointerNoFire::sInstance);
        return true;
    }

    return false;
}

void Meramera::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);

        MR::startBck(this, "Wait", nullptr);

        emitEffectHead(1);
        emitEffectHeatBody();
    }

    startWaitLevelSound();

    MR::addVelocityKeepHeightUseShadow(this, 130.0f, 0.5f, 0.1f, 15.0f, nullptr);
    MR::attenuateVelocity(this, 0.98f);
    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);

    if (!tryChase() && tryWalk()) {
        return;
    }
}

void Meramera::exeWalk() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);

        emitEffectHead(1);
        emitEffectHeatBody();
    }

    startWaitLevelSound();

    MR::addVelocityKeepHeightUseShadow(this, 130.0f, 0.5f, 0.1f, 15.0f, nullptr);
    addToTargetMovingAccel(_174, 0.05f, 0.995f);
    MR::attenuateVelocity(this, 0.98f);

    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);

    if (!tryChase() && tryWalkEnd()) {
        return;
    }
}

void Meramera::exeShrink() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(this);

        MR::startBck(this, "Wait", nullptr);

        emitEffectHead(1);
        emitEffectHeatBody();
    }

    turnFireDirectionToSpin(5.0f);

    MR::addVelocityKeepHeightUseShadow(this, 130.0f, 0.5f, 0.1f, 15.0f, nullptr);
    MR::attenuateVelocity(this, 0.98f);
    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);

    if (tryEndShrink()) {
        return;
    }
}

void Meramera::exeChaseStart() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);

        MR::startBck(this, "ChaseStart", nullptr);
        MR::startSound(this, "SE_EM_MERAMERA_FIND");

        if (mElementType == Meramera_ICE) {
            MR::startSound(this, "SE_EM_ICEMERA_POWER_UP");
        } else {
            MR::startSound(this, "SE_EM_MERAMERA_FIREPOWER_UP");
        }

        deleteEffectHead(false);
        emitEffectHeatBody();
    }

    startWaitLevelSound();

    MR::addVelocityKeepHeightUseShadow(this, 300.0f, 0.5f, 0.2f, 15.0f, nullptr);
    addToTargetMovingAccel(*MR::getPlayerPos(), 0.05f, 0.98f);
    MR::attenuateVelocity(this, 0.97f);
    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);

    if (!tryEndChase() && tryChaseStartToDash()) {
        return;
    }
}

void Meramera::exeChaseDash() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(this);

        MR::startBck(this, "ChaseDash", nullptr);

        MR::startSound(this, "SE_EM_MERAMERA_DASH");

        mPlayerPosition = *MR::getPlayerPos();
        _15C = mPlayerPosition - mPosition;

        emitEffectHead(2);
        emitEffectHeatBody();
    }

    startChaseLevelSound();

    if (MR::isLessStep(this, 30)) {
        MR::addVelocityKeepHeightUseShadow(this, 150.0f, 0.5f, 0.1f, 15.0f, nullptr);
    } else {
        MR::addVelocityKeepHeightUseShadow(this, 190.0f, 0.5f, 0.2f, 20.0f, nullptr);
    }

    addMovingAccel(_15C, 0.175f, 0.98f);
    addOverWallAccel(_15C);
    MR::attenuateVelocity(this, 0.98f);
    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);

    if (tryChaseDashToTurn()) {
        return;
    }
}

void Meramera::exeChaseTurn() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);

        MR::startBck(this, "ChaseTurn", nullptr);

        emitEffectHead(2);
        emitEffectHeatBody();
    }

    startChaseLevelSound();

    MR::addVelocityKeepHeightUseShadow(this, 190.0f, 0.3f, 0.3f, 20.0f, nullptr);
    addToTargetMovingAccel(*MR::getPlayerPos(), 0.05f, 0.99f);
    MR::attenuateVelocity(this, 0.97f);
    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);

    if (!tryEndChase() && tryChaseTurnToDash()) {
        return;
    }
}

void Meramera::exeDamage() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(this);

        MR::startBck(this, "Damage", nullptr);

        MR::turnVecToVecCosOnPlane(&_138, getDistanceToPlayer(), mGravity, -1.0f);

        if (mElementType == Meramera_ICE) {
            MR::startSound(this, "SE_EM_ICEMERA_BLOW_COLD");
        } else {
            MR::startSound(this, "SE_EM_MERAMERA_BLOW_FIRE");
        }

        emitEffectHead(2);
        emitEffectHeatBody();
    }

    if (MR::isStep(this, 3)) {
        mExtinguishMtx.identityAndSetTrans(MR::getJointPos(this, "Sin"));

        deleteEffectHead(true);
        emitEffectCoolDownBody();
        MR::emitEffect(this, "ExtinguishSide");
        MR::emitEffect(this, "ExtinguishSmoke");

        MR::startSound(this, "SE_EM_MERAMERA_SMOKE");
    }

    if (MR::isGreaterEqualStep(this, 3)) {
        updateExtinguishMtx();
    }

    MR::addVelocityKeepHeightUseShadow(this, 140.0f, 0.0f, 1.2f, 15.0f, nullptr);
    MR::attenuateVelocity(this, 0.9f);
    MR::reboundVelocityFromEachCollision(this, 0.0f, 0.0f, 0.0f, 0.0f);

    if (tryRunaway()) {
        return;
    }
}

void Meramera::exeRunaway() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(this);
        MR::validateHitSensor(this, "break");

        mCanDive = false;

        deleteEffectHead(false);
        emitEffectColdBody();

        mRunawayTimer = 0;
    }

    if (tryForceSink()) {
        return;
    }

    f32 dot = mGravity.dot(mVelocity);

    if (MR::isStep(this, 10)) {
        emitEffectHead(3);
    }

    if (MR::isBindedGround(this) && dot > 12.0f) {
        MR::emitEffect(this, "EscapeRock");
        MR::startSound(this, "SE_EM_MERAMERA_BOUND");
    }

    if (tryStartDiving()) {
        return;
    }

    if (MR::isGreaterStep(this, 70)) {
        if (MR::isBindedGround(this)) {
            TVec3f vec;
            TVec3f& vec1 = mVelocity;
            TVec3f& vec2 = mGravity;
            vec.scaleAdd(-vec2.dot(vec1), vec2, vec1);  // Looks like killElement but I can't make it match

            f32 scalar;
            MR::separateScalarAndDirection(&scalar, &vec, vec);

            if (scalar < 3.0f) {
                mRunawayTimer--;
            }

            if (mRunawayTimer <= 0) {
                addRunawayJumpPower();

                MR::emitEffect(this, "EscapeRock");

                MR::startSound(this, "SE_EM_MERAMERA_JUMP");

                mRunawayTimer = MR::getRandom(0L, 10L) + 30;
            }
        }

        if (!mCanDive) {
            f32 val = (getNerveStep() % 5 * 80.0f) / 5.0f;
            if (getNerveStep() % 2 == 1) {
                val = -val;
            }
            checkDivingPoint(250.0f, val);
        }
    }

    if (!MR::isBindedGround(this)) {
        MR::addVelocityToGravity(this, 2.0f);
    }

    MR::attenuateVelocity(this, MR::isBindedGround(this) ? 0.8f : 0.98f);
    MR::reboundVelocityFromEachCollision(this, 0.5f, 0.3f, 0.0f, 10.0f);

    if (tryRecovery()) {
        return;
    }
}

void Meramera::exeStartDiving() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(this);
        MR::validateHitSensor(this, "break");

        deleteEffectHead(false);
        emitEffectColdBody();

        MR::setBckRate(this, 0.0f);
    }

    if (MR::isLessStep(this, 35)) {
        checkDivingPointMore(1.0f / 0.35f);
    }

    if (tryDiving()) {
        return;
    }
}

void Meramera::exeDiving() {
    if (MR::isFirstStep(this)) {
        mParabolicPath->initFromUpVector(mPosition, _180, _198, 300.0f);

        deleteEffectHead(false);
        emitEffectColdBody();

        MR::turnVecToVecCosOnPlane(&_138, _180 - mPosition, mGravity, -1.0f);

        MR::invalidateClipping(this);

        MR::setBckRate(this, 1.4f);
        MR::emitEffect(this, "EscapeRock");
        MR::startSound(this, "SE_EM_MERAMERA_JUMP");
    }

    mVelocity.set(getParabolicPos(getNerveStep() / 40.0f) - mPosition);

    MR::turnVecToVecDegree(&_144, _144, -mGravity, 180.0f, _138);

    if (!tryForceSink() && tryEndDiving()) {
        return;
    }
}

void Meramera::exeSink() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(this);

        MR::emitEffect(this, "Fall");

        if (mElementType == Meramera_ICE) {
            MR::emitEffect(this, "Revival");
        }

        MR::startSound(this, "SE_EM_MERAMERA_LAVA_IN");
        MR::startBck(this, "Wait", nullptr);

        MR::hideModel(this);
        MR::offBind(this);

        MR::makeMtxUpNoSupportPos(&_F8, -mGravity, mPosition);

        deleteEffectHead(false);
        emitEffectColdBody();
    }

    MR::turnVecToVecCosOnPlane(&_138, getDistanceToPlayer(), mGravity, -1.0f);

    if (tryFloat() && mElementType == Meramera_ICE) {
        MR::deleteEffect(this, "Revival");
    }
}

void Meramera::exeFloat() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(this);

        MR::showModel(this);

        MR::onBind(this);

        MR::startBck(this, "Float", nullptr);
        MR::emitEffect(this, "Fall");
        MR::startSound(this, "SE_EM_MERAMERA_LAVA_OUT");

        deleteEffectHead(false);
        emitEffectHeatBody();

        MR::setVelocity(this, _198 * 50.0f);
    }

    MR::addVelocityKeepHeightUseShadow(this, 100.0f, 0.9f, 0.1f, 50.0f, nullptr);
    MR::attenuateVelocity(this, 0.94f);
    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);

    MR::turnVecToVecCosOnPlane(&_138, getDistanceToPlayer(), mGravity, -1.0f);

    if (tryEndFloat()) {
        return;
    }
}

void Meramera::exeIgnition() {
    if (MR::isStep(this, 15)) {
        MR::invalidateClipping(this);

        MR::startBck(this, "FloatIgnition", nullptr);

        if (mElementType == Meramera_ICE) {
            MR::startSound(this, "SE_EM_ICEMERA_IGNITION");
        } else {
            MR::startSound(this, "SE_EM_MERAMERA_IGNITION");
        }

        MR::addVelocity(this, _198 * 25.0f);
    }

    MR::addVelocityKeepHeightUseShadow(this, 100.0f, 0.5f, 0.1f, 50.0f, nullptr);
    MR::attenuateVelocity(this, 0.98f);
    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);

    if (tryEndIgnition()) {
        return;
    }
}

void Meramera::exeIgnitionForce() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(this);

        MR::startBck(this, "FloatIgnition", nullptr);

        MR::setVelocityJump(this, 15.0f);

        emitEffectHead(1);
        emitEffectHeatBody();
    }

    MR::addVelocityKeepHeightUseShadow(this, 130.0f, 0.5f, 0.1f, 15.0f, nullptr);
    MR::attenuateVelocity(this, 0.98f);
    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);

    if (MR::isBckStopped(this)) {
        setNerve(&NrvMeramera::MerameraNrvWait::sInstance);
    }
}

void Meramera::exeAttackSuccess() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(this);

        MR::startBck(this, "Attack", nullptr);

        MR::zeroVelocity(this);

        deleteEffectHead(false);
        emitEffectHeatBody();
    }

    MR::addVelocityKeepHeightUseShadow(this, 50.0f, 0.5f, 0.1f, 15.0f, nullptr);
    MR::attenuateVelocity(this, 0.98f);
    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);

    if (tryEndAttackSuccess()) {
        return;
    }
}

void Meramera::exeDown() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(this);

        MR::startBck(this, "Down", nullptr);

        MR::stopScene(2);

        MR::startSound(this, "SE_EM_MERAMERA_STOMPED");

        deleteEffectHead(false);
        emitEffectColdBody();

        MR::zeroVelocity(this);
    }

    if (MR::isStep(this, 0)) {
        mVelocity = -_138 * 0.0f - mGravity * 25.0f;
    }

    if (MR::isGreaterEqualStep(this, 0)) {
        if (!MR::isBindedGround(this)) {
            MR::addVelocityToGravity(this, 0.5f);
        }

        MR::attenuateVelocity(this, 0.99f);
        MR::reboundVelocityFromEachCollision(this, 0.9f, 0.9f, 0.9f, 0.0f);
    }

    if ((!MR::isGreaterStep(this, 0) || !MR::isBinded(this)) && !MR::isGreaterStep(this, 0)) {
        return;
    }

    MR::startSound(this, "SE_EM_MERAMERA_EXPLODE");

    kill();
}

void Meramera::exeFlatDown() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(this);

        MR::startBck(this, "FlatDown", nullptr);

        MR::stopScene(2);

        MR::startSound(this, "SE_EM_MERAMERA_STOMPED");

        MR::zeroVelocity(this);

        deleteEffectHead(false);
        emitEffectColdBody();
    }

    if (MR::isGreaterStep(this, 0)) {
        MR::startSound(this, "SE_EM_EXPLODE_S");

        kill();
    }
}

bool Meramera::isAffectedSpin() const {
    return isNerve(&NrvMeramera::MerameraNrvWait::sInstance) || isNerve(&NrvMeramera::MerameraNrvAttackSuccess::sInstance) ||
           isNerve(&NrvMeramera::MerameraNrvWalk::sInstance) || isNerve(&NrvMeramera::MerameraNrvShrink::sInstance) ||
           isNerve(&NrvMeramera::MerameraNrvChaseStart::sInstance) || isNerve(&NrvMeramera::MerameraNrvChaseDash::sInstance) ||
           isNerve(&NrvMeramera::MerameraNrvChaseTurn::sInstance) || isNerve(&NrvMeramera::MerameraNrvIgnition::sInstance) ||
           isNerve(&NrvMeramera::MerameraNrvIgnitionForce::sInstance) || isNerve(&NrvMeramera::MerameraNrvBindStarPointer::sInstance);
}

bool Meramera::isEnableAttack() const {
    return isNerve(&NrvMeramera::MerameraNrvWait::sInstance) || isNerve(&NrvMeramera::MerameraNrvWalk::sInstance) ||
           isNerve(&NrvMeramera::MerameraNrvChaseDash::sInstance) || isNerve(&NrvMeramera::MerameraNrvChaseTurn::sInstance) ||
           isNerve(&NrvMeramera::MerameraNrvBindStarPointer::sInstance);
}

bool Meramera::isPushMoved() const {
    return isNerve(&NrvMeramera::MerameraNrvWait::sInstance) || isNerve(&NrvMeramera::MerameraNrvWalk::sInstance) ||
           isNerve(&NrvMeramera::MerameraNrvDamage::sInstance) || isNerve(&NrvMeramera::MerameraNrvChaseDash::sInstance) ||
           isNerve(&NrvMeramera::MerameraNrvChaseTurn::sInstance) || isNerve(&NrvMeramera::MerameraNrvAttackSuccess::sInstance);
}

bool Meramera::isEnableFireball() const {
    if (isNerve(&NrvMeramera::MerameraNrvFloat::sInstance) || isNerve(&NrvMeramera::MerameraNrvIgnition::sInstance) ||
        isNerve(&NrvMeramera::MerameraNrvIgnitionForce::sInstance) || isNerve(&NrvMeramera::MerameraNrvDown::sInstance) ||
        isNerve(&NrvMeramera::MerameraNrvFlatDown::sInstance) || isNerve(&NrvMeramera::MerameraNrvReadyRestart::sInstance)) {
        return false;
    }

    return true;
}

bool Meramera::isEnableChase() const {
    return MR::isNearPlayer(this, mChaseDistance) && mHomePosition.distance(mPosition) <= 3000.0f;
}

void Meramera::addToTargetMovingAccel(const TVec3f& rVec, f32 f1, f32 f2) {
    addMovingAccel(rVec - mPosition, f1, f2);
}

void Meramera::addMovingAccel(const TVec3f& rVec, f32 f1, f32 f2) {
    TVec3f vec(mGravity);
    TVec3f vec2;
    vec2.killElement(rVec, vec);

    MR::separateScalarAndDirection(&_1A4, &vec2, vec2);

    if (!MR::isNearZero(vec2)) {
        if (-1.0f < f2 && f2 < 1.0f) {
            MR::turnVecToVecCos(&_138, _138, vec2, f2, vec, 0.02f);
        } else {
            _138 = vec2;
        }
    }

    mVelocity += _138 * f1;
}

void Meramera::addRunawayJumpPower() {
    // FIXME
    // https://decomp.me/scratch/vrE43
    TVec3f vec;
    f32 val = 1.0f;
    if (mCanDive) {
        vec.set(_180 - mPosition);

        TVec3f vec2;
        vec2.killElement(mGravity, vec);

        val = MR::normalize(vec2.length(), 10.0f, 500.0f);
    } else if (MR::isNearPlayer(this, 1000.0f)) {
        vec.set(getDistanceToPlayer());
    } else {
        vec.set(mHomePosition - mPosition);
    }

    MR::normalizeOrZero(&vec);
    MR::addRandomVector(&vec, vec, 0.5f * val);

    val = 20.0f * scaleAdd(val, 0.1f);
    f32 randomVal = MR::getRandom(45.0f, 900.0f);
    f32 val2 = scaleAdd(val, 0.2f) * randomVal;
    addMovingAccel(vec, val, -1.0f);
    mPosition -= mGravity * val2;
}

void Meramera::addOverWallAccel(const TVec3f& rVec) {
    TVec3f gravity;
    gravity.set(mGravity);

    TVec3f vec;
    vec.killElement(rVec, gravity);
    MR::normalizeOrZero(&vec);

    if (MR::getFirstPolyOnLineToMap(nullptr, nullptr, mPosition, vec * 200.0f) && MR::getShadowNearProjectionLength(this) < 250.0f) {
        mVelocity -= gravity * 0.9f;
    }
}

void Meramera::updatePose() {
    if (isActionDisabled()) {
        MR::rotateQuatRollBall(&_128, mVelocity, -mGravity, 50.0f);
    } else {
        MR::blendQuatUpFront(&_128, -mGravity, _138, 0.25f, 0.25f);
    }
}

void Meramera::updateExtinguishMtx() {
    turnFireDirectionToSpin(10.0f);

    TVec3f trans;
    mExtinguishMtx.getTrans(trans);

    TVec3f pullVelocity;
    MR::calcPlayerSpinPullVelocity(&pullVelocity, trans);

    trans += pullVelocity * 0.9f;

    TVec3f vec48;
    vec48.set(*MR::getPlayerPos() - trans);
    f32 length = vec48.length();

    if (0.0f < length && length < 100.0f) {
        trans = *MR::getPlayerPos() + vec48 * (100.0f / length);
    }

    if (MR::isNearZero(pullVelocity)) {
        mExtinguishMtx.identity();
        mExtinguishMtx.setTrans(trans);
    } else {
        MR::makeMtxUpNoSupportPos(&mExtinguishMtx, pullVelocity, trans);
    }
}

bool Meramera::checkDirectDivingPoint() {
    mCanDive = isHitRecover();

    if (mCanDive) {
        _180.set(mPosition);
        _198.set(-mGravity);

        mCanDive = true;
    }

    return mCanDive;
}

bool Meramera::checkDivingPoint(f32 f1, f32 f2) {
    mCanDive = false;

    if (checkDirectDivingPoint()) {
        return true;
    }

    TVec3f vec3C(mPosition);
    TVec3f vec48(_138 * f1);
    MR::rotateVecDegree(&vec48, mGravity, f2);
    vec3C += vec48;
    vec3C -= mGravity * 100.0f;

    TVec3f vec2(mGravity * 600.0f + vec48);
    mCanDive = findDivingPoint(vec3C, vec2);

    return mCanDive;
}

bool Meramera::checkDivingPointMore(f32 f1) {
    TVec3f vec30;
    vec30.set(_180);
    vec30 += _18C * f1;

    vec30 -= mGravity * 100.0f;

    TVec3f vec2(mGravity * 600.0f + _18C * f1);
    return findDivingPoint(vec30, vec2);
}

bool Meramera::isHitRecover() const {
    switch (mElementType) {
    case Meramera_FIRE:
        if (MR::isBindedGroundDamageFire(this)) {
            return true;
        }
        break;

    case Meramera_ICE:
        if (MR::isBindedGroundWater(this) || MR::isInWater(mPosition)) {
            return true;
        }
        break;
    }

    return false;
}

bool Meramera::findDivingPoint(TVec3f vec, const TVec3f& rVec) {
    // FIXME: there is some inline here
    // https://decomp.me/scratch/uajQ5
    Triangle triangle = Triangle();

    bool myBool = false;
    switch (mElementType) {
    case Meramera_ICE:
        break;

    case Meramera_FIRE:
        TVec3f vec84;
        if (!MR::isExistMapCollision(mPosition, vec - mPosition) && MR::getFirstPolyOnLineToMap(&vec84, &triangle, vec, rVec) &&
            MR::isGroundCodeDamageFire(&triangle)) {
            myBool = true;
        } else {
            myBool = false;
        }

        if (myBool) {
            _180.set(vec84);
            _198.set(-mGravity);

            _18C.killElement(vec84 - mPosition, _198);
            MR::normalizeOrZero(&_18C);
        }
        break;
    }

    return myBool;
}

void Meramera::resetAppear() {
    mPosition.set(mHomePosition);
    MR::zeroVelocity(this);
    MR::calcGravity(this);

    switch (mInitialBehaviour) {
    case 0:
        setNerve(&NrvMeramera::MerameraNrvRunaway::sInstance);
        break;

    case 1:
        MR::startBck(this, "Wait", nullptr);
        MR::hideModel(this);
        MR::offBind(this);

        MR::makeMtxUpNoSupportPos(&_F8, -mGravity, mPosition);

        deleteEffectHead(false);
        emitEffectColdBody();

        _198.set(-mGravity);
        _180.set(mPosition);

        setNerve(&NrvMeramera::MerameraNrvFloat::sInstance);
        break;

    default:
        setNerve(&NrvMeramera::MerameraNrvWait::sInstance);
        break;
    }

    MR::resetPosition(this);
}

void Meramera::emitEffectHead(s32 effectType) {
    if (mEffectType == effectType) {
        return;
    }

    deleteEffectHead(false);

    mEffectType = effectType;

    switch (mEffectType) {
    case Effect_NULL:
        return;

    case Effect_WAIT:
        MR::emitEffect(this, "Wait");
        break;

    case Effect_CHASE:
        MR::emitEffect(this, "Chase");
        break;

    case Effect_ESCAPE:
        MR::emitEffect(this, "Escape");
        break;
    }
}

void Meramera::deleteEffectHead(bool useForce) {
    const char* pEffectName;
    switch (mEffectType) {
    case Effect_NULL:
        return;

    case Effect_WAIT:
        pEffectName = "Wait";
        break;

    case Effect_CHASE:
        pEffectName = "Chase";
        break;

    case Effect_ESCAPE:
        pEffectName = "Escape";
        break;

    default:
        return;
    }

    if (useForce) {
        MR::forceDeleteEffect(this, pEffectName);
    } else {
        MR::deleteEffect(this, pEffectName);
    }

    mEffectType = Effect_NULL;
}

void Meramera::emitEffectHeatBody() {
    if (mBodyEffect == 1) {
        return;
    }

    mBodyEffect = 1;

    MR::startBtp(this, "OnFire");
    MR::startBrk(this, "OnFire");
}

void Meramera::emitEffectCoolDownBody() {
    if (mBodyEffect == 2) {
        return;
    }

    mBodyEffect = 2;

    MR::startBtp(this, "RedToBlack");
    MR::startBrk(this, "RedToBlack");
}

void Meramera::emitEffectColdBody() {
    if (mBodyEffect == 3) {
        return;
    }

    mBodyEffect = 3;

    MR::startBtp(this, "OffFire");
    MR::startBrk(this, "OffFire");
}

void Meramera::turnFireDirectionToSpin(f32 f1) {
    TVec3f jointPos;
    MR::copyJointPos(this, "Sin", &jointPos);

    TVec3f pullVelocity;
    MR::calcPlayerSpinPullVelocity(&pullVelocity, jointPos);

    if (MR::isNearZero(pullVelocity)) {
        return;
    }

    MR::normalize(pullVelocity, &pullVelocity);
    MR::turnVecToVecDegree(&_144, _144, pullVelocity, f1, _138);
}

void Meramera::startWaitLevelSound() {
    if (mElementType == Meramera_ICE) {
        MR::startLevelSound(this, "SE_EM_LV_ICEMERA_WAIT");
        return;
    }

    MR::startLevelSound(this, "SE_EM_LV_MERAMERA_WAIT");
}

void Meramera::startChaseLevelSound() {
    if (mElementType == Meramera_ICE) {
        MR::startLevelSound(this, "SE_EM_LV_ICEMERA_CHASE");
        return;
    }

    MR::startLevelSound(this, "SE_EM_LV_MERAMERA_CHASE");
}

void Meramera::endRunaway() {
    MR::invalidateHitSensor(this, "break");
}

void Meramera::endStartDiving() {
    MR::invalidateHitSensor(this, "break");
}

void Meramera::exeBindStarPointer() {
    MR::updateActorStateAndNextNerve(this, mWalkerStateBindStarPointer, &NrvMeramera::MerameraNrvWait::sInstance);
}

void Meramera::endBindStarPointer() {
    mWalkerStateBindStarPointer->kill();
}

void Meramera::exeBindStarPointerNoFire() {
    MR::updateActorStateAndNextNerve(this, mWalkerStateBindStarPointer, &NrvMeramera::MerameraNrvRunaway::sInstance);
}

void Meramera::endBindStarPointerNoFire() {
    mWalkerStateBindStarPointer->kill();
}

void Meramera::exeReadyRestart() {
    if (!MR::isGreaterStep(this, 180)) {
        return;
    }

    MR::showModel(this);
    resetAppear();
}

Meramera::~Meramera() {
}
