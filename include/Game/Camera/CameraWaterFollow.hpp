#pragma once

#include "Game/Camera/Camera.hpp"

class CameraWaterFollow : public Camera {
public:
    CameraWaterFollow(const char *);
    virtual ~CameraWaterFollow();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual bool isEnableToReset() const;
    virtual CamTranslatorBase *createTranslator();

    f32 mAxisY; // _4C
    f32 mAxisX; // _50
    f32 mDist;  // _54
    f32 _58;
    u32 _5C;
    f32 _60;
    f32 _64;
    f32 _68;
    u8 _6C;
    u8 _6D[3];
    u32 _70;
    f32 _74;
    f32 _78;
    f32 _7C;
};