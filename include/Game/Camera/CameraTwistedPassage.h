#pragma once

#include "Game/Camera/Camera.h"

class CameraTwistedPassage : public Camera {
public:
    CameraTwistedPassage(const char *);
    virtual ~CameraTwistedPassage();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorDummy *createTranslator();

    void setParam(long, long, float, float);
    void initStartPos();
    void calcNearestCoord(const TVec3f &) const;
    void calcNearestCoordBetweenTwoRailCoord(float, float, const TVec3f &) const;

    u32 _4C;
    f32 _50;
    f32 _54;
    u8 _58;
    u8 _59[3];
};