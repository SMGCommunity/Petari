#include "Game/Enemy/Poihana.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorStateUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include <JSystem/JMath/JMath.hpp>

#define POIHANA_BEHAVIOR_NORMAL 0
#define POIHANA_BEHAVIOR_SLEEP 1
#define POIHANA_BEHAVIOR_NEW_HOME 2

void Poihana_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.000003814697265625f;
    (void)0.5f;
    (void)3.0f;
    (void)2.0f;
}

namespace {
    static const f32 sGravity = 2.0f;
    static const f32 sReceivePushPower = 1.5f;
    static const f32 sVelocityDownRate = 0.95f;
    static const f32 sVelocityDownRateSide = 0.8f;
    static const f32 sActiveRadius = 3500.0f;
    static const f32 sNormalBinderRadius = 125.0f;
    static const f32 sTrampleBinderRadius = 200.0f;
    static const f32 sWalkSpeedMax = 5.0f;
    static const f32 sWalkAccel = 0.5f;
    static const f32 sWalkTurnRate = 2.0f;
    static const f32 sWalkAroundRadius = 350.0f;
    static const s32 sWalkAroundBreakTime = 180;
    static const s32 sWalkAroundKeepTime = 120;
    static const s32 sWalkAroundTurn = 2;
    static const f32 sJudgeForGetUpSpeed = 10.0f;
    static const f32 sGetUpRadius = 500.0f;
    static const f32 sSearchRadius = 800.0f;
    static const f32 sChaseSpeedMax = 10.0f;
    static const f32 sChaseAccel = 0.5f;
    static const f32 sChaseTurnRate = 4.0f;
    static const f32 sChaseRadius = 1100.0f;
    static const f32 sChargeStartDistance = 100.0f;
    static const s32 sDefaultShootHeight = 1000;
    // sStickTune
    static const s32 sBindOffTiming = 2;
    static const s32 sBindOnTiming = 30;
    static const f32 sDistanceForNeedBack = 2000.0f;
    static const f32 sJudgeForGoBackRadius = 100.0f;
    static const s32 sStepForNonSearch = 120;
    // sStepForShockStop
    static const f32 sShockReactionY = -30.0f;
    static const f32 sDistanceForHipDropShock = 500.0f;
    static const s32 sStepForSwoon = 110;
    static const s32 sStepToShockLand = 12;
    static const s32 sBoundTime = 40;
    static const f32 sBoundAmpMax = 0.4f;
    static const s32 sStepForShake = 20;
    static const f32 sShakeAmpMax = 0.2f;
    static const s32 sStepForHide = 150;
    static const s32 sStepToAppearEffectEnd = 60;
    static const f32 sGravityOffsetY = 20.0f;

    const Vec sNormalBinderPos = {0.0f, 130.0f, 120.0f};
    const Vec sTrampleBinderPos = {0.0f, 150.0f, 0.0f};
};  // namespace

namespace NrvPoihana {
    NEW_NERVE_ONEND(PoihanaNrvNonActive, Poihana, NonActive, NonActive);
    NEW_NERVE(PoihanaNrvWait, Poihana, Wait);
    NEW_NERVE(PoihanaNrvWalkAround, Poihana, WalkAround);
    NEW_NERVE(PoihanaNrvSleepStart, Poihana, SleepStart);
    NEW_NERVE(PoihanaNrvSleep, Poihana, Sleep);
    NEW_NERVE(PoihanaNrvGetUp, Poihana, GetUp);
    NEW_NERVE(PoihanaNrvSearch, Poihana, Search);
    NEW_NERVE(PoihanaNrvChasePlayer, Poihana, ChasePlayer);
    NEW_NERVE(PoihanaNrvShootUpCharge, Poihana, ShootUpCharge);
    NEW_NERVE_ONEND(PoihanaNrvShootUp, Poihana, ShootUp, ShootUp);
    NEW_NERVE(PoihanaNrvGoBack, Poihana, GoBack);
    NEW_NERVE(PoihanaNrvShock, Poihana, Shock);
    NEW_NERVE(PoihanaNrvSwoon, Poihana, Swoon);
    NEW_NERVE(PoihanaNrvSwoonLand, Poihana, SwoonLand);
    NEW_NERVE(PoihanaNrvRecover, Poihana, Recover);
    NEW_NERVE(PoihanaNrvShake, Poihana, Shake);
    NEW_NERVE(PoihanaNrvDrown, Poihana, Drown);
    NEW_NERVE(PoihanaNrvHide, Poihana, Hide);
    NEW_NERVE(PoihanaNrvAppear, Poihana, Appear);
    NEW_NERVE_ONEND(PoihanaNrvDPDSwoon, Poihana, DPDSwoon, DPDSwoon);
};  // namespace NrvPoihana

