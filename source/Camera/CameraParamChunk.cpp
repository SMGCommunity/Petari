#include "Camera/CameraParamChunk.h"
#include "MR/MathUtil.h"
#include "string.h"
#include "smg.h"

CameraGeneralParam& CameraGeneralParam::operator=(const CameraGeneralParam &rhs)
{
    // todo -- scheduling
    mDist = rhs.mDist;
    mAxis = rhs.mAxis;
    mWPoint = rhs.mWPoint;
    mUp = rhs.mUp;

    f32 angleA = rhs.mAngleA;
    f32 angleB = rhs.mAngleB;
    s32 num1 = rhs.mNum1;
    s32 num2 = rhs.mNum2;

    mAngleA = angleA;
    mAngleB = angleB;
    mNum1 = num1;
    mNum2 = num2;
    mString = rhs.mString;
    return *this;
}

CameraParamChunk::CameraParamChunk(CameraHolder *pHolder, const CameraParamChunkID &chunk)
{
    mChunk = new CameraParamChunkID(chunk);
    mDefaultCamera = pHolder->getIndexOfDefault();
    mGeneralParams = new CameraGeneralParam();
    _64 = 0;
    mParams.init();
}

s32 CameraParamChunk::getZoneID() const
{
    return mChunk->mChunkLength;
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

void CameraParamChunk::setCameraType(const char *pName, const CameraHolder *pHolder)
{
    mDefaultCamera = pHolder->getIndexOf(pName);
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
    const char* camType = nullptr;
    pReader->getValueString("camtype", &camType);
    u32 version = pReader->getVersion();
    arrangeCamTypeName(version, &camType);

    bool isTypePlanet = !(strcmp(camType, "CAM_TYPE_PLANET"));

    s32 idx = pHolder->getIndexOf(camType);

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
    pReader->getValueFloat("lplay", &mParams.mLPlay);
    pReader->getValueInt("pushdelay", &mParams.mPushDelay);
    pReader->getValueInt("pushdelaylow", &mParams.mPushDelayLow);
    pReader->getValueInt("udown", &mParams.mUDown);
    pReader->getValueInt("vpanuse", &mParams.mVPanUse);
    pReader->getValueVec("vpanaxis", &mParams.mPanAxis);

    for (s32 i = 0; i < 6; i++)
    {
        s32 val;
        bool ret = pReader->getValueInt(sFlagName[i], &val);

        if (ret)
        {
            mParams.mFlags = (mParams.mFlags | (val << i));
        }
    }

    pReader->getValueFloat("dist", &mGeneralParams->mDist);
    pReader->getValueVec("axis", &mGeneralParams->mAxis);
    pReader->getValueVec("wpoint", &mGeneralParams->mWPoint);
    pReader->getValueVec("up", &mGeneralParams->mUp);
    bool ret = pReader->getValueFloat("angleA", &mGeneralParams->mAngleA);

    if (!ret && isTypePlanet)
    {
        mGeneralParams->mAngleA = 30.0f;
    }

    pReader->getValueFloat("angleB", &mGeneralParams->mAngleB);
    pReader->getValueInt("num1", &mGeneralParams->mNum1);
    pReader->getValueInt("num2", &mGeneralParams->mNum2);

    const char* name;
    s32 res = pReader->getValueString("string", &name);

    if (res)
    {
        mGeneralParams->mString.setCharPtr(name);
    }
}

void CameraParamChunk::initiate()
{
    mParams.init();
    CameraGeneralParam params;
    *mGeneralParams = params;
}

const char* CameraParamChunk::getClassName() const
{
    return "Base";
}

void CameraParamChunk::arrangeCamTypeName(u32 cameraVersion, const char **pName)
{
    if (cameraVersion < 0x30004)
    {
        if (!strcmp(*pName, "CAM_TYPE_DONKETSU_TEST"))
        {
            *pName = "CAM_TYPE_BOSS_DONKETSU";
        }
    }
    else
    {
        if (cameraVersion < 0x30006)
        {
            if (!strcmp(*pName, "CAM_TYPE_BEHIND_DEBUG"))
            {
                *pName = "CAM_TYPE_SLIDER";
            }
            else
            {
                if (!strcmp(*pName, "CAM_TYPE_INWARD_TOWER_TEST"))
                {
                    *pName = "CAM_TYPE_INWARD_TOWER";
                }
                else
                {
                    if (!strcmp(*pName, "CAM_TYPE_EYE_FIXED_THERE_TEST"))
                    {
                        *pName = "CAM_TYPE_EYEPOS_FIX_THERE";
                    }
                }   
            }
        }
        else
        {
            if (cameraVersion < 0x30009)
            {
                if (!strcmp(*pName, "CAM_TYPE_ICECUBE_PLANET"))
                {
                    *pName = "CAM_TYPE_CUBE_PLANET";
                }
            }
        }
    }
}

CameraParamChunkGame::CameraParamChunkGame(CameraHolder *pHolder, const CameraParamChunkID &rChunk)
    : CameraParamChunk(pHolder, rChunk)
{
    mGameFlagThru = 1;
    mEnabledEndErpFrame = 0;
    mCamEndInt = 0x78;
}

void CameraParamChunkGame::load(DotCamReader *pReader, CameraHolder *pHolder)
{
    CameraParamChunk::load(pReader, pHolder);

    s32 temp;
    bool ret = pReader->getValueInt("gflag.thru", &temp);

    if (ret)
    {
        mGameFlagThru = temp;
    }
    else
    {
        mGameFlagThru = 0;
    }

    ret = pReader->getValueInt("gflag.enableEndErpFrame", &temp);

    if (ret)
    {
        mEnabledEndErpFrame = temp;
    }

    ret = pReader->getValueInt("gflag.camendint", &temp);

    if (ret)
    {
        mCamEndInt = temp;
    }
}

void CameraParamChunkGame::initiate()
{
    mParams.init();

    CameraGeneralParam params;

    *mGeneralParams = params;
    mGameFlagThru = 0;
    mEnabledEndErpFrame = 0;
}

void CameraParamChunkGame::copy(const CameraParamChunk *pChunk)
{
    CameraParamChunk::copy(pChunk);
}

CameraParamChunkEvent::CameraParamChunkEvent(CameraHolder *pHolder, const CameraParamChunkID &rChunk)
    : CameraParamChunk(pHolder, rChunk)
{
    mEnableErpFrame = 0;
    mEVFrm = 0;
    mEnableEndErpFrame = 0;
    mCamEndInt = 0x78;
    mEventPriority = 1;
}

void CameraParamChunkEvent::load(DotCamReader *pReader, CameraHolder *pHolder)
{
    CameraParamChunk::load(pReader, pHolder);

    s32 temp;

    if (pReader->getValueInt("eflag.enableErpFrame", &temp))
    {
        mEnableErpFrame = temp;
    }

    if (pReader->getValueInt("eflag.enableEndErpFrame", &temp))
    {
        mEnableEndErpFrame = temp;
    }

    if (pReader->getValueInt("camendint", &temp))
    {
        mCamEndInt = temp;
    }

    if (pReader->getValueInt("evfrm", &temp))
    {
        mEVFrm = temp;
    }

    if (pReader->getValueInt("evpriority", &temp))
    {
        mEventPriority = temp;
    }
}

void CameraParamChunkEvent::initiate()
{
    mParams.init();

    CameraGeneralParam param;
    *mGeneralParams = param;

    mEnableErpFrame = 0;
    mEnableEndErpFrame = 0;
    mEventPriority = 1;
}

void CameraParamChunkEvent::copy(const CameraParamChunk *pChunk)
{
    CameraParamChunk::copy(pChunk);
}