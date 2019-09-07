#ifndef LIVEACTORUTIL_H
#define LIVEACTORUTIL_H

#include "types.h"
#include "System/Resource/ResourceHolder.h"

class LiveActor;

namespace MR
{
    bool isDead(const LiveActor *);
    bool isHiddenModel(const LiveActor *);
    void showModel(LiveActor *);
    void hideModel(LiveActor *);
    void hideModelAndOnCalcAnim(LiveActor *);

    const char* getModelResName(const LiveActor *);

    void invalidateClipping();

    bool isClipped(const LiveActor *);
    bool isNoEntryDrawBuffer(const LiveActor *);
    bool isNoBind(const LiveActor *);
    void onBind(LiveActor *);
    void offBind(LiveActor *);
    void calcAnimDirect(LiveActor *);

    void setCollisionMtx(LiveActor *);
    void validateCollisionParts(LiveActor *);
    void invalidateCollisionParts(LiveActor *);
    
    bool isCalcGravity(const LiveActor *);

    void calcGravity(const LiveActor *);

    void initLightCtrl(LiveActor *);
    void updateLightCtrl(LiveActor *);
    void setBaseTRMtx(LiveActor *, Mtx);

    void copyTransRotateScale(const LiveActor *, LiveActor *);

    ResourceHolder* getModelResourceHolder(const LiveActor *);

    void setClippingFar(LiveActor *, f32);
    void setClippingTypeSphereContainsModelBoundingBox(LiveActor *, f32);

    const char* createLowModelObjName(const LiveActor *);
    const char* createMiddleModelObjName(const LiveActor *);
};

#endif // LIVEACTORUTIL_H