Poihana::Poihana(const char* pName)
    : LiveActor(pName), mAnimScaleCtrl(), mBindStarPointer(), mCamInfo(), mBindedActor(), mLaunchIntensity(), mRespawnPos(0.0f), mHomePos(0.0f),
      mFrontVec(0.0f, 0.0f, 1.0f), mBoundTimer(-1), mBehavior(POIHANA_BEHAVIOR_NORMAL), mCanDrown(), mWaterColumn(), mIsActive(), _E5() {
}

void Poihana::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::initActorCamera(this, rIter, &mCamInfo);
    initModelManagerWithAnm("Poihana", nullptr, false);

    MR::connectToSceneEnemy(this);

    initHitSensor(2);
    MR::addHitSensorPriorBinder(this, "binder", 8, ::sNormalBinderRadius, ::sNormalBinderPos);
    MR::addHitSensorAtJoint(this, "body", "Body", ATYPE_KILLER_TARGET_ENEMY, 8, 70.0f, TVec3f(0.0f, 0.0f, 0.0f));

    bool useSmallBinder = false;
    MR::getJMapInfoArg7NoInit(rIter, &useSmallBinder);

    if (useSmallBinder) {
        initBinder(100.0f, 100.0f, 0);
    } else {
        initBinder(150.0f, 150.0f, 0);
    }

    mIsActive = true;
    MR::setGroupClipping(this, rIter, 0x20);
    initEffectKeeper(1, nullptr, false);
    initSound(4, false);

    MR::initStarPointerTarget(this, 100.0f, TVec3f(0.0f, 100.0f, 0.0f));
    mAnimScaleCtrl = new AnimScaleController(nullptr);
    mBindStarPointer = new WalkerStateBindStarPointer(this, mAnimScaleCtrl);

    MR::initShadowVolumeSphere(this, 80.0f);
    MR::addToAttributeGroupSearchTurtle(this);

    mRespawnPos.set(mPosition);
    mHomePos.set(mRespawnPos);

    s32 launchIntensity = ::sDefaultShootHeight;
    MR::getJMapInfoArg0NoInit(rIter, &launchIntensity);
    mLaunchIntensity = MR::fastSqrtf(::sGravity * launchIntensity);

    MR::getJMapInfoArg3NoInit(rIter, &mBehavior);
    MR::getJMapInfoArg4NoInit(rIter, &mCanDrown);

    if (mCanDrown) {
        mWaterColumn = MR::createModelObjMapObj("エフェクト水柱", "WaterColumn", getBaseMtx());
        mWaterColumn->mScale.set(2.0f);
        makeActorDead();
    }

    MR::tryCreateMirrorActor(this, "Poihana");

    if (mBehavior == POIHANA_BEHAVIOR_SLEEP) {
        initNerve(GET_NERVE(Poihana, PoihanaNrvSleep));
    } else {
        initNerve(GET_NERVE(Poihana, PoihanaNrvWait));
    }

    makeActorAppeared();
}

void Poihana::initAfterPlacement() {
    TPos3f baseMtx;
    MR::makeMtxUpNoSupportPos(&baseMtx, -mGravity, mPosition);
    MR::setBaseTRMtx(this, baseMtx);
    MR::calcFrontVec(&mFrontVec, this);
    MR::trySetMoveLimitCollision(this);
}

void Poihana::control() {
    if (!isNerve(GET_NERVE(Poihana, PoihanaNrvHide))) {
        mAnimScaleCtrl->updateNerve();
        calcBound();
        controlVelocity();
        calcMyGravity();

        if (!tryDrown() && !tryDPDSwoon() && tryHipDropShock()) {
            return;
        }
    }
}

