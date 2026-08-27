#pragma once

#include "Game/Camera/Camera.hpp"

class CameraCharmedFix : public Camera {
public:
    CameraCharmedFix(const char* pName = "サンボカメラ");

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void setParam(const TVec3f& rBasePos, const TVec3f& rUp, const TVec3f& rWPoint) {
        mBasePos.set(rBasePos);
        mUp.set(rUp);
        mWPoint.set(rWPoint);
    }

    /* 0x4C */ TVec3f mBasePos;
    /* 0x58 */ TVec3f mUp;
    /* 0x64 */ TVec3f mWPoint;
};
