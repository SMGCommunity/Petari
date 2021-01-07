#pragma once

#include "LiveActor/Clipping/ClippingActorHolder.h"
#include "LiveActor/Clipping/ClippingGroupHolder.h"
#include "LiveActor/Clipping/ClippingJudge.h"
#include "LiveActor/LOD/LodCtrl.h"
#include "NameObj/NameObj.h"

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