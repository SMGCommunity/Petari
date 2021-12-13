#pragma once

#include "revolution.h"
#include "Game/NameObj/NameObj.h"
#include "JSystem/JGeometry/TMatrix.h"

class Camera;
class CameraHeightArrange;
class CameraPoseParam;

class CamTranslatorDummy {
public:
    inline CamTranslatorDummy(Camera *);

    //virtual void setParam(const CameraParamChunk *);
    virtual Camera *getCamera() const;

    Camera *mCamera;
};

class Camera : public NameObj {
public:
    Camera(const char *);

    CamTranslatorDummy *createTranslator();
    void createVPanObj();

    u32 _C;
    CameraPoseParam *mPosParam; // _10
    CameraHeightArrange *mVPan; // _14
    u8 _18;
    u8 _19[3];
    TMtx34f mZoneMatrix;        // _1C
};