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
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <JSystem/JMath/JMath.hpp>

#define POIHANA_BEHAVIOR_NORMAL 0
#define POIHANA_BEHAVIOR_SLEEP 1
#define POIHANA_BEHAVIOR_NEW_HOME 2

namespace {
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

Poihana::Poihana(const char* pName) : LiveActor(pName) {
    mAnimScaleCtrl = nullptr;
    mBindStarPointer = nullptr;
    mCamInfo = nullptr;
    mBindedActor = nullptr;
    mLaunchIntensity = 0.0f;
    mRespawnPos.set(0.0f);
    _AC = 0.0f;
    _B0 = 0.0f;
    mHomePos.set(0.0f);
    _C0 = 1.0f;
    mBoundTimer = -1;
    mBehavior = POIHANA_BEHAVIOR_NORMAL;
    mCanDrown = false;
    mWaterColumn = nullptr;
    mIsActive = false;
    _E5 = 0;
}

Poihana::~Poihana() {
}

/*void Poihana::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    MR::initActorCamera(this, rIter, &mCamInfo);
    initModelManagerWithAnm("Poihana", nullptr, false);

    MR::connectToSceneEnemy(this);

    // Initialize sensors
    initHitSensor(2);
    MR::addHitSensorPriorBinder(this, "binder", 8, 125.0f, ::sNormalBinderPos);
    MR::addHitSensorAtJoint(this, "body", "Body", ATYPE_KILLER_TARGET_ENEMY, 8, 70.0f, TVec3f(0.0f, 0.0f, 0.0f));

    // Initialize binder
    bool useSmallBinder = false;
    MR::getJMapInfoArg7NoInit(rIter, &useSmallBinder);

    if (useSmallBinder) {
        initBinder(100.0f, 100.0f, 0);
    }
    else {
        initBinder(150.0f, 150.0f, 0);
    }

    mIsActive = true;
    MR::setGroupClipping(this, rIter, 0x20);
    initEffectKeeper(1, nullptr, false);
    initSound(4, false);

    // Initialize 2P behavior
    MR::initStarPointerTarget(this, 150.0f, TVec3f(0.0f, 100.0f, 0.0f));
    mAnimScaleCtrl = new AnimScaleController(nullptr);
    mBindStarPointer = new WalkerStateBindStarPointer(this, mAnimScaleCtrl);

    MR::initShadowVolumeSphere(this, 80.0f);
    MR::addToAttributeGroupSearchTurtle(this);

    mRespawnPos.set(mPosition);
    mHomePos.set(mRespawnPos);

    // Calculate launch intensity, thanks to Shibbo for helping me on that one
    s32 launchIntensity = 1000;
    MR::getJMapInfoArg0NoInit(rIter, &launchIntensity);
    f32 launchIntensityF = 2.0f * launchIntensity;

    if (launchIntensityF > 0) {
        f32 factor;
        f32 reciprocal = __frsqrte(launchIntensityF);
        factor = reciprocal * launchIntensityF;
        launchIntensityF = -(factor * reciprocal - 3.0f) * factor * 0.5f;
    }

    mLaunchIntensity = -launchIntensityF;

    // Setup color
    //s32 color = 0;
    //MR::getJMapInfoArg2NoInit(rIter, &color);
    //MR::startBtpAndSetFrameAndStop(this, "ColorChange", color);

    // Setup behaviors
    //MR::getJMapInfoArg1NoInit(rIter, &mActiveRange);
    MR::getJMapInfoArg3NoInit(rIter, &mBehavior);
    MR::getJMapInfoArg4NoInit(rIter, &mCanDrown);

    if (mCanDrown) {
        mWaterColumn = MR::createModelObjMapObj("エフェクト水柱", "WaterColumn", (MtxPtr)getBaseMtx());
        mWaterColumn->mScale.setAll(2.0f);
    }

    MR::tryCreateMirrorActor(this, nullptr);
    //MR::useStageSwitchAwake(this, rIter);

    if (mBehavior == POIHANA_BEHAVIOR_SLEEP) {
        initNerve(GET_NERVE(Poihana, PoihanaNrvSleep));
    }
    else {
        initNerve(GET_NERVE(Poihana, PoihanaNrvWait));
    }

    makeActorAppeared();
}*/

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
    MR::calcMtxFromGravityAndZAxis(&baseMtx, this, mGravity, mHomePos);

