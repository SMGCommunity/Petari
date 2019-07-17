#ifndef CLIPPINGDIRECTOR_H
#define CLIPPINGDIRECTOR_H

#include "Actor/Clipping/ClippingActorHolder.h"
#include "Actor/Clipping/ClippingGroupHolder.h"
#include "Actor/Clipping/ClippingJudge.h"
#include "Actor/NameObj/NameObj.h"

class ClippingDirector : public NameObj
{
public:
    ClippingDirector();

    virtual void movement();

    void endInitActorSystemInfo();
    void registerActor(LiveActor *);
    void initActorSystemInfo(LiveActor *, const JMapInfoIter &);
    void joinToGroupClipping(LiveActor *, const JMapInfoIter &, s32);

    ClippingJudge* mJudge; // _C
    ClippingActorHolder* mActorHolder; // _10
    ClippingGroupHolder* mGroupHolder; // _14
};

namespace MR
{
    ClippingDirector* getClippingDirector();
    void addToClippingTarget(LiveActor *);
    void removeFromClippingTarget(LiveActor *);
};

#endif // CLIPPINGDIRECTOR_H