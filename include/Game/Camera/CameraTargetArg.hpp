#pragma once

#include "Inline.hpp"
#include "revolution.h"

class CameraTargetObj;
class CameraTargetMtx;
class LiveActor;
class MarioActor;

class CameraTargetArg {
public:
    CameraTargetArg();
    CameraTargetArg(const LiveActor*);

    CameraTargetArg(CameraTargetMtx* pTargetMtx) NO_INLINE {
        mTargetObj = nullptr;
        mTargetMtx = pTargetMtx;
        mLiveActor = nullptr;
        mMarioActor = nullptr;
    }

    inline INLINE_FUNC_DECL_NO_ARG(CameraTargetArg) {
        mTargetObj = nullptr;
        mTargetMtx = nullptr;
        mLiveActor = nullptr;
        mMarioActor = nullptr;
    }

    inline INLINE_FUNC_DECL(CameraTargetArg, CameraTargetMtx* pTargetMtx) {
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

    CameraTargetObj* mTargetObj;  // 0x0
    CameraTargetMtx* mTargetMtx;  // 0x4
    const LiveActor* mLiveActor;  // 0x8
    MarioActor* mMarioActor;      // 0xC
};