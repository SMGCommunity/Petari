#pragma once

#include "JSystem/JGeometry.hpp"

class CameraTargetObj;
class Binder;

class CameraViewInterpolator {
public:
    enum CalcState {
        /* 0x0 */ CalcState_Ready,
        /* 0x1 */ CalcState_Invalid,
    };

    CameraViewInterpolator();

    void updateCameraMtx(MtxPtr, const TVec3f&, const CameraTargetObj*, f32);
    void setInterpolation(u32);
    bool isInterpolating() const;
    bool isInterpolatingNearlyEnd() const;
    void lookAtCenter();
    bool calcCollision(const TPos3f&);
    bool calcBinder(TVec3f*, const TVec3f&, const TVec3f&);
    void reduceOscillation();
    void updateCalcState(const CameraTargetObj*);
    void interpolateCameraSwitching(MtxPtr, const TVec3f&, f32);
    void checkNearlyEnd(MtxPtr);
    bool translateByRepulsion();

    void resetGravity() {
        TVec3f grav(0.0f, -1.0f, 0.0f);
        mGravity.set(grav);
    }

    /* 0x00 */ u32 mInterpolateTimer;
    /* 0x04 */ u32 mInterpolateTime;
    /* 0x08 */ bool mIsAntiOscillation;
    /* 0x09 */ bool mIsForceCameraChange;
    /* 0x0A */ bool mIsInterpolationOff;
    /* 0x0C */ TPos3f mTargetMtx;
    /* 0x3C */ TPos3f mMtx;
    /* 0x6C */ TVec3f mTargetPosition;
    /* 0x78 */ f32 mRate;
    /* 0x7C */ bool mIsCollisionOff;
    /* 0x7D */ bool mIsRepulsionOff;
    /* 0x80 */ f32 mTargetFovy;
    /* 0x84 */ f32 mFovy;
    /* 0x88 */ bool mIsInterpolatingNearlyEnd;
    /* 0x89 */ bool mIsInterpolating;
    /* 0x8A */ bool mIsCorrectErpPositionOn;
    /* 0x8C */ u32 mCalcState;
    /* 0x90 */ const CameraTargetObj* mTargetObj;
    /* 0x94 */ Binder* mBinder;
    /* 0x98 */ TVec3f mPosition;
    /* 0xA4 */ TVec3f mGravity;
    /* 0xB0 */ bool mIsCollided;
    /* 0xB4 */ TVec3f mLErpOffset;  // TODO: is this name correct?
};
