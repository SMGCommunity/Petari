#pragma once

#include "Game/Camera/Camera.hpp"

class CameraSpiral : public Camera {
public:
    enum EaseType {
        EaseType_Linear = 0,
        EaseType_InOut = 1,
    };
    CameraSpiral(const char* pName = "螺旋カメラ（デモ用）");

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void setParam(s32 easeType, s32 endTime, s32 startTime, f32 offYStart, f32 offYEnd, f32 distStart, f32 distEnd, f32 angleStart, f32 angleEnd) {
        mEndTime = endTime;
        mStartTime = startTime;
        mEaseType = easeType;
        mOffsetYStart = offYStart;
        mOffsetYEnd = offYEnd;
        mDistStart = distStart;
        mDistEnd = distEnd;
        mAngleStart = angleStart;
        mAngleEnd = angleEnd;
    }

    inline s32 getInterval() const {
        return (mEndTime - mStartTime);
    }

    /* 0x4C */ s32 mEndTime;
    /* 0x50 */ s32 mTimer;
    /* 0x54 */ s32 mStartTime;
    /* 0x58 */ s32 mEaseType;
    /* 0x5C */ f32 mOffsetYStart;
    /* 0x60 */ f32 mOffsetYEnd;
    /* 0x64 */ f32 mDistStart;
    /* 0x68 */ f32 mDistEnd;
    /* 0x6C */ f32 mAngleStart;
    /* 0x70 */ f32 mAngleEnd;
};
