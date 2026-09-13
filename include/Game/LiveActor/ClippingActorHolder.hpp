#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class ClippingActorInfo;
class ClippingActorInfoList;
class LiveActor;
class LodCtrl;
class ViewGroupCtrl;

class ClippingActorHolder : public NameObj {
public:
    /// @brief Creates a new `ClippingActorHolder`.
    ClippingActorHolder();

    virtual void movement() override;

    void registerActor(LiveActor*);
    void initSystemInfo(LiveActor*, const JMapInfoIter&);
    void initViewGroupTable();
    void entryLodCtrl(LodCtrl*, const JMapInfoIter&);
    void validateClipping(LiveActor*);
    void invalidateClipping(LiveActor*);
    void addToClippingTarget(LiveActor*);
    void removeFromClippingTarget(LiveActor*);
    ClippingActorInfo* startGroupClipping(LiveActor*, const JMapInfoIter&);
    void setTypeToSphere(LiveActor*, f32, const TVec3f*);
    void setFarClipLevel(LiveActor*, s32);
    ClippingActorInfo* find(const LiveActor*) const;

    /* 0x0C */ s32 mActorNum;
    /* 0x10 */ ClippingActorInfoList* _10;
    /* 0x14 */ ClippingActorInfoList* _14;
    /* 0x18 */ ClippingActorInfoList* _18;
    /* 0x1C */ ClippingActorInfoList* _1C;
    /* 0x20 */ ViewGroupCtrl* mViewGroupCtrl;
};
