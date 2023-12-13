#include "Game/MapObj/LargeChainParts.hpp"

LargeChainParts::LargeChainParts(const char *pName) : LiveActor(pName) {
    
}

void LargeChainParts::breakChainParts() {
    kill();
}

void LargeChainParts::initChainParts(TVec3f *pPos, TVec3f *pRot, TVec3f *pScale, bool isFixedPoint) {
    mPosition.set(*pPos);
    mRotation.set(*pRot);
    mScale.set(*pScale);

    if (isFixedPoint) {
        initModelManagerWithAnm("LargeChainFixPoint", nullptr, false);
    }
    else {
        initModelManagerWithAnm("LargeChain", nullptr, false);
    }

    MR::connectToSceneMapObj(this);
    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);
    if (isFixedPoint) {
        MR::initCollisionParts(this, "LargeChainFixPoint", getSensor("body"), nullptr);
    }
    else {
        MR::initCollisionParts(this, "LargeChain", getSensor("body"), nullptr);
    }

    initEffectKeeper(0, "LargeChain", false);
    initSound(2, false);
    makeActorAppeared();
}

void LargeChainParts::kill() {
    MR::emitEffect(this, "Break");
    MR::startSound(this, "SE_OJ_LARGE_CHAIN_BREAK", -1, -1);
    LiveActor::kill();
}