#include "Game/Boss/TombSpiderStateSwoon.hpp"
#include "Game/Boss/TombSpider.hpp"
#include "Game/Util/LiveActorUtil.hpp"

namespace {
    static const s32 sLoopStep = 180;
}

namespace NrvTombSpiderStateSwoon {
    NEW_NERVE(TombSpiderStateSwoonNrvStart, TombSpiderStateSwoon, Start);
    NEW_NERVE(TombSpiderStateSwoonNrvLoop, TombSpiderStateSwoon, Loop);
    NEW_NERVE(TombSpiderStateSwoonNrvEnd, TombSpiderStateSwoon, End);
}  // namespace NrvTombSpiderStateSwoon

TombSpiderStateSwoon::TombSpiderStateSwoon(TombSpider* pParent) : ActorStateBase< TombSpider >("State[気絶]", pParent) {
}

void TombSpiderStateSwoon::init() {
    initNerve(&NrvTombSpiderStateSwoon::TombSpiderStateSwoonNrvStart::sInstance);
    kill();
}

void TombSpiderStateSwoon::appear() {
    mIsDead = false;
    setNerve(&NrvTombSpiderStateSwoon::TombSpiderStateSwoonNrvStart::sInstance);
}

void TombSpiderStateSwoon::exeStart() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "SwoonStart");
    }

    if (MR::isActionEnd(getHost())) {
        setNerve(&NrvTombSpiderStateSwoon::TombSpiderStateSwoonNrvLoop::sInstance);
    }
}

void TombSpiderStateSwoon::exeLoop() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "Swoon");
    }

    if (MR::isGreaterStep(this, sLoopStep)) {
        setNerve(&NrvTombSpiderStateSwoon::TombSpiderStateSwoonNrvEnd::sInstance);
    }
}

void TombSpiderStateSwoon::exeEnd() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "SwoonEnd");
    }

    if (MR::isBckStopped(getHost())) {
        kill();
    }
}
