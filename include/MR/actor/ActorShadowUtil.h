#ifndef ACTORSHADOWUTIL_H
#define ACTORSHADOWUTIL_H

#include "types.h"
#include "JGeometry/TVec3.h"

class LiveActor;

namespace MR
{
    void initShadowFromCSV(LiveActor *, const char *);
    void initShadowSurfaceCircle(LiveActor *, f32);
    void initShadowVolumeSphere(LiveActor *, f32);
    void initShadowVolumeOval(LiveActor *, const JGeometry::TVec3<f32> &);
    void initShadowVolumeCylinder(LiveActor *, f32);
    void initShadowVolumeBox(LiveActor *, const JGeometry::TVec3<f32> &);
    void initShadowVolumeFlatModel(LiveActor *, const char *);
    void initShadowController(LiveActor *, u32);
    void addShadowSurfaceCircle(LiveActor *, const char *, f32);
    void addShadowVolumeSphere(LiveActor *, const char *, f32);

    void addShadowVolumeCylinder(LiveActor *, const char *, f32);
    void addShadowVolumeBox(LiveActor *, const char *, const JGeometry::TVec3<f32> &);

    void addShadowVolumeLine(LiveActor *, const char *, LiveActor *, const char *, f32, LiveActor *, const char *, f32);
    void addShadowVolumeFlatModel(LiveActor *, const char *, const char *);
    void addShadowVolumeFlatModel(LiveActor *, const char *, const char *, Mtx *);

    void initShadowVolumeFlatModel(LiveActor *, const char *, Mtx *);

    void setShadowDropLength(LiveActor *, const char *, f32);

    void setShadowDropPosition(LiveActor *, const char *, const JGeometry::TVec3<f32> &);

    void onCalcShadowDropPrivateGravity(LiveActor *, const char *);

    void onCalcShadowDropGravityOneTime(LiveActor *, const char *);
    void onCalcShadowOneTime(LiveActor *, const char *);

    void onShadowVisibleSyncHostAll(LiveActor *);
    void offShadowVisibleSyncHostAll(LiveActor *);
};

#endif // ACTORSHADOWUTIL_H