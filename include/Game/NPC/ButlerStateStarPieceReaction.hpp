#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include <revolution/types.h>

class TalkMessageCtrl;

class ButlerStateStarPieceReaction : public ActorStateBase< LiveActor > {
public:
    ButlerStateStarPieceReaction(LiveActor*, const JMapInfoIter&, const char*);

    virtual void init();
    virtual void appear();

    void exeWait();

    /* 0x0C */ LiveActor* mHost;
    /* 0x10 */ TalkMessageCtrl* mTalkMessage;
    /* 0x14 */ bool _14;
};
