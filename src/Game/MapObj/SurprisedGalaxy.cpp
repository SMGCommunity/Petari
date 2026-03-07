#include "Game/MapObj/SurprisedGalaxy.hpp"
#include <cstring>

namespace NrvSurprisedGalaxy {
    NEW_NERVE(SurprisedGalaxyNrvExit, SurprisedGalaxy, Exit);
    NEW_NERVE(SurprisedGalaxyNrvWait, SurprisedGalaxy, Wait);
};  // namespace NrvSurprisedGalaxy

SurprisedGalaxy::SurprisedGalaxy(const char* pName) : LiveActor(pName) {}

void SurprisedGalaxy::init(const JMapInfoIter& rIter) {
    const char* name;
    MR::getObjectName(&name, rIter);
    if (!strcmp(name, "SurprisedGalaxy")) {
        mGalaxyName = "SurfingLv1Galaxy";
    } else {
        mGalaxyName = &name[strlen("Surp")];
    }
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("MiniSurprisedGalaxy", nullptr, false);
    MR::connectToSceneMapObj(this);
    f32 rad = 0.0f;
    MR::calcModelBoundingRadius(&rad, this);
    initHitSensor(1);
    MR::addHitSensorMapObj(this, "Body", 8, rad, TVec3f(0.0f, 0.0f, 0.0f));
    MR::setClippingFar200m(this);
    initNerve(&NrvSurprisedGalaxy::SurprisedGalaxyNrvWait::sInstance);
    MR::useStageSwitchSleep(this, rIter);
    MR::startAction(this, "MiniSurprisedGalaxy");
    MR::registerDemoSimpleCastAll(this);
    if (MR::useStageSwitchReadAppear(this, rIter)) {
        makeActorDead();
        MR::syncStageSwitchAppear(this);
    } else {
        makeActorAppeared();
        if (MR::isGalaxyAppearGreenDriver(mGalaxyName)) {
            if (!MR::isOnGameEventFlagGreenDriver()) {
                makeActorDead();
            }
        } else if (!MR::isOnGameEventFlagViewCompleteEndingMarioAndLuigi()) {
            makeActorDead();
        }
    }
}

void SurprisedGalaxy::appear() {
    LiveActor::appear();
}

void SurprisedGalaxy::kill() {
    LiveActor::kill();
}

void SurprisedGalaxy::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayerOrRide(pReceiver)) {
        if (!isNerve(&NrvSurprisedGalaxy::SurprisedGalaxyNrvExit::sInstance)) {
            setNerve(&NrvSurprisedGalaxy::SurprisedGalaxyNrvExit::sInstance);
        }
    }
}

void SurprisedGalaxy::exeExit() {
    if (MR::isFirstStep(this)) {
        MR::submitTrigSE();
        MR::closeSystemWipeWhiteFade(-1);
        MR::stopStageBGM(90);
        MR::stopSubBGM(90);
        MR::tryStartDemo(this, "ギャラクシー移動");
    }

    if (!MR::isSystemWipeActive()) {
        MR::onGameEventFlagGalaxyOpen(mGalaxyName);
        MR::endDemo(this, "ギャラクシー移動");
        MR::permitTrigSE();
        MR::requestStartScenarioSelect(mGalaxyName);
    }
}

void SurprisedGalaxy::exeWait() {}

bool SurprisedGalaxy::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    switch (msg) {
    case 152:
        if (isNerve(&NrvSurprisedGalaxy::SurprisedGalaxyNrvWait::sInstance)) {
            setNerve(&NrvSurprisedGalaxy::SurprisedGalaxyNrvExit::sInstance);
        }
        return false;
    default:
        break;
    }
    return false;
}
