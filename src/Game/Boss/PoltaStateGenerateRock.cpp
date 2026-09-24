#include "Game/Boss/PoltaStateGenerateRock.hpp"
#include "Game/Boss/Polta.hpp"
#include "Game/Boss/PoltaFunction.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

void PoltaStateGenerateRock_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

namespace {
    const s32 sRockPattern[][7] = {
        {1, 0, 0, 0, 1, 0, 0}, {0, 1, 0, 0, 0, 1, 0}, {0, 0, 1, 0, 0, 0, 1}, {0, 0, 0, 1, 0, 0, 0}, {0, 0, 0, 0, 1, 0, 0},
        {1, 0, 0, 1, 0, 0, 0}, {0, 1, 0, 0, 1, 0, 0}, {0, 0, 1, 0, 0, 1, 0}, {0, 0, 0, 1, 0, 0, 1}, {0, 0, 0, 0, 1, 0, 0},
    };
};  // namespace

namespace NrvPoltaStateGenerateRock {
    NEW_NERVE(PoltaStateGenerateRockNrvSign, PoltaStateGenerateRock, Sign);
    NEW_NERVE(PoltaStateGenerateRockNrvGenerate, PoltaStateGenerateRock, Generate);
    NEW_NERVE(PoltaStateGenerateRockNrvEnd, PoltaStateGenerateRock, End);
};  // namespace NrvPoltaStateGenerateRock

PoltaStateGenerateRock::PoltaStateGenerateRock(Polta* pPolta)
    : ActorStateBase< Polta >("ポルタ岩生成", pPolta), mPatternIndex(), mIndexIntoPattern(), mMaxIndexIntoPattern(5), mEndDelayStep(180) {
    initNerve(GET_NERVE(PoltaStateGenerateRock, PoltaStateGenerateRockNrvSign));
}

void PoltaStateGenerateRock::appear() {
    mIsDead = false;
    setNerve(GET_NERVE(PoltaStateGenerateRock, PoltaStateGenerateRockNrvSign));
    mIndexIntoPattern = 0;
}

void PoltaStateGenerateRock::exeSign() {
    if (MR::isFirstStep(this)) {
        PoltaFunction::requestStartControllArm(getHost());
        PoltaFunction::startAction(getHost(), "GenerateRockStart", true);
        MR::startSound(getHost(), "SE_BV_POLTA_GEN_ROCK");
    }

    getHost()->rotateToPlayer();
    if (MR::isActionEnd(getHost())) {
        NerveExecutor::setNerve(GET_NERVE(PoltaStateGenerateRock, PoltaStateGenerateRockNrvGenerate));
    }
}

void PoltaStateGenerateRock::exeGenerate() {
    s32 patternIndex;

    if (MR::isFirstStep(this)) {
        PoltaFunction::startAction(getHost(), "GenerateRock", true);
        MR::zeroVelocity(getHost());
        mPatternIndex = MR::getRandom(static_cast< s32 >(0), static_cast< s32 >(9));
    }

    getHost()->rotateToPlayer();
    if (MR::isIntervalStep(this, 30)) {
        patternIndex = mIndexIntoPattern % 7;
        if (::sRockPattern[mPatternIndex][patternIndex] && !PoltaFunction::isMaxGenerateBombTeresa(getHost())) {
            PoltaFunction::appearBlackRockCircle(getHost(), getHost()->mPosition, 600.0f, patternIndex, mMaxIndexIntoPattern);
        } else if (MR::getRandom(0.0f, 1.0f) < 0.08f && MR::getDeclareRemnantCoinCount(getHost()) > 0) {
            PoltaFunction::appearYellowRockCircle(getHost(), getHost()->mPosition, 600.0f, patternIndex, mMaxIndexIntoPattern);
        } else {
            PoltaFunction::appearWhiteRockCircle(getHost(), getHost()->mPosition, 600.0f, patternIndex, mMaxIndexIntoPattern);
        }

        mIndexIntoPattern++;
    }

    if (mIndexIntoPattern >= mMaxIndexIntoPattern) {
        setNerve(GET_NERVE(PoltaStateGenerateRock, PoltaStateGenerateRockNrvEnd));
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

PoltaStateGenerateRock::~PoltaStateGenerateRock() {
}
