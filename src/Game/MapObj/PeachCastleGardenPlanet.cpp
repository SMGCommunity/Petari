#include "Game/MapObj/PeachCastleGardenPlanet.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjActor.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include <cstddef>

namespace NrvPeachCastleGardenPlanet {
    NEW_NERVE(PeachCastleGardenPlanetNrvWait, PeachCastleGardenPlanet, Wait);
    NEW_NERVE(PeachCastleGardenPlanetNrvDamage, PeachCastleGardenPlanet, Damage);
} // namespace NrvPeachCastleGardenPlanet

PeachCastleGardenPlanet::PeachCastleGardenPlanet(const char* pName)
    : MapObjActor(pName) {
    _C4 = 0;
}

PeachCastleGardenPlanet::~PeachCastleGardenPlanet() {}

void PeachCastleGardenPlanet::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    MapObjActorUtil::setupInitInfoPlanet(&info);
    info.setupNerve(&NrvPeachCastleGardenPlanet::PeachCastleGardenPlanetNrvWait::sInstance);
    MapObjActor::initialize(rIter, info);
    _C4 = MR::createIndirectPlanetModel(this, nullptr);
}

void PeachCastleGardenPlanet::startDamage() {
    setNerve(&NrvPeachCastleGardenPlanet::PeachCastleGardenPlanetNrvDamage::sInstance);
}

void PeachCastleGardenPlanet::connectToScene(const MapObjActorInitInfo& Ifter) {
    MR::connectToScenePlanet(this);
}

void PeachCastleGardenPlanet::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "Before");
    }
}

void PeachCastleGardenPlanet::exeDamage() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "After");
    }
}

void PeachCastleGardenPlanet::initCaseUseSwitchA(const MapObjActorInitInfo& Ifter) {
    MR::listenStageSwitchOnA(this, MR::FunctorV0M<PeachCastleGardenPlanet*, void (PeachCastleGardenPlanet::*)(void)>(this, &PeachCastleGardenPlanet::startDamage));
}
