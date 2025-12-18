#pragma once

#include "Game/Camera/Camera.hpp"
#include "Game/NameObj/NameObj.hpp"

class CameraDead : public Camera {
public:
    CameraDead(const char* pName = "通常死亡カメラ");
    virtual ~CameraDead();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual bool isInterpolationOff() const;
    virtual CamTranslatorBase* createTranslator();

    u8 _4C[4];
    f32 mDist;  // 0x50
    s32 _54;
    s32 _58;
    u32 _5C;
};