void Poihana::calcAndSetBaseMtx() {
    TPos3f baseMtx;
    MR::calcMtxFromGravityAndZAxis(&baseMtx, this, mGravity, mFrontVec);

    if (isNerveTypeWalkOrWait()) {
        MR::blendMtx(getBaseMtx(), baseMtx.toMtxPtr(), 0.3f, baseMtx.toMtxPtr());
    }

    MR::setBaseTRMtx(this, baseMtx);

    TVec3f scale = mAnimScaleCtrl->_C * mScale;
    MR::setBaseScale(this, scale);
}

void Poihana::exeNonActive() {
    if (MR::isFirstStep(this)) {
        mVelocity.zero();
        MR::offBind(this);
        MR::offCalcShadow(this, nullptr);
        MR::offCalcAnim(this);

        mIsActive = false;

        MR::invalidateHitSensors(this);
    }

    if (MR::isNearPlayerAnyTime(this, ::sActiveRadius)) {
        if (mBehavior == POIHANA_BEHAVIOR_SLEEP) {
            setNerve(GET_NERVE(Poihana, PoihanaNrvSleep));
        } else {
            setNerve(GET_NERVE(Poihana, PoihanaNrvWait));
        }
    }
}

void Poihana::endNonActive() {
    MR::onBind(this);
    MR::onCalcShadow(this, nullptr);
    MR::onCalcAnim(this);

    mIsActive = true;

    MR::setSensorOffset(this, "binder", ::sNormalBinderPos);
    MR::setSensorRadius(this, "binder", ::sNormalBinderRadius);
    MR::validateHitSensor(this, "binder");
    MR::validateHitSensors(this);
}

void Poihana::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBckNoInterpole(this, "Wait");
        MR::setSensorOffset(this, "binder", ::sNormalBinderPos);
        MR::setSensorRadius(this, "binder", ::sNormalBinderRadius);
        MR::validateHitSensor(this, "binder");
    }

    if (MR::isNearPlayer(this, ::sSearchRadius)) {
        setNerve(GET_NERVE(Poihana, PoihanaNrvSearch));
    } else if (MR::isGreaterStep(this, ::sWalkAroundBreakTime)) {
        setNerve(GET_NERVE(Poihana, PoihanaNrvWalkAround));
    } else {
        tryNonActive();
    }
}

void Poihana::exeWalkAround() {
    if (MR::isFirstStep(this)) {
        if (!MR::isBckPlaying(this, "Walk")) {
            MR::startBck(this, "Walk");
        }

        mRandDir = MR::getRandom(-::sWalkAroundTurn, ::sWalkAroundTurn);
    }

    MR::rotateVecDegree(&mFrontVec, mGravity, mRandDir);
    mVelocity.scaleAdd(::sWalkAccel, mFrontVec, mVelocity);

    if (isNeedForBackHome()) {
        setNerve(GET_NERVE(Poihana, PoihanaNrvGoBack));
    } else if (MR::isNearPlayer(this, ::sSearchRadius)) {
        setNerve(GET_NERVE(Poihana, PoihanaNrvSearch));
    } else if (MR::isGreaterStep(this, ::sWalkAroundKeepTime)) {
        if (mBehavior == POIHANA_BEHAVIOR_SLEEP) {
            setNerve(GET_NERVE(Poihana, PoihanaNrvSleepStart));
        } else {
            setNerve(GET_NERVE(Poihana, PoihanaNrvWait));
        }
    }
}

void Poihana::exeSleepStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "SleepStart");
        MR::startSound(this, "SE_EV_POIHANA_SLEEP_START");
        MR::invalidateHitSensor(this, "binder");
    }

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(Poihana, PoihanaNrvSleep));
    }
}

void Poihana::exeSleep() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Sleep");
        MR::invalidateHitSensor(this, "binder");
    }

    if (isNeedForGetUp()) {
        setNerve(GET_NERVE(Poihana, PoihanaNrvGetUp));
    } else {
        tryNonActive();
    }
}

void Poihana::exeGetUp() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "GetUp");
        MR::startSound(this, "SE_EV_POIHANA_WAKEUP");
        MR::startSound(this, "SE_EM_POIHANA_WAKEUP");
    }

    if (MR::isBckStopped(this)) {
        MR::setSensorOffset(this, "binder", ::sNormalBinderPos);
        MR::setSensorRadius(this, "binder", ::sNormalBinderRadius);
        MR::validateHitSensor(this, "binder");
        setNerve(GET_NERVE(Poihana, PoihanaNrvWait));
    }
}

