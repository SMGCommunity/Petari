#pragma once

#include "Game/Camera/Camera.hpp"

class CameraTripodBossJoint : public Camera {
public:
    CameraTripodBossJoint(const char* pName = "三脚ボス関節カメラ");
    virtual ~CameraTripodBossJoint();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual bool isEnableToReset() const {
        return true;
    }
    virtual CamTranslatorBase* createTranslator();

    void setParam(const TVec2f& angle, f32 dist, s32 jointID, const TVec3f& axis) {
        mAngleX = angle.x;
        mAngleY = angle.y;
        mDist = dist;
        mJointID = jointID;
        mAxis.set(axis);
    }

    void calcIdealPose();

    /* 0x4C */ f32 mAngleX;
    /* 0x50 */ f32 mAngleY;
    /* 0x54 */ f32 mDist;
    /* 0x58 */ s32 mJointID;
    /* 0x5C */ TVec3f mAxis;
    /* 0x68 */ bool mIsRounding;
    /* 0x6C */ f32 mRoundTarget;
    /* 0x70 */ f32 mRoundAngle;
};
