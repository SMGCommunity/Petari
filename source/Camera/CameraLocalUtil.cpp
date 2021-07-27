#include "Camera/CameraLocalUtil.h"
#include "Camera/CameraPoseParam.h"

JGeometry::TVec3f& CameraLocalUtil::getWatchPos(const CameraMan *pCameraMan)
{
    return pCameraMan->mParams->mWatchPos;
}

JGeometry::TVec3f& CameraLocalUtil::getPos(const CameraMan *pCameraMan)
{
    return pCameraMan->mParams->mPosition;
}

JGeometry::TVec3f& CameraLocalUtil::getUpVec(const CameraMan *pCameraMan)
{
    return pCameraMan->mParams->mUpVector;
}

JGeometry::TVec3f& CameraLocalUtil::getWatchUpVec(const CameraMan *pCameraMan)
{
    return pCameraMan->mParams->mWatchUpVec;
}

f32 CameraLocalUtil::getFovy(const CameraMan *pCameraMan)
{
    return pCameraMan->mParams->mFovy;
}

JGeometry::TVec3f& CameraLocalUtil::getGlobalOffset(const CameraMan *pCameraMan)
{
    return pCameraMan->mParams->mGlobalOffset;
}

JGeometry::TVec3f& CameraLocalUtil::getLocalOffset(const CameraMan *pCameraMan)
{
    return pCameraMan->mParams->mLocalOffset;
}

f32 CameraLocalUtil::getRoll(const CameraMan *pCameraMan)
{
    return pCameraMan->mParams->mRoll;
}

void CameraLocalUtil::setWatchPos(CameraMan *pCameraMan, const JGeometry::TVec3f &rWatchPos)
{
    pCameraMan->mParams->mWatchPos.set<f32>(rWatchPos);
}

void CameraLocalUtil::setPos(CameraMan *pCameraMan, const JGeometry::TVec3f &rPos)
{
    pCameraMan->mParams->mPosition.set<f32>(rPos);
}

void CameraLocalUtil::setUpVec(CameraMan *pCameraMan, const JGeometry::TVec3f &rUpVec)
{
    pCameraMan->mParams->mUpVector.set<f32>(rUpVec);
}

void CameraLocalUtil::setWatchUpVec(CameraMan *pCameraMan, const JGeometry::TVec3f &rWatchUp)
{
    pCameraMan->mParams->mWatchUpVec.set<f32>(rWatchUp);
}

void CameraLocalUtil::setFovy(CameraMan *pCameraMan, f32 fovy)
{
    pCameraMan->mParams->mFovy = fovy;
}

void CameraLocalUtil::setGlobalOffset(CameraMan *pCameraMan, const JGeometry::TVec3f &rGlobalOffset)
{
    pCameraMan->mParams->mGlobalOffset.set<f32>(rGlobalOffset);
}

void CameraLocalUtil::setLocalOffset(CameraMan *pCameraMan, const JGeometry::TVec3f &rLocalOffset)
{
    pCameraMan->mParams->mLocalOffset.set<f32>(rLocalOffset);
}

void CameraLocalUtil::setFrontOffset(CameraMan *pCameraMan, f32 frontOffset)
{
    pCameraMan->mParams->mFrontOffset = frontOffset;
}

void CameraLocalUtil::setUpperOffset(CameraMan *pCameraMan, f32 upperOffset)
{
    pCameraMan->mParams->mUpperOffset = upperOffset;
}

void CameraLocalUtil::setRoll(CameraMan *pCameraMan, f32 roll)
{
    pCameraMan->mParams->mRoll = roll;
}