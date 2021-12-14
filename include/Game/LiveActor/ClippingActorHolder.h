#pragma once

#include "Game/NameObj/NameObj.h"
#include "Game/LiveActor/ClippingActorInfo.h"
#include "Game/LiveActor/ViewGroupCtrl.h"

namespace {
    static int sActorNumMax;
};

class ClippingActorHolder : public NameObj {
public:
    ClippingActorHolder();

    virtual void movement();

    void registerActor(LiveActor *);
    void initSystemInfo(LiveActor *, const JMapInfoIter &);

    ClippingActorInfo* find(const LiveActor *) const;

    u32 _C;
    ClippingActorInfoList* _10;
    ClippingActorInfoList* _14;
    ClippingActorInfoList* _18;
    ClippingActorInfoList* _1C;
    ViewGroupCtrl* mViewGroupCtrl;  // _20
};