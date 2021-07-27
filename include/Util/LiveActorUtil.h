#pragma once

#include <revolution.h>
#include "JSystem/JGeometry/TVec3.h"
#include "JSystem/JMap/JMapInfoIter.h"
#include "System/Resource/ResourceHolder.h"

class LiveActor;

namespace MR
{
    void copyTransRotateScale(const LiveActor *, LiveActor *);


    void initDefaultPos(LiveActor *, const JMapInfoIter &);
    void initDefaultPosNoRepeat(LiveActor *, const JMapInfoIter *);
    bool isValidMovement(const LiveActor *);
    bool isValidCalcAnim(const LiveActor *);
    bool isValidCalcViewAndEntry(const LiveActor *);
    bool isValidDraw(const LiveActor *);
    void calcAnimDirect(LiveActor *);

    bool isDead(const LiveActor *);
    bool isHiddenModel(const LiveActor *);
    void showModel(LiveActor *);
    void hideModel(LiveActor *);
    void hideModelAndOnCalcAnim(LiveActor *);
    void showModelIfHidden(LiveActor *);

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
    void onCalcGravity(const LiveActor *);
    void offCalcGravity(const LiveActor *);
    
    void calcGravity(const LiveActor *);

    bool isValidDraw(const LiveActor *);

    void initLightCtrl(LiveActor *);
    void updateLightCtrl(LiveActor *);
    void setBaseTRMtx(LiveActor *, Mtx);

    bool isStep(const LiveActor *, s32);
    bool isFirstStep(const LiveActor *);
    bool isLessStep(const LiveActor *, s32);
    bool isLessEqualStep(const LiveActor *, s32);

    f32 calcNerveEaseInValue(const LiveActor *, s32, f32, f32);

    void onEntryDrawBuffer(LiveActor *);
    void offEntryDrawBuffer(LiveActor *);

    void copyTransRotateScale(const LiveActor *, LiveActor *);

    ResourceHolder* getModelResourceHolder(const LiveActor *);
    void setClippingFar50m(LiveActor *);
    void setClippingFar100m(LiveActor *);
    void setClippingFar200m(LiveActor *);
    void setClippingFar300m(LiveActor *);
    void setClippingFarMax(LiveActor *);
    void setClippingFar(LiveActor *, f32);
    void setClippingTypeSphere(LiveActor *, f32, const JGeometry::TVec3<f32> &);
    void setClippingTypeSphereContainsModelBoundingBox(LiveActor *, f32);

    bool isExistIndirectTexture(const LiveActor *);

    const char* createLowModelObjName(const LiveActor *);
    const char* createMiddleModelObjName(const LiveActor *);
};