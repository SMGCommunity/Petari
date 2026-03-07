#include "Game/MapObj/SandUpDownTriRock.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include <cstdio>


namespace NrvSandUpDownTriRock {
    NEW_NERVE(SandUpDownTriRockNrvWait, SandUpDownTriRock, Wait);
    NEW_NERVE(SandUpDownTriRockNrvDown, SandUpDownTriRock, Down);
    NEW_NERVE(SandUpDownTriRockNrvRest, SandUpDownTriRock, Rest);
    NEW_NERVE(SandUpDownTriRockNrvUp, SandUpDownTriRock, Up);
    NEW_NERVE(SandUpDownTriRockNrvStop, SandUpDownTriRock, Stop);
}  // namespace NrvSandUpDownTriRock

SandUpDownTriRock::SandUpDownTriRock(const char* pName) : MapObjActor(pName), mTimer(0) {
    for (s32 idx = 0; idx < 3; idx++) {
        mRockCollisionParts[idx] = nullptr;
    }
}

void SandUpDownTriRock::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);

    MapObjActorInitInfo info;
    info.setupHioNode("地形オブジェ");
    info.setupDefaultPos();
    info.setupConnectToScene();
    info.setupHitSensor();
    info.setupEffect(nullptr);
    info.setupSound(4);
    info.setupNerve(&NrvSandUpDownTriRock::SandUpDownTriRockNrvWait::sInstance);
    initialize(rIter, info);

    MR::getJMapInfoArg0NoInit(rIter, &mTimer);

    char buffer[256];
    for (s32 idx = 0; idx < 3; idx++) {
        snprintf(buffer, 256, "Move0%d", idx);
        mRockCollisionParts[idx] = MR::createCollisionPartsFromLiveActor(this, buffer, getSensor("body"), MR::getJointMtx(this, buffer),
                                                                         static_cast< MR::CollisionScaleType >(2));
        MR::validateCollisionParts(mRockCollisionParts[idx]);
    }
}

void SandUpDownTriRock::exeWait() {}

void SandUpDownTriRock::exeDown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "On", nullptr);
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvSandUpDownTriRock::SandUpDownTriRockNrvRest::sInstance);
    }
}

void SandUpDownTriRock::exeRest() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(this);
    }

    if (MR::isStep(this, mTimer)) {
        MR::validateClipping(this);
        setNerve(&NrvSandUpDownTriRock::SandUpDownTriRockNrvUp::sInstance);
    }
}

void SandUpDownTriRock::exeUp() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Return", nullptr);
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvSandUpDownTriRock::SandUpDownTriRockNrvWait::sInstance);
    }
}

void SandUpDownTriRock::exeStop() {}

bool SandUpDownTriRock::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStartPowerStarGet(msg)) {
        setNerve(&NrvSandUpDownTriRock::SandUpDownTriRockNrvStop::sInstance);
        return true;
    }

    return false;
}

void SandUpDownTriRock::initCaseUseSwitchB(const MapObjActorInitInfo& info) {
    MR::listenStageSwitchOnB(this, MR::Functor_Inline(this, &SandUpDownTriRock::startDown));
}

void SandUpDownTriRock::control() {
    for (s32 idx = 0; idx < 3; idx++) {
        mRockCollisionParts[idx]->setMtx();
    }
}

void SandUpDownTriRock::startDown() {
    setNerve(&NrvSandUpDownTriRock::SandUpDownTriRockNrvDown::sInstance);
}
