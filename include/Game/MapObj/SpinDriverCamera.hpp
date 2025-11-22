#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/MultiEventCamera.hpp"

class SpinDriverCamera {
public:
    SpinDriverCamera();

    void startAppearCamera(LiveActor*, const TVec3f&, const TVec3f&, const TVec3f&);
    void endAppearCamera(LiveActor*);
    u32 getAppearCameraFrames() const;
    void start(const TVec3f&, const TVec3f&, const TVec3f&);
    void update(const TVec3f&, const TVec3f&);
    void cancel();
    void end();
    void updateTargetMatrix(const TVec3f&, const TVec3f&);
    BOOL isUseAppearCamera(LiveActor*) const;
    void init(const JMapInfoIter&, LiveActor*);
    void initAppearCamera(const JMapInfoIter&, LiveActor*);

    MultiEventCamera* mCamera;     // 0x0
    CameraTargetMtx* mTargetMtx;   // 0x4
    ActorCameraInfo* mCameraInfo;  // 0x8
    u32 mAppearCameraFrame;        // 0xC
};