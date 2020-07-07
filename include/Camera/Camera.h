#pragma once

#include "Camera/CameraPoseParam.h"
#include "JGeometry/TMatrix34.h"
#include "Actor/NameObj/NameObj.h"

class CamTranslatorDummy
{
public:
    inline CamTranslatorDummy();
};

class Camera : public NameObj
{
public:
    Camera(const char *);

    void setZoneMtx(s32);

    u32 _C;
    CameraPoseParam* mParams; // _10
    u32 _14;
    u32 _18;
    JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > mMtx; // _1C
};