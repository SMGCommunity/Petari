#pragma once

#include "JSystem/JGeometry/TMatrix.h"
#include "JSystem/JGeometry/TPosition3.h"
#include "JSystem/JGeometry/TVec.h"
#include <revolution.h>

class ActorCameraInfo;
class CameraTargetArg;

namespace MR {
    TVec3f getCamPos();

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

    void declareEventCameraAnim(const ActorCameraInfo *, const char *, void *);
};