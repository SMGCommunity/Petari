#include "Game/MapObj/SpringJetWater.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"
#include "Game/Util/CollisionShapeUtil.hpp"

void SpringJetWater_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

namespace {
    static const f32 sSensorRadius = 300.0f;
    static const f32 sSensorOffsetY = 300.0f;
    static const f32 sHitCylinderRange = 10.0f;
    static const f32 sHitSphereRadius = 70.0f;
    static const f32 sHitSphereOffsetY = 350.0f;
    static const f32 sClippingRadius = 1000.0f;
    static const s32 sEndTime = 60;
    static const f32 sHitCylinderHeight = 280.0f;
};  // namespace

namespace NrvSpringJetWater {
    NEW_NERVE(HostTypeWait, SpringJetWater, Wait);
    NEW_NERVE(HostTypeJet, SpringJetWater, Jet);
    NEW_NERVE(HostTypeEnd, SpringJetWater, End);
};  // namespace NrvSpringJetWater

SpringJetWater::SpringJetWater(const char* pName) : LiveActor(pName), mObjName(), mUp(0.0f, 1.0f, 0.0f) {
}

// https://decomp.me/scratch/e7fUQ
void SpringJetWater::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    TPos3f mtx;
    mtx.identity();
    mtx.setRotateDegree(mRotation);
    mtx.getYDir(mUp);
    MR::normalize(&mUp);
    MR::getObjectName(&mObjName, rIter);
    initEffectKeeper(0, mObjName, false);
    initSound(4, false);
    initHitSensor(1);

    MR::addHitSensorMapObj(this, "body", 0x10, ::sSensorRadius, TVec3f(0.0f, ::sSensorOffsetY, 0.0f));
    MR::setClippingTypeSphere(this, ::sClippingRadius);
    initNerve(GET_NERVE(SpringJetWater, HostTypeWait));
    MR::connectToSceneMapObjMovement(this);
    MR::tryRegisterDemoCast(this, rIter);
    appear();
}

void SpringJetWater::appear() {
    LiveActor::appear();
    setNerve(GET_NERVE(SpringJetWater, HostTypeWait));
}

void SpringJetWater::startClipped() {
    if (isNerve(GET_NERVE(SpringJetWater, HostTypeWait))) {
        MR::deleteEffect(this, "Wait");
    }

    LiveActor::startClipped();
}

void SpringJetWater::endClipped() {
    LiveActor::endClipped();

    if (isNerve(GET_NERVE(SpringJetWater, HostTypeWait))) {
        MR::emitEffect(this, "Wait");
    }
}

void SpringJetWater::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
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

void SpringJetWater_DUMMY() {
    MR::isDemoPartFirstStep("湧き水上昇");
}

void SpringJetWater::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensor(this, "body");
        MR::emitEffect(this, "Wait");
    }

    MR::startLevelSound(this, "SE_OJ_LV_SPR_JET_WATER_WAIT");

    if (MR::isDemoPartFirstStep("湧き水上昇")) {
        MR::deleteEffect(this, "Wait");
        setNerve(GET_NERVE(SpringJetWater, HostTypeJet));
    }
}

void SpringJetWater::exeJet() {
    if (MR::isFirstStep(this)) {
        MR::validateHitSensor(this, "body");
        MR::emitEffect(this, "Jet");
        MR::startSound(this, "SE_OJ_SPR_JET_WATER_JET_ST");
        MR::setStageBGMState(1, 180);
    }

    MR::startLevelSound(this, "SE_OJ_LV_SPR_JET_WATER_JET");

    if (MR::isDemoPartLastStep("湧き水上昇")) {
        setNerve(GET_NERVE(SpringJetWater, HostTypeEnd));
    }
}

void SpringJetWater::exeEnd() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensor(this, "body");
        MR::deleteEffect(this, "Jet");
    }

    if (MR::isStep(this, ::sEndTime)) {
        kill();
    }
}
