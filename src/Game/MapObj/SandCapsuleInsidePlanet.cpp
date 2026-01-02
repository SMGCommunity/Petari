#include "Game/MapObj/SandCapsuleInsidePlanet.hpp"

namespace NrvSandCapsuleInsidePlanet {
    NEW_NERVE(SandCapsuleInsidePlanetNrvWait, SandCapsuleInsidePlanet, Wait);
};

SandCapsuleInsidePlanet::SandCapsuleInsidePlanet(const char* pName) : MapObjActor(pName) {}

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
    void (SandCapsuleInsidePlanet::*offFunc)(void) = &SandCapsuleInsidePlanet::startOff;
    void (SandCapsuleInsidePlanet::*onFunc)(void) = &SandCapsuleInsidePlanet::startOn;
    MR::listenStageSwitchOnOffA(this, MR::Functor(this, onFunc), MR::Functor(this, offFunc));
}

void SandCapsuleInsidePlanet::exeWait() {}

void SandCapsuleInsidePlanet::startOn() {
    MR::startBrk(this, "On");
}

void SandCapsuleInsidePlanet::startOff() {
    MR::startBrk(this, "Off");
}

SandCapsuleInsidePlanet::~SandCapsuleInsidePlanet() {}
