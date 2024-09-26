#include "Game/LiveActor/AnimationRandomPlayer.hpp"
#include "Game/Util.hpp"

NrvAnimationRandomPlayer::HostTypeWait NrvAnimationRandomPlayer::HostTypeWait::sInstance;
NrvAnimationRandomPlayer::HostTypePlay NrvAnimationRandomPlayer::HostTypePlay::sInstance;

AnimationRandomPlayer::AnimationRandomPlayer(const LiveActor *pActor, const char *a2, const char *a3, s32 a4, f32 a5) : NerveExecutor(a2) {
    mActor = pActor;
    _C = a2;
    _10 = a3;
    _18 = a4;
    _1C = a5;
    initNerve(&NrvAnimationRandomPlayer::HostTypeWait::sInstance);
    updateStartStep();
}

void AnimationRandomPlayer::updateStartStep() {
    s32 v5 = _18 * _1C;
    s32 min = _18 - v5;
    s32 max = _18 + v5;
    _14 = MR::getRandom(min, max);
}

void AnimationRandomPlayer::exeWait() {
    if (MR::isFirstStep(this)) {
        if (_10) {
            MR::startAllAnim(mActor, _10);
        }
    } 

    if (MR::isStep(this, _14)) {
        setNerve(&NrvAnimationRandomPlayer::HostTypePlay::sInstance);
    }
}

void AnimationRandomPlayer::exePlay() {
    if (MR::isFirstStep(this)) {
        MR::startAllAnim(mActor, _C);
    }

    if (MR::isAnyAnimOneTimeAndStopped(mActor, _C)) {
        updateStartStep();
        MR::setAllAnimFrame(mActor, _C, 0.0f);
        setNerve(&NrvAnimationRandomPlayer::HostTypeWait::sInstance);
    }
}

AnimationRandomPlayer::~AnimationRandomPlayer() {

}