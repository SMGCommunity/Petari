#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/ClippingActorInfo.hpp"
#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/LiveActor/ViewGroupCtrl.hpp"

namespace {
    static int sActorNumMax;
};

class ClippingActorHolder : public NameObj {
public:
    ClippingActorHolder();

    virtual ~ClippingActorHolder();
    virtual void movement();

    void registerActor(LiveActor *);
    void initSystemInfo(LiveActor *, const JMapInfoIter &);
    void initViewGroupTable();
    void entryLodCtrl(LodCtrl *, const JMapInfoIter &);
    void validateClipping(LiveActor *);
    void invalidateClipping(LiveActor *);
    void addToClippingTarget(LiveActor *);
    void removeFromClippingTarget(LiveActor *);
    ClippingActorInfo* startGroupClipping(LiveActor *, const JMapInfoIter &);
    void setTypeToSphere(LiveActor *, f32, const TVec3f *);
    void setFarClipLevel(LiveActor *, s32);
    ClippingActorInfo* find(const LiveActor *) const;

    u32 _C;
    ClippingActorInfoList* _10;
    ClippingActorInfoList* _14;
    ClippingActorInfoList* _18;
    ClippingActorInfoList* _1C;
    ViewGroupCtrl* mViewGroupCtrl;  // 0x20
};