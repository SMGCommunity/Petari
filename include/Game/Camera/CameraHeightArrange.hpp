#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class Camera;
class CameraPoseParam;
class CameraTargetObj;

class CameraHeightArrange : public NameObj {
public:
    enum JumpState {
        JumpState_NoMove = 0,
        JumpState_Rise = 1,
        JumpState_Fall = 2,
    };

    CameraHeightArrange(Camera*);

    virtual ~CameraHeightArrange();

    void updateJump();
    void calcWatchPos(CameraTargetObj*);
    void arrange();
    void resetJump();
    void reset();
    void resetParameter();
    void checkState();
    void arrangeHeight();
    void arrangeVPan();
    void calcPose();
    void updateUpper();
    void updateLower();
    f32 calcOffset(f32);
    void chase();
    void updateHeightAndOffset();
    const TVec3f& getGlobalAxis();

    /* 0x0C */ Camera* mCamera;
    /* 0x10 */ bool mIsJumping;
    /* 0x11 */ bool mIsStartJump;
    /* 0x12 */ bool mIsEndJump;
    /* 0x14 */ CameraPoseParam* mNextParam;
    /* 0x18 */ CameraPoseParam* mCurrParam;
    /* 0x1C */ u32 _1C;
    /* 0x20 */ u32 _20;
    /* 0x24 */ u32 _24;
    /* 0x28 */ f32 mCameraWatchPosBaseHeight;
    /* 0x2C */ f32 mCameraTargetPosHeight;
    /* 0x30 */ f32 mCameraWatchPosHeight;
    /* 0x34 */ f32 mCameraPosHeight;
    /* 0x38 */ f32 mCameraWatchPosOffset;
    /* 0x3C */ f32 mCameraPosOffset;
    /* 0x40 */ u32 mJumpState;
    /* 0x44 */ s32 mStateTime;
    /* 0x48 */ s32 _48;
    /* 0x4C */ bool mIsCatchup;
    /* 0x50 */ s32 mChaseTime;
    /* 0x54 */ s32 mRiseTime;
    /* 0x58 */ s32 mDropTime;
    /* 0x5C */ s32 mRiseEaseTime;
    /* 0x60 */ bool mUpdateGlobalAxis;
    /* 0x64 */ TVec3f mGlobalAxis;
    /* 0x70 */ f32 mOffsetScale;
    /* 0x74 */ f32 mOffsetScaleMax;
    /* 0x78 */ f32 mFocalScaleUpper;
    /* 0x7C */ f32 mFocalScaleLower;
    /* 0x80 */ s32 mGndInt;
    /* 0x84 */ f32 mPosOffsetMinRiseLag;
    /* 0x88 */ f32 mPosOffsetMinDropLag;
    /* 0x8C */ s32 mRiseDelay;
    /* 0x90 */ s32 mDropDelay;
    /* 0x94 */ s32 mMaxRiseEaseTime;
    /* 0x98 */ bool mVPanUse;
    /* 0x9C */ TVec3f mVPanAxis;
};
