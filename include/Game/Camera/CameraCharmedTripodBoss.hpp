#pragma once

#include "Game/Camera/Camera.hpp"

class CameraCharmedTripodBoss : public Camera {
public:
    CameraCharmedTripodBoss(const char* pName = "三脚ボスジョイント注視カメラ");
    virtual ~CameraCharmedTripodBoss();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void setParam(s32, TVec3f, const TVec3f&, const TVec2f&);

    s32 _4C;
    TVec3f _50;
    TVec3f _5C;
    TVec2f _68;
};
