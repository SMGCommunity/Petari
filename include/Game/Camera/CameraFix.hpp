#pragma once

#include "Game/Camera/Camera.hpp"

class CameraTargetMtx;

class CameraFix : public Camera {
public:
    CameraFix(const char* pName = "完全固定カメラ");

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void setParam(const TVec3f&, const TVec3f&, const TVec3f&);

    /* 0x4C */ TVec3f mWPoint;
    /* 0x58 */ TVec3f mAxis;
    /* 0x64 */ TVec3f mUp;
    /* 0x70 */ CameraTargetMtx* mTarget;
};
