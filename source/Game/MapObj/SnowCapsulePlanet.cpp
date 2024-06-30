#include "Game/MapObj/SnowCapsulePlanet.hpp"
#include "Game/Map/CollisionParts.hpp"

SnowCapsulePlanet::SnowCapsulePlanet(const char *pName) : MapObjActor(pName) {
    mDoor1 = nullptr;
    mDoor2 = nullptr;
}

SnowCapsulePlanet::~SnowCapsulePlanet() {

}

void SnowCapsulePlanet::init(const JMapInfoIter &rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    MapObjActorUtil::setupInitInfoPlanet(&info);
    info.setupHitSensor();
    info.setupSound(6);
    info.setupNerve(&NrvSnowCapsulePlanet::SnowCapsulePlanetNrvCloseWait::sInstance);
    initialize(rIter, info);
    mDoor1 = MR::createCollisionPartsFromLiveActor(this, "Open", getSensor("body"), (MR::CollisionScaleType)2);
    mDoor2 = MR::createCollisionPartsFromLiveActor(this, "Close", getSensor("body"), (MR::CollisionScaleType)2);
    MR::validateCollisionParts(mDoor2);
    if ( MR::isDemoCast(this, nullptr))
    {
        MR::tryRegisterDemoActionNerve(this, &NrvSnowCapsulePlanet::SnowCapsulePlanetNrvOpen::sInstance, nullptr);
    }
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
        MR::startSystemSE("SE_SY_READ_RIDDLE_S", -1, -1);
        setNerve(&NrvSnowCapsulePlanet::SnowCapsulePlanetNrvOpenWait::sInstance);
    }
}

namespace NrvSnowCapsulePlanet {
    INIT_NERVE(SnowCapsulePlanetNrvCloseWait);
    INIT_NERVE(SnowCapsulePlanetNrvOpenWait);
    INIT_NERVE(SnowCapsulePlanetNrvOpen);

	void SnowCapsulePlanetNrvCloseWait::execute(Spine *pSpine) const {
	}    

	void SnowCapsulePlanetNrvOpenWait::execute(Spine *pSpine) const {
	}    

	void SnowCapsulePlanetNrvOpen::execute(Spine *pSpine) const {
        SnowCapsulePlanet* planet = reinterpret_cast<SnowCapsulePlanet*>(pSpine->mExecutor);
		planet->exeOpen();
	}        
};

