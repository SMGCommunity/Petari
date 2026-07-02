#include "Game/Boss/DodoryuStateWait.hpp"
#include "Game/Boss/Dodoryu.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SequenceUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    NEW_NERVE(DodoryuStateWaitNrvWait, DodoryuStateWait, Wait);
};  // namespace

DodoryuStateWait::DodoryuStateWait(Dodoryu* pHost, const char* pName) : DodoryuStateBase(pHost, pName), mNextStateCounter() {
    initWithoutIter();
}

void DodoryuStateWait::init(const JMapInfoIter& rIter) {
    MR::invalidateClipping(this);
    initNerve(&::DodoryuStateWaitNrvWait::sInstance);
    makeActorAppeared();
}

void DodoryuStateWait::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::invalidateShadowAll(mHost);
        MR::startBck(mHost, "UnderGroundWalk", nullptr);
        MR::startBtp(mHost, "Normal");
        mHost->setHillAppearNumHalf();
        mHost->startHill();
        mHost->shiftMoveStateRail(8.0f);

        if (MR::isStageSuddenDeathDodoryu() && MR::hasRetryGalaxySequence()) {
            MR::forceCloseWipeCircle();
        }
    }

    MR::startLevelSound(mHost, "SE_BM_LV_DODORYU_MOVE_U_SHALLOW");

    if (mHost->_144 && !MR::isPlayerInBind()) {
        mNextStateCounter++;
    } else {
        mNextStateCounter = 0;
    }

    if (mNextStateCounter > 30) {
        if (MR::isStageSuddenDeathDodoryu() && MR::hasRetryGalaxySequence()) {
            MR::openWipeCircle();
        }

        mHost->nextState();
    }
}
