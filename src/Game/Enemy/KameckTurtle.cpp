#include "Game/Enemy/KameckTurtle.hpp"
#include "Game/Enemy/Kameck.hpp"
#include "Game/Enemy/KameckBeam.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/JetTurtle.hpp"
#include "Game/Util.hpp"

void KameckTurtle_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
}

namespace {
    // const f32 sBeamRadius =
    const f32 sUpVecBlendRate = 0.2f;
    const f32 sFrontVecBlendRate = 0.2f;
    const s32 sRunTime = 600;
    const s32 sEnableAttackTime = 300;
    const s32 sDeccelStartTime = 200;
    const s32 sDeccelEndTime = 400;
    const f32 sRunSpeed = 15.0f;
    const f32 sRunGravity = 0.7f;
    const f32 sRunFric = 0.98f;
    const f32 sRunSpinDegree = -25.0f;
    // const s32 sMorphTime =
};  // namespace

namespace NrvKameckTurtle {
    NEW_NERVE(KameckTurtleNrvRun, KameckTurtle, Run);
    NEW_NERVE(KameckTurtleNrvWait, KameckTurtle, Wait);
};  // namespace NrvKameckTurtle

KameckTurtle::KameckTurtle(const char* pName)
    : LiveActor(pName), mTurtle(), mRotateQuat(0.0f, 0.0f, 0.0f, 1.0f), _A4(0.0f, 0.0f, 1.0f), _B0(0.0f, 0.0f, 1.0f) {
}

void KameckTurtle::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("Koura", nullptr, false);
    MR::connectToSceneNoSilhouettedMapObjStrongLight(this);
    MR::startBrk(this, "Koura");
    MR::setBrkFrameAndStop(this, 0.0f);
    initJetTurtle();
    initSound(4, false);
    MR::onCalcGravity(this);
    initBinder(40.0f, 10.0f, 0);
    MR::setKameckBeamCollisionFilter(this);
    initHitSensor(1);
    MR::initShadowVolumeSphere(this, 60.0f);
    MR::invalidateClipping(this);
    MR::addHitSensorEnemyAttack(this, "attack", 8, 50.0f, TVec3f(0.0f));
    initEffectKeeper(0, "BossKameckBeam", false);
    initNerve(GET_NERVE(KameckTurtle, KameckTurtleNrvWait));
    makeActorAppeared();
}

void KameckTurtle::initJetTurtle() {
    mTurtle = new JetTurtle("カメックビーム用ジェット亀");
    mTurtle->initWithoutIter();
    mTurtle->becomeSlowType();

    MR::setKameckBeamCollisionFilter(mTurtle);
}

void KameckTurtle::makeActorAppeared() {
    LiveActor::makeActorAppeared();
    setNerve(GET_NERVE(KameckTurtle, KameckTurtleNrvRun));
    mTurtle->kill();
    MR::onBind(this);
    MR::showModel(this);
    MR::validateShadow(this, nullptr);
    MR::validateHitSensors(this);
}

void KameckTurtle::makeActorDead() {
    LiveActor::makeActorDead();
    mEventListener = nullptr;
    mTurtle->kill();
}

void KameckTurtle::control() {
    MR::blendQuatFromGroundAndFront(&mRotateQuat, this, _A4, ::sUpVecBlendRate, ::sFrontVecBlendRate);
}

void KameckTurtle::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, mRotateQuat);
    MR::setBaseScale(this, mScale);
}

void KameckTurtle::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isEnableAttack()) {
        if (MR::isSensorPlayer(pReceiver) && MR::sendMsgEnemyAttackFlipJump(pReceiver, pSender)) {
            MR::reflectVelocityH(this, pSender, pReceiver, 0.8f);

            if (!MR::isNearZero(mVelocity)) {
                MR::normalize(mVelocity, &_B0);
            }

            if (mEventListener != nullptr) {
                mEventListener->hitBeam(KameckBeam::BeamType_Turtle);
            }
        }
    } else if (!isNerve(GET_NERVE(KameckTurtle, KameckTurtleNrvWait))) {
        appearJetTurtle(false);
        setNerve(GET_NERVE(KameckTurtle, KameckTurtleNrvWait));
    }
}

