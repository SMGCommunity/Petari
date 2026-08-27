#pragma once

#include "Game/Camera/Camera.hpp"

class CameraCharmedVecRegTower : public Camera {
public:
    CameraCharmedVecRegTower(const char* pName = "CameraCharmedVecRegTower");

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void setParam(const char* pName, const TVec3f& rWPoint, const TVec3f& rAxis, f32 angleX, f32 angleYMax, f32 dist, bool arrange) {
        mName = pName;
        mWPoint.set(rWPoint);
        mAxis.set(rAxis);
        mAngleX = angleX;
        mAngleYMax = angleYMax;
        mDist = dist;
        mArrange = arrange;
    }

    /* 0x4C */ TVec3f mWPoint;
    /* 0x58 */ TVec3f mAxis;
    /* 0x64 */ f32 mAngleX;
    /* 0x68 */ f32 mAngleYMax;
    /* 0x6C */ f32 mDist;
    /* 0x70 */ bool mArrange;
    /* 0x74 */ const char* mName;
};
