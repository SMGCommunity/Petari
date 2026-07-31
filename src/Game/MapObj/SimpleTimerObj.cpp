#include "Game/MapObj/SimpleTimerObj.hpp"
#include "Game/LiveActor/FlashingCtrl.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"

namespace {
    static const s32 sDefaultTimeLimit = 600;
    static const s32 sBlinkTime = 120;
};  // namespace

namespace NrvSimpleTimerObj {
    NEW_NERVE(SimpleTimerObjNrvTimer, SimpleTimerObj, Timer);
};  // namespace NrvSimpleTimerObj

SimpleTimerObj::SimpleTimerObj(const char* pName) : MapObjActor(pName), mFlashingCtrl(), mTimeLimit(::sDefaultTimeLimit), mTimeLeft() {
}

void SimpleTimerObj::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    info.setupNerve(&NrvSimpleTimerObj::SimpleTimerObjNrvTimer::sInstance);
    initialize(rIter, info);
    MR::getJMapInfoArg7NoInit(rIter, &mTimeLimit);
    mFlashingCtrl = new FlashingCtrl(this, true);
    MR::invalidateShadow(this, 0);
}

void SimpleTimerObj::exeTimer() {
    if (MR::isFirstStep(this)) {
        mTimeLeft = mTimeLimit;
    }
}

void SimpleTimerObj::appear() {
    MapObjActor::appear();
    MR::showModel(this);
    MR::validateShadow(this, 0);
    setNerve(&NrvSimpleTimerObj::SimpleTimerObjNrvTimer::sInstance);
}

void SimpleTimerObj::kill() {
    mFlashingCtrl->end();
    MR::invalidateShadow(this, 0);
    MR::emitEffect(this, "Appear");
    MapObjActor::kill();
}

void SimpleTimerObj::control() {
    mTimeLeft--;

    if (mTimeLeft == ::sBlinkTime) {
        mFlashingCtrl->start(::sBlinkTime);
    } else if (mTimeLeft == 0) {
        kill();
    }
}
