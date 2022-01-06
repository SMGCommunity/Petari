#pragma once

#include <revolution.h>
#include "JSystem/JGeometry.h"

class ActorCameraInfo;
class CameraTargetArg;

namespace MR {
    TVec3f getCamPos();

    f32 calcCameraDistanceZ(const TVec3f &);

    bool isCameraInterpolateNearlyEnd();

    void setShakeOffset(float, float);

    void cleanEventCameraTarget_temporally();

    MtxPtr getCameraViewMtx();
    TPos3f *getCameraInvViewMtx();

    void loadProjectionMtx();
    void setCameraViewMtx(const TPos3f &, bool, bool, const TVec3f &);

    void startEventCamera(const ActorCameraInfo *, const char *, const CameraTargetArg &, long);

    void endEventCamera(const ActorCameraInfo *,const char *, bool, long);

    f32 getNearZ();
    f32 getFarZ();

    void setNearZ(f32);

    TVec3f getCamXDir();
    TVec3f getCamYDir();
    TVec3f getCamZDir();

    void declareEventCamera(const ActorCameraInfo *, const char *);
    void declareEventCameraAnim(const ActorCameraInfo *, const char *, void *);

    bool isExistMirrorCamera();
};