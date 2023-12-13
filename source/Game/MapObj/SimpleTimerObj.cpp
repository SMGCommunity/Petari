#include "Game/MapObj/SimpleTimerObj.hpp"

SimpleTimerObj::SimpleTimerObj(const char *pName) : MapObjActor(pName) {
    mFlashingCtrl = 0;
    mTimer = 600;
    mTimeLeft = 0;
}

void SimpleTimerObj::init(const JMapInfoIter &rIter) {
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
    }
    else if (mTimeLeft != 0) {
        kill();
    }
}

SimpleTimerObj::~SimpleTimerObj() {

}

namespace NrvSimpleTimerObj {
    INIT_NERVE(SimpleTimerObjNrvTimer);

    void SimpleTimerObjNrvTimer::execute(Spine *pSpine) const {
        SimpleTimerObj* timer_obj = reinterpret_cast<SimpleTimerObj*>(pSpine->mExecutor);
        timer_obj->exeTimer();
    }
};