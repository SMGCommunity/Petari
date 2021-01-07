#pragma once


#include "Camera/CameraPoseParam.h"
#include "JGeometry/TMatrix34.h"
#include "NameObj/NameObj.h"

class CamTranslatorDummy;
class CameraParamChunk;

class Camera : public NameObj
{
public:
    Camera(const char *);

    virtual ~Camera();
    virtual void reset();
    virtual s32 calc() = 0;
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
    virtual CamTranslatorDummy* createTranslator();

    void setZoneMtx(s32);

    u32 _C;
    CameraPoseParam* mParams; // _10
    u32 _14;
    u32 _18;
    JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > mMtx; // _1C
};

class CamTranslatorDummy
{
public:
    inline CamTranslatorDummy(Camera *pCamera) : mCamera(pCamera) { }

    virtual void setParam(const CameraParamChunk *);
    virtual Camera* getCamera() const;

    Camera* mCamera; // _4
};