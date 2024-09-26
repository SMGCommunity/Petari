#include "Game/MapObj/MapParts.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "math_types.hpp"

namespace {
    const char* cFollowjointName = "Move";
};

MapParts::~MapParts() {

}

MapParts::MapParts(const char *pName) : LiveActor(pName) {
    _8C.setZero();
}

void MapParts::init(const JMapInfoIter &rIter) {
    if (MR::isConnectedWithRail(rIter)) {
        initRailRider(rIter);
    }

    MR::initDefaultPosNoRepeat(this, rIter);
    initModelAndCollision(rIter);
    connectToScene();
    MR::initMapPartsClipping(this, rIter, &_8C, false);
    MR::joinToGroupArray(this, rIter, "MapParts", 0x40);
    initSound(4, false);
}

void MapParts::appear() {
    LiveActor::appear();
    MR::tryStartAllAnim(this, MR::getModelResName(this));
}

s32 MapParts::getSensorNumMax() const {
    return 0;
}

s32 MapParts::getMoveStartSignalTime() {
    return 0x32;
}

void MapParts::connectToScene() {
    MR::connectToSceneMapParts(this);
}

void MapParts::initModelAndCollision(const JMapInfoIter &rIter) {
    char name[0x30];
    MR::getMapPartsObjectName(name, 0x30, rIter);
    initModelManagerWithAnm(name, nullptr, false);
    initHitSensor(2);
    TVec3f dist;
    TVec3f lerpVec;
    TVec3f sensor_offs;
    sensor_offs.x = 0.0f;
    sensor_offs.y = 0.0f;
    sensor_offs.z = 0.0f;
    u32 sensorNum = getSensorNumMax();
    HitSensor* sensor = MR::addHitSensorMapObj(this, "body", sensorNum, 100.0f, sensor_offs);
    if (MR::isExistJoint(this, cFollowjointName)) {
        MtxPtr jointMtx = MR::getJointMtx(this, cFollowjointName);
        MR::initCollisionParts(this, name, sensor, jointMtx);
        MR::tryCreateCollisionAllOtherCategory(this, jointMtx, sensor, nullptr, nullptr, nullptr);
    }
    else {
        MR::initCollisionParts(this, name, sensor, nullptr);
        MR::tryCreateCollisionAllOtherCategory(this, sensor, nullptr, nullptr, nullptr);
    }

    f32 sensorRange;
    if (MR::getJ3DModel(this)) {
        TBox3f box;
        MR::calcModelBoundingBox(&box, this);
        JMathInlineVEC::PSVECSubtract(box.mMax.toCVec(), box.mMin.toCVec(), dist.toVec());
        sensorRange = 0.5f * PSVECMag(dist.toCVec());
        JMAVECLerp(box.mMax.toCVec(), box.mMin.toCVec(), lerpVec.toVec(), 0.5f);
        TVec3f trueSensorOffset;
        trueSensorOffset.setInlinePS(lerpVec);
        JMathInlineVEC::PSVECSubtract(trueSensorOffset.toCVec(), mPosition.toCVec(), trueSensorOffset.toVec());
        MR::setSensorOffset(this, "body", trueSensorOffset);
    }
    else {
        sensorRange = MR::getCollisionBoundingSphereRange(this);
    }

    sensor->mRadius = sensorRange;
}