void Poihana::exeSearch() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Search");
        MR::startSound(this, "SE_EV_POIHANA_FIND");
    }

    MR::turnDirectionToTargetUseGroundNormalDegree(this, &mFrontVec, *MR::getPlayerPos(), ::sChaseTurnRate);

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(Poihana, PoihanaNrvChasePlayer));
    }
}

void Poihana::exeChasePlayer() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Run");
        MR::setSensorOffset(this, "binder", ::sNormalBinderPos);
        MR::setSensorRadius(this, "binder", ::sNormalBinderRadius);
        MR::validateHitSensor(this, "binder");
    }

    MR::turnDirectionToTargetUseGroundNormalDegree(this, &mFrontVec, *MR::getPlayerPos(), ::sChaseTurnRate);
    mVelocity.scaleAdd(::sChaseAccel, mFrontVec, mVelocity);

    if (isNeedForBackHome()) {
        setNerve(GET_NERVE(Poihana, PoihanaNrvGoBack));
    }
}

void Poihana::exeShootUpCharge() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "ThrowStart");
    }

    TVec3f& rGravity = mGravity;
    f32 dot = rGravity.dot(mVelocity);
    mVelocity.scale(dot, rGravity);

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(Poihana, PoihanaNrvShootUp));
    }
}

void Poihana::exeShootUp() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Throw");
        MR::startSound(this, "SE_EV_POIHANA_SHOOT_UP");
        MR::startActorCameraNoTarget(this, mCamInfo, -1);
    }

    TVec3f& rGravity = mGravity;
    f32 dot = rGravity.dot(mVelocity);
    mVelocity.scale(dot, rGravity);

    if (MR::isStep(this, ::sBindOffTiming)) {
        endBind();
        MR::invalidateHitSensor(this, "binder");
    }

    if (MR::isStep(this, ::sBindOnTiming)) {
        MR::setSensorOffset(this, "binder", ::sNormalBinderPos);
        MR::setSensorRadius(this, "binder", ::sNormalBinderRadius);
        MR::validateHitSensor(this, "binder");
    }

    if (MR::isBckStopped(this)) {
        if (mBehavior == POIHANA_BEHAVIOR_NEW_HOME) {
            mHomePos = mPosition;
            setNerve(GET_NERVE(Poihana, PoihanaNrvWalkAround));
        } else {
            setNerve(GET_NERVE(Poihana, PoihanaNrvGoBack));
        }
    }
}

void Poihana::endShootUp() NO_INLINE {
    endBind();

    MR::setSensorOffset(this, "binder", ::sNormalBinderPos);
    MR::setSensorRadius(this, "binder", ::sNormalBinderRadius);
    MR::validateHitSensor(this, "binder");

    MR::endActorCamera(this, mCamInfo, true, -1);
}

void Poihana::exeGoBack() {
    if (MR::isFirstStep(this) && !MR::isBckPlaying(this, "Walk")) {
        MR::startBck(this, "Walk");
    }

    MR::turnDirectionToTargetUseGroundNormalDegree(this, &mFrontVec, mHomePos, ::sWalkTurnRate);
    mVelocity.scaleAdd(::sWalkAccel, mFrontVec, mVelocity);

    if (MR::isNearPlayer(this, ::sSearchRadius) && MR::isGreaterStep(this, ::sStepForNonSearch)) {
        setNerve(GET_NERVE(Poihana, PoihanaNrvSearch));
    } else if (MR::isNear(this, mHomePos, ::sJudgeForGoBackRadius)) {
        setNerve(GET_NERVE(Poihana, PoihanaNrvWait));
    }
}

void Poihana::exeShock() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "PunchDamage");
        MR::startBlowHitSound(this);
        MR::invalidateHitSensor(this, "binder");
    }

    if (MR::isOnGround(this) && MR::isGreaterStep(this, ::sStepToShockLand)) {
        setNerve(GET_NERVE(Poihana, PoihanaNrvSwoonLand));
    }
}

void Poihana::exeSwoonLand() {
    if (MR::isFirstStep(this)) {
        MR::startBckNoInterpole(this, "SwoonLand");
        MR::startSound(this, "SE_EV_POIHANA_SWOON");
    }

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(Poihana, PoihanaNrvSwoon));
    }
}

