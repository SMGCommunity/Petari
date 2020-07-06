#include "Camera/CameraTargetObj.h"
#include "smg.h"

CameraTargetObj::CameraTargetObj(const char *pName) : NameObj(pName)   
{
    mIsCameraWall = false;
}

void CameraTargetObj::init(const JMapInfoIter &pIter)
{

}

f32 CameraTargetObj::getRadius() const
{
    return 150.0f;
}

bool CameraTargetObj::isTurning() const
{
    return false;
}

bool CameraTargetObj::isJumping() const
{
    return false;
}

bool CameraTargetObj::isLongDrop() const
{
    return false;
}

bool CameraTargetObj::isFastDrop() const
{
    return false;
}

bool CameraTargetObj::isFastRise() const
{
    return false;
}

bool CameraTargetObj::isOnWaterSurface() const
{
    return false;
}

bool CameraTargetObj::isBeeMode() const
{
    return false;
}

bool CameraTargetObj::isFooFighterMode() const
{
    return false;
}

u32 CameraTargetObj::getSpecialMode() const
{
    return 0;
}

bool CameraTargetObj::isCameraStateOn(u32 state) const
{
    return false;
}

AreaObj* CameraTargetObj::getCubeCameraArea() const
{
    return nullptr;
}

u32* CameraTargetObj::getGroundTriangle() const
{
    return nullptr;
}

GravityInfo* CameraTargetObj::getGravityInfo() const
{
    return nullptr;
}

void CameraTargetObj::enableCameraWall()
{
    mIsCameraWall = true;
}

void CameraTargetObj::disableCameraWall()
{
    mIsCameraWall = false;
}

void CameraTargetObj::setCameraWall(bool cameraWall)
{
    mIsCameraWall = cameraWall;
}

bool CameraTargetObj::isDebugMode() const
{
    return false;
}

Mtx* CameraTargetObj::getMapBaseMtx() const
{
    return nullptr;
}

void CameraTargetObj::resetStatus()
{

}