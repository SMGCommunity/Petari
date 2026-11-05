#include "Game/Boss/PoltaStateStagger.hpp"
#include "Game/Boss/Polta.hpp"
#include "Game/Boss/PoltaActionBase.hpp"
#include "Game/Boss/PoltaFunction.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <JSystem/JGeometry/TVec.hpp>

namespace {
    static const f32 sNormalFric = 0.9f;
    static const f32 sAwayAccel = 0.7f;
    static const f32 sSideAccel = 0.8f;
    static const f32 sGravityAccel = 0.5f;
    static const f32 sAwayDistance = 1500.0f;
    static const f32 sAwayRadius = 1800.0f;
    static const f32 sGenerateSpeedH = 20.0f;
    static const f32 sGenerateSpeedV = 15.0f;
    static const s32 sGenerateInterval = 210;
};  // namespace

namespace NrvPoltaStateStagger {
    NEW_NERVE(PoltaStateStaggerNrvWait, PoltaStateStagger, Wait);
};  // namespace NrvPoltaStateStagger

PoltaStateStagger::PoltaStateStagger(Polta* pPolta) : ActorStateBase< Polta >("[state]ポルタ弱り状態", pPolta), mActionName("Stagger"), mIsClockwiseToPlayer(true) {
    initNerve(GET_NERVE(PoltaStateStagger, PoltaStateStaggerNrvWait));
}

void PoltaStateStagger::setActionName(const char* pActionName) {
    mActionName = pActionName;
}

void PoltaStateStagger::appear() {
    mIsDead = false;
    setNerve(GET_NERVE(PoltaStateStagger, PoltaStateStaggerNrvWait));
}

void PoltaStateStagger::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::zeroVelocity(getHost());
        PoltaFunction::startBckBody(getHost(), mActionName);
    }
    TVec3f v2;

    f32 v17;
    MR::separateScalarAndDirection(&v17, &v2, getHost()->mStartPoseTrans - getHost()->mPosition);

    if (v17 >= ::sAwayRadius) {
        MR::addVelocity(getHost(), v2 * (v17 - ::sAwayRadius));
        mIsClockwiseToPlayer = MR::isClockwiseToPlayer(getHost(), v2);
    }

    if (MR::isNearPlayerHorizontal(getHost(), ::sAwayDistance)) {
        MR::addVelocityAwayFromTarget(getHost(), *MR::getPlayerPos(), ::sAwayAccel);
    }

    MR::addVelocityClockwiseToPlayer(getHost(), mIsClockwiseToPlayer ? ::sSideAccel : -::sSideAccel);
    MR::attenuateVelocity(getHost(), ::sNormalFric);
    MR::addVelocityKeepHeight(getHost(), getHost()->mStartPoseTrans, ::sGravityAccel, 50.0f);
    getHost()->rotateToPlayer();
    if (MR::isIntervalStep(this, ::sGenerateInterval) &&
        PoltaFunction::appearBombTeresaFromRoot(getHost(), ::sGenerateSpeedH, ::sGenerateSpeedV, 1)) {
        MR::startSound(getHost(), "SE_BM_POLTA_GEN_BOMB_TERESA");
    }
}

bool PoltaStateStagger::isEnableSensor() const {
    return isNerve(GET_NERVE(PoltaStateStagger, PoltaStateStaggerNrvWait));
}

PoltaStateStagger::~PoltaStateStagger() {
}
