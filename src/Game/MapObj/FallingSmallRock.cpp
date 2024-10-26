#include "Game/MapObj/FallingSmallRock.hpp"

FallingSmallRock::FallingSmallRock(const char *pName) : LiveActor(pName) {

}

void FallingSmallRock::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    initSound(4, false);
    initBinder(10.0f, 800.0f, 0);
    initEffectKeeper(0, "FallingSmallRock", false);
    MR::needStageSwitchReadA(this, rIter);
    MR::setClippingTypeSphere(this, 800.0f);
    initNerve(&NrvFallingSmallRock::HostTypeWait::sInstance);
    MR::connectToSceneMapObjMovement(this);
    makeActorAppeared();
}

void FallingSmallRock::startClipped() {
    LiveActor::startClipped();
    if (isNerve(&NrvFallingSmallRock::HostTypeMove::sInstance)) {
        MR::deleteEffectAll(this);
    }
}

void FallingSmallRock::endClipped() {
    LiveActor::endClipped();
    if (isNerve(&NrvFallingSmallRock::HostTypeMove::sInstance)) {
        MR::emitEffect(this, "FallingSmallRock");
    }
}

void FallingSmallRock::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "FallingSmallRock");
    }

    MR::startLevelSound(this, "SE_AT_LV_FALLING_ROCK", -1, -1, -1);
    
    if (MR::isBinded(this)) {
        makeActorDead();
    }
}

void FallingSmallRock::exeWait() {
    if (MR::isOnSwitchA(this)) {
        setNerve(&NrvFallingSmallRock::HostTypeMove::sInstance);
    }
}

FallingSmallRock::~FallingSmallRock() {

}

namespace NrvFallingSmallRock {
    INIT_NERVE(HostTypeWait);
    INIT_NERVE(HostTypeMove);
};