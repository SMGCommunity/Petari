#include "Game/Enemy/KameckBeam.hpp"
#include "Game/Enemy/KameckBeamHolder.hpp"
#include "Game/Enemy/KameckFireBall.hpp"
#include "Game/Enemy/KameckTurtle.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/ClipAreaHolder.hpp"
#include "Game/Util.hpp"
#include "revolution/types.h"

namespace {
    // const s32 sMaxBeamLife =
    // const f32 sBeamRadius =
    // const s32 sMorphTime =
    // const s32 sBurningTime =
    // const f32 sStormRange =
}

KameckBeamEventListener::KameckBeamEventListener() {
}

KameckBeamCollisionFilter::KameckBeamCollisionFilter(const TVec3f* pVec, f32 f) : _4(pVec), _8(f) {
}

bool KameckBeamCollisionFilter::isInvalidParts(const CollisionParts* pCollisionParts) const {
    u32 sensorType = MR::getCollisionSensorType(pCollisionParts);
    if (sensorType == ATYPE_KAMECK_BARRIER) {
        return true;
    }
    if (!MR::isExistClipAreaHolder()) {
        return false;
    }
    if (sensorType == ATYPE_CLIP_FIELD_MAP_PARTS && !MR::isInClipArea(*_4, _8)) {
        return true;
    }
    return false;
}

namespace {
    const f32 sFireAngleLebel1[] = {0.0f};
    const f32 sFireAngleLebel2[] = {30.0f, -30.0f};
    const f32 sFireAngleLebel3[] = {0.0f, 120.0f, -120.0f, 0.0f};
    const f32* sFireAngleList[] = {sFireAngleLebel1, sFireAngleLebel2, sFireAngleLebel3};
};  // namespace

namespace NrvKameckBeam {
    NEW_NERVE(KameckBeamNrvFollowWand, KameckBeam, FollowWand);
    NEW_NERVE(KameckBeamNrvShoot, KameckBeam, Shoot);
    NEW_NERVE(KameckBeamNrvExplosion, KameckBeam, Explosion);
    NEW_NERVE(KameckBeamNrvJetTurtle, KameckBeam, JetTurtle);
    NEW_NERVE(KameckBeamNrvFire, KameckBeam, Fire);
};  // namespace NrvKameckBeam

KameckBeam::KameckBeam(const char* pName)
    : LiveActor(pName), mEventListener(nullptr), mKameckTurtle(), _A0(0), _A4(0, 0, 1), mWandLocalPosition(0, 0, 0), mBeamKind(2) {
    for (s32 i = 0; i < ARRAY_SIZEU(mKameckFireBalls); i++) {
        mKameckFireBalls[i] = nullptr;
    }
}

void KameckBeam::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToSceneEnemyDecorationMovementCalcAnim(this);
    initNerve(&NrvKameckBeam::KameckBeamNrvFollowWand::sInstance);
    initSound(4, false);
    initBinder(80.0f, 0.0f, 0);
    KameckBeamCollisionFilter* pFilter = new KameckBeamCollisionFilter(&mPosition, 10.0f);
    MR::setBinderCollisionPartsFilter(this, pFilter);
    initEffectKeeper(0, "BossKameckBeam", false);
    MR::setEffectHostSRT(this, "BeamTurtleReady", &mPosition, nullptr, nullptr);
    MR::setEffectHostSRT(this, "BeamFireReady", &mPosition, nullptr, nullptr);
    MR::setEffectHostSRT(this, "BeamTurtle", &mPosition, nullptr, nullptr);
    MR::setEffectHostSRT(this, "BeamFire", &mPosition, nullptr, nullptr);
    initHitSensor(1);
    MR::addHitSensorEnemyAttack(this, "attack", 8, 80.0f, TVec3f(0.0f));

    MR::initShadowVolumeSphere(this, 80.0f);
    MR::onCalcShadow(this, nullptr);
    MR::invalidateClipping(this);
    makeActorDead();
}

void KameckBeam::control() {
}

void KameckBeam::kill() {
    LiveActor::kill();
    resetBeam();
}

void KameckBeam::calcAnim() {
    if (_A0) {
        PSMTXMultVec(_A0, mWandLocalPosition, mPosition);
    }
}

void KameckBeam::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorPlayer(pReceiver)) {
        return;
    }
    switch (mBeamKind) {
    case BeamType_Turtle:
        if (MR::sendMsgEnemyAttackStrong(pReceiver, pSender)) {
            if (mEventListener != nullptr) {
                mEventListener->hitBeam(mBeamKind);
            }
            kill();
        }
        break;
    case BeamType_1FireBall:
    case BeamType_2FireBalls:
    case BeamType_3FireBalls:
        if (MR::sendMsgEnemyAttackFire(pReceiver, pSender)) {
            if (mEventListener != nullptr) {
                mEventListener->hitBeam(mBeamKind);
            }
            kill();
        }
        break;
    default:
        if (MR::sendMsgEnemyAttack(pReceiver, pSender)) {
            kill();
        }
    }
}