    if (isNerveTypeWalkOrWait()) {
        MR::blendMtx((MtxPtr)getBaseMtx(), (MtxPtr)&baseMtx, 0.3f, (MtxPtr)&baseMtx);
    }

    MR::setBaseTRMtx(this, baseMtx);

    TVec3f scale = mAnimScaleCtrl->_C * mScale;
    MR::setBaseScale(this, scale);
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

    TVec3f pushVelocity = mPosition - pReceiver->mHost->mPosition;

    MR::normalizeOrZero(&pushVelocity);

    if (mVelocity.dot(pushVelocity) < 0.0f) {
        const TVec3f& vel = mVelocity;
        mVelocity.scaleAdd(-pushVelocity.dot(vel), pushVelocity, vel);
    }
}

bool Poihana::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorEnemy(pSender) || MR::isSensorMapObj(pSender)) {
        TVec3f pushOffset(mPosition - pSender->mHost->mPosition);
        MR::normalizeOrZero(&pushOffset);
        mVelocity.scaleAdd(1.5f, pushOffset, mVelocity);

        return true;
    }

    return false;
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
        MR::setSensorRadius(this, "binder", 125.0f);
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

bool Poihana::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgAutoRushBegin(msg) && MR::isSensorPlayer(pSender)) {
        if (isNerve(GET_NERVE(Poihana, PoihanaNrvShootUpCharge))) {
            return false;
        } else if (MR::isOnGroundPlayer()) {
            if (isNerve(GET_NERVE(Poihana, PoihanaNrvShootUp))) {
                if (getNerveStep() < 30) {
                    if (!tryToStartBind(pSender)) {
                        return false;
                    }
                }
            } else if (isNerve(GET_NERVE(Poihana, PoihanaNrvShootUpCharge)) || !MR::isNear(pSender, pReceiver, 100.0f)) {
                return false;
            } else {
                setNerve(GET_NERVE(Poihana, PoihanaNrvShootUpCharge));
            }
        } else if (tryToStartBind(pSender)) {
            setNerve(GET_NERVE(Poihana, PoihanaNrvShootUp));
            return true;
        }
    } else if (MR::isMsgUpdateBaseMtx(msg) && mBindedActor != nullptr) {
        updateBindActorMtx();
        return true;
    }

    return false;
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

    if (MR::isNearPlayerAnyTime(this, 3500.0f)) {
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
    MR::setSensorRadius(this, "binder", 125.0f);
    MR::validateHitSensor(this, "binder");
    MR::validateHitSensors(this);
}

void Poihana::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBckNoInterpole(this, "Wait");
        MR::setSensorOffset(this, "binder", ::sNormalBinderPos);
        MR::setSensorRadius(this, "binder", 125.0f);
        MR::validateHitSensor(this, "binder");
    }

    if (MR::isNearPlayer(this, 800.0f)) {
        setNerve(GET_NERVE(Poihana, PoihanaNrvSearch));
    } else if (MR::isGreaterStep(this, 180)) {
        setNerve(GET_NERVE(Poihana, PoihanaNrvWalkAround));
    } else {
        tryNonActive();
    }
}

void Poihana::exeWalkAround() {
    if (MR::isFirstStep(this)) {
        if (!MR::isBckPlaying(this, "Walk")) {
            MR::startBck(this, "Walk", nullptr);
        }

        mRandDir = MR::getRandom((s32)-2, (s32)2);
    }

    MR::rotateVecDegree(&mFrontVec, mGravity, mRandDir);
    mVelocity.scaleAdd(0.5f, mFrontVec, mVelocity);

    if (isNeedForBackHome()) {
        setNerve(GET_NERVE(Poihana, PoihanaNrvGoBack));
    } else if (MR::isNearPlayer(this, 800.0f)) {
        setNerve(GET_NERVE(Poihana, PoihanaNrvSearch));
    } else if (MR::isGreaterStep(this, 120)) {
        if (mBehavior == POIHANA_BEHAVIOR_SLEEP) {
            setNerve(GET_NERVE(Poihana, PoihanaNrvSleepStart));
        } else {
            setNerve(GET_NERVE(Poihana, PoihanaNrvWait));
        }
    }
}

