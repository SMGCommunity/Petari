#pragma once

#include "Game/Camera/CameraTowerBase.h"

class CameraTowerPos : public CameraTowerBase {
public:
    CameraTowerPos(const char *);

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual bool isEnableToReset() const;
    virtual CamTranslatorDummy *createTranslator();

    void calcIdealPose(bool);
    void arrangeHorizontalPose(const TVec3f &, TVec3f &);
    void arrangeWatchPos(TVec3f &, const TVec3f &);

    f32 _6C;
    f32 _70;
    f32 _74;
    f32 _78;
    f32 _7C;
    f32 _80;
    f32 _84;
    f32 _88;
    f32 _8C;
    f32 _90;
    f32 _94;
    f32 _98;
    f32 _9C;
};