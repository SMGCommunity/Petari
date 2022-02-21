#pragma once

#include "Game/Camera/CameraTowerBase.h"

class CameraTowerPos : public CameraTowerBase {
public:
    CameraTowerPos(const char *);
    virtual ~CameraTowerPos();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual bool isEnableToReset() const;
    virtual CamTranslatorBase *createTranslator();

    void calcIdealPose(bool);
    void arrangeHorizontalPose(const TVec3f &, TVec3f &);
    void arrangeWatchPos(TVec3f &, const TVec3f &);

    TVec3f mWPoint; // _6C
    TVec3f mAxis;   // _78
    f32 mAngleA;    // _84
    f32 mAngleB;    // _88
    f32 mUpX;       // _8C
    f32 mUpY;       // _90
    f32 _94;
    f32 _98;
    f32 _9C;
};