void Poihana::exeSwoon() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Swoon");
    }

    MR::startLevelSound(this, "SE_EM_LV_SWOON_S");

    if (MR::isStep(this, ::sStepForSwoon)) {
        setNerve(GET_NERVE(Poihana, PoihanaNrvRecover));
    }
}

void Poihana::exeRecover() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Recover");
        MR::startSound(this, "SE_EM_POIHANA_RECOVER");
        MR::startSound(this, "SE_EV_POIHANA_RECOVER");
        mScale.set(1.0f);
    }

    if (MR::isBckStopped(this)) {
        MR::setSensorOffset(this, "binder", ::sNormalBinderPos);
        MR::setSensorRadius(this, "binder", ::sNormalBinderRadius);
        MR::validateHitSensor(this, "binder");
        setNerve(GET_NERVE(Poihana, PoihanaNrvWait));
    }
}

void Poihana::exeShake() {
    f32 amplitude = ::sShakeAmpMax - 0.01f * getNerveStep();
    f32 scale = MR::sinDegree(getNerveStep() * 36.0f) * amplitude + 1.0f;
    mScale.set(scale);

    if (MR::isStep(this, ::sStepForShake)) {
        mScale.set(1.0f);
        setNerve(GET_NERVE(Poihana, PoihanaNrvSearch));
    }
}

void Poihana::exeDrown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Drown");
        MR::invalidateClipping(this);
        MR::invalidateHitSensors(this);
        MR::startSound(this, "SE_EM_FALL_INTO_WATER_S");

        mWaterColumn->appear();
        MR::tryStartAllAnim(mWaterColumn, "WaterColumn");
    }

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(Poihana, PoihanaNrvHide));
    }
}

void Poihana::exeHide() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_EM_EXPLODE_S_WATER");
        MR::emitEffect(this, "DeathWater");
        MR::hideModel(this);
    }

    if (MR::isStep(this, ::sStepForHide)) {
        setNerve(GET_NERVE(Poihana, PoihanaNrvAppear));
    }
}

void Poihana::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::resetPosition(this, mRespawnPos);
        MR::emitEffect(this, "Appear");
    }

    if (MR::isLessStep(this, ::sStepToAppearEffectEnd)) {
        MR::startLevelSound(this, "SE_EM_LV_POIHANA_REVIVE_EFFECT");
    }

    if (MR::isStep(this, ::sStepToAppearEffectEnd)) {
        MR::showModel(this);
        MR::startBck(this, "Appear");
        MR::startSound(this, "SE_EM_POIHANA_REVIVE_APPEAR");
    } else if (MR::isBckStopped(this) && MR::isGreaterStep(this, ::sStepToAppearEffectEnd)) {
        MR::validateClipping(this);
        MR::validateHitSensors(this);
        MR::setSensorOffset(this, "binder", ::sNormalBinderPos);
        MR::setSensorRadius(this, "binder", ::sNormalBinderRadius);
        MR::validateHitSensor(this, "binder");
        setNerve(GET_NERVE(Poihana, PoihanaNrvWait));
    }
}

void Poihana::exeDPDSwoon() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensor(this, "binder");
    }

    MR::updateActorStateAndNextNerve(this, mBindStarPointer, GET_NERVE(Poihana, PoihanaNrvWait));
}

void Poihana::endDPDSwoon() NO_INLINE {
    mBindStarPointer->kill();

    MR::setSensorOffset(this, "binder", ::sNormalBinderPos);
    MR::setSensorRadius(this, "binder", ::sNormalBinderRadius);
    MR::validateHitSensor(this, "binder");
}

void Poihana::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    bool ret = MR::isSensorPlayer(pReceiver) || MR::isSensorEnemy(pReceiver) || MR::isSensorMapObj(pReceiver);

    if (!ret) {
        return;
    }

    if (!MR::isSensorEnemy(pSender)) {
        return;
    }

    if (MR::isSensorPlayer(pReceiver)) {
        contactMario(pSender, pReceiver);
    }

    if (!MR::sendMsgPush(pReceiver, pSender)) {
        return;
    }

    if (MR::isSensorPlayer(pReceiver)) {
        ret = isNerve(GET_NERVE(Poihana, PoihanaNrvShock)) || isNerve(GET_NERVE(Poihana, PoihanaNrvSwoonLand)) ||
              isNerve(GET_NERVE(Poihana, PoihanaNrvSwoon)) || isNerve(GET_NERVE(Poihana, PoihanaNrvRecover));

        if (ret) {
            return;
        }
    }

    TVec3f pushVelocity;
    pushVelocity.sub(mPosition, pReceiver->mHost->mPosition);

    MR::normalizeOrZero(&pushVelocity);

    if (mVelocity.dot(pushVelocity) < 0.0f) {
        const TVec3f& rVelocity = mVelocity;
        mVelocity.scaleAdd(-pushVelocity.dot(rVelocity), pushVelocity, rVelocity);
    }
}

