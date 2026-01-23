#pragma once

#include <JSystem/JGeometry/TMatrix.hpp>
#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/types.h>

class LiveActor;
class ActorCameraInfo;
class CameraTargetArg;
class NameObj;
class JMapInfoIter;

namespace MR {
    const TVec3f getCamPos();

    f32 calcCameraDistanceZ(const TVec3f&);

    bool isCameraInterpolateNearlyEnd();

    void setShakeOffset(f32, f32);

    void cleanEventCameraTarget_temporally();

    const TPos3f* getCameraProjectionMtx();
    const MtxPtr getCameraViewMtx();
    const MtxPtr getMirrorCameraViewMtx();
    const MtxPtr getMirrorModelTexMtx();
    TPos3f* getCameraInvViewMtx();

    void loadViewMtx();
    void loadProjectionMtx();

    void startEventCamera(const ActorCameraInfo*, const char*, const CameraTargetArg&, s32);
    void startEventCameraNoTarget(const ActorCameraInfo*, const char*, s32);
    void startEventCameraTargetPlayer(const ActorCameraInfo*, const char*, long);

    f32 getAspect();
    f32 getNearZ();
    f32 getFarZ();
    f32 getFovy();
    void setCameraViewMtx(const TPos3f&, bool, bool, const TVec3f&);
    void setNearZ(f32);
    void setFovy(f32);
    void setShakeOffset(f32, f32);
    TVec3f getCamXdir();
    TVec3f getCamYdir();
    TVec3f getCamZdir();
    void createMirrorCamera();
    bool isExistMirrorCamera();

    void completeCameraParameters();
    void resetCameraMan();
    void startCameraInterpolation(u32);
    void declareEventCamera(const ActorCameraInfo*, const char*);
    void endEventCamera(const ActorCameraInfo*, const char*, bool, s32);
    void endEventCameraAtLanding(const ActorCameraInfo*, const char*, s32);
    void declareGlobalEventCameraAbyss(const char*);
    void declareGlobalEventCameraFixedThere(const char*, bool, f32);
    void declareGlobalEventCameraDead(const char*, f32, s32, s32);
    void declareEventCameraAnim(const ActorCameraInfo*, const char*, void*);
    void declareBlackHoleCamera(const char*);
    void declareGlobalEventCamera(const char*);
    void declareEventCameraProgrammable(const char*);
    void setGameCameraTargetToPlayer();
    void setGameCameraTarget(const CameraTargetArg&);

    void startGlobalEventCamera(const char*, const CameraTargetArg&, s32);

    void startGlobalEventCameraNoTarget(const char*, s32);

    void setProgrammableCameraParam(const char*, const TVec3f&, const TVec3f&, const TVec3f&, bool);
    void setProgrammableCameraParamFovy(const char*, f32);

    bool calcScreenPosition(TVec2f*, const TVec3f&);
    bool calcScreenPosition(TVec3f*, const TVec3f&);
    bool calcNormalizedScreenPosition(TVec3f*, const TVec3f&);
    bool calcNormalizedScreenPositionFromView(TVec3f*, const TVec3f&);
    bool calcWorldPositionFromScreen(TVec3f*, const TVec2f&, f32);
    bool calcWorldRayDirectionFromScreen(TVec3f*, const TVec2f&);

    void declareEventCameraAnim(const ActorCameraInfo*, const char*, void*);

    void pauseOnAnimCamera(const ActorCameraInfo*, const char*);
    void pauseOffAnimCamera(const ActorCameraInfo*, const char*);

    void startBlackHoleCamera(const char*, const TVec3f&, const TVec3f&);

    bool createActorCameraInfoIfExist(const JMapInfoIter&, ActorCameraInfo**);

    void pauseOnCameraDirector();
    void pauseOffCameraDirector();

    bool endActorCamera(const LiveActor*, const ActorCameraInfo*, bool, s32);

    bool isStartAnimCameraEnd();

    void overlayWithPreviousScreen(u32);

    void resetCamerLocalOffset();

    void drawInitFor2DModel();

    void declareCameraRegisterMtx(const NameObj*, u32, MtxPtr);
    void declareCameraRegisterVec(const NameObj*, u32, TVec3f*);

    void startStartPosCamera(bool);
    void endStartPosCamera();
    bool isStartPosCameraEnd();
    bool hasStartAnimCamera();

    void endStartAnimCamera();

    void pauseOffCameraDirector();

    bool isCameraInWater();

    void startGlobalEventCameraTargetPlayer(const char*, s32);

    bool isEventCameraActive();
    bool isEventCameraActive(const ActorCameraInfo*, const char*);

    void endGlobalEventCamera(const char*, s32, bool);

    void startStartAnimCamera();
    s32 getStartAnimCameraFrame();

    void startEventCameraAnim(const ActorCameraInfo*, const char*, const CameraTargetArg&, long, float);

    s32 getAnimCameraFrame(const ActorCameraInfo*, const char*);

    u32 getEventCameraFrames(const ActorCameraInfo*, const char*);

    bool isAnimCameraEnd(const ActorCameraInfo*, const char*);

    void startBlackHoleCamera(const char*, const TVec3f&, const TVec3f&);
    void startSubjectiveCamera(s32);
    void endSubjectiveCamera(s32);

    bool isSubjectiveCameraOnForObjClipping();

    bool isFirstPersonCamera();
    bool isPossibleToShiftToFirstPersonCamera();

    bool isCameraPossibleToRoundLeft();
    bool isCameraPossibleToRoundRight();

    bool isCameraControlNG();

    void startTalkCamera(const TVec3f&, const TVec3f&, f32, f32, s32);
    void endTalkCamera(bool, s32);

    TVec3f* getCameraWatchPos();

    void zoomInTargetGameCamera();
    void zoomOutTargetGameCamera();

    void changeEventCameraTarget(const ActorCameraInfo*, const char*, const CameraTargetArg&);
};  // namespace MR
