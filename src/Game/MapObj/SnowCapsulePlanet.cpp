#include "Game/MapObj/SnowCapsulePlanet.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util.hpp"

namespace NrvSnowCapsulePlanet {
    NEW_NERVE(SnowCapsulePlanetNrvCloseWait, SnowCapsulePlanet, CloseWait);
    NEW_NERVE(SnowCapsulePlanetNrvOpenWait, SnowCapsulePlanet, OpenWait);
    NEW_NERVE(SnowCapsulePlanetNrvOpen, SnowCapsulePlanet, Open);
};  // namespace NrvSnowCapsulePlanet

SnowCapsulePlanet::SnowCapsulePlanet(const char* pName) : MapObjActor(pName) {
    mDoor1 = nullptr;
    mDoor2 = nullptr;
}

SnowCapsulePlanet::~SnowCapsulePlanet() {
}

void SnowCapsulePlanet::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    MapObjActorUtil::setupInitInfoPlanet(&info);
    info.setupHitSensor();
    info.setupSound(6);
    info.setupNerve(&NrvSnowCapsulePlanet::SnowCapsulePlanetNrvCloseWait::sInstance);
    initialize(rIter, info);
    mDoor1 = MR::createCollisionPartsFromLiveActor(this, "Open", getSensor("body"), MR::CollisionScaleType_Unk2);
    mDoor2 = MR::createCollisionPartsFromLiveActor(this, "Close", getSensor("body"), MR::CollisionScaleType_Unk2);
    MR::validateCollisionParts(mDoor2);
    if (MR::isDemoCast(this, nullptr)) {
        MR::tryRegisterDemoActionNerve(this, &NrvSnowCapsulePlanet::SnowCapsulePlanetNrvOpen::sInstance, nullptr);
    }
}

void SnowCapsulePlanet::exeCloseWait() {
}

void SnowCapsulePlanet::exeOpenWait() {
}

void SnowCapsulePlanet::exeOpen() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Open", nullptr);
        MR::invalidateCollisionParts(mDoor2);
        MR::validateCollisionParts(mDoor1);
        MR::shakeCameraWeak();
        MR::tryRumblePadMiddle(this, 0);
    }

    if (MR::isBckStopped(this)) {
        MR::startSystemSE("SE_SY_READ_RIDDLE_S");
        setNerve(&NrvSnowCapsulePlanet::SnowCapsulePlanetNrvOpenWait::sInstance);
    }
}
