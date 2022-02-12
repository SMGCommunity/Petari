#pragma once

#include <revolution.h>

class LiveActor;

namespace MR {
    void initShadowVolumeBox(LiveActor *, const TVec3f &);
    void initShadowVolumeBox(LiveActor *, const TVec3f &, MtxPtr);
    void initShadowVolumeCylinder(LiveActor *, f32);
    void setShadowDropLength(LiveActor *, const char *, f32);

    void setShadowVolumeStartDropOffset(LiveActor *, const char *, f32);
    void setShadowVolumeEndDropOffset(LiveActor *, const char *, f32);

    void setShadowDropPosition(LiveActor *, const char *, const TVec3f &);
    void setShadowDropDirection(LiveActor *, const char *, const TVec3f &);

    void setClippingRangeIncludeShadow(LiveActor *, TVec3f *, f32);

    void initShadowVolumeSphere(LiveActor *, f32);
    void onCalcShadowOneTime(LiveActor *, const char *);

    void onShadowVisibleSyncHostAll(LiveActor *);
    void offShadowVisibleSyncHostAll(LiveActor *);
    void onShadowFollowHostScale(LiveActor *, const char *);
    void onShadowFollowHostScaleAll(LiveActor *);

    void onCalcShadowDropPrivateGravity(LiveActor *, const char *);
    void onCalcShadowDropPrivateGravityOneTime(LiveActor *, const char *);

    void onShadowVolumeCutDropLength(LiveActor *, const char *);

    void onCalcShadow(LiveActor *, const char *);

    void validateShadow(LiveActor *, const char *);
    void invalidateShadow(LiveActor *, const char *);
};