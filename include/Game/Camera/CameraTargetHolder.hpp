#pragma once

#include "revolution.h"

class CameraTargetActor;
class CameraTargetObj;
class CameraTargetPlayer;
class LiveActor;
class MarioActor;

class CameraTargetHolder {
public:
    CameraTargetHolder();
    virtual ~CameraTargetHolder();

    void movement();
    CameraTargetObj* get();
    void set(CameraTargetObj*);
    void set(const LiveActor*);
    void set(const MarioActor*);
    bool isOnGround() const;
    bool isMoving() const;

    CameraTargetObj* mTarget;           // 0x4
    CameraTargetActor* mTargetActor;    // 0x8
    CameraTargetPlayer* mTargetPlayer;  // 0xC
};
