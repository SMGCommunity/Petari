#include "Camera/Camera.h"

#include <revolution/wpad.h>

Camera::Camera(const char *pName) : NameObj(pName)
{
    mParams = new CameraPoseParam();
    _14 = 0;
    _18 = 0;
    mMtx.identity();
}

void Camera::reset()
{
    
}

bool Camera::isInterpolationOff() const
{
    return false;
}

bool Camera::isCollisionOff() const
{
    return false;
}

bool Camera::isZeroFrameMoveOff() const
{
    return false;
}

bool Camera::isSubjectiveCameraOff() const
{
    return false;
}

bool Camera::isCorrectingErpPositionOff() const
{
    return false;
}

void Camera::roundLeft()
{

}

void Camera::roundRight()
{

}

bool Camera::isEnableToRoundLeft() const
{
    return false;
}

bool Camera::isEnableToRoundRight() const
{
    return false;
}

bool Camera::isEnableToReset() const
{
    return false;
}

CamTranslatorDummy* Camera::createTranslator()
{
    return new CamTranslatorDummy(this);
}

void CamTranslatorDummy::setParam(const CameraParamChunk *pChunk)
{

}

Camera* CamTranslatorDummy::getCamera() const
{
    return mCamera;
}