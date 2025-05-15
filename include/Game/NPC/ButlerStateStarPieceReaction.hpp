#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NPC/TalkMessageCtrl.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "revolution/types.h"

class ButlerStateStarPieceReaction : public ActorStateBase<LiveActor> {
public:
    ButlerStateStarPieceReaction(LiveActor *, const JMapInfoIter &, const char *);

    virtual ~ButlerStateStarPieceReaction();
    virtual void init();
    virtual void appear();

    void exeWait();

    u8 _8;
    LiveActor *mActor; // _C
    TalkMessageCtrl *mTalkMessage; // _10
    bool _14;
};