void Poihana::exeSleepStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "SleepStart", nullptr);
        MR::startSound(this, "SE_EV_POIHANA_SLEEP_START");
        MR::invalidateHitSensor(this, "binder");
    }

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(Poihana, PoihanaNrvSleep));
    }
}

void Poihana::exeSleep() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Sleep", nullptr);
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
        MR::startBck(this, "GetUp", nullptr);
        MR::startSound(this, "SE_EV_POIHANA_WAKEUP");
        MR::startSound(this, "SE_EM_POIHANA_WAKEUP");
    }

    if (MR::isBckStopped(this)) {
        MR::setSensorOffset(this, "binder", ::sNormalBinderPos);
        MR::setSensorRadius(this, "binder", 125.0f);
        MR::validateHitSensor(this, "binder");
        setNerve(GET_NERVE(Poihana, PoihanaNrvWait));
    }
}

void Poihana::exeSearch() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Search", nullptr);
        MR::startSound(this, "SE_EV_POIHANA_FIND");
    }

    MR::turnDirectionToTargetUseGroundNormalDegree(this, &mFrontVec, *MR::getPlayerPos(), 4.0f);

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(Poihana, PoihanaNrvChasePlayer));
    }
}

void Poihana::exeChasePlayer() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Run", nullptr);
        MR::setSensorOffset(this, "binder", ::sNormalBinderPos);
        MR::setSensorRadius(this, "binder", 125.0f);
        MR::validateHitSensor(this, "binder");
    }

    MR::turnDirectionToTargetUseGroundNormalDegree(this, &mFrontVec, *MR::getPlayerPos(), 4.0f);
    mVelocity.scaleAdd(0.5f, mFrontVec, mVelocity);

    if (isNeedForBackHome()) {
        setNerve(GET_NERVE(Poihana, PoihanaNrvGoBack));
    }
}

void Poihana::exeShootUpCharge() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "ThrowStart", nullptr);
    }

    TVec3f& gravity = mGravity;
    f32 dot = gravity.dot(mVelocity);
    mVelocity.scale(dot, gravity);

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(Poihana, PoihanaNrvShootUp));
    }
}

/*void Poihana::exeShootUp() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Throw", nullptr);
        MR::startSound(this, "SE_EV_POIHANA_SHOOT_UP");
        MR::startActorCameraNoTarget(this, mCamInfo, -1);
    }

    f32 dot = mGravity.dot(mVelocity);
    mVelocity.scale(dot, mGravity);

    if (MR::isStep(this, 2)) {
        endBind();
        MR::invalidateHitSensor(this, "binder");
    }

    if (MR::isStep(this, 30)) {
        MR::setSensorOffset(this, "binder", ::sNormalBinderPos);
        MR::setSensorRadius(this, "binder", 125.0f);
        MR::validateHitSensor(this, "binder");
    }

    if (MR::isBckStopped(this)) {
        if (mBehavior == POIHANA_BEHAVIOR_NEW_HOME) {
            mHomePos.set(mPosition);
            setNerve(GET_NERVE(Poihana, PoihanaNrvWalkAround));
        }
        else {
            setNerve(GET_NERVE(Poihana, PoihanaNrvGoBack));
        }
    }
}*/

void Poihana::endShootUp() {
    endBind();

    MR::setSensorOffset(this, "binder", ::sNormalBinderPos);
    MR::setSensorRadius(this, "binder", 125.0f);
    MR::validateHitSensor(this, "binder");

    MR::endActorCamera(this, mCamInfo, true, -1);
}

void Poihana::exeGoBack() {
    if (MR::isFirstStep(this) && !MR::isBckPlaying(this, "Walk")) {
        MR::startBck(this, "Walk", nullptr);
    }

    MR::turnDirectionToTargetUseGroundNormalDegree(this, &mFrontVec, mHomePos, 2.0f);
    mVelocity.scaleAdd(0.5f, mFrontVec, mVelocity);

    if (MR::isNearPlayer(this, 800.0f) && MR::isGreaterStep(this, 120)) {
        setNerve(GET_NERVE(Poihana, PoihanaNrvSearch));
    } else if (MR::isNear(this, mHomePos, 100.0f)) {
        setNerve(GET_NERVE(Poihana, PoihanaNrvWait));
    }
}

