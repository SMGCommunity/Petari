#pragma once

#include <revolution.h>
#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/ClippingJudge.hpp"
#include "Game/LiveActor/ClippingActorHolder.hpp"
#include "Game/LiveActor/ClippingGroupHolder.hpp"
#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/Util.hpp"

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
