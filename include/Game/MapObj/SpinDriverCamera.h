#pragma once

#include "Game/LiveActor/LiveActor.h"
#include "Game/Util/MultiEventCamera.h"

class SpinDriverCamera {
public:
    SpinDriverCamera();

    void startAppearCamera(LiveActor *, const TVec3f &, const TVec3f &, const TVec3f &);
    void endAppearCamera(LiveActor *);
    u32 getAppearCameraFrames() const;
    void start(const TVec3f &, const TVec3f &, const TVec3f &);
    void update(const TVec3f &, const TVec3f &);
    void cancel();
    void end();
    void updateTargetMatrix(const TVec3f &, const TVec3f &);
    BOOL isUseAppearCamera(LiveActor *) const;
    void init(const JMapInfoIter &, LiveActor *);
    void initAppearCamera(const JMapInfoIter &, LiveActor *);

    MultiEventCamera* mCamera;      // _0
    CameraTargetMtx* mTargetMtx;    // _4
    ActorCameraInfo* mCameraInfo;   // _8
    u32 mAppearCameraFrame;         // _C
};