void Poihana::exeShock() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "PunchDamage", nullptr);
        MR::startBlowHitSound(this);
        MR::invalidateHitSensor(this, "binder");
    }

    if (MR::isOnGround(this) && MR::isGreaterStep(this, 12)) {
        setNerve(GET_NERVE(Poihana, PoihanaNrvSwoon));
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
        MR::startBck(this, "Swoon", nullptr);
    }

    MR::startLevelSound(this, "SE_EM_LV_SWOON_S");

    if (MR::isStep(this, 110)) {
        setNerve(GET_NERVE(Poihana, PoihanaNrvRecover));
    }
}

void Poihana::exeRecover() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Recover", nullptr);
        MR::startSound(this, "SE_EM_POIHANA_RECOVER");
        MR::startSound(this, "SE_EV_POIHANA_RECOVER");
        mScale.set(1.0f);
    }

    if (MR::isBckStopped(this)) {
        MR::setSensorOffset(this, "binder", ::sNormalBinderPos);
        MR::setSensorRadius(this, "binder", 125.0f);
        MR::validateHitSensor(this, "binder");
        setNerve(GET_NERVE(Poihana, PoihanaNrvWait));
    }
}

// Needs to be reviewed
void Poihana::exeShake() {
    f32 _f31 = 0.2f - 0.01f * getNerveStep();
    f32 scale = MR::sinDegree(getNerveStep() * 0.01745329251f) * _f31 + 36.0f;
    mScale.set(scale);

    if (MR::isStep(this, 20)) {
        mScale.set(1.0f);
        setNerve(GET_NERVE(Poihana, PoihanaNrvSearch));
    }
}

void Poihana::exeDrown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Drown", nullptr);
        MR::invalidateClipping(this);
        MR::invalidateHitSensors(this);
        MR::startSound(this, "SE_EM_FALL_INTO_WATER_S");

        mWaterColumn->appear();
        MR::tryStartAllAnim(mWaterColumn, "Splash");
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

    if (MR::isStep(this, 150)) {
        setNerve(GET_NERVE(Poihana, PoihanaNrvAppear));
    }
}

void Poihana::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::resetPosition(this, mRespawnPos);
        MR::emitEffect(this, "Appear");
    }

    if (MR::isLessStep(this, 60)) {
        MR::startLevelSound(this, "SE_EM_LV_POIHANA_REVIVE_EFFECT");
    }

    if (MR::isStep(this, 60)) {
        MR::showModel(this);
        MR::startBck(this, "Appear", nullptr);
        MR::startSound(this, "SE_EM_POIHANA_REVIVE_APPEAR");
    } else if (MR::isBckStopped(this) && MR::isGreaterStep(this, 60)) {
        MR::validateClipping(this);
        MR::validateHitSensors(this);
        MR::setSensorOffset(this, "binder", ::sNormalBinderPos);
        MR::setSensorRadius(this, "binder", 125.0f);
        MR::validateHitSensor(this, "binder");
        setNerve(GET_NERVE(Poihana, PoihanaNrvWait));
    }
}

void Poihana::exeDPDSwoon() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensor(this, "Binder");
    }

    MR::updateActorStateAndNextNerve(this, (ActorStateBaseInterface*)mBindStarPointer, GET_NERVE(Poihana, PoihanaNrvWait));
}

void Poihana::endDPDSwoon() {
    mBindStarPointer->kill();

    MR::setSensorOffset(this, "binder", ::sNormalBinderPos);
    MR::setSensorRadius(this, "binder", 125.0f);
    MR::validateHitSensor(this, "binder");
}

