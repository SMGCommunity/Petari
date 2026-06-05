#include "Game/Boss/KoopaFireShort.hpp"
#include "Game/Boss/Koopa.hpp"

namespace NrvKoopaFireShort {
    NEW_NERVE(KoopaFireShortNrvFly, KoopaFireShort, Fly);
    NEW_NERVE(KoopaFireShortNrvDisappear, KoopaFireShort, Disappear);
    NEW_NERVE(KoopaFireShortNrvErase2P, KoopaFireShort, Erase2P);
}  // namespace NrvKoopaFireShort

KoopaFireShort::KoopaFireShort(const Koopa* pKoopa)
    : LiveActor("クッパの炎（ショート）"), mKoopa(pKoopa), mFront(0.0f, 0.0f, 1.0f), mSpeed(), mOffset() {
}

void KoopaFireShort::init(const JMapInfoIter& rIter) {
    mScale.set(1.0f, 1.0f, 1.0f);

    initModelManagerWithAnm("KoopaFire", nullptr, false);
    MR::tryStartAllAnim(this, "KoopaFire");

    MR::connectToSceneEnemy(this);
    initHitSensor(1);

    // smth wrong here
    TVec3f zeroVec = TVec3f(0.0f, 0.0f, 0.0f);
    MR::addHitSensor(this, "Attack", ATYPE_KOOPA_FIRE, 8, 130.0f, zeroVec);
    initBinder(50.0f, 0.0f, 0);

    MR::offBind(this);
    MR::onCalcGravity(this);

    initEffectKeeper(1, nullptr, false);

    MR::addEffectHitNormal(this, "Hit");

    initSound(4, false);
    initNerve(&NrvKoopaFireShort::KoopaFireShortNrvFly::sInstance);

    zeroVec.set(0.0f);
    MR::initStarPointerTarget(this, 100.0f, zeroVec);
    MR::initShadowVolumeSphere(this, 200.0f);

    MR::invalidateClipping(this);

    MR::createBloomModel(this, getBaseMtx());

    makeActorDead();
}

void KoopaFireShort::appear() {
    mPosition.set(mKoopa->mPosition);

    MR::calcGravity(this);

    TPos3f transform;
    transform.setInline(MR::getJointMtx(mKoopa, "Tongue2"));
    transform.getTransInline(mPosition);
    transform.getYDir(mVelocity);

    MR::vecKillElement(mVelocity, mGravity, &mVelocity);

    MR::isNearZero(mVelocity) ? transform.getYDir(mVelocity) : MR::normalize(&mVelocity);

    f32 radius = 80.0f * mScale.x;
    mVelocity.x *= 15.0f;
    mVelocity.y *= 15.0f;
    mVelocity.z *= 15.0f;

    MR::setBinderRadius(this, radius);
    MR::setSensorRadius(this, "Attack", radius);
    MR::setShadowVolumeSphereRadius(this, nullptr, radius);

    LiveActor::appear();

    MR::invalidateHitSensors(this);

    setNerve(&NrvKoopaFireShort::KoopaFireShortNrvFly::sInstance);
}

void KoopaFireShort::emitNormal() {
    appear();

    mSpeed = 15.0f;
    mDuration = 180;
    mOffset = 0.0f;
}

void KoopaFireShort::emitFast() {
    appear();

    mSpeed = 30.0f;
    mDuration = 180;
    mOffset = 0.0f;
}

void KoopaFireShort::emitCurve() {
    appear();

    mDuration = 300;
    mSpeed = 30.0f;

    MR::isPlayerLeftSide(this) ? mOffset = 0.1f : mOffset = -0.1f;
}

void KoopaFireShort::emitLongTime() {
    appear();

    mSpeed = 15.0f;
    mDuration = 300;
    mOffset = 0.0f;
}

