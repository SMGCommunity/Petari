#include "Game/Boss/PoltaStateGroundRockAttack.hpp"
#include "Game/Boss/Polta.hpp"
#include "Game/Boss/PoltaFunction.hpp"
#include "Game/Boss/PoltaStateAttackGround.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    static const s32 sGenerateInterval = 15;
    static const s32 sGenerateAfterTime = 90;
    static const f32 sGenerateStartRadius = 850.0f;
    static const f32 sGenerateStepRadius = 400.0f;
    static const f32 sForecastAngle = 20.0f;
};  // namespace

namespace NrvPoltaStateGenerateBombTeresa {
    NEW_NERVE(PoltaStateGroundRockAttackNrvSign, PoltaStateGroundRockAttack, Sign);
    NEW_NERVE(PoltaStateGroundRockAttackNrvGenerate, PoltaStateGroundRockAttack, Generate);
    NEW_NERVE(PoltaStateGroundRockAttackNrvEnd, PoltaStateGroundRockAttack, End);
};  // namespace NrvPoltaStateGenerateBombTeresa

PoltaStateGroundRockAttack::PoltaStateGroundRockAttack(Polta* pPolta)
    : ActorStateBase< Polta >("[state]ポルタ地面岩攻撃", pPolta), mRockCount(4), mIsAppearClockwise() {
    initNerve(GET_NERVE(PoltaStateGenerateBombTeresa, PoltaStateGroundRockAttackNrvSign));
}

void PoltaStateGroundRockAttack::appear() {
    mIsDead = false;
    setNerve(GET_NERVE(PoltaStateGenerateBombTeresa, PoltaStateGroundRockAttackNrvSign));
}

void PoltaStateGroundRockAttack::exeSign() {
    if (MR::isFirstStep(this)) {
        PoltaFunction::requestStartControllArm(getHost());
        PoltaFunction::startAction(getHost(), "GenerateGroundRockSign", true);
        MR::startSound(getHost(), "SE_BV_POLTA_PREP_GND_ROCK");
    }
    getHost()->rotateToPlayer();
    if (MR::isActionEnd(getHost())) {
        setNerve(GET_NERVE(PoltaStateGenerateBombTeresa, PoltaStateGroundRockAttackNrvGenerate));
    }
}

void PoltaStateGroundRockAttack::exeGenerate() {
    if (MR::isFirstStep(this)) {
        PoltaFunction::startAction(getHost(), "GenerateGroundRockStart", true);
        MR::zeroVelocity(getHost());
        mIsAppearClockwise = MR::isClockwiseToDir(getHost(), *MR::getPlayerLastMove(), *MR::getPlayerPos() - getHost()->mPosition);
    }

    if (MR::isBckOneTimeAndStopped(getHost())) {
        PoltaFunction::startAction(getHost(), "GenerateGroundRock", true);
        MR::startSound(getHost(), "SE_BV_POLTA_GEN_GND_ROCK");
        MR::startSound(getHost(), "SE_BM_POLTA_GEN_GND_ROCK");
    }
    if (MR::isLessEqualStep(this, ::sGenerateInterval * (mRockCount - 1)) && MR::isIntervalStep(this, ::sGenerateInterval)) {
        s32 v14 = getNerveStep() / ::sGenerateInterval;
        s32 v16 = v14 % 4;

        if (v14 > 0 && !v16) {
            mIsAppearClockwise = !mIsAppearClockwise;
        }
        PoltaFunction::appearGroundRock(getHost(), (::sGenerateStartRadius + (::sGenerateStepRadius * v16)),
                                        mIsAppearClockwise ? ::sForecastAngle : -::sForecastAngle);
    }
    if (MR::isGreaterStep(this, ::sGenerateInterval * mRockCount + ::sGenerateAfterTime)) {
        setNerve(GET_NERVE(PoltaStateGenerateBombTeresa, PoltaStateGroundRockAttackNrvEnd));
    }
}

void PoltaStateGroundRockAttack::exeEnd() {
    if (MR::isFirstStep(this)) {
        PoltaFunction::startAction(getHost(), "GenerateGroundRockEnd", true);
    }
    if (MR::isActionEnd(getHost())) {
        kill();
    }
}

PoltaStateGroundRockAttack::~PoltaStateGroundRockAttack() {
}
