#include "Game/MapObj/ReverseGravityRoomPlanet.hpp"

namespace NrvReverseGravityRoomPlanet {
    NEW_NERVE(ReverseGravityRoomPlanetNrvDownStart, ReverseGravityRoomPlanet, DownStart);
    NEW_NERVE(ReverseGravityRoomPlanetNrvDownWait, ReverseGravityRoomPlanet, DownWait);
    NEW_NERVE(ReverseGravityRoomPlanetNrvUpStart, ReverseGravityRoomPlanet, UpStart);
    NEW_NERVE(ReverseGravityRoomPlanetNrvUpWait, ReverseGravityRoomPlanet, UpWait);
};  // namespace NrvReverseGravityRoomPlanet

ReverseGravityRoomPlanet::ReverseGravityRoomPlanet(const char* pName) : MapObjActor(pName) {}

void ReverseGravityRoomPlanet::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    info.setupHioNode("惑星");
    info.setupDefaultPos();
    info.setupConnectToScene();
    info.setupEffect(0);
    info.setupFarClipping(-1.0f);
    info.setupNerve(&NrvReverseGravityRoomPlanet::ReverseGravityRoomPlanetNrvDownWait::sInstance);
    initialize(rIter, info);
}

void ReverseGravityRoomPlanet::exeDownStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "ChangeRed", 0);
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvReverseGravityRoomPlanet::ReverseGravityRoomPlanetNrvDownWait::sInstance);
    }
}

void ReverseGravityRoomPlanet::exeDownWait() {}

void ReverseGravityRoomPlanet::exeUpStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "ChangeBlue", 0);
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvReverseGravityRoomPlanet::ReverseGravityRoomPlanetNrvUpWait::sInstance);
    }
}

void ReverseGravityRoomPlanet::exeUpWait() {}

void ReverseGravityRoomPlanet::initCaseUseSwitchA(const MapObjActorInitInfo& rIter) {
    void (ReverseGravityRoomPlanet::*startOff)(void) = &ReverseGravityRoomPlanet::startSwitchOff;
    void (ReverseGravityRoomPlanet::*startOn)(void) = &ReverseGravityRoomPlanet::startSwitchOn;
    MR::listenStageSwitchOnOffA(this, MR::Functor(this, startOff), MR::Functor(this, startOn));
}

void ReverseGravityRoomPlanet::startSwitchOn() {
    setNerve(&NrvReverseGravityRoomPlanet::ReverseGravityRoomPlanetNrvUpStart::sInstance);
}

void ReverseGravityRoomPlanet::startSwitchOff() {
    setNerve(&NrvReverseGravityRoomPlanet::ReverseGravityRoomPlanetNrvDownStart::sInstance);
}

ReverseGravityRoomPlanet::~ReverseGravityRoomPlanet() {}
