#include "Game/MapObj/LavaSteam.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/CollisionShapeUtil.hpp"

void LavaSteam_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

namespace {
    // static const s32 sIntervalTime = 120;
    static const s32 sSteamTime = 90;
    static const s32 sAppearTime = 82;
    static const s32 sDisappearTime = 90;
    static const s32 sSteamWaitTime = 120;
    static const s32 sSteamEndTime = 90;
    static const f32 sSensorRadius = 250.0f;
    static const f32 sSensorOffsetY = 250.0f;
    static const f32 sHitCylinderRange = 10.0f;
    static const f32 sHitSphereRadius = 70.0f;
    static const f32 sHitSphereOffsetY = 400.0f;
    static const s32 sMaxNumGroupClipping = 16;
    static const f32 sHitCylinderHeight = 330.0f;
};  // namespace

namespace NrvLavaSteam {
    NEW_NERVE(HostTypeWait, LavaSteam, Wait);
    NEW_NERVE(HostTypeWaitForSwitchOn, LavaSteam, WaitForSwitchOn);
    NEW_NERVE(HostTypeSteam, LavaSteam, Steam);
    NEW_NERVE(HostTypeSteamEnd, LavaSteam, SteamEnd);
};  // namespace NrvLavaSteam

LavaSteam::LavaSteam(const char* pName) : LiveActor(pName), mUp(0.0f, 1.0f, 0.0f), mEffectScale(1.0f, 1.0f, 1.0f) {
}

void LavaSteam::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("LavaSteam", nullptr, false);
    initEffectKeeper(0, nullptr, false);
    MR::setEffectHostSRT(this, "Sign", &mPosition, &mRotation, &mEffectScale);

    initHitSensor(1);
    HitSensor* pSensor = MR::addHitSensorMapObj(this, "body", 8, ::sSensorRadius, TVec3f(0.0f, ::sSensorOffsetY, 0.0f));
    MR::setClippingTypeSphere(this, ::sSensorRadius, &pSensor->mPosition);

    MR::setGroupClipping(this, rIter, ::sMaxNumGroupClipping);
    initNerve(&NrvLavaSteam::HostTypeWait::sInstance);

    if (MR::useStageSwitchReadA(this, rIter)) {
        setNerve(&NrvLavaSteam::HostTypeWaitForSwitchOn::sInstance);
        MR::listenStageSwitchOnA(this, MR::Functor(this, &LavaSteam::startSteam));
    } else if (MR::tryRegisterDemoCast(this, rIter)) {
        setNerve(&NrvLavaSteam::HostTypeWaitForSwitchOn::sInstance);
        MR::registerDemoActionFunctor(this, MR::Functor(this, &LavaSteam::startSteam), nullptr);
    }
    MR::useStageSwitchSleep(this, rIter);

    initSound(4, false);

    MR::connectToSceneNoSilhouettedMapObj(this);

    makeActorAppeared();
}

void LavaSteam::initAfterPlacement() {
    TPos3f mtx;
    mtx.identity();
    mtx.setRotateDegree(mRotation);
    mtx.getYDir(mUp);
    MR::normalize(&mUp);
}

void LavaSteam::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvLavaSteam::HostTypeWait::sInstance)) {
        return;
    }

    if (isNerve(&NrvLavaSteam::HostTypeWaitForSwitchOn::sInstance)) {
        return;
    }

    if (MR::isSensorPlayerOrRide(pReceiver)) {
        f32 playerRadius = pReceiver->getRadius() * 0.7f;

        if (isNerve(&NrvLavaSteam::HostTypeSteam::sInstance)) {
            HitSphere sphere(mPosition + mUp * ::sHitSphereOffsetY, ::sHitSphereRadius);

            if (sphere.isHit(HitSphere(pReceiver->getPosition(), playerRadius))) {
                if (MR::sendMsgEnemyAttackFire(pReceiver, pSender)) {
                    return;
                }
            }
        }

        if (isNerve(&NrvLavaSteam::HostTypeSteam::sInstance)) {
            HitCylinder cyl(mPosition, mUp * ::sHitCylinderHeight);

            f32 proj, ortho;
            cyl.getParams(pReceiver->getPosition(), &proj, &ortho);

            if (0.0f <= proj && proj <= 1.0f && ortho < playerRadius + ::sHitCylinderRange) {
                if (MR::sendMsgEnemyAttackFire(pReceiver, pSender)) {
                    return;
                }
            }
        }
    }
}

void LavaSteam::startClipped() {
    LiveActor::startClipped();
    MR::deleteEffectAll(this);
}

void LavaSteam::endClipped() {
    LiveActor::endClipped();

    if (isNerve(&NrvLavaSteam::HostTypeWaitForSwitchOn::sInstance)) {
        return;
    }
}

void LavaSteam::startSteam() {
    setNerve(&NrvLavaSteam::HostTypeSteam::sInstance);
}

void LavaSteam::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensors(this);
        MR::emitEffect(this, "Sign");
        mEffectScale.set(1.0f, 1.0f, 1.0f);
    }

    if (MR::isGreaterStep(this, ::sAppearTime)) {
        mEffectScale.setAll< f32 >(
            MR::getEaseInValue(static_cast< f32 >(::sDisappearTime - getNerveStep()) / (::sDisappearTime - ::sAppearTime), 0.001f, 1.0f, 1.0f));
    }

    if (MR::isStep(this, ::sDisappearTime)) {
        MR::forceDeleteEffect(this, "Sign");
    }

    if (MR::isStep(this, ::sSteamWaitTime)) {
        setNerve(&NrvLavaSteam::HostTypeSteam::sInstance);
    }
}

void LavaSteam::exeWaitForSwitchOn() {
}

void LavaSteam::exeSteam() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "Steam");
        MR::validateHitSensors(this);
    }

    MR::startLevelSound(this, "SE_OJ_LV_LAVA_STEAM_OUT");

    if (MR::isStep(this, ::sSteamTime)) {
        MR::deleteEffect(this, "Steam");
        setNerve(&NrvLavaSteam::HostTypeSteamEnd::sInstance);
    }
}

void LavaSteam::exeSteamEnd() {
    if (MR::isStep(this, ::sSteamEndTime)) {
        setNerve(&NrvLavaSteam::HostTypeWait::sInstance);
    }
}
