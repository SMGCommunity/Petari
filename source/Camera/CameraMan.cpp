#include "Camera/CameraMan.h"
#include "Camera/CameraPoseParam.h"
#include "smg.h"

CameraMan::CameraMan(const char *pName) : NameObj(pName)
{
    mDirector = nullptr;
    mParams = new CameraPoseParam();
    mIsActive = false;
    _15 = 0;
    mMtx.identity();
}

void CameraMan::owned(CameraDirector *pDirector)
{
    mDirector = pDirector;
}

void CameraMan::released(CameraDirector *pDirector)
{
    mDirector = nullptr;
}

void CameraMan::activate(CameraDirector *pDirector)
{
    mIsActive = true;
    notifyActivate();
}

void CameraMan::deactivate(CameraDirector *pDirector)
{
    mIsActive = false;
    notifyDeactivate();
}

void CameraMan::movement()
{
    if (mIsActive)
    {
        calc();
        _15 = 0;
    }
}

void CameraMan::notifyActivate()
{

}

void CameraMan::notifyDeactivate()
{

}

void CameraMan::calc()
{

}

void CameraMan::init(const JMapInfoIter &iter)
{

}

bool CameraMan::isCollisionOff() const
{
    return false;
}

bool CameraMan::isZeroFrameMoveOff() const
{
    return false;
}

bool CameraMan::isInterpolationOff() const
{
    return false;
}

bool CameraMan::isSubjectiveCameraOff() const
{
    return false;
}

bool CameraMan::isEnableToRoundLeft() const
{
    return false;
}

bool CameraMan::isEnableToRoundRight() const
{
    return false;
}

bool CameraMan::isEnableToReset() const
{
    return false;
}

void CameraMan::roundLeft()
{

}

void CameraMan::roundRight()
{

}