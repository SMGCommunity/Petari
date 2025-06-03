#include "Game/MapObj/SimpleClipPartsObj.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjActor.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace NrvSimpleClipPartsObj {
    NEW_NERVE(HostTypeWait, SimpleClipPartsObj, Wait);
}

SimpleClipPartsObj::SimpleClipPartsObj(const char* pName) : MapObjActor(pName) {}

SimpleClipPartsObj::~SimpleClipPartsObj() {}

void SimpleClipPartsObj::init(const JMapInfoIter &rIfter) {
    MapObjActor::init(rIfter);
    MapObjActorInitInfo info;
    info.setupRailMover();
    info.setupRotator();
    MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    MapObjActorUtil::setupInitInfoTypical(&info, mObjectName);
    MapObjActor::initialize(rIfter, info);
    HitSensor* sensor = getSensor("body");
    sensor->setType(73);
}

void SimpleClipPartsObj::control() {
    MapObjActor::control();
    if (MR::isValidSwitchA(this)) {
        if (MR::isOnSwitchA(this)) {
            MR::validateCollisionParts(this);
        } else {
            MR::invalidateCollisionParts(this);
        }
    }
}

void SimpleClipPartsObj::connectToScene(const MapObjActorInitInfo &Ifter) {
    if (Ifter.mConnectToScene) {
        MR::connectToClippedMapParts(this);
    }
}

void SimpleClipPartsObj::initCaseUseSwitchA(const MapObjActorInitInfo &Ifter) {}

void SimpleClipPartsObj::initCaseNoUseSwitchA(const MapObjActorInitInfo &Ifter) {}

void SimpleClipPartsObj::initCaseUseSwitchB(const MapObjActorInitInfo &Ifter) {
    void (MapObjActor::*start)(void) = &MapObjActor::startMapPartsFunctions;
    void (MapObjActor::*end)(void) = &MapObjActor::endMapPartsFunctions;
    MR::listenStageSwitchOnOffB(this, MR::Functor<MapObjActor>(this, start), MR::Functor<MapObjActor>(this, end));
    MapObjActor::setStateWait();
}

void SimpleClipPartsObj::initCaseNoUseSwitchB(const MapObjActorInitInfo &Ifter) {
    MapObjActor::startMapPartsFunctions();
}

void SimpleClipPartsObj::exeWait() {
    MapObjActor::exeWait();
}
