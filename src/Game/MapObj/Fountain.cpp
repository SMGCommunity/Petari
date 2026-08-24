#include "Game/MapObj/Fountain.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/CollisionShapeUtil.hpp"

void Fountain_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

namespace {
    static const f32 sSensorRadius = 300.0f;
    static const f32 sSensorOffsetY = 300.0f;
    static const f32 sHitCylinderRange = 10.0f;
    static const f32 sHitSphereRadius = 70.0f;
    static const f32 sHitSphereOffsetY = 350.0f;
    static const f32 sClippingRadius = 3500.0f;
    static const f32 sHitCylinderHeight = 280.0f;
};  // namespace

namespace NrvFountain {
    NEW_NERVE(HostTypeWait, Fountain, Wait);
    NEW_NERVE(HostTypeMove, Fountain, Move);
};  // namespace NrvFountain

Fountain::Fountain(const char* pName) : LiveActor(pName), mFountainName(), mUp(0.0f, 1.0f, 0.0f) {
}

void Fountain::init(const JMapInfoIter& rIter) {
    // FIXME: stack order issue
    // https://decomp.me/scratch/y0a6l

    MR::initDefaultPos(this, rIter);
    TPos3f mtx;
    mtx.identity();
    mtx.setRotateDegree(mRotation);
    mtx.getYDir(mUp);
    MR::normalize(&mUp);
    MR::getObjectName(&mFountainName, rIter);
    initEffectKeeper(0, mFountainName, false);
    initSound(4, false);
    initHitSensor(1);
    MR::addHitSensorMapObj(this, "body", 16, ::sSensorRadius, TVec3f(0.0f, ::sSensorOffsetY, 0.0f));
    MR::setClippingTypeSphere(this, ::sClippingRadius);
    MR::useStageSwitchReadA(this, rIter);
    initNerve(&NrvFountain::HostTypeMove::sInstance);
    MR::connectToSceneMapObjMovement(this);

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    } else {
        appear();
    }
}

void Fountain::appear() {
    LiveActor::appear();

    if (MR::isValidSwitchA(this)) {
        setNerve(&NrvFountain::HostTypeWait::sInstance);
    } else {
        setNerve(&NrvFountain::HostTypeMove::sInstance);
    }
}

void Fountain::startClipped() {
    LiveActor::startClipped();

    if (!isNerve(&NrvFountain::HostTypeWait::sInstance)) {
        MR::deleteEffectAll(this);
    }
}

void Fountain::endClipped() {
    LiveActor::endClipped();

    if (!isNerve(&NrvFountain::HostTypeWait::sInstance)) {
        MR::emitEffect(this, mFountainName);
    }
}

void Fountain::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        f32 playerRadius = pReceiver->getRadius() * 1.0f;

        HitSphere sphere(mPosition + mUp * ::sHitSphereOffsetY, ::sHitSphereRadius);

        if (sphere.isHit(HitSphere(pReceiver->getPosition(), playerRadius))) {
            if (MR::sendArbitraryMsg(ACTMES_FOUNTAINJUMP, pReceiver, pSender)) {
                return;
            }
        }

        HitCylinder cyl(mPosition, mUp * ::sHitCylinderHeight);

        f32 proj, ortho;
        cyl.getParams(pReceiver->getPosition(), &proj, &ortho);

        if (0.0f <= proj && proj <= 1.0f && ortho < playerRadius + ::sHitCylinderRange) {
            if (MR::sendArbitraryMsg(ACTMES_FOUNTAINJUMP, pReceiver, pSender)) {
                return;
            }
        }
    }
}

void Fountain::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensor(this, "body");
    }

    if (MR::isOnSwitchA(this)) {
        setNerve(&NrvFountain::HostTypeMove::sInstance);
    }
}

void Fountain::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::validateHitSensor(this, "body");
        MR::emitEffect(this, mFountainName);
    }

    MR::startLevelSound(this, "SE_OJ_LV_FOUNTAIN");
}
