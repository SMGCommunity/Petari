#include "Game/MapObj/QuarterRollGravityRoomArrow.hpp"
#include "Game/LiveActor/Spine.hpp"
#include "Game/MapObj/MapObjActor.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"

QuarterRollGravityRoomArrow::QuarterRollGravityRoomArrow(const char* pName) : MapObjActor(pName) {}

void QuarterRollGravityRoomArrow::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info = MapObjActorInitInfo();
    MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    info.setupNerve(&NrvQuarterRollGravityRoomArrow::QuarterRollGravityRoomArrowNrvWait::sInstance);
    initialize(rIter, info);
}

void QuarterRollGravityRoomArrow::exeRollOnA() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "RollGravityA", 0);
    }
}

void QuarterRollGravityRoomArrow::exeRollOffA() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "RollGravityAReverse", 0);
    }
    if (MR::isBckStopped(this)) {
        setNerve(&NrvQuarterRollGravityRoomArrow::QuarterRollGravityRoomArrowNrvWait::sInstance);
    }
}

void QuarterRollGravityRoomArrow::exeRollOnB() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "RollGravityB", 0);
    }
}

void QuarterRollGravityRoomArrow::exeRollOffB() {
    if (MR::isFirstStep(this))
        MR::startBck(this, "RollGravityBReverse", 0);
    if (MR::isBckStopped(this))
        setNerve(&NrvQuarterRollGravityRoomArrow::QuarterRollGravityRoomArrowNrvWait::sInstance);
}

void QuarterRollGravityRoomArrow::initCaseUseSwitchA(const MapObjActorInitInfo& rInfo) {
    MR::listenStageSwitchOnOffA(this, MR::Functor(this, &QuarterRollGravityRoomArrow::onRollA),
                                MR::Functor(this, &QuarterRollGravityRoomArrow::offRollA));
}

void QuarterRollGravityRoomArrow::initCaseNoUseSwitchA(const MapObjActorInitInfo& rInfo) {}

void QuarterRollGravityRoomArrow::initCaseUseSwitchB(const MapObjActorInitInfo& rInfo) {
    MR::listenStageSwitchOnOffB(this, MR::Functor(this, &QuarterRollGravityRoomArrow::onRollB),
                                MR::Functor(this, &QuarterRollGravityRoomArrow::offRollB));
}
void QuarterRollGravityRoomArrow::initCaseNoUseSwitchB(const MapObjActorInitInfo& rInfo) {}

void QuarterRollGravityRoomArrow::onRollA() {
    setNerve(&NrvQuarterRollGravityRoomArrow::QuarterRollGravityRoomArrowNrvRollOnA::sInstance);
}

void QuarterRollGravityRoomArrow::offRollA() {
    setNerve(&NrvQuarterRollGravityRoomArrow::QuarterRollGravityRoomArrowNrvRollOffA::sInstance);
}

void QuarterRollGravityRoomArrow::onRollB() {
    setNerve(&NrvQuarterRollGravityRoomArrow::QuarterRollGravityRoomArrowNrvRollOnB::sInstance);
}

void QuarterRollGravityRoomArrow::offRollB() {
    setNerve(&NrvQuarterRollGravityRoomArrow::QuarterRollGravityRoomArrowNrvRollOffB::sInstance);
}

QuarterRollGravityRoomArrow::~QuarterRollGravityRoomArrow() {}

namespace NrvQuarterRollGravityRoomArrow {
    QuarterRollGravityRoomArrowNrvWait(QuarterRollGravityRoomArrowNrvWait::sInstance);
    QuarterRollGravityRoomArrowNrvRollOnA(QuarterRollGravityRoomArrowNrvRollOnA::sInstance);
    QuarterRollGravityRoomArrowNrvRollOffA(QuarterRollGravityRoomArrowNrvRollOffA::sInstance);
    QuarterRollGravityRoomArrowNrvRollOnB(QuarterRollGravityRoomArrowNrvRollOnB::sInstance);
    QuarterRollGravityRoomArrowNrvRollOffB(QuarterRollGravityRoomArrowNrvRollOffB::sInstance);
}  // namespace NrvQuarterRollGravityRoomArrow
