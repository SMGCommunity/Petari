#include "Game/MapObj/SimpleClipPartsObj.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjActor.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace NrvSimpleClipPartsObj {
    NEW_NERVE(HostTypeWait, SimpleClipPartsObj, Wait);
};  // namespace NrvSimpleClipPartsObj

SimpleClipPartsObj::SimpleClipPartsObj(const char* pName) : MapObjActor(pName) {
}

SimpleClipPartsObj::~SimpleClipPartsObj() {
}

void SimpleClipPartsObj::init(const JMapInfoIter& rrIter) {
    MapObjActor::init(rrIter);
    MapObjActorInitInfo info;
    info.setupRailMover();
    info.setupRotator();
    MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    MapObjActorUtil::setupInitInfoTypical(&info, mObjectName);
    MapObjActor::initialize(rrIter, info);
    getSensor("body")->setType(73);
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

void SimpleClipPartsObj::connectToScene(const MapObjActorInitInfo& rIter) {
    if (rIter.mConnectToScene) {
        MR::connectToClippedMapParts(this);
    }
}

void SimpleClipPartsObj::initCaseUseSwitchA(const MapObjActorInitInfo& rIter) {
}

void SimpleClipPartsObj::initCaseNoUseSwitchA(const MapObjActorInitInfo& rIter) {
}

void SimpleClipPartsObj::initCaseUseSwitchB(const MapObjActorInitInfo& rIter) {
    MR::listenStageSwitchOnOffB(this, MR::Functor< MapObjActor >(this, &MapObjActor::startMapPartsFunctions),
                                MR::Functor< MapObjActor >(this, &MapObjActor::endMapPartsFunctions));
    setStateWait();
}

void SimpleClipPartsObj::initCaseNoUseSwitchB(const MapObjActorInitInfo& rIter) {
    MapObjActor::startMapPartsFunctions();
}

void SimpleClipPartsObj::exeWait() {
    MapObjActor::exeWait();
}
