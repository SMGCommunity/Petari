#pragma once

#include "revolution.h"

class CameraTargetObj;
class CameraTargetMtx;
class LiveActor;
class MarioActor;

class CameraTargetArg {
public:
    CameraTargetArg();
    CameraTargetArg(const LiveActor *);
    CameraTargetArg(CameraTargetMtx *);

    void setTarget() const;

    CameraTargetObj *mTargetObj;    // _0
    CameraTargetMtx *mTargetMtx;    // _4
    const LiveActor *mLiveActor;    // _8
    MarioActor *mMarioActor;        // _C
};