bool Poihana::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceAttack(msg)) {
        return true;
    }

    if (MR::isMsgPlayerTrample(msg) || MR::isMsgPlayerHipDrop(msg)) {
        bool flag = isNerve(GET_NERVE(Poihana, PoihanaNrvShock)) || isNerve(GET_NERVE(Poihana, PoihanaNrvSwoonLand)) ||
                    isNerve(GET_NERVE(Poihana, PoihanaNrvSwoon)) || isNerve(GET_NERVE(Poihana, PoihanaNrvRecover));

        if (flag) {
            goto here;
        }

        flag = isNerve(GET_NERVE(Poihana, PoihanaNrvSleepStart)) || isNerve(GET_NERVE(Poihana, PoihanaNrvSleep)) ||
               isNerve(GET_NERVE(Poihana, PoihanaNrvGetUp));

        if (flag) {
        here:
            startBound();
            MR::startSound(this, "SE_EV_POIHANA_TRAMPLE");

            if (MR::isMsgPlayerHipDrop(msg)) {
                MR::sendMsgAwayJump(pSender, pReceiver);
            }

            return true;
        }

        MR::setSensorOffset(this, "binder", ::sTrampleBinderPos);
        MR::setSensorRadius(this, "binder", ::sTrampleBinderRadius);
        MR::validateHitSensor(this, "binder");
    }

    if (MR::isMsgPlayerHitAll(msg) && tryShock()) {
        MR::stopSceneForDefaultHit(3);
        return true;
    }

    return false;
}

bool Poihana::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgExplosionAttack(msg) && tryShock()) {
        return true;
    }

    return false;
}

bool Poihana::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorEnemy(pSender) || MR::isSensorMapObj(pSender)) {
        TVec3f pushOffset;
        pushOffset.sub(mPosition, pSender->mHost->mPosition);
        MR::normalizeOrZero(&pushOffset);
        mVelocity.scaleAdd(::sReceivePushPower, pushOffset, mVelocity);

        return true;
    }

    return false;
}

bool Poihana::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgAutoRushBegin(msg) && MR::isSensorPlayer(pSender)) {
        if (isNerve(GET_NERVE(Poihana, PoihanaNrvShootUpCharge))) {
            return false;
        }

        if (!MR::isOnGroundPlayer()) {
            if (!tryToStartBind(pSender)) {
                return false;
            }

            setNerve(GET_NERVE(Poihana, PoihanaNrvShootUp));
            return true;
        }

        if (isNerve(GET_NERVE(Poihana, PoihanaNrvShootUp))) {
            if (getNerveStep() >= ::sBindOnTiming) {
                return false;
            }

            return tryToStartBind(pSender);
        }

        if (!isNerve(GET_NERVE(Poihana, PoihanaNrvShootUpCharge)) && MR::isNear(pSender, pReceiver, ::sChargeStartDistance)) {
            setNerve(GET_NERVE(Poihana, PoihanaNrvShootUpCharge));
            return false;
        }

        return false;
    } else if (msg == ACTMES_UPDATE_BASEMTX && mBindedActor != nullptr) {
        updateBindActorMtx();
        return true;
    }

    return false;
}

bool Poihana::tryToStartBind(HitSensor* pSender) {
    if (mBindedActor != nullptr) {
        return false;
    }

    LiveActor* pBindedActor = pSender->mHost;
    if (MR::isInWater(pBindedActor, TVec3f(0.0f, 0.0f, 0.0f))) {
        return false;
    }

    MR::tryRumblePadMiddle(this, WPAD_CHAN0);
    mBindedActor = pSender->mHost;
    MR::startBckPlayer("Rise");
    MR::invalidateClipping(this);
    return true;
}

