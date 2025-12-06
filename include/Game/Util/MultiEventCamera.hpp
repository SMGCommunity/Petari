#pragma once

#include "Game/Camera/CameraTargetArg.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"

class MultiEventCamera {
public:
    MultiEventCamera();

    bool isEnd() const;
    void setUp(const char*, const ActorCameraInfo*, s32);
    void setEndCameraTypeSoon();
    void setEndCameraTypeAtLanding();
    void start(s32);
    void start(const CameraTargetArg&, s32);
    void update();
    void nextForce();
    void endForce();
    void endForceSoon();
    void endForceAtLanding();
    void changeTargetPlayer();
    bool isActive(s32) const;
    void declareEventCamera(s32);
    void startCamera();
    void endCamera();
    void changeTarget(const CameraTargetArg&);

    const char* mName;  // 0x0
    s32 mCameraCount;   // 0x4
    s32 _8;
    u32 _C;
    u32 _10;
    u32 mCameraType;               // 0x14
    ActorCameraInfo* mCameraInfo;  // 0x18
    CameraTargetArg mTarget;       // 0x1C
};