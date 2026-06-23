#include "Game/MapObj/SandCapsuleInsidePlanet.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/LiveActorUtil.hpp"

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
