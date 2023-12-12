#pragma once

#include "Game/Enemy/AnimScaleController.h"
#include "Game/LiveActor/LiveActor.h"
#include "Game/LiveActor/ActorStateBase.h"

class WalkerStateBindStarPointer : public ActorStateBase<LiveActor> {
public:
    WalkerStateBindStarPointer(LiveActor *, AnimScaleController *);

    virtual ~WalkerStateBindStarPointer();
    virtual void appear();
    virtual void kill();

    bool tryStartPointBind() const;
    void exeBind();

    /* 0xC */   LiveActor* mHostActor;                      ///< The host actor that is bound.
    /* 0x10 */  AnimScaleController* mScaleController;      ///< The controller for scaling the actor during animations.
    /* 0x14 */  s32 mUpdateCounter;                         ///< Counter that is incremented in exeBind. Once it exceeds 5, it updates the actor.
    /* 0x18 */  bool mHasEffect;                            ///< Does the host actor contain the "Touch" effect name?
};

namespace NrvWalkerStateBindStarPointer {
    NERVE_DECL(WalkerStateBindStarPointerNrvBind, WalkerStateBindStarPointer, WalkerStateBindStarPointer::exeBind);
};