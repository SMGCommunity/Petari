#include "Game/MapObj/InvisiblePolygonObj.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include <cstdio>

InvisiblePolygonObj::InvisiblePolygonObj(const char* pName) : LiveActor(pName) {
    mBaseMtx.identity();
}

void InvisiblePolygonObj::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);

    initCollision(rIter);
    
    MR::connectToSceneMapObjMovement(this);
    MR::setClippingTypeSphere(this, MR::getCollisionBoundingSphereRange(this));

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    } else {
        makeActorAppeared();
    }
}

void InvisiblePolygonObj::initBaseMtx() {
    if (0.0f == mRotation.x && 0.0f == mRotation.z) {
        MR::makeMtxTransRotateY(mBaseMtx.toMtxPtr(), this);
    } else {
        MR::makeMtxTR(mBaseMtx.toMtxPtr(), this);
    }
}

InvisiblePolygonObj::~InvisiblePolygonObj() {
}

void InvisiblePolygonObj::initCollision(const JMapInfoIter& rIter) {
    initBaseMtx();

    const char* pName = nullptr;
    char fileName[0x40];
    MR::getObjectName(&pName, rIter);
    snprintf(fileName, sizeof(fileName), "%s.arc", pName);

    ResourceHolder* resource = MR::createAndAddResourceHolder(fileName);

    initHitSensor(1);

    HitSensor* sensor = MR::addBodyMessageSensorMapObj(this);

    MR::initCollisionPartsFromResourceHolder(this, pName, sensor, resource, nullptr);
}
