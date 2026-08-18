#pragma once

#include "Game/Camera/Camera.hpp"

class CameraFrontAndBack : public Camera {
public:
    CameraFrontAndBack(const char* pName = "表裏カメラ");
    virtual ~CameraFrontAndBack();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void calcBaseQuat(TQuat4f*, const TVec3f&);
    void calcDipQuat(TQuat4f*, const TVec3f&, const TVec3f&);

    void setParam(const TVec3f& wPoint, const TVec3f& axis, f32 rotateRate, f32 maxDipAngle, f32 dist) {
        mWPoint = wPoint;
        mAxis = axis;
        mRotateRate = rotateRate;
        mMaxDipAngle = maxDipAngle;
        mDist = dist;
    }

    /* 0x4C */ TVec3f mWPoint;
    /* 0x58 */ TVec3f mAxis;
    /* 0x64 */ f32 mRotateRate;
    /* 0x68 */ f32 mMaxDipAngle;
    /* 0x6C */ f32 mDist;
};
