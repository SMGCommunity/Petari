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
    : ActorStateBase< Polta >("[state]ポルタ地面岩攻撃", pPolta), _10(4), _14(false) {
    initNerve(&NrvPoltaStateGenerateBombTeresa::PoltaStateGroundRockAttackNrvSign::sInstance);
}

void PoltaStateGroundRockAttack::appear() {
    mIsDead = false;
    setNerve(&NrvPoltaStateGenerateBombTeresa::PoltaStateGroundRockAttackNrvSign::sInstance);
}

void PoltaStateGroundRockAttack::exeSign() {
    if (MR::isFirstStep(this)) {
        PoltaFunction::requestStartControllArm(getHost());
        PoltaFunction::startAction(getHost(), "GenerateGroundRockSign", true);
        MR::startSound(getHost(), "SE_BV_POLTA_PREP_GND_ROCK", -1, -1);
    }
    getHost()->rotateToPlayer();
    if (MR::isActionEnd(getHost())) {
        setNerve(&NrvPoltaStateGenerateBombTeresa::PoltaStateGroundRockAttackNrvGenerate::sInstance);
    }
}

void PoltaStateGroundRockAttack::exeGenerate() {
    if (MR::isFirstStep(this)) {
        PoltaFunction::startAction(getHost(), "GenerateGroundRockStart", true);
        MR::zeroVelocity(getHost());
        _14 = MR::isClockwiseToDir(getHost(), *MR::getPlayerLastMove(), *MR::getPlayerPos() - getHost()->mPosition);
    }

    if (MR::isBckOneTimeAndStopped(getHost())) {
        PoltaFunction::startAction(getHost(), "GenerateGroundRock", true);
        MR::startSound(getHost(), "SE_BV_POLTA_GEN_GND_ROCK", -1, -1);
        MR::startSound(getHost(), "SE_BM_POLTA_GEN_GND_ROCK", -1, -1);
    }
    if (MR::isLessEqualStep(this, 15 * (_10 - 1)) && MR::isIntervalStep(this, 15)) {
        s32 v14 = getNerveStep() / 15;
        s32 v16 = v14 % 4;

        if (v14 > 0 && !v16) {
            _14 = !_14;
        }
        PoltaFunction::appearGroundRock(getHost(), (850.0f + (400.0f * v16)), _14 ? 20.0f : -20.0f);
    }
    if (MR::isGreaterStep(this, 15 * _10 + 90)) {
        setNerve(&NrvPoltaStateGenerateBombTeresa::PoltaStateGroundRockAttackNrvEnd::sInstance);
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
