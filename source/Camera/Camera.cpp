#include "Camera/Camera.h"

#include "Util/SceneUtil.h"

#include <revolution/wpad.h>

Camera::Camera(const char *pName) : NameObj(pName)
{
    mParams = new CameraPoseParam();
    mHeightArrangeCamera = 0;
    _18 = 0;
    mMtx.identity();
}

void Camera::setZoneMtx(s32 zoneNum)
{
    if (zoneNum < 0)
    {
        mMtx.identity();
    }
    else
    {
        mMtx.identity();

        register JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> >* mtx = MR::getZonePlacementMtx(zoneNum);

        register JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> >* dest = &mMtx;

        asm
        {
            psq_l f0, 0(mtx), 0, 0
            psq_l f1, 8(mtx), 0, 0
            psq_l f2, 0x10(mtx), 0, 0
            psq_l f3, 0x18(mtx), 0, 0
            psq_l f4, 0x20(mtx), 0, 0
            psq_l f5, 0x28(mtx), 0, 0
            psq_st f0, 0x1C(dest), 0, 0
            psq_st f1, 0x24(dest), 0, 0
            psq_st f2, 0x2C(dest), 0, 0
            psq_st f3, 0x34(dest), 0, 0
            psq_st f4, 0x3C(dest), 0, 0
            psq_st f5, 0x44(dest), 0, 0
        };
    }
    
    if (mHeightArrangeCamera)
    {
        mHeightArrangeCamera->_60 = 1;
    }
}

CamTranslatorDummy* Camera::createTranslator()
{
    return new CamTranslatorDummy(this);
}

void Camera::createVPanObj()
{
    mHeightArrangeCamera = new CameraHeightArrange(this);
}

Camera::~Camera() { }

Camera* CamTranslatorDummy::getCamera() const
{
    return mCamera;
}

void CamTranslatorDummy::setParam(const CameraParamChunk *pChunk)
{

}

bool Camera::isEnableToReset() const
{
    return false;
}

bool Camera::isEnableToRoundRight() const
{
    return false;
}

bool Camera::isEnableToRoundLeft() const
{
    return false;
}

void Camera::roundRight()
{

}

void Camera::roundLeft()
{

}

bool Camera::isCorrectingErpPositionOff() const
{
    return false;
}

bool Camera::isSubjectiveCameraOff() const
{
    return false;
}

bool Camera::isZeroFrameMoveOff() const
{
    return false;
}

bool Camera::isCollisionOff() const
{
    return false;
}

bool Camera::isInterpolationOff() const
{
    return false;
}

void Camera::reset()
{
    
}