bool KameckBeam::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgSpinStormRange(msg)) {
        return requestStorm(pSender, pReceiver);
    }
    if (MR::isMsgStartPowerStarGet(msg)) {
        makeActorDead();
        return true;
    }
    return false;
}

void KameckBeam::setWandLocalPosition(const TVec3f& rVec) {
    mWandLocalPosition.set(rVec);
}

void KameckBeam::setBeamKind(s32 type) {
    mBeamKind = type;
}

void KameckBeam::setEventListener(KameckBeamEventListener* pListener) {
    mEventListener = pListener;
}

void KameckBeam::resetBeam() {
    if (mKameckTurtle) {
        if (!MR::isDead(mKameckTurtle)) {
            mKameckTurtle->kill();
        }
        mKameckTurtle = nullptr;
    }

    for (s32 i = 0; i < ARRAY_SIZEU(mKameckFireBalls); i++) {
        if (mKameckFireBalls[i] != nullptr) {
            if (!MR::isDead(mKameckFireBalls[i])) {
                mKameckFireBalls[i]->kill();
            }
            mKameckFireBalls[i] = nullptr;
        }
    }
    mEventListener = nullptr;
}

bool KameckBeam::requestFollowWand(MtxPtr mtx, f32 f) {
    mScale.set(f);
    MR::setShadowVolumeSphereRadius(this, nullptr, f * 80.0f);
    MR::setBinderRadius(this, f * 80.0f);
    MR::setSensorRadius(this, "attack", f * 80.0f);
    emitBeamReadyEffect();
    _A0 = mtx;
    makeActorAppeared();
    setNerve(&NrvKameckBeam::KameckBeamNrvFollowWand::sInstance);
    MR::offBind(this);
    MR::invalidateHitSensors(this);
    MR::invalidateShadow(this, nullptr);
    return true;
}

void KameckBeam::requestShootToPlayerGround(f32 f) {
    TVec3f groundPos;
    MR::getPlayerGroundPos(&groundPos);
    TVec3f vec(mGravity * 80.0f);
    groundPos -= vec;
    groundPos -= mPosition;
    MR::normalizeOrZero(&groundPos);
    if (MR::isNearZero(groundPos)) {
        MR::calcGravity(this);
        groundPos.set(mGravity);
    }
    requestShoot(groundPos, f);
}

void KameckBeam::requestShootToPlayerCenter(f32 f) {
    TVec3f vec;
    vec.set(*MR::getPlayerCenterPos() - mPosition);
    MR::normalizeOrZero(&vec);
    if (MR::isNearZero(vec)) {
        MR::calcGravity(this);
        vec.set(mGravity);
    }
    requestShoot(vec, f);
}

void KameckBeam::requestShoot(const TVec3f& rVec, f32 f) {
    emitBeamEffect();
    MR::onBind(this);
    MR::validateHitSensors(this);
    MR::validateShadow(this, nullptr);
    _A0 = nullptr;
    setNerve(&NrvKameckBeam::KameckBeamNrvShoot::sInstance);
    _A4.set(rVec);
    mVelocity.set(rVec * f);
}

bool KameckBeam::requestStorm(HitSensor* pSender, HitSensor* pReceiver) {
    f32 distance = PSVECDistance(MR::getSensorPos(pSender), MR::getSensorPos(pReceiver));
    if (distance >= 500.0f) {
        return false;
    }
    if (!isNerve(&NrvKameckBeam::KameckBeamNrvShoot::sInstance)) {
        return false;
    }
    if (mBeamKind == BeamType_Turtle) {
        if (tryChangeTurtle()) {
            return true;
        }
        kill();
        return true;
    }
    return false;
}

bool KameckBeam::tryShootEnd() {
    if (MR::isBinded(this)) {
        MR::sendMsgEnemyAttackExplosionToBindedSensor(this, getSensor("attack"));
        MR::invalidateHitSensors(this);
        MR::zeroVelocity(this);
        setNerve(&NrvKameckBeam::KameckBeamNrvExplosion::sInstance);
        return true;
    }
    if (MR::isGreaterStep(this, 360)) {
        kill();
        return true;
    }
    return false;
}

bool KameckBeam::tryChangeTurtle() {
    mKameckTurtle = MR::getKameckBeamTurtle();
    if (mKameckTurtle == nullptr) {
        return false;
    }
    MR::offBind(this);
    MR::invalidateShadow(this, nullptr);
    MR::invalidateHitSensors(this);
    MR::forceDeleteEffectAll(this);
    mKameckTurtle->mPosition.set(mPosition);
    mKameckTurtle->appearDirection(_A4);
    setNerve(&NrvKameckBeam::KameckBeamNrvJetTurtle::sInstance);
    return true;
}

