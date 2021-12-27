#pragma once

#include <revolution.h>
#include "Game/NameObj/NameObj.h"
#include "Game/LiveActor/ClippingJudge.h"
#include "Game/LiveActor/ClippingActorHolder.h"
#include "Game/LiveActor/ClippingGroupHolder.h"
#include "Game/LiveActor/LodCtrl.h"
#include "Game/Util.h"

class LiveActor;

class ClippingDirector : public NameObj {
public:
    ClippingDirector();

    virtual ~ClippingDirector();
    virtual void movement();

    void endInitActorSystemInfo();
    void registerActor(LiveActor *);
    void initActorSystemInfo(LiveActor *, const JMapInfoIter &);
    void joinToGroupClipping(LiveActor *, const JMapInfoIter &, int);
    void entryLodCtrl(LodCtrl *, const JMapInfoIter &);

    ClippingJudge* mJudge;              // _C
    ClippingActorHolder* mActorHolder;  // _10
    ClippingGroupHolder* mGroupHolder;  // _14
};

namespace MR {
    ClippingDirector* getClippingDirector();
    void addToClippingTarget(LiveActor *);
    void removeFromClippingTarget(LiveActor *);
};