void Poihana::updateBindActorMtx() NO_INLINE {
    TPos3f binderMtx;
    MR::makeMtxTR(binderMtx.toMtxPtr(), mBindedActor);
    MR::setBaseTRMtx(mBindedActor, binderMtx);
}

void Poihana::endBind() {
    MR::validateClipping(this);

    if (MR::isPlayerInRush()) {
        TVec3f jumpVec;
        jumpVec.scale(-mLaunchIntensity, mGravity);
        MR::endBindAndPlayerJump(this, jumpVec, 0);
        MR::startSound(this, "SE_PM_HELI_JUMP");
    }

    mBindedActor = nullptr;
}

void Poihana::startBound() {
    mBoundTimer = 0;
    mScale.x = 1.0f;
    mScale.y = 1.0f;
    mScale.z = 1.0f;
}

void Poihana::calcBound() {
    if (mBoundTimer == -1) {
        return;
    }

    if (mBoundTimer >= ::sBoundTime) {
        mScale.set(1.0f, 1.0f, 1.0f);
        mBoundTimer = -1;
        return;
    }

    mBoundTimer++;

    f32 amplitude = ::sBoundAmpMax - mBoundTimer * 0.01f;

    f32 scaleX = amplitude * 0.9f * MR::sinDegree(mBoundTimer * 9.0f) + 1.0f;
    f32 scaleY = amplitude * MR::sinDegree(mBoundTimer * 27.0f) + 1.0f;
    f32 scaleZ = amplitude * 0.9f * MR::sinDegree(mBoundTimer * 18.0f) + 1.0f;
    mScale.set(scaleX, scaleY, scaleZ);
}

void Poihana::contactMario(HitSensor* pSender, HitSensor* pReceiver) {
    bool isShooting;

    if (!isNerveTypeWalkOrWait()) {
        isShooting = isNerve(GET_NERVE(Poihana, PoihanaNrvShootUpCharge)) || isNerve(GET_NERVE(Poihana, PoihanaNrvShootUp));
    } else {
        goto doFlip;
    }

    if (isShooting) {
    doFlip:
        if (!isBackAttack(pReceiver)) {
            return;
        }

        if (MR::sendMsgEnemyAttackFlipWeak(pReceiver, pSender)) {
            setNerve(GET_NERVE(Poihana, PoihanaNrvShake));
        }
    } else if (isNerve(GET_NERVE(Poihana, PoihanaNrvSleep))) {
        setNerve(GET_NERVE(Poihana, PoihanaNrvGetUp));
    }
}

void Poihana::controlVelocity() {
    if (isNerve(GET_NERVE(Poihana, PoihanaNrvNonActive))) {
        return;
    }

    TVec3f gravity = MR::isBindedGround(this) ? -*MR::getGroundNormal(this) : mGravity;

    TVec3f frontVec(mFrontVec);
    MR::turnVecToPlane(&mFrontVec, frontVec, gravity);

    if (MR::isBindedGround(this)) {
        TVec3f addVel;
        const TVec3f& rVelocity = mVelocity;
        addVel.scaleAdd(-mFrontVec.dot(rVelocity), mFrontVec, rVelocity);
        addVel.scale(::sVelocityDownRateSide);

        mVelocity.scale(mFrontVec.dot(mVelocity), mFrontVec);
        mVelocity.add(addVel);

        if (mVelocity.dot(gravity) > 0.0f) {
            const TVec3f& rVelocity = mVelocity;
            mVelocity.scaleAdd(-gravity.dot(rVelocity), gravity, rVelocity);
        }

        mVelocity.mult(::sVelocityDownRate);
    }

    mVelocity.scaleAdd(::sGravity, gravity, mVelocity);

    if (!isNerve(GET_NERVE(Poihana, PoihanaNrvShock))) {
        f32 magVel = isNerve(GET_NERVE(Poihana, PoihanaNrvChasePlayer)) ? ::sChaseSpeedMax : ::sWalkSpeedMax;

        if (mVelocity.length() > magVel) {
            mVelocity.setLength(magVel);
        }

        if (MR::isNearZero(mVelocity)) {
            mVelocity.zero();
        }
    }
}

void Poihana::calcMyGravity() {
    if (!mIsActive) {
        return;
    }

    TVec3f upVec, gravityPos;
    MR::calcUpVec(&upVec, this);
    gravityPos.scaleAdd(::sGravityOffsetY, upVec, mPosition);
    MR::calcGravity(this, gravityPos);
}

