#pragma once

#include "Game/Camera/Camera.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"

class CameraDPD : public Camera {
public:
    enum CameraState {
        CameraState_0 = 0,
        CameraState_1 = 1,
        CameraState_2 = 2,
    };
    CameraDPD(const char* pName = "DPD操作カメラ");

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    /* 0x4C */ f32 mCorePadDistToDisplay;
    /* 0x50 */ TVec2f mRealPointPos;
    /* 0x58 */ TVec2f mBlendPointPos;
    /* 0x60 */ TPos3f mMtx;
    /* 0x90 */ bool mResetPointingPos;
    /* 0x94 */ s32 mCameraState;
    /* 0x98 */ f32 mDist;
    /* 0x9C */ f32 mFovy;
    /* 0xA0 */ f32 mAngleXRange;
    /* 0xA4 */ f32 mAngleYRange;
    /* 0xA8 */ f32 mRotate;
    /* 0xAC */ f32 mSpeedBlendRate;
    /* 0xB0 */ f32 mBlendFriction;
    /* 0xB4 */ bool _B4;
    /* 0xB8 */ f32 _B8;
};
