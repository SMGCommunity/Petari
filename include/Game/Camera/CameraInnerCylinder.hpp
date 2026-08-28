#pragma once

#include "Game/Camera/Camera.hpp"

class CameraInnerCylinder : public Camera {
public:
    CameraInnerCylinder(const char* pName = "CameraInnerCylinder");

    /* 0x24 */ virtual void reset();
    /* 0x28 */ virtual CameraTargetObj* calc();
    /* 0x54 */ virtual CamTranslatorBase* createTranslator();

    void setParam(const TVec3f& rWPoint, const TVec3f& rAxis, const TVec2f& angle, f32 dist, f32 sideDist) {
        mWPoint.set(rWPoint);
        mAxis.set(rAxis);
        mAngleA = angle.x;
        mAngleB = angle.y;
        mDist = dist;
        mSideDist = sideDist;
    }

    /* 0x4C */ TVec3f mWPoint;
    /* 0x58 */ TVec3f mAxis;
    /* 0x64 */ f32 mAngleA;
    /* 0x68 */ f32 mAngleB;
    /* 0x6C */ f32 mDist;
    /* 0x70 */ f32 mSideDist;
    /* 0x74 */ TVec3f mLastWatchPoint;
    /* 0x80 */ bool mIsNotInitialized;
};
