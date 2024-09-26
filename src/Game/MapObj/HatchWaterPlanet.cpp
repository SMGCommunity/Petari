#include "Game/MapObj/HatchWaterPlanet.hpp"
#include "Game/LiveActor/LodCtrl.hpp"

HatchWaterPlanet::HatchWaterPlanet(const char *pName) : LiveActor(pName) {
    mPlanetLODCtrl = nullptr;
    mCollisionParts = nullptr;
}

void HatchWaterPlanet::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("HatchWaterPlanet", nullptr, false);
    MR::connectToScenePlanet(this);
    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);
    MR::initCollisionParts(this, "HatchWaterPlanetBefore", getSensor(nullptr), nullptr);
    mCollisionParts = MR::createCollisionPartsFromLiveActor(this, "HatchWaterPlanetAfter", getSensor(nullptr), (MR::CollisionScaleType)2);
    MR::invalidateCollisionParts(mCollisionParts);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    if (MR::tryRegisterDemoCast(this, rIter)) {
        MR::registerDemoActionNerve(this, &NrvHatchWaterPlanet::HatchWaterPlanetNrvOpen::sInstance, nullptr);
    }

    MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);
    MR::setClippingFarMax(this);
    mPlanetLODCtrl = MR::createLodCtrlPlanet(this, rIter, -1.0f, -1);
    mPlanetLODCtrl->invalidate();
    initNerve(&NrvHatchWaterPlanet::HatchWaterPlanetNrvWait::sInstance);
    makeActorAppeared();
}

void HatchWaterPlanet::control() {
    mPlanetLODCtrl->update();
}

void HatchWaterPlanet::exeOpen() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "HatchWaterPlanet", nullptr);
        MR::startBtk(this, "HatchWaterPlanet");
        MR::startBck(mPlanetLODCtrl->_14, "HatchWaterPlanetLow", nullptr);
        MR::setBckFrameAndStop(mPlanetLODCtrl->_14, 1.0f);
        MR::startSound(this, "SE_OJ_HATCH_WATER_PNT_ST", -1, -1);
    }

    MR::startLevelSound(this, "SE_OJ_LV_HATCH_WATER_PNT_OP", -1, -1, -1);

    if (MR::isBckStopped(this)) {
        MR::startSound(this, "SE_OJ_HATCH_WATER_PNT_ED", -1, -1);
        MR::startSystemSE("SE_SY_READ_RIDDLE_S", -1, -1);
        setNerve(&NrvHatchWaterPlanet::HatchWaterPlanetNrvWaitAfterOpen::sInstance);
    }
}

void HatchWaterPlanet::exeWaitAfterOpen() {
    if (MR::isFirstStep(this)) {
        MR::invalidateCollisionParts(this);
        MR::validateCollisionParts(mCollisionParts);
    }
}

HatchWaterPlanet::~HatchWaterPlanet() {

}

namespace NrvHatchWaterPlanet {
    INIT_NERVE(HatchWaterPlanetNrvWait);
    INIT_NERVE(HatchWaterPlanetNrvOpen);
    INIT_NERVE(HatchWaterPlanetNrvWaitAfterOpen);

    void HatchWaterPlanetNrvWaitAfterOpen::execute(Spine *pSpine) const {
        HatchWaterPlanet* planet = reinterpret_cast<HatchWaterPlanet*>(pSpine->mExecutor);
        planet->exeWaitAfterOpen();
    }

    void HatchWaterPlanetNrvOpen::execute(Spine *pSpine) const {
        HatchWaterPlanet* planet = reinterpret_cast<HatchWaterPlanet*>(pSpine->mExecutor);
        planet->exeOpen();
    }

    void HatchWaterPlanetNrvWait::execute(Spine *pSpine) const {

    }
};