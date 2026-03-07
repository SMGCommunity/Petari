#include "Game/Boss/PoltaStateStagger.hpp"
#include "Game/Boss/Polta.hpp"
#include "Game/Boss/PoltaActionBase.hpp"
#include "Game/Boss/PoltaFunction.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "revolution/mtx.h"

namespace NrvPoltaStateStagger {
    NEW_NERVE(PoltaStateStaggerNrvWait, PoltaStateStagger, Wait);
};

PoltaStateStagger::PoltaStateStagger(Polta* pPolta) : ActorStateBase< Polta >("[state]ポルタ弱り状態", pPolta) {
    mActionName = "Stagger";
    _20 = true;
    initNerve(&NrvPoltaStateStagger::PoltaStateStaggerNrvWait::sInstance);
}

void PoltaStateStagger::setActionName(const char* pActionName) {
    mActionName = pActionName;
}
void PoltaStateStagger::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::zeroVelocity(getHost());
        PoltaFunction::startBckBody(getHost(), mActionName);
    }
    TVec3f v2;

    float v17;
    MR::separateScalarAndDirection(&v17, &v2, getHost()->_E0 - getHost()->mPosition);

    if (v17 >= 1800.0f) {
        MR::addVelocity(getHost(), v2.multInLine(v17 - 1800.0f));
        _20 = MR::isClockwiseToPlayer(getHost(), v2);
    }

    if (MR::isNearPlayerHorizontal(getHost(), 1500.0f)) {
        MR::addVelocityAwayFromTarget(getHost(), *MR::getPlayerPos(), 0.7f);
    }

    MR::addVelocityClockwiseToPlayer(getHost(), _20 ? 0.8f : -0.8f);
    MR::attenuateVelocity(getHost(), 0.9f);
    MR::addVelocityKeepHeight(getHost(), getHost()->_E0, 0.5f, 50.0f);
    getHost()->rotateToPlayer();
    if (MR::isIntervalStep(this, 210) && PoltaFunction::appearBombTeresaFromRoot(getHost(), 20.0f, 15.0f, 1)) {
        MR::startSound(getHost(), "SE_BM_POLTA_GEN_BOMB_TERESA", -1, -1);
    }
}
void PoltaStateStagger::appear() {
    mIsDead = false;
    setNerve(&NrvPoltaStateStagger::PoltaStateStaggerNrvWait::sInstance);
}

bool PoltaStateStagger::isEnableSensor() const {
    return isNerve(&NrvPoltaStateStagger::PoltaStateStaggerNrvWait::sInstance);
}

PoltaStateStagger::~PoltaStateStagger() {}