void KoopaFireShort::exeFly() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
        MR::startSound(this, "SE_OJ_KOOPA_BULLET_SHOOT", -1, -1);
    }

    if (MR::isStep(this, 30)) {
        MR::validateHitSensors(this);
    }

    if (MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        TVec3f gravity = -mGravity;
        MR::calcStarPointerWorldVelocityDirectionOnPlane(&mVelocity, mPosition, gravity, 1);

        mSpeed = 80.0f;

        MR::tryRumblePadMiddle(this, 1);

        MR::scaleVelocity(this, mSpeed);

        MR::invalidateHitSensors(this);

        setNerve(&NrvKoopaFireShort::KoopaFireShortNrvErase2P::sInstance);
    } else {
        updateFly();

        if (MR::isLessStep(this, 50)) {
            TVec3f gravity = mGravity;
            gravity.scale(4.0f);
            mPosition.add(gravity);
        }

        if (MR::isGreaterStep(this, mDuration)) {
            setNerve(&NrvKoopaFireShort::KoopaFireShortNrvDisappear::sInstance);
        }
    }
}

void KoopaFireShort::exeDisappear() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Disappear", nullptr);
        MR::startSound(this, "SE_OJ_KOOPA_BULLET_EXPLODE", -1, -1);
    }

    updateFly();

    if (MR::isBckStopped(this)) {
        kill();
    }
}

void KoopaFireShort::exeErase2P() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensors(this);

        MR::startBck(this, "Disappear", nullptr);
        MR::start2PAttackAssistSound();
        MR::startSound(this, "SE_EM_FIRE_BUBBLE_REFLECT", -1, -1);

        MR::setBckRate(this, 3.0f);
    }

    updateFrontAndVelocity();

    if (MR::isBckStopped(this)) {
        kill();
    }
}

void KoopaFireShort::calcAndSetBaseMtx() {
    MR::calcAndSetBaseMtxFromGravityAndZAxis(this, mFront);
}

void KoopaFireShort::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!isNerve(&NrvKoopaFireShort::KoopaFireShortNrvFly::sInstance)) {
        return;
    }

    if (MR::isSensorPlayer(pReceiver) && MR::sendMsgEnemyAttackFire(pReceiver, pSender)) {
        mSpeed = 0.0f;
        MR::zeroVelocity(this);
        setNerve(&NrvKoopaFireShort::KoopaFireShortNrvDisappear::sInstance);
    } else {
        if (!MR::isSensorPlayer(pReceiver) && !MR::sendMsgPush(pReceiver, pSender) && !MR::isSensorType(pReceiver, ATYPE_KOOPA_FIRE) &&
            !MR::isSensorType(pReceiver, ATYPE_KOOPA_DAMAGE_PLATE) && !MR::isSensorType(pReceiver, ATYPE_KOOPA_COIN_PLATE) &&
            !MR::isSensorType(pReceiver, ATYPE_KOOPA_PLATE)) {
            mSpeed = 0.0f;
            MR::zeroVelocity(this);
            setNerve(&NrvKoopaFireShort::KoopaFireShortNrvDisappear::sInstance);
        }
    }
}

void KoopaFireShort::updateFly() {
    TVec3f difference = *MR::getPlayerPos() - mPosition;
    MR::normalizeOrZero(&difference);
    difference.scale(0.0f);
    mVelocity.add(difference);
    updateFrontAndVelocity();

    MR::rotateVecDegree(&mVelocity, -mGravity, mOffset);

    MR::startLevelSound(this, "SE_OJ_LV_KOOPA_BULLET_FLY", -1, -1, -1);
}

void KoopaFireShort::updateFrontAndVelocity() {
    MR::vecKillElement(mVelocity, mGravity, &mVelocity);
    MR::normalizeOrZero(&mVelocity);

    if (!MR::isNearZero(mVelocity)) {
        mFront.set(mVelocity);
    }

    mVelocity.scale(mSpeed);
}

KoopaFireShort::~KoopaFireShort() {
}
