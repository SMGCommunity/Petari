#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class Camera;
class CameraHeightArrange;
class CameraMan;
class CameraParamChunk;
class CameraPoseParam;
class CameraTargetObj;

class CamTranslatorBase {
public:
    CamTranslatorBase() {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*) = 0;
    /* 0x0C */ virtual Camera* getCamera() const = 0;
};

class Camera : public NameObj {
public:
    Camera(const char*);

    /* 0x24 */ virtual void reset(){};
    /* 0x28 */ virtual CameraTargetObj* calc() = 0;
    /* 0x2C */ virtual bool isInterpolationOff() const {
        return false;
    }
    /* 0x30 */ virtual bool isCollisionOff() const {
        return false;
    }
    /* 0x34 */ virtual bool isZeroFrameMoveOff() const {
        return false;
    }
    /* 0x38 */ virtual bool isSubjectiveCameraOff() const {
        return false;
    }
    /* 0x3C */ virtual bool isCorrectingErpPositionOff() const {
        return false;
    }
    /* 0x40 */ virtual void roundLeft(){};
    /* 0x44 */ virtual void roundRight(){};
    /* 0x48 */ virtual bool isEnableToRoundLeft() const {
        return false;
    }
    /* 0x4C */ virtual bool isEnableToRoundRight() const {
        return false;
    }
    /* 0x50 */ virtual bool isEnableToReset() const {
        return false;
    }
    /* 0x54 */ virtual CamTranslatorBase* createTranslator();

    void setZoneMtx(s32);
    void createVPanObj();

    bool doesVPanExist() const {
        return mVPan != nullptr;
    }

    /* 0x0C */ CameraMan* mCameraMan;
    /* 0x10 */ CameraPoseParam* mPoseParam;
    /* 0x14 */ CameraHeightArrange* mVPan;
    /* 0x18 */ bool mIsLOfsErpOff;
    /* 0x1C */ TPos3f mZoneMatrix;
};

class CamTranslatorDummy : public CamTranslatorBase {
public:
    CamTranslatorDummy(Camera* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*){};
    /* 0x0C */ virtual Camera* getCamera() const {
        return mCamera;
    }

    /* 0x04 */ Camera* mCamera;
};
