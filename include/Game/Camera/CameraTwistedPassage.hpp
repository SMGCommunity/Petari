#pragma once

#include "Game/Camera/Camera.hpp"

class CameraTwistedPassage : public Camera {
public:
    CameraTwistedPassage(const char* pName = "ねじれ回廊カメラ");
    virtual ~CameraTwistedPassage();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void setParam(s32, s32, f32, f32);
    void initStartPos();
    void calcNearestCoord(const TVec3f&) const;
    void calcNearestCoordBetweenTwoRailCoord(f32, f32, const TVec3f&) const;

    u32 _4C;
    f32 _50;
    f32 _54;
    u8 _58;
    u8 _59[3];
};