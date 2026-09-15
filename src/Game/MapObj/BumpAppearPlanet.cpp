#include "Game/MapObj/BumpAppearPlanet.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util.hpp"

namespace NrvBumpAppearPlanet {
    NEW_NERVE(BumpAppearPlanetNrvWait, BumpAppearPlanet, Wait);
    NEW_NERVE(BumpAppearPlanetNrvBumpOut, BumpAppearPlanet, BumpOut);
    NEW_NERVE(BumpAppearPlanetNrvBumpIn, BumpAppearPlanet, BumpIn);
};  // namespace NrvBumpAppearPlanet

BumpAppearPlanet::BumpAppearPlanet(const char* pName) : MapObjActor(pName), _C4(), _C8() {
}

void BumpAppearPlanet::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    MapObjActorUtil::setupInitInfoPlanet(&info);
    info.setupSound(4);
    info.setupNerve(GET_NERVE(BumpAppearPlanet, BumpAppearPlanetNrvWait));
    initialize(rIter, info);

    _C4 = MR::createCollisionPartsFromLiveActor(this, "BumpAppearPlanetB", getSensor("body"), MR::getJointMtx(this, "BumpAppearPlanetB"),
                                                MR::CollisionScaleType_Unk2);
    _C8 = MR::createCollisionPartsFromLiveActor(this, "BumpAppearPlanetC", getSensor("body"), MR::getJointMtx(this, "BumpAppearPlanetC"),
                                                MR::CollisionScaleType_Unk2);
    MR::validateCollisionParts(_C8);
}

void BumpAppearPlanet::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::invalidateCollisionParts(_C4);
    }
}

void BumpAppearPlanet::exeBumpOut() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "BumpAppearPlanetPush", nullptr);
        MR::startSound(this, "SE_OJ_BUMP_APPEAR_PLN_OUT");
        MR::validateCollisionParts(_C4);
    }
}

void BumpAppearPlanet::exeBumpIn() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "BumpAppearPlanetPull", nullptr);
        MR::startSound(this, "SE_OJ_BUMP_APPEAR_PLN_IN");
    }

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(BumpAppearPlanet, BumpAppearPlanetNrvWait));
    }
}

void BumpAppearPlanet::control() {
    if (_C4->_CC) {
        _C4->setMtx();
    }

    if (_C8->_CC) {
        _C8->setMtx();
    }
}

void BumpAppearPlanet::initCaseUseSwitchA(const MapObjActorInitInfo& rInfo) {
    MR::listenStageSwitchOnOffA(this, MR::Functor(this, &BumpAppearPlanet::startBumpOut), MR::Functor(this, &BumpAppearPlanet::startBumpIn));
}

void BumpAppearPlanet::startBumpOut() {
    setNerve(GET_NERVE(BumpAppearPlanet, BumpAppearPlanetNrvBumpOut));
}

void BumpAppearPlanet::startBumpIn() {
    setNerve(GET_NERVE(BumpAppearPlanet, BumpAppearPlanetNrvBumpIn));
}
