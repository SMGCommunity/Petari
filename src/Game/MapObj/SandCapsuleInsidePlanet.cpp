#include "Game/MapObj/SandCapsuleInsidePlanet.hpp"

namespace NrvSandCapsuleInsidePlanet {
    NEW_NERVE(SandCapsuleInsidePlanetNrvWait, SandCapsuleInsidePlanet, Wait);
};  // namespace NrvSandCapsuleInsidePlanet

SandCapsuleInsidePlanet::SandCapsuleInsidePlanet(const char* pName) : MapObjActor(pName) {
}

void SandCapsuleInsidePlanet::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    MapObjActorUtil::setupInitInfoPlanet(&info);
    info.setupNerve(&NrvSandCapsuleInsidePlanet::SandCapsuleInsidePlanetNrvWait::sInstance);
    initialize(rIter, info);
    MR::startBrk(this, "Off");
    MR::setBrkFrameEndAndStop(this);
}

void SandCapsuleInsidePlanet::initCaseUseSwitchA(const MapObjActorInitInfo& rInfo) {
    MR::listenStageSwitchOnOffA(this, MR::Functor(this, &SandCapsuleInsidePlanet::startOn), MR::Functor(this, &SandCapsuleInsidePlanet::startOff));
}

void SandCapsuleInsidePlanet::exeWait() {
}

void SandCapsuleInsidePlanet::startOn() {
    MR::startBrk(this, "On");
}

void SandCapsuleInsidePlanet::startOff() {
    MR::startBrk(this, "Off");
}

SandCapsuleInsidePlanet::~SandCapsuleInsidePlanet() {
}
