#include "Game/Boss/PoltaStateGenerateRock.hpp"
#include "Game/Boss/Polta.hpp"
#include "Game/Boss/PoltaFunction.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"
#include "Game/LiveActor/Nerve.hpp"

namespace NrvPoltaStateGenerateRock {
    NEW_NERVE(PoltaStateGenerateRockNrvSign, PoltaStateGenerateRock, Sign);
    NEW_NERVE(PoltaStateGenerateRockNrvGenerate, PoltaStateGenerateRock, Generate);
    NEW_NERVE(PoltaStateGenerateRockNrvEnd, PoltaStateGenerateRock, End);
};  // namespace NrvPoltaStateGenerateRock

namespace {
    const s32 sRockPattern[] = {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
                                  1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0};
};

PoltaStateGenerateRock::PoltaStateGenerateRock(Polta* pPolta)
    : ActorStateBase< Polta >("ポルタ岩生成", pPolta), mPatternIndex(0), mIndexIntoPattern(0), mMaxIndexIntoPattern(5), mEndDelayStep(180) {
    initNerve(&NrvPoltaStateGenerateRock::PoltaStateGenerateRockNrvSign::sInstance);
}

void PoltaStateGenerateRock::appear() {
    mIsDead = false;
    setNerve(&NrvPoltaStateGenerateRock::PoltaStateGenerateRockNrvSign::sInstance);
    mIndexIntoPattern = 0;
}

void PoltaStateGenerateRock::exeSign() {
    if (MR::isFirstStep(this)) {
        PoltaFunction::requestStartControllArm(getHost());
        PoltaFunction::startAction(getHost(), "GenerateRockStart", true);
        MR::startSound(getHost(), "SE_BV_POLTA_GEN_ROCK", -1, -1);
    }
    getHost()->rotateToPlayer();
    if (MR::isActionEnd(getHost())) {
        NerveExecutor::setNerve(&NrvPoltaStateGenerateRock::PoltaStateGenerateRockNrvGenerate::sInstance);
    }
}

void PoltaStateGenerateRock::exeGenerate() {
    s32 v2;  // r31

    if (MR::isFirstStep(this)) {
        PoltaFunction::startAction(getHost(), "GenerateRock", true);
        MR::zeroVelocity(getHost());
        mPatternIndex = MR::getRandom((s32)0, (s32)9);
    }
    getHost()->rotateToPlayer();
    if (MR::isIntervalStep(this, 30)) {
        v2 = mIndexIntoPattern % 7;
        if (::sRockPattern[28 * mPatternIndex + v2] && !PoltaFunction::isMaxGenerateBombTeresa(getHost())) {
            PoltaFunction::appearBlackRockCircle(getHost(), getHost()->mPosition, 600.0f, v2, mMaxIndexIntoPattern);
        } else if (MR::getRandom(0.0f, 1.0f) < 0.08f && MR::getDeclareRemnantCoinCount(getHost()) > 0) {
            PoltaFunction::appearYellowRockCircle(getHost(), getHost()->mPosition, 600.0f, v2, mMaxIndexIntoPattern);
        } else {
            PoltaFunction::appearWhiteRockCircle(getHost(), getHost()->mPosition, 600.0f, v2, mMaxIndexIntoPattern);
        }
        mIndexIntoPattern++;
    }
    if (mIndexIntoPattern >= mMaxIndexIntoPattern) {
        setNerve(&NrvPoltaStateGenerateRock::PoltaStateGenerateRockNrvEnd::sInstance);
    }
}

void PoltaStateGenerateRock::exeEnd() {
    if (MR::isStep(this, mEndDelayStep)) {
        PoltaFunction::startAction(getHost(), "GenerateRockToWait", true);
    }
    getHost()->rotateToPlayer();
    if (MR::isGreaterStep(this, mEndDelayStep)) {
        if (MR::isActionEnd(getHost())) {
            kill();
        }
    }
}

PoltaStateGenerateRock::~PoltaStateGenerateRock() {}
