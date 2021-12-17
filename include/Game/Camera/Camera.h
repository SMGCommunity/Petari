#pragma once

#include "revolution.h"
#include "Game/NameObj/NameObj.h"
#include "JSystem/JGeometry/TMatrix.h"

class Camera;
class CameraHeightArrange;
class CameraParamChunk;
class CameraPoseParam;

class CamTranslatorDummy {
public:
    inline CamTranslatorDummy(Camera *pCamera) {
        mCamera = pCamera;
    }

    virtual void setParam(const CameraParamChunk *);
    virtual Camera *getCamera() const;

    Camera *mCamera;
};

class Camera : public NameObj {
public:
    Camera(const char *);
    virtual ~Camera();

    virtual void reset();
    virtual void calc() = 0;
    virtual bool isInterpolationOff() const;
    virtual bool isCollisionOff() const;
    virtual bool isZeroFrameMoveOff() const;
    virtual bool isSubjectiveCameraOff() const;
    virtual bool isCorrectingErpPositionOff() const;
    virtual void roundLeft();
    virtual void roundRight();
    virtual bool isEnableToRoundLeft() const;
    virtual bool isEnableToRoundRight() const;
    virtual bool isEnableToReset() const;
    virtual CamTranslatorDummy *createTranslator();

    void setZoneMtx(long);
    void createVPanObj();

    u32 _C;
    CameraPoseParam *mPosParam; // _10
    CameraHeightArrange *mVPan; // _14
    u8 _18;
    u8 _19[3];
    TMtx34f mZoneMatrix;        // _1C
};