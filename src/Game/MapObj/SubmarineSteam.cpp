#include "Game/MapObj/SubmarineSteam.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Util.hpp"
#include "Game/Util/CollisionShapeUtil.hpp"

void SubmarineSteam_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

namespace {
    static const f32 sSensorRadius = 400.0f;
    static const f32 sSensorOffsetY = 250.0f;
    static const f32 sHitCylinderRange = 20.0f;
    static const f32 sHitSphereRadius = 120.0f;
    static const f32 sHitSphereOffsetY = 600.0f;
    static const s32 sMaxNumGroupClipping = 16;
    static const f32 sHitCylinderHeight = 480.0f;
};  // namespace

namespace NrvSubmarineSteam {
    NEW_NERVE(HostTypeWaitForSwitchOn, SubmarineSteam, WaitForSwitchOn);
    NEW_NERVE(HostTypeSteam, SubmarineSteam, Steam);
};  // namespace NrvSubmarineSteam

SubmarineSteam::SubmarineSteam(const char* pName) : LiveActor(pName), mUp(0.0f, 1.0f, 0.0f) {
}

void SubmarineSteam::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToSceneMapObjMovement(this);

    const char* objName = nullptr;
    MR::getObjectName(&objName, rIter);
    initEffectKeeper(0, objName, false);

    initHitSensor(1);
    HitSensor* sensor = MR::addHitSensorMapObj(this, "body", 8, ::sSensorRadius, TVec3f(0.0f, ::sSensorOffsetY, 0.0f));

    MR::setClippingTypeSphere(this, 800.0f, &sensor->mPosition);
    MR::setGroupClipping(this, rIter, ::sMaxNumGroupClipping);
    MR::joinToGroupArray(this, rIter, nullptr, 32);

    MR::useStageSwitchReadA(this, rIter);

    initSound(4, false);

    if (MR::isValidSwitchA(this)) {
        initNerve(&NrvSubmarineSteam::HostTypeWaitForSwitchOn::sInstance);
    } else {
        initNerve(&NrvSubmarineSteam::HostTypeSteam::sInstance);
    }

    appear();
}

void SubmarineSteam::initAfterPlacement() {
    TPos3f mtx;
    mtx.identity();
    mtx.setRotateDegree(mRotation);
    mtx.getYDir(mUp);
    MR::normalize(&mUp);
}

void SubmarineSteam::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvSubmarineSteam::HostTypeWaitForSwitchOn::sInstance)) {
        return;
    }

    if (MR::isSensorPlayer(pReceiver)) {
        f32 playerRadius = pReceiver->getRadius() * 0.7f;
        bool hit = false;

        if (!hit) {
            HitSphere sphere(mPosition + mUp * ::sHitSphereOffsetY, ::sHitSphereRadius);

            if (sphere.isHit(HitSphere(pReceiver->getPosition(), playerRadius))) {
                hit = true;
            }
        }

        if (!hit) {
            HitCylinder cyl(mPosition, mUp * ::sHitCylinderHeight);

            f32 proj, ortho;
            cyl.getParams(pReceiver->getPosition(), &proj, &ortho);

            if (0.0f <= proj && proj <= 1.0f && ortho < playerRadius + ::sHitCylinderRange) {
                hit = true;
            }
        }

        if (hit) {
            if (MR::sendMsgEnemyAttackStrong(pReceiver, pSender)) {
                return;
            }
        }
    }
}

void SubmarineSteam::startClipped() {
    LiveActor::startClipped();
    MR::deleteEffectAll(this);
}

void SubmarineSteam::endClipped() {
    LiveActor::endClipped();

    if (!isNerve(&NrvSubmarineSteam::HostTypeWaitForSwitchOn::sInstance)) {
        MR::emitEffect(this, "Steam");
    }
}

void SubmarineSteam::exeSteam() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "Steam");
    }

    if (MR::isEqualStageName("SkullSharkGalaxy")) {
        MR::startLevelSound(this, "SE_OJ_LV_MARINE_STEAM_OUT_S");
    } else {
        MR::startLevelSound(this, "SE_OJ_LV_MARINE_STEAM_OUT");
    }
}

void SubmarineSteam::exeWaitForSwitchOn() {
    if (MR::isOnSwitchA(this)) {
        setNerve(&NrvSubmarineSteam::HostTypeSteam::sInstance);
    }
}
