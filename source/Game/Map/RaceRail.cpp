#include "Game/Map/RaceRail.h"
#include "Game/Util.h"

RaceRail::RaceRail(const char *pName) : NameObj(pName) {

}

PlayerRacer::PlayerRacer(const char *pName) : LiveActor(pName) {
    _B0 = 0; 
    _B4 = 0;
    mCameraInfo = 0; 
}

void PlayerRacer::appear() {
    RaceManagerFunction::startRaceImmediately();
}

void PlayerRacer::init(const JMapInfoIter &rIter) {
    MR::connectToSceneMapObjMovement(this);
    initRailRider(rIter);
    mCameraInfo = MR::createActorCameraInfo(rIter);
    MR::initActorCamera(this, rIter, &mCameraInfo);

    bool usesAppear = MR::useStageSwitchReadAppear(this, rIter);

    if (usesAppear) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    }
    else {
        makeActorAppeared();
    }

    MR::invalidateClipping(this);
}

void PlayerRacer::startRacer() {
    MR::endActorCamera(this, mCameraInfo, true, -1);
}

// PlayerRacer::updateRacer

bool PlayerRacer::goalRacer() {
    return true;
}

void PlayerRacer::loseRacer() {

}

void PlayerRacer::exitRacer() {

}

PlayerRacer::~PlayerRacer() {

}

RaceRail::~RaceRail() {

}

void RaceRail::init(const JMapInfoIter &rIter) {
    mPlayerRacer = new PlayerRacer("レース判定用レール");
    mPlayerRacer->init(rIter);
    RaceManagerFunction::entryRacerPlayer(mPlayerRacer);
}

void PlayerRacer::initRacer() {
    AbstractRacer::initRacer();
    MR::startBckPlayer("Watch", (char*)nullptr);
}