bool KameckBeam::tryChangeFire() {
    s32 fireAngleListIndex = 0;
    switch (mBeamKind) {
    case BeamType_1FireBall:
        fireAngleListIndex = 1;
        break;

    case BeamType_2FireBalls:
        fireAngleListIndex = 2;
        break;
    case BeamType_3FireBalls:
        fireAngleListIndex = 3;
        break;
    }
    TVec3f result;

    TVec3f* grav = &mGravity;

    JMAVECScaleAdd(grav, _A4, result, -grav->dot(_A4));
    if (MR::isNearZero(result)) {
        MR::makeAxisVerticalZX(&result, mGravity);
    } else {
        MR::normalize(&result);
    }

    bool b1 = false;
    for (u32 i = 0; i < fireAngleListIndex; i++) {
        mKameckFireBalls[i] = MR::getKameckFireBall();
        if (mKameckFireBalls[i] != nullptr) {
            mKameckFireBalls[i]->setEventListener(mEventListener);
            mKameckFireBalls[i]->mPosition.set(mPosition);
            TVec3f result2;
            MR::rotateVecDegree(&result2, result, mGravity, sFireAngleList[fireAngleListIndex][i]);
            mKameckFireBalls[i]->appearDirection(result2);
            b1 = true;
        }
    }

    if (b1) {
        MR::offBind(this);
        MR::invalidateShadow(this, nullptr);
        MR::invalidateHitSensors(this);
        MR::forceDeleteEffectAll(this);
        setNerve(&NrvKameckBeam::KameckBeamNrvFire::sInstance);
        return true;
    }
    return false;
}

void KameckBeam::exeShoot() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_BM_KAMECK_FIRE_SHOOT");
    }
    if (tryShootEnd()) {
        return;
    }
    if (MR::isInWater(mPosition)) {
        switch (mBeamKind) {
        case BeamType_Turtle:
            MR::emitEffect(this, "BeamTurtleBreak");
            break;
        case BeamType_1FireBall:
        case BeamType_2FireBalls:
        case BeamType_3FireBalls:
            MR::emitEffect(this, "BeamFireBreak");
            break;
        }
        kill();
        return;
    }
    startBeamLevelSound();
    MR::startLevelSound(this, "SE_BM_LV_KAMECK_MAGIC_COMM");
}

void KameckBeam::exeExplosion() {
    switch (mBeamKind) {
    case BeamType_Turtle:
        if (tryChangeTurtle()) {
            MR::shakeCameraNormalWeak();
            MR::tryRumblePadStrong(this, WPAD_CHAN0);
            MR::emitEffect(this, "BeamTurtleBreak");
            return;
        }
        break;
    case BeamType_1FireBall:
    case BeamType_2FireBalls:
    case BeamType_3FireBalls:
        if (tryChangeFire()) {
            MR::shakeCameraNormalWeak();
            MR::tryRumblePadStrong(this, WPAD_CHAN0);
            MR::emitEffect(this, "BeamFireBreak");
            MR::startSound(this, "SE_BM_KAMECK_FIRE_EXPLOSION");
            return;
        }
        break;
    }
    kill();
}

void KameckBeam::exeJetTurtle() {
    if (MR::isFirstStep(this)) {
        MR::startSystemSE("SE_SY_APPEAR_TURTLE_JET");
    }
    if (MR::isDead(mKameckTurtle)) {
        kill();
    }
}

void KameckBeam::exeFire() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "BeamFireBurn");
    }
    if (MR::isStep(this, 40)) {
        MR::deleteEffect(this, "BeamFireBurn");
    }

    // Possible inline or a while loop
    bool killBeam = true;
    for (s32 i = 0; i < ARRAY_SIZEU(mKameckFireBalls); i++) {
        if (mKameckFireBalls[i] != nullptr && !MR::isDead(mKameckFireBalls[i])) {
            killBeam = false;
            break;
        }
    }

    if (killBeam) {
        kill();
    }
}

void KameckBeam::startBeamLevelSound() {
    switch (mBeamKind) {
    case BeamType_Turtle:
        MR::startLevelSound(this, "SE_BM_LV_KAMECK_MAGIC_TURT");
        break;
    case BeamType_1FireBall:
    case BeamType_2FireBalls:
    case BeamType_3FireBalls:
        MR::startLevelSound(this, "SE_BM_LV_KAMECK_MAGIC_FIRE");
        break;
    }
}

void KameckBeam::emitBeamReadyEffect() {
    switch (mBeamKind) {
    case BeamType_Turtle:
        MR::emitEffect(this, "BeamTurtleReady");
        break;
    case BeamType_1FireBall:
    case BeamType_2FireBalls:
    case BeamType_3FireBalls:
        MR::emitEffect(this, "BeamFireReady");
        break;
    }
}

void KameckBeam::emitBeamEffect() {
    switch (mBeamKind) {
    case BeamType_Turtle:
        MR::emitEffect(this, "BeamTurtle");
        break;
    case BeamType_1FireBall:
    case BeamType_2FireBalls:
    case BeamType_3FireBalls:
        MR::emitEffect(this, "BeamFire");
        break;
    }
}

void KameckBeam::exeFollowWand() {
    startBeamLevelSound();
}

namespace MR {
    void setKameckBeamCollisionFilter(LiveActor* pActor) {
        MR::setBinderCollisionPartsFilter(pActor, new KameckBeamCollisionFilter(&pActor->mPosition, 10.0f));
    }
};  // namespace MR
