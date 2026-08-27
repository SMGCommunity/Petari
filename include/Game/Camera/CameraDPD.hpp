#pragma once

#include "Game/Camera/Camera.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"

class CameraDPD : public Camera {
public:
    enum CameraType {
        CameraType_FixedPos = 0,
        CameraType_UpdateWithTarget = 1,
        CameraType_FollowWorldPos = 2,
    };
    CameraDPD(const char* pName = "DPD操作カメラ");

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void setParam(s32 cameraType, f32 dist, f32 angleXRange, f32 angleYRange, f32 angleX, f32 speedBlendRate, f32 blendFriction, bool isVertical,
                  f32 angleYMin) {
        mCameraType = cameraType;
        mDist = dist;
        mAngleXRange = angleXRange;
        mAngleYRange = angleYRange;
        mAngleX = angleX;
        mSpeedBlendRate = speedBlendRate;
        mBlendFriction = blendFriction;
        mIsVertical = isVertical;
        mAngleYMin = angleYMin;
    }

    /* 0x4C */ f32 mCorePadDistToDisplay;
    /* 0x50 */ TVec2f mRealPointPos;
    /* 0x58 */ TVec2f mBlendPointPos;
    /* 0x60 */ TPos3f mMtx;
    /* 0x90 */ bool mResetPointingPos;
    /* 0x94 */ s32 mCameraType;
    /* 0x98 */ f32 mDist;
    /* 0x9C */ f32 mFovy;
    /* 0xA0 */ f32 mAngleXRange;
    /* 0xA4 */ f32 mAngleYRange;
    /* 0xA8 */ f32 mAngleX;
    /* 0xAC */ f32 mSpeedBlendRate;
    /* 0xB0 */ f32 mBlendFriction;
    /* 0xB4 */ bool mIsVertical;
    /* 0xB8 */ f32 mAngleYMin;
};
