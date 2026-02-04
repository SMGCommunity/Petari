#include "Game/Boss/PoltaStateGroundRockAttack.hpp"
#include "Game/Boss/Polta.hpp"
#include "Game/Boss/PoltaFunction.hpp"
#include "Game/Boss/PoltaStateAttackGround.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JMath/JMath.hpp"


namespace NrvPoltaStateGenerateBombTeresa {
    NEW_NERVE(PoltaStateGroundRockAttackNrvSign, PoltaStateGroundRockAttack, Sign);
    NEW_NERVE(PoltaStateGroundRockAttackNrvGenerate, PoltaStateGroundRockAttack, Generate);
    NEW_NERVE(PoltaStateGroundRockAttackNrvEnd, PoltaStateGroundRockAttack, End);
};  // namespace NrvPoltaStateGenerateBombTeresa

PoltaStateGroundRockAttack::PoltaStateGroundRockAttack(Polta* pPolta)
    : ActorStateBase< Polta >("[state]ポルタ地面岩攻撃"), mOwner(pPolta), _10(4), _14(false) {
    initNerve(&NrvPoltaStateGenerateBombTeresa::PoltaStateGroundRockAttackNrvSign::sInstance);
}

void PoltaStateGroundRockAttack::appear() {
    mIsDead = false;
    setNerve(&NrvPoltaStateGenerateBombTeresa::PoltaStateGroundRockAttackNrvSign::sInstance);
}

void PoltaStateGroundRockAttack::exeSign() {
    if (MR::isFirstStep(this)) {
        PoltaFunction::requestStartControllArm(mOwner);
        PoltaFunction::startAction(mOwner, "GenerateGroundRockSign", true);
        MR::startSound(mOwner, "SE_BV_POLTA_PREP_GND_ROCK", -1, -1);
    }
    mOwner->rotateToPlayer();
    if (MR::isActionEnd(mOwner)) {
        setNerve(&NrvPoltaStateGenerateBombTeresa::PoltaStateGroundRockAttackNrvGenerate::sInstance);
    }
}

void PoltaStateGroundRockAttack::exeGenerate() {
    if (MR::isFirstStep(this)) {
        PoltaFunction::startAction(mOwner, "GenerateGroundRockStart", true);
        MR::zeroVelocity(mOwner);
        _14 = MR::isClockwiseToDir(getPolta(), *MR::getPlayerLastMove(), *MR::getPlayerPos() - getPolta()->mPosition);
    }

    if (MR::isBckOneTimeAndStopped(mOwner)) {
        PoltaFunction::startAction(mOwner, "GenerateGroundRock", true);
        MR::startSound(mOwner, "SE_BV_POLTA_GEN_GND_ROCK", -1, -1);
        MR::startSound(mOwner, "SE_BM_POLTA_GEN_GND_ROCK", -1, -1);
    }
    if (MR::isLessEqualStep(this, 15 * (_10 - 1)) && MR::isIntervalStep(this, 15)) {
        s32 v14 = getNerveStep() / 15;
        s32 v16 = v14 % 4;

        if (v14 > 0 && !v16) {
            _14 = !_14;
        }
        PoltaFunction::appearGroundRock(mOwner, (800.0f + (400.0f * v16)), _14 ? 20.0f : -20.0f);
    }
    if (MR::isGreaterStep(this, 15 * _10 + 90)) {
        setNerve(&NrvPoltaStateGenerateBombTeresa::PoltaStateGroundRockAttackNrvEnd::sInstance);
    }
}

void PoltaStateGroundRockAttack::exeEnd() {
    if (MR::isFirstStep(this)) {
        PoltaFunction::startAction(mOwner, "GenerateGroundRockEnd", true);
    }
    if (MR::isActionEnd(mOwner)) {
        kill();
    }
}

PoltaStateGroundRockAttack::~PoltaStateGroundRockAttack() {}
