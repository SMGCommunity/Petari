#include "Game/MapObj/CollisionBlocker.hpp"

CollisionBlocker::CollisionBlocker(const char *pName) : LiveActor(pName) {

}

void CollisionBlocker::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    initHitSensor(1);

    TVec3f sensor_radius;
    sensor_radius.x = 0.0f;
    sensor_radius.y = 0.0f;
    sensor_radius.z = 0.0f;
    MR::addHitSensorEye(this, "eye", 4, 50.0f * mScale.y, sensor_radius);
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
            MR::FunctorV0M<CollisionBlocker *, void (CollisionBlocker::*)()> breakFunc = MR::Functor<CollisionBlocker>(this, &CollisionBlocker::forceBreak);
            MR::listenStageSwitchOnB(this, breakFunc);
        }
    }
}

void CollisionBlocker::forceBreak() {
    kill();
}

void CollisionBlocker::attackSensor(HitSensor *a1, HitSensor *a2) {
    if (MR::isSensorPlayer(a2)) {
        MR::sendArbitraryMsg(42, a2, a1);
    }
}

CollisionBlocker::~CollisionBlocker() {

}