bool KameckTurtle::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerTrample(msg)) {
        setNerve(GET_NERVE(KameckTurtle, KameckTurtleNrvWait));
        appearJetTurtle(false);

        return true;
    }

    return false;
}

bool KameckTurtle::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_IS_PULL_ENABLE) {
        return true;
    }

    if (MR::isMsgItemPull(msg)) {
        setNerve(GET_NERVE(KameckTurtle, KameckTurtleNrvWait));
        appearJetTurtle(true);

        return true;
    }

    return false;
}

void KameckTurtle::appearDirection(const TVec3f& rDir) {
    MR::calcGravity(this);
    mVelocity.set(rDir);
    _B0.killElement(rDir, mGravity);

    if (MR::normalizeOrZero(&_B0)) {
        MR::makeAxisVerticalZX(&_B0, mGravity);
    }

    setNerve(GET_NERVE(KameckTurtle, KameckTurtleNrvRun));
    appear();
}

void KameckTurtle::exeRun() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait");
    }

    _B0.orthogonalize(mGravity);

    if (MR::normalizeOrZero(&_B0)) {
        MR::makeAxisVerticalZX(&_B0, mGravity);
    }

    if (MR::isBindedGround(this)) {
        MR::startLevelSound(this, "SE_EM_LV_TURTLE_SLIDE");
    }

    if (!MR::isBindedGround(this)) {
        MR::addVelocityToGravity(this, ::sRunGravity);
    }

    f32 easeInOut = 1.0f;

    if (MR::isGreaterStep(this, ::sDeccelStartTime)) {
        easeInOut = 1.0f - MR::calcNerveEaseInOutRate(this, ::sDeccelStartTime, ::sDeccelEndTime);
    }

    MR::addVelocityMoveToDirection(this, _B0, easeInOut * 0.5f);
    MR::rotateDirectionGravityDegree(this, &_A4, ::sRunSpinDegree * easeInOut);
    MR::turnDirectionToGround(this, &_A4);
    MR::attenuateVelocity(this, ::sRunFric);

    if (MR::isBindedWall(this) && MR::calcReboundVelocity(&mVelocity, *MR::getWallNormal(this), 0.5f)) {
        TVec3f dir = MR::getVelocityHorizon(this);

        if (!MR::normalizeOrZero(&dir)) {
            _B0.set(dir);
        }

        MR::addVelocityJump(this, ::sRunSpeed);
        MR::startSound(this, "SE_EM_TURTLE_REBOUND");
    }

    if (MR::isGreaterStep(this, ::sRunTime)) {
        MR::emitEffectHit(this, mPosition, "BeamTurtleVanish");
        MR::startSound(this, "SE_BM_KAMECK_DISAPPEAR_TURT");
        kill();
    }
}

void KameckTurtle::exeWait() {
    if (MR::isFirstStep(this)) {
    }

    if (mTurtle->isRestart() || (mTurtle->isWait() && MR::isGreaterStep(this, 600))) {
        MR::emitEffectHit(this, mTurtle->mPosition, "BeamTurtleVanish");
        MR::startSound(this, "SE_BM_KAMECK_DISAPPEAR_TURT");
        kill();
    }
}

bool KameckTurtle::isEnableAttack() const {
    if (!isNerve(GET_NERVE(KameckTurtle, KameckTurtleNrvRun))) {
        return false;
    }

    if (MR::isLessStep(this, ::sEnableAttackTime)) {
        return true;
    }

    return false;
}

void KameckTurtle::appearJetTurtle(bool tryTaken) {
    MR::zeroVelocity(this);
    MR::offBind(this);
    MR::invalidateShadow(this, nullptr);
    MR::invalidateHitSensors(this);
    MR::hideModel(this);
    mTurtle->mPosition.set(mPosition);
    mTurtle->becomeSlowType();

    if (tryTaken) {
        mTurtle->appearAndTryTaken();
    } else {
        mTurtle->appear();
    }
}
