#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <revolution/types.h>

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

    ClippingJudge* mJudge;              // 0xC
    ClippingActorHolder* mActorHolder;  // 0x10
    ClippingGroupHolder* mGroupHolder;  // 0x14
};

namespace MR {
    ClippingDirector* getClippingDirector();
    void addToClippingTarget(LiveActor*);
    void removeFromClippingTarget(LiveActor*);
};  // namespace MR
