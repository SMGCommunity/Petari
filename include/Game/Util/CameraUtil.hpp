#pragma once

#include <JSystem/JGeometry/TMatrix.hpp>

class LiveActor;
class ActorCameraInfo;
class CameraTargetArg;
class NameObj;
class JMapInfoIter;

namespace MR {
    bool calcScreenPosition(TVec2f*, const TVec3f&);
    bool calcScreenPosition(TVec3f*, const TVec3f&);
    bool calcNormalizedScreenPosition(TVec3f*, const TVec3f&);
    bool calcNormalizedScreenPositionFromView(TVec3f*, const TVec3f&);
    bool calcWorldPositionFromScreen(TVec3f*, const TVec2f&, f32);
    bool calcWorldPositionFromCenterScreen(TVec3f*, const TVec2f&, f32);
    bool calcWorldRayDirectionFromScreen(TVec3f*, const TVec2f&);

    f32 calcCameraDistanceZ(const TVec3f&);
    void loadProjectionMtx();
    void loadViewMtx();
    const TPos3f& getCameraViewMtx();
    const TPos3f& getCameraInvViewMtx();
    const TProj3f& getCameraProjectionMtx();

    f32 getAspect();
    f32 getNearZ();
    f32 getFarZ();
    f32 getFovy();
    void setCameraViewMtx(const TPos3f&, bool, bool, const TVec3f&);
    void setNearZ(f32);
    void setFovy(f32);
    void setShakeOffset(f32, f32);
    const TVec3f getCamPos();
    TVec3f getCamXdir();
    TVec3f getCamYdir();
    TVec3f getCamZdir();

    void createMirrorCamera();
    bool isExistMirrorCamera();
    const TPos3f& getMirrorCameraViewMtx();
    const TPos3f& getMirrorModelTexMtx();

    void completeCameraParameters();
    void resetCameraMan();
    void startCameraInterpolation(u32);

    void declareEventCamera(const ActorCameraInfo*, const char*);
    void endEventCamera(const ActorCameraInfo*, const char*, bool, s32);
    void endEventCameraAtLanding(const ActorCameraInfo*, const char*, s32);
    bool isEventCameraActive();
    bool isEventCameraActive(const ActorCameraInfo*, const char*);
    void declareGlobalEventCamera(const char*);
    void endGlobalEventCamera(const char*, s32, bool);
    void declareGlobalEventCameraAbyss(const char*);
    void declareGlobalEventCameraFixedThere(const char*, bool, f32);
    void declareGlobalEventCameraDead(const char*, f32, s32, s32);
    void declareEventCameraAnim(const ActorCameraInfo*, const char*, void*);

    bool isAnimCameraEnd(const ActorCameraInfo*, const char*);
    s32 getAnimCameraFrame(const ActorCameraInfo*, const char*);
    void pauseOnAnimCamera(const ActorCameraInfo*, const char*);
    void pauseOffAnimCamera(const ActorCameraInfo*, const char*);

    void declareBlackHoleCamera(const char*);
    void startBlackHoleCamera(const char*, const TVec3f&, const TVec3f&);

    void declareLauncherCamera();
    void endLauncherCamera();
    void setLauncherCameraAngle(f32, f32, f32, f32);
    void declareLauncherFlightCamera();
    void endLauncherFlightCamera();
    bool isActiveLauncherCamera();
    bool isActiveLauncherFlightCamera();

    void startSubjectiveCamera(s32);
    void endSubjectiveCamera(s32);

    void declareEventCameraProgrammable(const char*);
    void setProgrammableCameraParam(const char*, const TVec3f&, const TVec3f&, const TVec3f&, bool);
    void setProgrammableCameraParamFovy(const char*, f32);

    u32 getEventCameraFrames(const ActorCameraInfo*, const char*);

    void declareCameraRegisterMtx(const NameObj*, u32, MtxPtr);
    void declareCameraRegisterVec(const NameObj*, u32, TVec3f*);

    void startStartPosCamera(bool);
    void endStartPosCamera();
    bool isStartPosCameraEnd();
    bool hasStartAnimCamera();
    void startStartAnimCamera();
    bool isStartAnimCameraEnd();
    s32 getStartAnimCameraFrame();
    void endStartAnimCamera();

    bool isCameraInterpolatingNearlyEnd();
    bool isFirstPersonCamera();
    bool isPossibleToShiftToFirstPersonCamera();
    bool isCameraPossibleToRoundLeft();
    bool isCameraPossibleToRoundRight();
    bool isCameraControlNG();

    void startTalkCamera(const TVec3f&, const TVec3f&, f32, f32, s32);
    void endTalkCamera(bool, s32);

    void pauseOnCameraDirector();
    void pauseOffCameraDirector();

    const TVec3f& getCameraWatchPos();

    void zoomInTargetGameCamera();
    void zoomOutTargetGameCamera();

    bool isCameraInWater();

    void resetCameraLocalOffset();
    void overlayWithPreviousScreen(u32);

    bool isSubjectiveCameraOnForObjClipping();

    void setGameCameraTargetToPlayer();
    void setGameCameraTarget(const CameraTargetArg&);

    void changeEventCameraTarget(const ActorCameraInfo*, const char*, const CameraTargetArg&);
    void startEventCameraNoTarget(const ActorCameraInfo*, const char*, s32);
    void startEventCameraTargetPlayer(const ActorCameraInfo*, const char*, s32);
    void startEventCamera(const ActorCameraInfo*, const char*, const CameraTargetArg&, s32);
    void startGlobalEventCameraNoTarget(const char*, s32);
    void startGlobalEventCameraTargetPlayer(const char*, s32);
    void startGlobalEventCamera(const char*, const CameraTargetArg&, s32);
    void startEventCameraAnim(const ActorCameraInfo*, const char*, const CameraTargetArg&, s32, f32);

    void startLauncherCamera(const CameraTargetArg&);
    void startLauncherFlightCamera(s32);

    void cleanEventCameraTarget_temporally();
};  // namespace MR
