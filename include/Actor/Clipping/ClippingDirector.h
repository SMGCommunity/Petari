#pragma once

#include "Actor/Clipping/ClippingActorHolder.h"
#include "Actor/Clipping/ClippingGroupHolder.h"
#include "Actor/Clipping/ClippingJudge.h"
#include "Actor/LOD/LodCtrl.h"
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
    void entryLodCtrl(LodCtrl *, const JMapInfoIter &);

    ClippingJudge* mJudge; // _C
    ClippingActorHolder* mActorHolder; // _10
    ClippingGroupHolder* mGroupHolder; // _14
};