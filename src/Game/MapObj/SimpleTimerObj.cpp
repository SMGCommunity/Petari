#include "Game/MapObj/SimpleTimerObj.hpp"
#include "Game/LiveActor/FlashingCtrl.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"

namespace NrvSimpleTimerObj {
    NEW_NERVE(SimpleTimerObjNrvTimer, SimpleTimerObj, Timer);
};  // namespace NrvSimpleTimerObj

SimpleTimerObj::SimpleTimerObj(const char* pName) : MapObjActor(pName) {
    mFlashingCtrl = 0;
    mTimer = 600;
    mTimeLeft = 0;
}

void SimpleTimerObj::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    info.setupNerve(&NrvSimpleTimerObj::SimpleTimerObjNrvTimer::sInstance);
    initialize(rIter, info);
    MR::getJMapInfoArg7NoInit(rIter, &mTimer);
    mFlashingCtrl = new FlashingCtrl(this, true);
    MR::invalidateShadow(this, 0);
}

void SimpleTimerObj::exeTimer() {
    if (MR::isFirstStep(this)) {
        mTimeLeft = mTimer;
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
    s32 curTime = mTimeLeft;
    mTimeLeft = curTime - 1;

    if (mTimeLeft == 120) {
        mFlashingCtrl->start(120);
    } else if (mTimeLeft == 0) {
        kill();
    }
}

SimpleTimerObj::~SimpleTimerObj() {
}
