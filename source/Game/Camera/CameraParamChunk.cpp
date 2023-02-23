#include "Game/Camera/CameraHolder.h"
#include "Game/Camera/CameraParamChunk.h"
#include "Game/Camera/CameraParamChunkID.h"
#include "Game/Camera/DotCamParams.h"
#include "Game/Util/MathUtil.h"
#include <cstring>

#ifdef NON_MATCHING
// String constructor called first instead of last
CameraGeneralParam::CameraGeneralParam() : mString() {
    mDist = 1200.0f;
    mAxis.x = 0.0f;
    mAxis.y = 1.0f;
    mAxis.z = 0.0f;
    mWPoint.x = 0.0f;
    mWPoint.y = 0.0f;
    mWPoint.z = 0.0f;
    mUp.x = 0.0f;
    mUp.y = 0.0f;
    mUp.z = 0.0f;
    mAngleA = 0.0f;
    mAngleB = 0.3f;
    mNum1 = 0;
    mNum2 = 0;
}
#endif

CameraGeneralParam& CameraGeneralParam::operator=(const CameraGeneralParam &rOther) {
    mDist = rOther.mDist;
    mAxis = rOther.mAxis;
    mWPoint = rOther.mWPoint;
    mUp = rOther.mUp;
    mAngleA = rOther.mAngleA;
    mAngleB = rOther.mAngleB;
    mNum1 = rOther.mNum1;
    mNum2 = rOther.mNum2;
    mString = rOther.mString;

    return *this;
}

CameraParamChunk::ExParam::ExParam() {

}

void CameraParamChunk::ExParam::init() {
    mWOffset.set(0.0f, 100.0f, 0.0f);
    mLOffset = 0.0f;
    mLOffsetV = 0.0f;
    mRoll = 0.0f;
    mFovy = 45.0f;
    mCamInt = 120;
    mFlag = 0;
    mUpper = 0.3f;
    mLower = 0.1f;
    mGndInt = 160;
    mUPlay = 300.0f;
    mLPlay = 800.0f;
    mPushDelay = 120;
    mPushDelayLow = 120;
    mUDown = 120;
    mVPanUse = 1;
    mVPanAxis.x = 0.0f;
    mVPanAxis.y = 1.0f;
    mVPanAxis.z = 0.0f;
}

CameraParamChunk::CameraParamChunk(CameraHolder *pHolder, const CameraParamChunkID &rChunk) {
    mParamChunkID = new CameraParamChunkID(rChunk);
    mCameraTypeIndex = pHolder->getIndexOfDefault();
    mGeneralParam = new CameraGeneralParam();
    _64 = false;
    mExParam.init();
}

void CameraParamChunk::copy(const CameraParamChunk *pOther) {
    mCameraTypeIndex = pOther->mCameraTypeIndex;
    mExParam.mWOffset = pOther->mExParam.mWOffset;
    mExParam.mLOffset = pOther->mExParam.mLOffset;
    mExParam.mLOffsetV = pOther->mExParam.mLOffsetV;
    mExParam.mRoll = pOther->mExParam.mRoll;
    mExParam.mFovy = pOther->mExParam.mFovy;
    mExParam.mCamInt = pOther->mExParam.mCamInt;
    mExParam.mFlag = pOther->mExParam.mFlag;
    mExParam.mUpper = pOther->mExParam.mUpper;
    mExParam.mLower = pOther->mExParam.mLower;
    mExParam.mGndInt = pOther->mExParam.mGndInt;
    mExParam.mUPlay = pOther->mExParam.mUPlay;
    mExParam.mLPlay = pOther->mExParam.mLPlay;
    mExParam.mPushDelay = pOther->mExParam.mPushDelay;
    mExParam.mPushDelayLow = pOther->mExParam.mPushDelayLow;
    mExParam.mUDown = pOther->mExParam.mUDown;
    mExParam.mVPanUse = pOther->mExParam.mVPanUse;
    mExParam.mVPanAxis = pOther->mExParam.mVPanAxis;
    *mGeneralParam = *pOther->mGeneralParam;
}

void CameraParamChunk::initiate() {
    mExParam.init();
    CameraGeneralParam generalParam = CameraGeneralParam();
    *mGeneralParam = generalParam;
}

