#include "Game/MapObj/LargeChain.h"

namespace {
    static f32 sPartsLength = 200.0f;
};

LargeChain::LargeChain(const char *pName) : LiveActor(pName) {
    _8C = nullptr;
    _90 = nullptr;
    mChainArray = nullptr;
    mChainCount = 1;
}

void LargeChain::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    s32 chainLen;
    MR::getJMapInfoArg0NoInit(rIter, &chainLen);
    mChainCount = chainLen / sPartsLength;
    createChainParts();
    MR::connectToSceneMapObjMovement(this);

    if (MR::tryRegisterDemoCast(this, rIter)) {
        MR::registerDemoActionNerve(this, &NrvLargeChain::LargeChainNrvBreak::sInstance, nullptr);
    }

    MR::useStageSwitchSleep(this, rIter);
    initNerve(&NrvLargeChain::LargeChainNrvWait::sInstance);
    makeActorAppeared();
}

void LargeChain::makeActorAppeared() {
    for (s32 i = 0; i < (u32)mChainCount; i++) {
        mChainArray[i].makeActorAppeared();
    }

    _8C->makeActorAppeared();
    _90->makeActorAppeared();
    LiveActor::makeActorAppeared();
}

void LargeChain::makeActorDead() {
    for (s32 i = 0; i < (u32)mChainCount; i++) {
        mChainArray[i].makeActorDead();
    }

    _8C->makeActorDead();
    _90->makeActorDead();
    LiveActor::makeActorDead();
}

void LargeChain::exeBreak() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(this);
        invalidateClippingAllChainParts();
    }

    if (!(getNerveStep() % 5)) {
        s32 step = getNerveStep() / 5;
        
        if (step == 0) {
             _8C->breakChainParts();
        }
        else { 
           if (step == mChainCount - 1) {
                _90->breakChainParts();
            }
        }

        mChainArray[step].breakChainParts();
        if (step == mChainCount - 1) {
            kill();
        }
    }
}

LargeChainParts::~LargeChainParts() {

}

void LargeChain::invalidateClippingAllChainParts() {
    for (s32 i = 0; i < (u32)mChainCount; i++) {
        MR::invalidateClipping(&mChainArray[i]);
    }
}

LargeChain::~LargeChain() {

}

namespace NrvLargeChain {
    INIT_NERVE(LargeChainNrvWait);
    INIT_NERVE(LargeChainNrvBreak);
};