#include "Game/Enemy/KameckTurtle.hpp"
#include "Game/Enemy/KameckBeam.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/JetTurtle.hpp"
#include "Game/Util.hpp"

namespace {
    // const f32 beamRadius =
    // const s32 sUpVecBlendRate =
    // const s32 sFrontVecBlendRate =
    // const s32 sRunTime =
    // const s32 sEnableAttackTime =
    // const s32 sDeccelStartTime =
    // const s32 sDeccelEndTime =
    // const f32 sRunSpeed =
    // const f32 sRunGravity =
    // const f32 sRunFric =
    // const f32 sRunSpinDegree =
    // const s32 sMorphTime =
}

namespace NrvKameckTurtle {
    NEW_NERVE(KameckTurtleNrvRun, KameckTurtle, Run);
    NEW_NERVE(KameckTurtleNrvWait, KameckTurtle, Wait);
};  // namespace NrvKameckTurtle

KameckTurtle::KameckTurtle(const char* pName)
    : LiveActor(pName), mTurtle(), _94(0.0f, 0.0f, 0.0f, 1.0f), _A4(0.0f, 0.0f, 1.0f), _B0(0.0f, 0.0f, 1.0f) {
}

void KameckTurtle::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("Koura", nullptr, false);
    MR::connectToSceneNoSilhouettedMapObjStrongLight(this);
    MR::startBrk(this, nullptr);
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
    initNerve(&NrvKameckTurtle::KameckTurtleNrvWait::sInstance);
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
    setNerve(&NrvKameckTurtle::KameckTurtleNrvRun::sInstance);
    mTurtle->kill();
    MR::onBind(this);
    MR::showModel(this);
    MR::validateShadow(this, nullptr);
    MR::validateHitSensors(this);
}

void KameckTurtle::makeActorDead() {
    LiveActor::makeActorDead();
    _8C = nullptr;
    mTurtle->kill();
}

void KameckTurtle::control() {
    MR::blendQuatFromGroundAndFront(&_94, this, _A4, 0.2f, 0.2f);
}

void KameckTurtle::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _94);
    MR::setBaseScale(this, mScale);
}

void KameckTurtle::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isEnableAttack()) {
        if (MR::isSensorPlayer(pReceiver) && MR::sendMsgEnemyAttackFlipJump(pReceiver, pSender)) {
            MR::reflectVelocityH(this, pSender, pReceiver, 0.8f);
            if (!MR::isNearZero(mVelocity)) {
                MR::normalize(mVelocity, &_B0);
            }
            if (_8C != nullptr) {
                _8C->hitBeam(1);
                return;
            }
        }
    } else if (!isNerve(&NrvKameckTurtle::KameckTurtleNrvWait::sInstance)) {
        appearJetTurtle(false);
        setNerve(&NrvKameckTurtle::KameckTurtleNrvWait::sInstance);
    }
}

bool KameckTurtle::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerTrample(msg)) {
        setNerve(&NrvKameckTurtle::KameckTurtleNrvWait::sInstance);
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
        setNerve(&NrvKameckTurtle::KameckTurtleNrvWait::sInstance);
        appearJetTurtle(true);
        return true;
    }
    return false;
}

void KameckTurtle::todo(const TVec3f& rVec1) {
}

void KameckTurtle::appearDirection(const TVec3f& rDir) {
    MR::calcGravity(this);
    mVelocity.set(rDir);
    _B0.killElement(rDir, mGravity);
    if (MR::normalizeOrZero(&_B0)) {
        MR::makeAxisVerticalZX(&_B0, mGravity);
    }
    setNerve(&NrvKameckTurtle::KameckTurtleNrvRun::sInstance);
    appear();
}

void KameckTurtle::exeRun() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Koura", nullptr);
    }

    _B0.orthogonalize(mGravity);
    if (MR::normalizeOrZero(&_B0)) {
        MR::makeAxisVerticalZX(&_B0, mGravity);
    }
    if (MR::isBindedGround(this)) {
        MR::startLevelSound(this, "SE_EM_LV_TURTLE_SLIDE");
    }
    if (!MR::isBindedGround(this)) {
        MR::addVelocityToGravity(this, 0.7f);
    }
    f32 easeInOut = 1.0f;
    if (MR::isGreaterStep(this, 200)) {
        easeInOut = 1.0f - MR::calcNerveEaseInOutRate(this, 200, 400);
    }
    MR::addVelocityMoveToDirection(this, _B0, 0.5f * easeInOut);
    MR::rotateDirectionGravityDegree(this, &_A4, -25.0f * easeInOut);
    MR::turnDirectionToGround(this, &_A4);
    MR::attenuateVelocity(this, 0.98f);
    if (MR::isBindedWall(this) && MR::calcReboundVelocity(&mVelocity, *MR::getWallNormal(this), 0.5f)) {
        TVec3f dir = MR::getVelocityHorizon(this);
        if (!MR::normalizeOrZero(&dir)) {
            _B0.set(dir);
        }
        MR::addVelocityJump(this, 15.0f);
        MR::startSound(this, "SE_EM_TURTLE_REBOUND");
    }
    if (MR::isGreaterStep(this, 600)) {
        MR::emitEffectHit(this, mPosition, "BeamTurtleVanish");
        MR::startSound(this, "SE_BM_KAMECK_DISAPPEAR_TURT");
        kill();
    }
}

void KameckTurtle::exeWait() {
    MR::isFirstStep(this);
    if (mTurtle->isRestart() || (mTurtle->isWait() && MR::isGreaterStep(this, 600))) {
        MR::emitEffectHit(this, mTurtle->mPosition, "BeamTurtleVanish");
        MR::startSound(this, "SE_BM_KAMECK_DISAPPEAR_TURT");
        kill();
    }
}

bool KameckTurtle::isEnableAttack() const {
    if (!isNerve(&NrvKameckTurtle::KameckTurtleNrvRun::sInstance)) {
        return false;
    }
    if (MR::isLessStep(this, 300)) {
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
        return;
    }
    mTurtle->appear();
}