bool Poihana::tryNonActive() {
    if (!MR::isStageStateScenarioOpeningCamera()) {
        bool notNear = !MR::isNearPlayerAnyTime(this, ::sActiveRadius);
        if (notNear && MR::isBindedGround(this)) {
            setNerve(GET_NERVE(Poihana, PoihanaNrvNonActive));
            return true;
        }
    }

    return false;
}

bool Poihana::tryDrown() {
    if (!mCanDrown) {
        return false;
    }

    if (isNerve(GET_NERVE(Poihana, PoihanaNrvAppear))) {
        return false;
    }

    if (isNerve(GET_NERVE(Poihana, PoihanaNrvDrown))) {
        return false;
    }

    if (!MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
        return false;
    }

    setNerve(GET_NERVE(Poihana, PoihanaNrvDrown));
    return true;
}

bool Poihana::tryDPDSwoon() {
    if (!isNerveTypeWalkOrWait()) {
        return false;
    }

    if (!mBindStarPointer->tryStartPointBind()) {
        return false;
    }

    setNerve(GET_NERVE(Poihana, PoihanaNrvDPDSwoon));
    return true;
}

bool Poihana::tryShock() {
    bool ret =
        isNerve(GET_NERVE(Poihana, PoihanaNrvDrown)) || isNerve(GET_NERVE(Poihana, PoihanaNrvHide)) || isNerve(GET_NERVE(Poihana, PoihanaNrvAppear));

    if (ret) {
        return false;
    }

    if (isNerve(GET_NERVE(Poihana, PoihanaNrvShock))) {
        return false;
    }

    if (isNerve(GET_NERVE(Poihana, PoihanaNrvSwoonLand))) {
        return false;
    }

    mVelocity.scale(::sShockReactionY, mGravity);
    setNerve(GET_NERVE(Poihana, PoihanaNrvShock));
    return true;
}

bool Poihana::tryHipDropShock() {
    bool isNotNear = !MR::isNearPlayerAnyTime(this, ::sDistanceForHipDropShock);

    if (isNotNear) {
        return false;
    }

    if (MR::isPlayerHipDropLand()) {
        return tryShock();
    }

    return false;
}

bool Poihana::isNerveTypeWalkOrWait() const {
    return isNerve(GET_NERVE(Poihana, PoihanaNrvWait)) || isNerve(GET_NERVE(Poihana, PoihanaNrvSearch)) ||
           isNerve(GET_NERVE(Poihana, PoihanaNrvWalkAround)) || isNerve(GET_NERVE(Poihana, PoihanaNrvChasePlayer)) ||
           isNerve(GET_NERVE(Poihana, PoihanaNrvGoBack));
}

bool Poihana::isNeedForBackHome() const {
    if (isNerve(GET_NERVE(Poihana, PoihanaNrvWalkAround))) {
        return !MR::isNear(this, mHomePos, ::sWalkAroundRadius);
    }

    if (isNerve(GET_NERVE(Poihana, PoihanaNrvChasePlayer))) {
        bool ret;

        if (mBehavior == POIHANA_BEHAVIOR_NEW_HOME) {
            ret = !MR::isNearPlayer(this, ::sChaseRadius);
        } else {
            ret = !MR::isNear(this, mHomePos, ::sDistanceForNeedBack) || !MR::isNearPlayer(this, ::sChaseRadius);
        }

        return ret;
    }

    return false;
}

bool Poihana::isNeedForGetUp() const {
    bool ret = false;

    if (MR::isNearPlayer(this, ::sGetUpRadius)) {
        bool flag = true;

        f32 mag = MR::getPlayerVelocity()->length();

        if (!(mag >= ::sJudgeForGetUpSpeed) && !MR::isPlayerSwingAction()) {
            flag = false;
        }

        if (flag) {
            ret = true;
        }
    }

    return ret;
}

bool Poihana::isBackAttack(HitSensor* pMySensor) const NO_INLINE {
    TVec3f frontVec;
    MR::calcFrontVec(&frontVec, this);
    frontVec.negate();

    TVec3f offset;
    offset.sub(pMySensor->mPosition, mPosition);
    return offset.dot(frontVec) > 0.0f;
}

Poihana::~Poihana() {
}
