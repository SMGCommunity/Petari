#pragma once

#include "revolution.h"
#include "Game/NameObj/NameObj.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"

class Camera;
class CameraHeightArrange;
class CameraMan;
class CameraParamChunk;
class CameraPoseParam;
class CameraTargetObj;

class CamTranslatorBase {
public:
    inline CamTranslatorBase() {

    }

    virtual void setParam(const CameraParamChunk *) = 0;
    virtual Camera *getCamera() const = 0;
};

class CamTranslatorDummy : public CamTranslatorBase {
public:
    inline CamTranslatorDummy(Camera *pCamera) {
        mCamera = pCamera;
    }

    virtual void setParam(const CameraParamChunk *);
    virtual Camera *getCamera() const;

    Camera *mCamera;    // 0x4
};

class Camera : public NameObj {
public:
    Camera(const char *);

    virtual inline ~Camera() {

    }

    virtual void reset();
    virtual CameraTargetObj* calc() = 0;
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
    virtual CamTranslatorBase *createTranslator();

    void setZoneMtx(long);
    void createVPanObj();

    CameraMan *mCameraMan;      // 0xC
    CameraPoseParam *mPosParam; // 0x10
    CameraHeightArrange *mVPan; // 0x14
    bool mIsLOfsErpOff;         // 0x18
    u8 _19[3];
    TMtx34f mZoneMatrix;        // 0x1C
};