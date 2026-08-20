#pragma once

#include "Game/Camera/Camera.hpp"

class CameraTower : public Camera {
public:
    CameraTower(const char* pName = "塔カメラ");

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual bool isEnableToRoundLeft() const;
    virtual bool isEnableToRoundRight() const;
    virtual bool isEnableToReset() const {
        return true;
    }
    virtual CamTranslatorBase* createTranslator();
    virtual void arrangeRound();

    void setParam(const TVec3f& wPoint, const TVec3f& axis, f32 dist, const TVec2f& angle) {
        mWPoint.set(wPoint);
        mAxis.set(axis);
        mDist = dist;
        mAngleX = angle.x;
        mAngleYRoundSpeed = angle.y;
    }

    void calcIdealPos();
    void arrangeRoundingPose();
    void arrangeHorizontalPose();
    bool isUpsideDownByRoll() const;

    /* 0x4C */ TVec3f mWPoint;
    /* 0x58 */ TVec3f mAxis;
    /* 0x64 */ f32 mAngleX;
    /* 0x68 */ f32 mAngleYRoundSpeed;
    /* 0x6C */ f32 mDist;
    /* 0x70 */ TVec3f mPos;
    /* 0x7C */ bool mIsRounding;
    /* 0x80 */ s32 mRoundIntervalNum;
    /* 0x84 */ f32 mAngleYSpeed;
    /* 0x88 */ bool mIsRoundLeftValid;
    /* 0x89 */ bool mIsRoundRightValid;
};
