#include "Camera/CameraParamChunk.h"
#include "MR/MathUtil.h"
#include "string.h"

CameraParamChunk::CameraParamChunk(CameraHolder *pHolder, const CameraParamChunkID &chunk)
{
    mChunk = new CameraParamChunkID(chunk);
    mDefaultCamera = pHolder->getIndexOfDefault();
    mGeneralParams = new CameraGeneralParam();
    _64 = 0;
    mParams.init();
}

bool CameraParamChunk::isOnNoReset() const
{
    return mParams.mFlags & 0x1;
}

bool CameraParamChunk::isOnUseFovy() const
{
    return (mParams.mFlags >> 1) & 0x1;
}

bool CameraParamChunk::isLOfsErpOff() const
{
    return (mParams.mFlags >> 2) & 0x1;
}

bool CameraParamChunk::isAntiBlurOff() const
{
    return (mParams.mFlags >> 3) & 0x1;
}

bool CameraParamChunk::isCollisionOff() const
{
    return (mParams.mFlags >> 4) & 0x1;
}

bool CameraParamChunk::isSubjectiveCameraOff() const
{
    return (mParams.mFlags >> 5) & 0x1;
}

void CameraParamChunk::getVPanAxis(JGeometry::TVec3<f32> *out) const
{
    out->x = mParams.mPanAxis.x;
    out->y = mParams.mPanAxis.y;
    out->z = mParams.mPanAxis.z;

    if (MR::isNearZero(*out, 0.001f))
    {
        out->set<f32>(0.0f, 1.0f, 0.0f);
    }

    MR::normalize(out);
}

void CameraParamChunk::setUseFovy(bool flag)
{
    mParams.mFlags = (mParams.mFlags & 0xFFFFFFFD);

    if (flag)
    {
        mParams.mFlags |= 0x2;
    }
}

void CameraParamChunk::setLOFsErpOff(bool flag)
{
    mParams.mFlags = (mParams.mFlags & 0xFFFFFFFB);

    if (flag)
    {
        mParams.mFlags |= 0x4;
    }
}

void CameraParamChunk::setCollisionOff(bool flag)
{
    mParams.mFlags = (mParams.mFlags & 0xFFFFFFEF);

    if (flag)
    {
        mParams.mFlags |= 0x10;
    }
}

void CameraParamChunk::load(DotCamReader *pReader, CameraHolder *pHolder)
{
    const char* camType;
    pReader->getValueString("camtype", &camType);
    u32 version = pReader->getVersion();
    arrangeCamTypeName(version, &camType);

    bool isType = !(strcmp(camType, "CAM_TYPE_PLANET"));

    s8 idx = pHolder->getIndexOf(camType);

    if (idx == -1)
    {
       mDefaultCamera = pHolder->getIndexOfDefault();
    }
    else
    {
        mDefaultCamera = idx;
    }

    pReader->getValueVec("woffset", &mParams.mWOffset);
    pReader->getValueFloat("loffset", &mParams.mLOffset);
    pReader->getValueFloat("loffsetv", &mParams.mLOffsetV);
    pReader->getValueFloat("roll", &mParams.mRoll);
    pReader->getValueFloat("fovy", &mParams.mFovy);
    pReader->getValueInt("camint", &mParams.mCamInt);
    pReader->getValueFloat("upper", &mParams.mUpper);
    pReader->getValueFloat("lower", &mParams.mLower);
    pReader->getValueInt("gndint", &mParams.mGNDInt);
    pReader->getValueFloat("uplay", &mParams.mUPlay);
}

void CameraParamChunk::initiate()
{
    mParams.init();
    CameraGeneralParam params;
    mGeneralParams = &params;
}