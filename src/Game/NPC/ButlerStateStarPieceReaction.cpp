#include "Game/NPC/ButlerStateStarPieceReaction.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/TalkUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"

namespace NrvButlerStateStarPieceReaction {
    NEW_NERVE(ButlerStateStarPieceReactionNrvWait, ButlerStateStarPieceReaction, Wait);
};

ButlerStateStarPieceReaction::ButlerStateStarPieceReaction(LiveActor *actor, const JMapInfoIter &rIter, const char *pName) : ActorStateBase<LiveActor>("バトラースターピース反応") {
    mActor = actor;
    mTalkMessage = nullptr;
    _14 = false;
    TVec3f vec;
    vec.x = 0.0f;
    vec.y = 180.0f;
    vec.z = 0.0f;
    mTalkMessage = MR::createTalkCtrlDirectOnRootNodeAutomatic(actor, rIter, pName, vec, nullptr);
}

ButlerStateStarPieceReaction::~ButlerStateStarPieceReaction() {
    
}

void ButlerStateStarPieceReaction::init() {
    initNerve(&NrvButlerStateStarPieceReaction::ButlerStateStarPieceReactionNrvWait::sInstance);
}

void ButlerStateStarPieceReaction::appear() {
    _14 = true;
    _8 = false;
    setNerve(&NrvButlerStateStarPieceReaction::ButlerStateStarPieceReactionNrvWait::sInstance);
}

void ButlerStateStarPieceReaction::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(mActor, "StarPieceReaction", nullptr);
        MR::limitedStarPieceHitSound();
        MR::startSound(mActor, "SE_SM_BUTLER_ABSORB", -1, -1);
    }
    if (_14) {
        MR::tryTalkForce(mTalkMessage); 
    }
    if (MR::isBckStopped(mActor)) {
        kill();
    }
}