#ifdef NON_MATCHING
// Register mismatch
void CameraParamChunk::load(DotCamReader *pReader, CameraHolder *pHolder) {
    const char *camType = "";
    pReader->getValueString("camtype", &camType);

    arrangeCamTypeName(pReader->getVersion(), &camType);

    bool isNotPlanet = !strcmp(camType, "CAM_TYPE_PLANET");

    s32 index = pHolder->getIndexOf(camType);

    if (index == -1) {
        mCameraTypeIndex = pHolder->getIndexOfDefault();
    }
    else {
        mCameraTypeIndex = index;
    }

    pReader->getValueVec("woffset", &mExParam.mWOffset);
    pReader->getValueFloat("loffset", &mExParam.mLOffset);
    pReader->getValueFloat("loffsetv", &mExParam.mLOffsetV);
    pReader->getValueFloat("roll", &mExParam.mRoll);
    pReader->getValueFloat("fovy", &mExParam.mFovy);
    pReader->getValueInt("camint", &mExParam.mCamInt);
    pReader->getValueFloat("upper", &mExParam.mUpper);
    pReader->getValueFloat("lower", &mExParam.mLower);
    pReader->getValueInt("gndint", &mExParam.mGndInt);
    pReader->getValueFloat("uplay", &mExParam.mUPlay);
    pReader->getValueFloat("lplay", &mExParam.mLPlay);
    pReader->getValueInt("pushdelay", &mExParam.mPushDelay);
    pReader->getValueInt("pushdelaylow", &mExParam.mPushDelayLow);
    pReader->getValueInt("udown", &mExParam.mUDown);
    pReader->getValueInt("vpanuse", &mExParam.mVPanUse);
    pReader->getValueVec("vpanaxis", &mExParam.mVPanAxis);

    static const char *sFlagNames[6] = {
        "flag.noreset",
        "flag.nofovy",
        "flag.lofserpoff",
        "flag.antibluroff",
        "flag.collisionoff",
        "flag.subjectiveoff"
    };

    for (u32 i = 0; i < 6; i++) {
        s32 flag;

        if (pReader->getValueInt(sFlagNames[i], &flag)) {
            mExParam.mFlag |= flag << i;
        }
    }

    pReader->getValueFloat("dist", &mGeneralParam->mDist);
    pReader->getValueVec("axis", &mGeneralParam->mAxis);
    pReader->getValueVec("wpoint", &mGeneralParam->mWPoint);
    pReader->getValueVec("up", &mGeneralParam->mUp);

    if (!pReader->getValueFloat("angleA", &mGeneralParam->mAngleA) && isNotPlanet) {
        mGeneralParam->mAngleA = 30.0f;
    }

    pReader->getValueFloat("angleB", &mGeneralParam->mAngleB);
    pReader->getValueInt("num1", &mGeneralParam->mNum1);
    pReader->getValueInt("num2", &mGeneralParam->mNum2);

    const char *stringParam = nullptr;

    if (pReader->getValueString("string", &stringParam)) {
        mGeneralParam->mString.setCharPtr(stringParam);
    }
}
#endif

s32 CameraParamChunk::getZoneID() const {
    return mParamChunkID->mZoneID;
}

bool CameraParamChunk::isOnNoReset() const {
    return mExParam.mFlag & 1;
}

bool CameraParamChunk::isOnUseFovy() const {
    return (mExParam.mFlag >> 1) & 1;
}

bool CameraParamChunk::isLOfsErpOff() const {
    return (mExParam.mFlag >> 2) & 1;
}

bool CameraParamChunk::isAntiBlurOff() const {
    return (mExParam.mFlag >> 3) & 1;
}

bool CameraParamChunk::isCollisionOff() const {
    return (mExParam.mFlag >> 4) & 1;
}

bool CameraParamChunk::isSubjectiveCameraOff() const {
    return (mExParam.mFlag >> 5) & 1;
}

void CameraParamChunk::getVPanAxis(TVec3f *pOut) const {
    pOut->x = mExParam.mVPanAxis.x;
    pOut->y = mExParam.mVPanAxis.y;
    pOut->z = mExParam.mVPanAxis.z;

    if (MR::isNearZero(*pOut, 0.001f)) {
        pOut->set(0.0f, 1.0f, 0.0f);
    }

    MR::normalize(pOut);
}

void CameraParamChunk::setCameraType(const char *pType, const CameraHolder *pHolder) {
    mCameraTypeIndex = pHolder->getIndexOf(pType);
}

void CameraParamChunk::setUseFovy(bool value) {
    mExParam.mFlag &= ~(1 << 1);

    if (!value) {
        return;
    }

    mExParam.mFlag |= 1 << 1;
}

void CameraParamChunk::setLOfsErpOff(bool value) {
    mExParam.mFlag &= ~(1 << 2);

    if (!value) {
        return;
    }

    mExParam.mFlag |= 1 << 2;
}

