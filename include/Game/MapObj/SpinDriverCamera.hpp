#pragma once

#include <JSystem/JGeometry/TVec.hpp>

class ActorCameraInfo;
class CameraTargetMtx;
class JMapInfoIter;
class LiveActor;
class MultiEventCamera;

class SpinDriverCamera {
public:
    /// @brief Creates a new `SpinDriverCamera`.
    SpinDriverCamera();

    void startAppearCamera(LiveActor*, const TVec3f&, const TVec3f&, const TVec3f&);
    void endAppearCamera(LiveActor*);
    s32 getAppearCameraFrames() const;
    void start(const TVec3f&, const TVec3f&, const TVec3f&);
    void update(const TVec3f&, const TVec3f&);
    void cancel();
    void end();
    void updateTargetMatrix(const TVec3f&, const TVec3f&);
    bool isUseAppearCamera(LiveActor*) const;
    void init(const JMapInfoIter&, LiveActor*);
    void initAppearCamera(const JMapInfoIter&, LiveActor*);

    /* 0x00 */ MultiEventCamera* mCamera;
    /* 0x04 */ CameraTargetMtx* mTargetMtx;
    /* 0x08 */ ActorCameraInfo* mCameraInfo;
    /* 0x0C */ s32 mAppearCameraFrame;
};
