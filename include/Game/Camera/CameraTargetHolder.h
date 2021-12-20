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
    CameraTargetObj *get();
    void set(CameraTargetObj *);
    void set(const LiveActor *);
    //void set(const MarioActor *);
    bool isOnGround() const;
    bool isMoving() const;

    CameraTargetObj *mTarget;           // _4
    CameraTargetActor *mTargetActor;    // _8
    CameraTargetPlayer *mTargetPlayer;  // _C
};
