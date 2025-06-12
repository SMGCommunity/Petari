#include "Game/MapObj/CannonFortressBreakStep.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util/Functor.hpp"

namespace NrvCannonFortressBreakStep {
    NEW_NERVE(CannonFortressBreakStepNrvWait, CannonFortressBreakStep, Wait);
    NEW_NERVE(CannonFortressBreakStepNrvFallStart, CannonFortressBreakStep, FallStart);
    NEW_NERVE(CannonFortressBreakStepNrvFall, CannonFortressBreakStep, Fall);
    NEW_NERVE(CannonFortressBreakStepNrvBreak, CannonFortressBreakStep, Break);
};

CannonFortressBreakStep::CannonFortressBreakStep(const char *pName) : MapObjActor(pName) {
    
}

void CannonFortressBreakStep::init(const JMapInfoIter &rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    info.setupHioNode("地形オブジェ");
    info.setupDefaultPos();
    info.setupConnectToScene();
    info.setupRotator();
    info.setupEffect(nullptr);
    info.setupSound(6);
    info.setupNerve(&NrvCannonFortressBreakStep::CannonFortressBreakStepNrvWait::sInstance);
    initialize(rIter, info);
    MapObjActorUtil::startAllMapPartsFunctions(this);
}

void CannonFortressBreakStep::exeWait() {
    if (!MR::isEqualString("CannonFortressBreakStep", mObjectName)) {
        MR::startLevelSound(this, "SE_OJ_LV_CNFORT_BKSTEP_ROT", -1, -1, -1);
    }
}

void CannonFortressBreakStep::exeFallStart() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_CNFORT_BKSTEP_FALL_ST", -1, -1);
    }

    setNerve(&NrvCannonFortressBreakStep::CannonFortressBreakStepNrvFall::sInstance);
}

void CannonFortressBreakStep::exeFall() {
    TVec3f up;
    MR::calcUpVec(&up, this);
    mVelocity.scale(-15.0f, up);
    MR::startLevelSound(this, "SE_OJ_LV_CNFORT_BKSTEP_FALL", -1, -1, -1);

    if (MR::isStep(this, 350)) {
        setNerve(&NrvCannonFortressBreakStep::CannonFortressBreakStepNrvBreak::sInstance);
    }
}


void CannonFortressBreakStep::exeBreak() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_CNFORT_BKSTEP_FALL_ED", -1, -1);
    }

    kill();
}

void CannonFortressBreakStep::startFall() {
    setNerve(&NrvCannonFortressBreakStep::CannonFortressBreakStepNrvFallStart::sInstance);
}

void CannonFortressBreakStep::initCaseUseSwitchB(const MapObjActorInitInfo &rInfo) {
    MR::listenStageSwitchOnB(this, MR::Functor_Inline(this, &CannonFortressBreakStep::startFall));
}

void CannonFortressBreakStep::initCaseNoUseSwitchB(const MapObjActorInitInfo &) {

}

CannonFortressBreakStep::~CannonFortressBreakStep() {
    
}
