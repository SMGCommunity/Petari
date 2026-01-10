#include "Game/Map/PlanetMap.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"
#include "Game/Map/OceanHomeMapCtrl.hpp"
#include <cstdio>
#include "Game/LiveActor/ModelObj.hpp"

namespace {
    const char* cFollowJointName = "Move";

    static struct PlanetMapClippingInfo sClippingInfo = {
        "PhantomShipA",
        3000.0f,
        800.0f,
        1300.0f,
        0.0f,
        0
    };
};

PlanetMap::PlanetMap(const char* pName, const char* pModelName) : LiveActor(pName),
    mModelName(pModelName),
    _90(0.0f,0.0f,0.0f),
    mLODCtrl(nullptr),
    mBloomModel(nullptr),
    mWaterModel(nullptr),
    mIndirectModel(nullptr) 
{

}

void PlanetMap::init(const JMapInfoIter& rIter) {
    MR::getObjectName(&mModelName, rIter);
    if (MR::isValidInfo(rIter)) {
        f32 v11 = -1.0f;
        MR::getJMapInfoArg0NoInit(rIter, &v11);
        f32 v12 = -1.0f;
        MR::getJMapInfoArg2NoInit(rIter, &v12);
        float unknownUse = -1.0f > v11 ? 1.0f : -1.0f;
    }
    MR::initDefaultPos(this, rIter);
    initModel(mModelName, rIter);
    MR::connectToScenePlanet(this);
    initEffectKeeper(0, nullptr, false);
    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);

    if (MR::isExistCollisionResource(this, mModelName)) {
        MtxPtr jointMTX = nullptr;
        if (MR::isExistJoint(this, ::cFollowJointName)) {
                jointMTX = MR::getJointMtx(this, ::cFollowJointName);
        }
        MR::initCollisionParts(this, mModelName, getSensor("body"), jointMTX);
    }
    MR::tryCreateCollisionMoveLimit(this, getSensor("body"));
    MR::tryCreateCollisionWaterSurface(this, getSensor("body"));
    initSound(4, false);

    if (MR::isExistAnim(this, mModelName)) {
        MR::startAllAnim(this, mModelName);
    } else if(!MR::isExistEffectTexMtx(this)) {
        calcAnim();
        MR::offCalcAnim(this);
    }
    initClipping(rIter);
    OceanHomeMapFunction::tryEntryOceanHomeMap(this);
    makeActorAppeared();
    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    }

    if(MR::tryRegisterDemoCast(this, rIter) && MR::isRegisteredDemoActionAppear(this)) {
        makeActorDead();
    }
    MR::useStageSwitchSleep(this, rIter);
}


void PlanetMap::makeActorAppeared() {
    LiveActor::makeActorAppeared();
    tryEmitMyEffect();
    mLODCtrl->validate();
}

void PlanetMap::makeActorDead() {
    mLODCtrl->invalidate();
    tryDeleteMyEffect();
    LiveActor::makeActorDead();
}

void PlanetMap::initClipping(const JMapInfoIter& rIter) {
    f32 v2 = 0.0f;
    if (MR::isValidInfo(rIter)) {
        MR::getJMapInfoArg1NoInit(rIter, &v2);
    }
    struct PlanetMapClippingInfo* info = &::sClippingInfo;

    if (MR::isEqualStringCase(mModelName, ::sClippingInfo.name)) {
        info = nullptr;
    }

    if (info) {
        TVec3f _4 = TVec3f( info->_4, info->_8, info->_C);
        TVec3f _8;
        _8.addInLine(_4);
        _90.set(_8);
        MR::setClippingTypeSphere(this, v2, &_90);
    } else {
        if (v2 <= 0.0f) {
            MR::calcModelBoundingRadius(&v2, this);
            v2 += 100.0f;
        }
        MR::setClippingTypeSphere(this, v2);
    }

    if (MR::isValidInfo(rIter)) {
        MR::setGroupClipping(this, rIter, 128);
    }
}


s32 PlanetMap::getLowMovementType() const {
    return -1;
}

void PlanetMap::initModel(const char* pModelName, const JMapInfoIter& rIter) {
    initModelManagerWithAnm(pModelName, nullptr,false);
    s32 lowMovementType = getLowMovementType();
    f32 farClippingDistance = getFarClipDistance();
    mLODCtrl = MR::createLodCtrlPlanet(this, rIter, farClippingDistance, lowMovementType);
    initBloomModel(pModelName);
    mWaterModel = MR::createWaterModel(this, nullptr);
    mIndirectModel = MR::createIndirectPlanetModel(this, nullptr);
}

void PlanetMap::initBloomModel(const char* pModelName) {
    if (!MR::isExistSubModel(pModelName, "Bloom")) {
        mBloomModel = nullptr;
        return;
    }
    char str[256];
    snprintf(str, 256, "%sBloom", mModelName);
    const char* name = mName;
    mBloomModel = MR::createModelObjBloomModel(name, str, getBaseMtx());
    mBloomModel->mScale.set(mScale);

    f32 radiusBound = 500.0f;
    MR::calcModelBoundingRadius(&radiusBound, this);
    MR::setClippingFarMax(mBloomModel);
    MR::setClippingTypeSphere(mBloomModel, radiusBound);
}

void PlanetMap::control() {
    mLODCtrl->update();
}

void PlanetMap::startClipped() {
    LiveActor::startClipped();
    tryDeleteMyEffect();
}

void PlanetMap::endClipped() {
    LiveActor::endClipped();
    tryEmitMyEffect();
}

bool PlanetMap::tryEmitMyEffect() {
    if (!MR::isRegisteredEffect(this, mModelName)) {
        return false;
    }

    MR::emitEffect(this, mModelName);
    return true;
}

bool PlanetMap::tryDeleteMyEffect() {
    if (!MR::isRegisteredEffect(this, mModelName)) {
        return false;
    }

    MR::deleteEffect(this, mModelName);
    return true;
}

FurPlanetMap::FurPlanetMap(const char* pName) : PlanetMap(pName, nullptr) {}

PlanetMap::~PlanetMap() { }

void FurPlanetMap::init(const JMapInfoIter& rIter) {
    PlanetMap::init(rIter);
    MR::initFurPlanet(this);
}

RailPlanetMap::RailPlanetMap(const char* pName) : PlanetMap(pName, nullptr) {}

void RailPlanetMap::init(const JMapInfoIter& rIter) {
    PlanetMap::init(rIter);
    initRailRider(rIter);
}

s32 PlanetMapAnimLow::getLowMovementType() const {
    return 29;
}

FurPlanetMap::~FurPlanetMap() {}
RailPlanetMap::~RailPlanetMap() {}
PlanetMapAnimLow::~PlanetMapAnimLow() {}

f32 PlanetMap::getFarClipDistance() const {
    return -1.0f;
}

void keep() {  
    TVec3f one(0.0f);
    TVec3f two(2.0f);
    one.set(two);
}
