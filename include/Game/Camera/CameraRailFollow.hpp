#pragma once

#include "Game/Camera/Camera.hpp"

class CameraRailFollow : public Camera {
public:
    CameraRailFollow(const char* pName = "レールフォローカメラ");
    virtual ~CameraRailFollow();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void setParam(s32, s32, f32, f32, f32, s32);

    u32 _4C;
    f32 _50;
    f32 _54;
    f32 _58;
    f32 _5C;
    f32 _60;
    u8 _64[4];
};