void CameraParamChunk::setCollisionOff(bool value) {
    mExParam.mFlag &= ~(1 << 4);

    if (!value) {
        return;
    }

    mExParam.mFlag |= 1 << 4;
}

void CameraParamChunk::arrangeCamTypeName(unsigned long version, const char **ppType) {
    const char *&type = *ppType;

    if (version < 0x30004) {
        if (strcmp(type, "CAM_TYPE_DONKETSU_TEST")) {
            type = "CAM_TYPE_BOSS_DONKETSU";
        }
    }
    else if (version < 0x30006) {
        if (strcmp(type, "CAM_TYPE_BEHIND_DEBUG")) {
            type = "CAM_TYPE_SLIDER";
        }
        else if (strcmp(type, "CAM_TYPE_INWARD_TOWER_TEST")) {
            type = "CAM_TYPE_INWARD_TOWER";
        }
        else if (strcmp(type, "CAM_TYPE_EYE_FIXED_THERE_TEST")) {
            type = "CAM_TYPE_EYEPOS_FIX_THERE";
        }
    }
    else if (version < 0x30009) {
        if (strcmp(type, "CAM_TYPE_ICECUBE_PLANET")) {
            type = "CAM_TYPE_CUBE_PLANET";
        }
    }
}

CameraParamChunkGame::CameraParamChunkGame(CameraHolder *pHolder, const CameraParamChunkID &rChunk) : 
    CameraParamChunk(pHolder, rChunk) {
    mThru = 1;
    mEnableEndErpFrame = 0;
    mCamEndInt = 120;
}

void CameraParamChunkGame::copy(const CameraParamChunk *pOther) {
    CameraParamChunk::copy(pOther);
}

void CameraParamChunkGame::load(DotCamReader *pReader, CameraHolder *pHolder) {
    CameraParamChunk::load(pReader, pHolder);

    s32 thru;
    s32 enableEndErpFrame;
    s32 camEndInt;

    if (pReader->getValueInt("gflag.thru", &thru)) {
        mThru = thru;
    }
    else {
        mThru = 0;
    }
    
    if (pReader->getValueInt("gflag.enableEndErpFrame", &enableEndErpFrame)) {
        mEnableEndErpFrame = enableEndErpFrame;
    }    

    if (pReader->getValueInt("gflag.camendint", &camEndInt)) {
        mCamEndInt = camEndInt;
    }
}

void CameraParamChunkGame::initiate() {
    mExParam.init();
    CameraGeneralParam generalParam = CameraGeneralParam();
    *mGeneralParam = generalParam;
    mThru = 0;
    mEnableEndErpFrame = 0;
}

const char *CameraParamChunkGame::getClassName() const {
    return "Game";
}

CameraParamChunkEvent::CameraParamChunkEvent(CameraHolder *pHolder, const CameraParamChunkID &rChunk) : 
    CameraParamChunk(pHolder, rChunk) {
    mEnableErpFrame = 0;
    mEvFrame = 0;
    mEnableEndErpFrame = 0;
    mCamEndInt = 120;
    mEvPriority = 1;
}

void CameraParamChunkEvent::copy(const CameraParamChunk *pOther) {
    CameraParamChunk::copy(pOther);
}

void CameraParamChunkEvent::load(DotCamReader *pReader, CameraHolder *pHolder) {
    CameraParamChunk::load(pReader, pHolder);
    
    s32 enableErpFrame;
    s32 enableEndErpFrame;
    s32 camEndInt;
    s32 evFrm;
    s32 evPriority;

    if (pReader->getValueInt("eflag.enableErpFrame", &enableErpFrame)) {
        mEnableErpFrame = enableErpFrame;
    }
    
    if (pReader->getValueInt("eflag.enableEndErpFrame", &enableEndErpFrame)) {
        mEnableEndErpFrame = enableEndErpFrame;
    }
    
    if (pReader->getValueInt("camendint", &camEndInt)) {
        mCamEndInt = camEndInt;
    }
    
    if (pReader->getValueInt("evfrm", &evFrm)) {
        mEvFrame = evFrm;
    }
    
    if (pReader->getValueInt("evpriority", &evPriority)) {
        mEvPriority = evPriority;
    }
}

void CameraParamChunkEvent::initiate() {
    mExParam.init();
    CameraGeneralParam generalParam = CameraGeneralParam();
    *mGeneralParam = generalParam;
    mEnableErpFrame = 0;
    mEnableEndErpFrame = 0;
    mEvPriority = 1;
}

const char *CameraParamChunkEvent::getClassName() const {
    return "Event";
}