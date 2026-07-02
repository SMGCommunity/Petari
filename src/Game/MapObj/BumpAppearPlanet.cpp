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

BumpAppearPlanet::BumpAppearPlanet(const char* pName) : MapObjActor(pName) {
    _C4 = nullptr;
    _C8 = nullptr;
}

void BumpAppearPlanet::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    MapObjActorUtil::setupInitInfoPlanet(&info);
    info.setupSound(4);
    info.setupNerve(&NrvBumpAppearPlanet::BumpAppearPlanetNrvWait::sInstance);
    initialize(rIter, info);
    _C4 = MR::createCollisionPartsFromLiveActor(this, "BumpAppearPlanetB", getSensor("body"), MR::getJointMtx(this, "BumpAppearPlanetB"),
                                                (MR::CollisionScaleType)2);
    _C8 = MR::createCollisionPartsFromLiveActor(this, "BumpAppearPlanetC", getSensor("body"), MR::getJointMtx(this, "BumpAppearPlanetC"),
                                                (MR::CollisionScaleType)2);
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
        setNerve(&NrvBumpAppearPlanet::BumpAppearPlanetNrvWait::sInstance);
    }
}

void BumpAppearPlanet::control() {
    if (_C4->_CC != 0) {
        _C4->setMtx();
    }

    if (_C8->_CC != 0) {
        _C8->setMtx();
    }
}

void BumpAppearPlanet::initCaseUseSwitchA(const MapObjActorInitInfo& rInfo) {
    MR::listenStageSwitchOnOffA(this, MR::Functor(this, &BumpAppearPlanet::startBumpIn), MR::Functor(this, &BumpAppearPlanet::startBumpOut));
}

void BumpAppearPlanet::startBumpOut() {
    setNerve(&NrvBumpAppearPlanet::BumpAppearPlanetNrvBumpOut::sInstance);
}

void BumpAppearPlanet::startBumpIn() {
    setNerve(&NrvBumpAppearPlanet::BumpAppearPlanetNrvBumpIn::sInstance);
}

BumpAppearPlanet::~BumpAppearPlanet() {
}
