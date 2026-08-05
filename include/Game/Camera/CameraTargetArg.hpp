#pragma once

#include <revolution.h>

class CameraTargetObj;
class CameraTargetMtx;
class LiveActor;
class MarioActor;

class CameraTargetArg {
public:
    CameraTargetArg() : mTargetObj(), mTargetMtx(), mLiveActor(), mMarioActor() {
    }
    CameraTargetArg(const LiveActor*);

    CameraTargetArg(CameraTargetMtx* pTargetMtx) {
        mTargetObj = nullptr;
        mTargetMtx = pTargetMtx;
        mLiveActor = nullptr;
        mMarioActor = nullptr;
    }

    inline CameraTargetArg(CameraTargetObj* pTargetObj, CameraTargetMtx* pMtx, const LiveActor* pActor, MarioActor* pMario) {
        mTargetObj = pTargetObj;
        mTargetMtx = pMtx;
        mLiveActor = pActor;
        mMarioActor = pMario;
    }

    void setTarget() const;

    /* 0x00 */ CameraTargetObj* mTargetObj;
    /* 0x04 */ CameraTargetMtx* mTargetMtx;
    /* 0x08 */ const LiveActor* mLiveActor;
    /* 0x0C */ MarioActor* mMarioActor;
};
