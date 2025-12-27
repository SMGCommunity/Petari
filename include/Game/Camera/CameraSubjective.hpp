#pragma once

#include "Game/Camera/Camera.hpp"

class CameraSubjective : public Camera {
public:
    CameraSubjective(const char* pName = "主観カメラ");
    virtual ~CameraSubjective();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual bool isCollisionOff() const;
    virtual CamTranslatorBase* createTranslator();

    f32 _4C;
    f32 _50;
    f32 _54;
    f32 _58;
    TMtx34f _5C;
};