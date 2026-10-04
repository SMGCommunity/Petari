#pragma once

#include "Game/NameObj/NameObj.hpp"

class ClippingActorHolder;
class ClippingGroupHolder;
class ClippingJudge;
class LiveActor;
class LodCtrl;

class ClippingDirector : public NameObj {
public:
    ClippingDirector();

    virtual void movement();

    void endInitActorSystemInfo();
    void registerActor(LiveActor*);
    void initActorSystemInfo(LiveActor*, const JMapInfoIter&);
    void joinToGroupClipping(LiveActor*, const JMapInfoIter&, int);
    void entryLodCtrl(LodCtrl*, const JMapInfoIter&);

    /* 0x0C */ ClippingJudge* mJudge;
    /* 0x10 */ ClippingActorHolder* mActorHolder;
    /* 0x14 */ ClippingGroupHolder* mGroupHolder;
};

namespace MR {
    ClippingDirector* getClippingDirector();
    void addToClippingTarget(LiveActor*);
    void removeFromClippingTarget(LiveActor*);
};  // namespace MR
