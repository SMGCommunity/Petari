#pragma once

#include "Game/Camera/Camera.hpp"

class CameraDead : public Camera {
public:
    enum CameraType {
        CameraType_FixedPos = 0,
        CameraType_Interpolate = 1,
    };

    CameraDead(const char* pName = "通常死亡カメラ");

    /* 0x24 */ virtual void reset();
    /* 0x28 */ virtual CameraTargetObj* calc();
    /* 0x2C */ virtual bool isInterpolationOff() const {
        return true;
    }
    /* 0x54 */ virtual CamTranslatorBase* createTranslator();

    void setParam(f32 dist, s32 deadTime, s32 cameraType) {
        mDist = dist;
        mDeadTime = deadTime;
        mCameraType = cameraType;
    }

    /* 0x4C */ f32 mFovy;
    /* 0x50 */ f32 mDist;
    /* 0x54 */ s32 mDeadTime;
    /* 0x58 */ s32 mCameraType;
    /* 0x5C */ s32 mDeadFrame;
};