bool Poihana::tryToStartBind(HitSensor* pSender) {
    if (mBindedActor != nullptr) {
        return false;
    }

    LiveActor* bindedActor = pSender->mHost;
    if (MR::isInWater(bindedActor, TVec3f(0.0f, 0.0f, 0.0f))) {
        return false;
    }

    MR::tryRumblePadMiddle(this, WPAD_CHAN0);
    mBindedActor = pSender->mHost;
    MR::startBckPlayer("Rise", (const char*)nullptr);
    MR::invalidateClipping(this);
    return true;
}

void Poihana::updateBindActorMtx() {
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
    // FIXME
    // https://decomp.me/scratch/580s2

    if (mBoundTimer == -1) {
        return;
    }

    if (mBoundTimer >= 40) {
        mScale.set(1.0f, 1.0f, 1.0f);
        mBoundTimer = -1;
        return;
    }

    mBoundTimer++;

    f32 s = 0.4f - mBoundTimer * 0.01f;

    mScale.set(s * 0.9f * MR::sinDegree(mBoundTimer * (9.0f * 1.0f)) + 1.0f, s * 1.0f * MR::sinDegree(mBoundTimer * (9.0f * 3.0f)) + 1.0f,
               s * 0.9f * MR::sinDegree(mBoundTimer * (9.0f * 2.0f)) + 1.0f);
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

// Needs review
void Poihana::controlVelocity() {
    if (isNerve(GET_NERVE(Poihana, PoihanaNrvNonActive))) {
        return;
    }

    // Calculate front vector
    TVec3f gravity = MR::isBindedGround(this) ? -*MR::getGroundNormal(this) : mGravity;

    TVec3f frontVec(mFrontVec);
    MR::turnVecToPlane(&mFrontVec, frontVec, gravity);

    // Calculate velocity
    if (MR::isBindedGround(this)) {
        TVec3f addVel = mVelocity.killElement(mFrontVec);
        addVel.scale(0.8f);

        mVelocity.scale(mFrontVec.dot(mVelocity), mFrontVec);
        mVelocity.add(addVel);

        if (mVelocity.dot(gravity) > 0.0f) {
            mVelocity.orthogonalize(gravity);
        }

        mVelocity.scale(0.95f);
    }

    mVelocity.scaleAdd(2.0f, gravity, mVelocity);

    if (!isNerve(GET_NERVE(Poihana, PoihanaNrvShock))) {
        f32 magVel = isNerve(GET_NERVE(Poihana, PoihanaNrvChasePlayer)) ? 10.0f : 5.0f;

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
    gravityPos.scaleAdd(20.0f, upVec, mPosition);
    MR::calcGravity(this, gravityPos);
}

bool Poihana::tryNonActive() {
    if (!MR::isStageStateScenarioOpeningCamera()) {
        bool notNear = !MR::isNearPlayerAnyTime(this, 3500.0f);
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

    mVelocity.scale(-30.0f, mGravity);
    setNerve(GET_NERVE(Poihana, PoihanaNrvShock));
    return true;
}

bool Poihana::tryHipDropShock() {
    bool isNotNear = !MR::isNearPlayerAnyTime(this, 500.0f);

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
        return !MR::isNear(this, mHomePos, 350.0f);
    }

    if (isNerve(GET_NERVE(Poihana, PoihanaNrvChasePlayer))) {
        bool ret;

        if (mBehavior == POIHANA_BEHAVIOR_NEW_HOME) {
            ret = !MR::isNearPlayer(this, 1100.0f);
        } else {
            ret = !MR::isNear(this, mHomePos, 2000.0f) || !MR::isNearPlayer(this, 1110.0f);
        }

        return ret;
    }

    return false;
}

bool Poihana::isNeedForGetUp() const {
    bool ret = false;

    if (MR::isNearPlayer(this, 500.0f)) {
        bool flag = true;

        f32 mag = MR::getPlayerVelocity()->length();

        if (!(mag >= 10.0f) && !MR::isPlayerSwingAction()) {
            flag = false;
        }

        if (flag) {
            ret = true;
        }
    }

    return ret;
}

bool Poihana::isBackAttack(HitSensor* pMySensor) const {
    TVec3f frontVec;
    MR::calcFrontVec(&frontVec, this);
    frontVec.negate();

    return (pMySensor->mPosition - mPosition).dot(frontVec) > 0.0f;
}
