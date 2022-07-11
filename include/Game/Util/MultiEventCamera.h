#pragma once

#include "Game/Camera/CameraTargetArg.h"
#include "Game/LiveActor/ActorCameraInfo.h"

class MultiEventCamera {
public:
    MultiEventCamera();

    bool isEnd() const;
    void setUp(const char *, const ActorCameraInfo *, s32);
    void setEndCameraTypeSoon();
    void setEndCameraTypeAtLanding();
    void start(s32);
    void start(const CameraTargetArg &, s32);
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
    void changeTarget(const CameraTargetArg &);

    const char* mName;              // _0
    s32 mCameraCount;               // _4
    s32 _8;
    u32 _C;
    u32 _10;
    u32 mCameraType;                // _14
    ActorCameraInfo* mCameraInfo;   // _18
    CameraTargetArg mTarget;        // _1C
};