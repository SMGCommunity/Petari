#pragma once

#include "revolution.h"
#include "Inline.h"

class CameraTargetObj;
class CameraTargetMtx;
class LiveActor;
class MarioActor;

class CameraTargetArg {
public:
    CameraTargetArg();
    CameraTargetArg(const LiveActor *);
    CameraTargetArg(CameraTargetMtx *);
    
    inline INLINE_FUNC_DECL_NO_ARG(CameraTargetArg) {
        mTargetObj = NULL;
        mTargetMtx = NULL;
        mLiveActor = NULL;
        mMarioActor = NULL;
    }
    
    inline INLINE_FUNC_DECL(CameraTargetArg, CameraTargetMtx *pTargetMtx) {
        mTargetObj = NULL;
        mTargetMtx = pTargetMtx;
        mLiveActor = NULL;
        mMarioActor = NULL;
    }
    
    void setTarget() const;

    CameraTargetObj *mTargetObj;    // _0
    CameraTargetMtx *mTargetMtx;    // _4
    const LiveActor *mLiveActor;    // _8
    MarioActor *mMarioActor;        // _C
};