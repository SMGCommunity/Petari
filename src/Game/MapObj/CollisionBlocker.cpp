#include "Game/MapObj/CollisionBlocker.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

namespace {
    static f32 cRadius = 50.0f;
};  // namespace

CollisionBlocker::CollisionBlocker(const char* pName) : LiveActor(pName) {
}

void CollisionBlocker::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initHitSensor(1);

    MR::addHitSensorEye(this, "eye", 4, ::cRadius * mScale.y, TVec3f(0.0f, 0.0f, 0.0f));
    MR::connectToSceneMapObjMovement(this);
    MR::setClippingFar50m(this);
    makeActorAppeared();

    if (MR::isValidInfo(rIter)) {
        MR::useStageSwitchSleep(this, rIter);

        if (MR::useStageSwitchReadAppear(this, rIter)) {
            makeActorDead();
            MR::syncStageSwitchAppear(this);
        }

        if (MR::useStageSwitchReadB(this, rIter)) {
            MR::listenStageSwitchOnB(this, MR::Functor_Inline(this, &CollisionBlocker::forceBreak));
        }
    }
}

void CollisionBlocker::forceBreak() {
    kill();
}

void CollisionBlocker::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        MR::sendArbitraryMsg(ACTMES_PUSH_FORCE, pReceiver, pSender);
    }
}

CollisionBlocker::~CollisionBlocker() {
}
