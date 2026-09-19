#include "Game/MapObj/SideSpikeMoveStep.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util.hpp"

namespace NrvSideSpikeMoveStep {
    NEW_NERVE(SideSpikeMoveStepNrvWait, SideSpikeMoveStep, Wait);
    NEW_NERVE_ONEND(SideSpikeMoveStepNrvDPDStop, SideSpikeMoveStep, DPDStop, DPDStop);
    NEW_NERVE_ONEND(SideSpikeMoveStepNrvDPDStopSync, SideSpikeMoveStep, DPDStopSync, DPDStopSync);
};  // namespace NrvSideSpikeMoveStep

SideSpikeMoveStep::SideSpikeMoveStep(const char* pName)
    : MapObjActor(pName), mScaleController(nullptr), mStateBindStarPointer(nullptr), mRailClipping(0.0f, 0.0f, 0.0f) {
}

void SideSpikeMoveStep::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    info.setupHioNode("地形オブジェ");
    info.setupDefaultPos();
    info.setupConnectToScene();
    info.setupGroupClipping(32);
    info.setupSound(6);
    info.setupRailMover();
    info.setupShadow(nullptr);
    info.setupNerve(GET_NERVE(SideSpikeMoveStep, SideSpikeMoveStepNrvWait));
    MapObjActorUtil::setupInitInfoTypical(&info, mObjectName);
    initialize(rIter, info);
    initEffectKeeper(1, mObjectName, false);
    f32 radius = 0.0f;
    MR::calcModelBoundingRadius(&radius, this);
    MR::initAndSetRailClipping(&mRailClipping, this, 100.0f, radius);
    MR::joinToGroupArray(this, rIter, nullptr, 32);
    MR::initStarPointerTarget(this, 300.0f, TVec3f(0.0f, 0.0f, 0.0f));
    mScaleController = new AnimScaleController(nullptr);
    mScaleController->setParamTight();
    mStateBindStarPointer = new WalkerStateBindStarPointer(this, mScaleController);
}

void SideSpikeMoveStep::exeWait() {
    if (MR::isFirstStep(this)) {
        MapObjActorUtil::resumeAllMapPartsFunctions(this);
    }
    if (MapObjActorUtil::isRailMoverWorking(this)) {
        MR::startLevelSound(this, "SE_OJ_LV_SIDE_SPIKE_STEP_MV");
        if (MapObjActorUtil::isRailMoverReachedEnd(this)) {
            MR::startSound(this, "SE_OJ_SIDE_SPIKE_STEP_ED");
        }
    }
    if (mStateBindStarPointer->tryStartPointBind()) {
        MR::sendMsgToGroupMember(ACTMES_GROUP_MOVE_STOP, this, getSensor("body"), "body");
        setNerve(GET_NERVE(SideSpikeMoveStep, SideSpikeMoveStepNrvDPDStop));
    }
}

void SideSpikeMoveStep::exeDPDStop() {
    if (MR::isFirstStep(this)) {
        MapObjActorUtil::pauseAllMapPartsFunctions(this);
    }
    if (MR::updateActorState(this, mStateBindStarPointer)) {
        MR::sendMsgToGroupMember(ACTMES_GROUP_MOVE_START, this, getSensor("body"), "body");
    }
}

void SideSpikeMoveStep::endDPDStop() {
    mStateBindStarPointer->kill();
}

void SideSpikeMoveStep::exeDPDStopSync() {
    if (MR::isFirstStep(this)) {
        MapObjActorUtil::pauseAllMapPartsFunctions(this);
        mScaleController->startDpdHitVibration();
    }
}

void SideSpikeMoveStep::endDPDStopSync() {
    mStateBindStarPointer->kill();
}

void SideSpikeMoveStep::calcAndSetBaseMtx() {
    MapObjActor::calcAndSetBaseMtx();
    if (MR::isInitializeStateEnd()) {
        TVec3f scale = mScaleController->_C * mScale;
        MR::setBaseScale(this, scale);
    }
}

void SideSpikeMoveStep::control() {
    mScaleController->updateNerve();
    MapObjActor::control();
}

bool SideSpikeMoveStep::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_GROUP_MOVE_STOP) {
        if (!isNerve(GET_NERVE(SideSpikeMoveStep, SideSpikeMoveStepNrvWait))) {
            return false;
        }
        setNerve(GET_NERVE(SideSpikeMoveStep, SideSpikeMoveStepNrvDPDStopSync));
        return true;
    } else if (msg == ACTMES_GROUP_MOVE_START) {
        if (isNerve(GET_NERVE(SideSpikeMoveStep, SideSpikeMoveStepNrvWait))) {
            return false;
        }
        setNerve(GET_NERVE(SideSpikeMoveStep, SideSpikeMoveStepNrvWait));
        return true;
    }

    return false;
}
