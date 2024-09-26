#pragma once

#include "revolution.h"
#include "Inline.hpp"

class CameraTargetObj;
class CameraTargetMtx;
class LiveActor;
class MarioActor;

class CameraTargetArg {
public:
    CameraTargetArg();
    CameraTargetArg(const LiveActor *);

    CameraTargetArg(CameraTargetMtx *pTargetMtx) NO_INLINE {
        mTargetObj = NULL;
        mTargetMtx = pTargetMtx;
        mLiveActor = NULL;
        mMarioActor = NULL;
    }
    
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

    inline CameraTargetArg(CameraTargetObj *pTargetObj, CameraTargetMtx *pMtx, const LiveActor *pActor, MarioActor *pMario) {
        mTargetObj = pTargetObj;
        mTargetMtx = pMtx;
        mLiveActor = pActor;
        mMarioActor = pMario;
    }
    
    void setTarget() const;

    CameraTargetObj *mTargetObj;    // 0x0
    CameraTargetMtx *mTargetMtx;    // 0x4
    const LiveActor *mLiveActor;    // 0x8
    MarioActor *mMarioActor;        // 0xC
};