#include "Camera/CameraHolder.h"
#include "Camera/Camera.h"
#include "string.h"

CameraHolder::CameraHolder(const char *pName) : NameObj(pName)
{
    createCameras();
    s32 idx = getIndexOf("CAM_TYPE_XZ_PARA");
    mDefaultCameraIdx = idx;
    mDefaultCamera = mCameras[idx];
}

CamTranslatorDummy* CameraHolder::getTranslator(s32 idx)
{
    return mTranslators[idx];
}

s32 CameraHolder::getIndexOf(const char *pCamName) const
{
    for (int i = 0; i < 0x2D; i++)
    {
        if (!strcmp(pCamName, sCameraTable[i].mName))
        {
            return i;
        }
    }

    return -1;
}

Camera* CameraHolder::getDefaultCamera()
{
    return mCameras[mDefaultCameraIdx];
}

s32 CameraHolder::getIndexOfDefault() const
{
    return mDefaultCameraIdx;
}

s32 CameraHolder::getIndexOf(Camera *pCamera) const
{
    for (int i = 0; i < 0x2D; i++)
    {
        if (mCameras[i] == pCamera)
        {
            return i;
        }
    }

    return -1;
}

void CameraHolder::createCameras()
{
    mCameras = (Camera**)new u8[0xB4];
    mTranslators = (CamTranslatorDummy**)new u8[0xB4];

    for (int i = 0; i < 0x2D; i++)
    {
        mCameras[i] = sCameraTable[i].mCreationFunc();
        mTranslators[i] = mCameras[i]->createTranslator();
    }
}

Camera* CameraHolder::getCameraInner(s32 idx) const
{
    return mCameras[idx];
}