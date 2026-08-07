#pragma once

#include "Game/Camera/Camera.hpp"

class CameraInwardTower : public Camera {
public:
    CameraInwardTower(const char* pName = "塔内部カメラ");
    virtual ~CameraInwardTower();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual bool isEnableToReset() const {
        return true;
    }
    virtual CamTranslatorBase* createTranslator();

    void calcIdealPose();

    void setParam(const TVec3f& rWPoint, const TVec3f& rAxis, const TVec2f& angle, f32 dist) {
        mWPoint.set(rWPoint);
        mAxis.set(rAxis);
        mAngleX = angle.x;
        mAngleMax = angle.y;
        mDist = dist;
    }

    /* 0x4C */ TVec3f mWPoint;
    /* 0x58 */ TVec3f mAxis;
    /* 0x64 */ f32 mAngleX;
    /* 0x68 */ f32 mAngleMax;
    /* 0x6C */ f32 mDist;
    /* 0x70 */ TVec3f mPos;
    /* 0x7C */ f32 mAngleY;
    /* 0x80 */ bool mIsResetting;
};
