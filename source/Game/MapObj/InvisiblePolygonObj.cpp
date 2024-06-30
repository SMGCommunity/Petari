#include "Game/MapObj/InvisiblePolygonObj.hpp"
#include <cstdio>

InvisiblePolygonObj::InvisiblePolygonObj(const char *pName) : LiveActor(pName) {
    mMatrix.identity();
}

void InvisiblePolygonObj::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    initCollision(rIter);
    MR::connectToSceneMapObj(this);
    MR::setClippingTypeSphere(this, MR::getCollisionBoundingSphereRange(this));
    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this); 
        makeActorDead();
    }
    else {   
        makeActorAppeared();
    }
}

void InvisiblePolygonObj::initBaseMtx() {
    if (0.0f == mRotation.x && 0.0f == mRotation.z) {
        MR::makeMtxTransRotateY(mMatrix.toMtxPtr(), this);
    }
    else {
        MR::makeMtxTR(mMatrix.toMtxPtr(), this);
    }
}

void InvisiblePolygonObj::initCollision(const JMapInfoIter &rIter) {
    InvisiblePolygonObj::initBaseMtx();
    const char* name = nullptr;
    char pName[0x40];
    MR::getObjectName(&name, rIter);
    snprintf(pName, sizeof(pName), "%s.arc", name); 
    ResourceHolder *resource = MR::createAndAddResourceHolder(pName);
    initHitSensor(1);
    HitSensor* sensor = MR::addBodyMessageSensorMapObj(this);
    MR::initCollisionPartsFromResourceHolder(this, name, sensor, resource, nullptr);
}

MtxPtr InvisiblePolygonObj::getBaseMtx() const {
    return (MtxPtr)&mMatrix;
}