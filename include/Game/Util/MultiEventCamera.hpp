#pragma once

#include "Game/Camera/CameraTargetArg.hpp"

class ActorCameraInfo;

class MultiEventCamera {
public:
    enum CameraType {
        /* 0x00 */ CameraType_Soon,
        /* 0x01 */ CameraType_AtLanding,
    };

    /// @brief Creates a new `MultiEventCamera`.
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
    void endCamera(s32);
    void changeTarget(const CameraTargetArg&);

    /* 0x00 */ const char* mName;
    /* 0x04 */ s32 mCameraNum;
    /* 0x08 */ s32 _8;
    /* 0x0C */ s32 _C;
    /* 0x10 */ s32 _10;
    /* 0x14 */ s32 mCameraType;
    /* 0x18 */ const ActorCameraInfo* mCameraInfo;
    /* 0x1C */ CameraTargetArg mTarget;
};
