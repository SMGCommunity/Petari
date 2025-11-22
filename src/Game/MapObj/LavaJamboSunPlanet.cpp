#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapPartsUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "math_types.hpp"
#include "revolution/types.h"
#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/MapObj/LavaJamboSunPlanet.hpp"

namespace NrvLavaJamboSunPlanet {
    NEW_NERVE(HostTypeWait, LavaJamboSunPlanet, Wait);
    NEW_NERVE(HostTypePause, LavaJamboSunPlanet, Wait);
    NEW_NERVE(HostTypeMove, LavaJamboSunPlanet, Move);
}; // namespace NrvLavaJamboSunPlanet

LavaJamboSunPlanet::LavaJamboSunPlanet(const char* pName)
    : LiveActor(pName) {
    mBloomModel = nullptr;
    mLodCtrl = nullptr;
    _94 = 0.0f;
    _98.setPSZeroVec();
}

void LavaJamboSunPlanet::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("LavaJamboSunPlanet", nullptr, false);
    initHitSensor(1);
    HitSensor* bodySensor = MR::addBodyMessageSensorMapObj(this);
    MR::initCollisionParts(this, "LavaJamboSunPlanet", bodySensor, nullptr);
    initNerve(&NrvLavaJamboSunPlanet::HostTypeWait::sInstance);
    initRailRider(rIter);
    MR::initAndSetRailClipping(&_98, this, 100.0f, 500.0f);
    MR::needStageSwitchReadA(this, rIter);
    MR::needStageSwitchReadB(this, rIter);
    MR::connectToScenePlanet(this);
    mBloomModel = MR::createBloomModel(this, getBaseMtx());
    initSound(4, false);
    MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);
    MR::setClippingFarMax(this);
    MR::startBtk(this, "LavaJamboSunPlanet");
    mLodCtrl = MR::createLodCtrlPlanet(this, rIter, -1.0f, -1);
    MR::tryRegisterDemoCast(this, rIter);
    makeActorAppeared();
}

void LavaJamboSunPlanet::initAfterPlacement() {
    MR::moveCoordAndTransToRailPoint(this, 0);
}

void LavaJamboSunPlanet::control() {
    mLodCtrl->update();
}

void LavaJamboSunPlanet::exeWait() {
    if (isNerve(&NrvLavaJamboSunPlanet::HostTypeWait::sInstance) && MR::isOnSwitchA(this)) {
        setNerve(&NrvLavaJamboSunPlanet::HostTypeMove::sInstance);
        return;
    }
    if (isNerve(&NrvLavaJamboSunPlanet::HostTypePause::sInstance) && MR::isOnSwitchB(this)) {
        setNerve(&NrvLavaJamboSunPlanet::HostTypeMove::sInstance);
    }
}

void LavaJamboSunPlanet::exeMove() {
    MR::getMapPartsArgMoveSpeed(&_94, this);
    _94 *= 0.1f;
    MR::moveCoordAndFollowTrans(this, _94);
    MR::startLevelSound(this, "SE_AT_LV_JUMBO_SUN_MOVE", -1, -1, -1);
    s32 v3 = -1;
    MR::getCurrentRailPointArg1NoInit(this, &v3);
    if (v3 == 1) {
        setNerve(&NrvLavaJamboSunPlanet::HostTypePause::sInstance);
    } else if (MR::isRailReachedGoal(this)) {
        setNerve(&NrvLavaJamboSunPlanet::HostTypeWait::sInstance);
    }
}

LavaJamboSunPlanet::~LavaJamboSunPlanet() {
}
