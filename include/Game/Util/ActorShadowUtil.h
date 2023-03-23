#pragma once

#include "JSystem/JGeometry/TVec.h"
#include <revolution.h>

class LiveActor;

namespace MR {
    void initShadowVolumeBox(LiveActor *, const TVec3f &);
    void initShadowVolumeBox(LiveActor *, const TVec3f &, MtxPtr);
    void initShadowVolumeCylinder(LiveActor *, f32);
    void setShadowDropLength(LiveActor *, const char *, f32);

    void initShadowVolumeFlatModel(LiveActor *, const char *, MtxPtr);

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

    void excludeCalcShadowToMyCollision(LiveActor *, const char *);

    void validateShadow(LiveActor *, const char *);
    void validateShadowGroup(LiveActor *, const char *);
    void invalidateShadow(LiveActor *, const char *);
    void invalidateShadowAll(LiveActor *);

    void initShadowFromCSV(LiveActor *, const char *);

    void initShadowSurfaceCircle(LiveActor *, f32);

    void setShadowDropPositionPtr(LiveActor *, const char *, const TVec3f *);

    void setShadowVolumeCylinderRadius(LiveActor *, const char *, f32);

    void onCalcShadowDropGravity(LiveActor *, const char *);

    void initShadowController(LiveActor *, u32);
    void addShadowVolumeSphere(LiveActor *, const char *, float);
    void setShadowDropDirectionPtr(LiveActor *, const char *, const TVec3f *);
    void addShadowVolumeLine(LiveActor *, const char *, LiveActor *, const char *, float, LiveActor *, const char *, float);
    bool isShadowProjected(const LiveActor *, const char *);
    void getShadowProjectionPos(const LiveActor *, const char *, TVec3f *);
    void getShadowProjectionNormal(const LiveActor *, const char *, TVec3f *);
};