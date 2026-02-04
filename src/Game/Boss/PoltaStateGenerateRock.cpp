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
    const s32 sRockPattern[70] = {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
                                  1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0};
};

PoltaStateGenerateRock::PoltaStateGenerateRock(Polta* pPolta)
    : ActorStateBase< Polta >("ポルタ岩生成"), mOwner(pPolta), mPatternIndex(0), mIndexIntoPattern(0), mMaxIndexIntoPattern(5), mEndDelayStep(180) {
    initNerve(&NrvPoltaStateGenerateRock::PoltaStateGenerateRockNrvSign::sInstance);
}

void PoltaStateGenerateRock::appear() {
  mIsDead = false;
  setNerve(&NrvPoltaStateGenerateRock::PoltaStateGenerateRockNrvSign::sInstance);
  mIndexIntoPattern = 0;
}

void PoltaStateGenerateRock::exeSign() {
    if (MR::isFirstStep(this)) {
        PoltaFunction::requestStartControllArm(mOwner);
        PoltaFunction::startAction(mOwner, "GenerateRockStart", true);
        MR::startSound(mOwner, "SE_BV_POLTA_GEN_ROCK", -1, -1);
    }
    mOwner->rotateToPlayer();
    if (MR::isActionEnd(mOwner)) {
        NerveExecutor::setNerve(&NrvPoltaStateGenerateRock::PoltaStateGenerateRockNrvGenerate::sInstance);
    }
}

void PoltaStateGenerateRock::exeGenerate() {
    s32 v2;  // r31

    if (MR::isFirstStep(this)) {
        PoltaFunction::startAction(mOwner, "GenerateRock", true);
        MR::zeroVelocity(mOwner);
        mPatternIndex = MR::getRandom((s32)0, (s32)9);
    }
    mOwner->rotateToPlayer();
    if (MR::isIntervalStep(this, 30)) {
        v2 = mIndexIntoPattern % 7;
        if (::sRockPattern[28 * mPatternIndex + v2] && !PoltaFunction::isMaxGenerateBombTeresa(mOwner)) {
            PoltaFunction::appearBlackRockCircle(mOwner, mOwner->mPosition, 600.0f, v2, mMaxIndexIntoPattern);
        } else if (MR::getRandom(0.0f, 1.0f) < 0.08f && MR::getDeclareRemnantCoinCount(mOwner) > 0) {
            PoltaFunction::appearYellowRockCircle(mOwner, mOwner->mPosition, 600.0f, v2, mMaxIndexIntoPattern);
        } else {
            PoltaFunction::appearWhiteRockCircle(mOwner, mOwner->mPosition, 600.0f, v2, mMaxIndexIntoPattern);
        }
        mIndexIntoPattern++;
    }
    if (mIndexIntoPattern >= mMaxIndexIntoPattern) {
        setNerve(&NrvPoltaStateGenerateRock::PoltaStateGenerateRockNrvEnd::sInstance);
    }
}

void PoltaStateGenerateRock::exeEnd() {
    if (MR::isStep(this, mEndDelayStep)) {
        PoltaFunction::startAction(mOwner, "GenerateRockToWait", true);
    }
    mOwner->rotateToPlayer();
    if (MR::isGreaterStep(this, mEndDelayStep)) {
        if (MR::isActionEnd(mOwner)) {
            kill();
        }
    }
}

PoltaStateGenerateRock::~PoltaStateGenerateRock() {}
