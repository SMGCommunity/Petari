#include "Game/MapObj/SwingLight.hpp"
#include "Game/LiveActor/VolumeModelDrawer.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util.hpp"
#include <cstdio>

namespace {
    const char* getGlaringLightModelName(const char* pName) {
        if (MR::isEqualString(pName, "TeresaMansionLightA")) {
            return "TeresaMansionGlaringLightA";
        }

        if (MR::isEqualString(pName, "TeresaMansionLightB")) {
            return "TeresaMansionGlaringLightB";
        }

        if (MR::isEqualString(pName, "SandRiverLightA")) {
            return "SandRiverGlaringLightA";
        }

        if (MR::isEqualString(pName, "GravityLightA")) {
            return "GravityLightA";
        }

        return nullptr;
    }
}  // namespace

LightCylinder::LightCylinder(const char* pName, MtxPtr pHostMtx, const Color8& rColor)
    : MapObjActor(pName), mHostMtx(pHostMtx), mLightVolume(), mColor() {
    if (rColor == 0) {
        GXColor color = {255, 218, 100, 160};
        mColor.set(color);
    } else {
        mColor.set(rColor);
    }
}

void LightCylinder::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    info.setupHitSensor();
    info.setupBaseMtxFollowTarget();
    info.setupRotator();
    info.setupRailMover();
    info.setupNoAppearRiddleSE();
    initialize(rIter, info);
    initLightVolume(nullptr);
}

void LightCylinder::initLightCylinderByOwner(const JMapInfoIter& rIter) {
    MapObjActorInitInfo info;
    const char* objectName;
    MR::getObjectName(&objectName, rIter);
    info.setupModelName(getGlaringLightModelName(objectName));
    MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    info.setupNoAppearRiddleSE();
    initialize(rIter, info);
    initLightVolume(objectName);
}

void LightCylinder::initLightVolume(const char* pName) {
    char modelName[0x100];
    snprintf(modelName, sizeof(modelName), "%sLightVolume", pName ? pName : mObjectName);
    VolumeModelDrawer* lightVolume = new VolumeModelDrawer("光", modelName, mHostMtx ? mHostMtx : getBaseMtx());
    mLightVolume = lightVolume;
    lightVolume->mColor.set(mColor);
}

void LightCylinder::calcAndSetBaseMtx() {
    if (mHostMtx) {
        MR::setBaseTRMtx(this, mHostMtx);
    } else {
        MapObjActor::calcAndSetBaseMtx();
    }
}

void LightCylinder::draw() const {
    mLightVolume->draw();
}

void LightCylinder::connectToScene(const MapObjActorInitInfo&) {
    MR::connectToScene(this, MR::MovementType_MapObj, MR::CalcAnimType_MapObj, MR::DrawBufferType_MapObj, MR::DrawType_VolumeModel);
}

SwingLight::SwingLight(const char* pName) : MapObjActor(pName), mLightCylinder() {
}

void SwingLight::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    info.setupHitSensor();
    info.setupBaseMtxFollowTarget();
    info.setupRotator();
    info.setupRailMover();
    info.setupClippingRadius(2000.0f);
    info.setupNoAppearRiddleSE();
    initialize(rIter, info);
}

void SwingLight::initCaseUseSwitchA(const MapObjActorInitInfo&) {
    MR::listenStageSwitchOnA(this, MR::Functor_Inline(this, &SwingLight::appearLight));
}

void SwingLight::initCaseNoUseSwitchA(const MapObjActorInitInfo&) {
    appearLight();
}

void SwingLight::makeSubModels(const JMapInfoIter& rIter, const MapObjActorInitInfo&) {
    const char* modelName = getGlaringLightModelName(mObjectName);
    mLightCylinder = new LightCylinder(modelName, getBaseMtx(), Color8(0));
    mLightCylinder->initLightCylinderByOwner(rIter);
    mLightCylinder->makeActorDead();
}

void SwingLight::appearLight() {
    mLightCylinder->appear();
    MR::tryStartAllAnim(mLightCylinder, mLightCylinder->mName);
}
