#pragma once

#include "JSystem/JGeometry/TVec.h"
#include <revolution.h>

class LiveActor;
class Nerve;
class ResourceHolder;
class ActorLightCtrl;

namespace MR {
    void initDefaultPos(LiveActor *, const JMapInfoIter &);
    void validateClipping(LiveActor *);
    void invalidateClipping(LiveActor *);
    void setClippingTypeSphere(LiveActor *, f32);
    void setClippingFar200m(LiveActor *);
    void startBtk(const LiveActor *, const char *);

    void copyTransRotateScale(const LiveActor *, LiveActor *);
    bool isDead(const LiveActor *);

    bool isValidDraw(const LiveActor *);

    void invalidateClipping(LiveActor *);

    bool isClipped(const LiveActor *);
    bool isInvalidClipping(const LiveActor *);

    bool isHiddenModel(const LiveActor *);

    void showModel(LiveActor *);

    void hideModelAndOnCalcAnim(LiveActor *);

    ResourceHolder* getResourceHolder(const LiveActor *);

    bool isNoEntryDrawBuffer(const LiveActor *);

    void onEntryDrawBuffer(LiveActor *);
    void offEntryDrawBuffer(LiveActor *);

    const char* getModelResName(const LiveActor *);

    void calcAnimDirect(LiveActor *);

    void initLightCtrl(LiveActor *);
    void initLightCtrlForPlayer(LiveActor *);
    void initLightCtrlNoDrawEnemy(LiveActor *);
    void initLightCtrlNoDrawMapObj(LiveActor *);
    void updateLightCtrl(LiveActor *);
    void updateLightCtrlDirect(LiveActor *);
    void loadActorLight(const LiveActor *);
    void calcLightPos0(TVec3f *, const LiveActor *);
    void calcLightPos1(TVec3f *, const LiveActor *);
    const GXColor* getLightAmbientColor(const LiveActor *);
    ActorLightCtrl* getLightCtrl(const LiveActor *);
    bool isStep(const LiveActor *, s32);
    bool isFirstStep(const LiveActor *);
    bool isLessStep(const LiveActor *, s32);
    bool isLessEqualStep(const LiveActor *, s32);
    bool isGreaterStep(const LiveActor *, s32);
    bool isGreaterEqualStep(const LiveActor *, s32);
    bool isIntervalStep(const LiveActor *, s32);
    bool isNewNerve(const LiveActor *);
    f32 calcNerveRate(const LiveActor *, s32);
    f32 calcNerveRate(const LiveActor *, s32, s32);
    f32 calcNerveEaseInRate(const LiveActor *, s32);
    f32 calcNerveEaseOutRate(const LiveActor *, s32);
    f32 calcNerveEaseOutRate(const LiveActor *, s32, s32);
    f32 calcNerveEaseInOutRate(const LiveActor *, s32);
    f32 calcNerveEaseInOutRate(const LiveActor *, s32, s32);
    f32 calcNerveValue(const LiveActor *, s32, f32, f32);
    f32 calcNerveValue(const LiveActor *, s32, s32, f32, f32);
    f32 calcNerveEaseInValue(const LiveActor *, s32, f32, f32);
    f32 calcNerveEaseInValue(const LiveActor *, s32, s32, f32, f32);
    f32 calcNerveEaseOutValue(const LiveActor *, s32, f32, f32);
    f32 calcNerveEaseInOutValue(const LiveActor *, s32, f32, f32);
    f32 calcNerveEaseInOutValue(const LiveActor *, s32, s32, f32, f32);
    void setNerveAtStep(LiveActor *, const Nerve *, s32);
    void setNerveAtBckStopped(LiveActor *, const Nerve *);
    bool trySetNerve(LiveActor *, const Nerve *);

    bool isNoCalcAnim(const LiveActor *);
    
    void setBaseTRMtx(LiveActor *, MtxPtr);

    void setClippingFar(LiveActor *, f32);

    void setClippingTypeSphereContainsModelBoundingBox(LiveActor *, f32);
}
