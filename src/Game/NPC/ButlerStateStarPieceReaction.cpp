#include "Game/NPC/ButlerStateStarPieceReaction.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NPC/TalkMessageCtrl.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/TalkUtil.hpp"
#include <JSystem/JGeometry/TVec.hpp>

namespace NrvButlerStateStarPieceReaction {
    NEW_NERVE(ButlerStateStarPieceReactionNrvWait, ButlerStateStarPieceReaction, Wait);
};

ButlerStateStarPieceReaction::ButlerStateStarPieceReaction(LiveActor* pHost, const JMapInfoIter& rIter, const char* pName)
    : ActorStateBase< LiveActor >("バトラースターピース反応"), mHost(pHost), mTalkMessage(nullptr), _14(false) {
    mTalkMessage = MR::createTalkCtrlDirectOnRootNodeAutomatic(pHost, rIter, pName, TVec3f(0.0f, 180.0f, 0.0f), nullptr);
}

void ButlerStateStarPieceReaction::init() {
    initNerve(&NrvButlerStateStarPieceReaction::ButlerStateStarPieceReactionNrvWait::sInstance);
}

void ButlerStateStarPieceReaction::appear() {
    _14 = true;
    mIsDead = false;

    setNerve(&NrvButlerStateStarPieceReaction::ButlerStateStarPieceReactionNrvWait::sInstance);
}

void ButlerStateStarPieceReaction::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(mHost, "StarPieceReaction", nullptr);
        MR::limitedStarPieceHitSound();
        MR::startSound(mHost, "SE_SM_BUTLER_ABSORB", -1, -1);
    }

    if (_14) {
        MR::tryTalkForce(mTalkMessage);
    }

    if (MR::isBckStopped(mHost)) {
        kill();
    }
}
