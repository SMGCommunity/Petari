#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Camera/CameraHolder.hpp"
#include "Game/Camera/CameraParamChunkID.hpp"
#include "Game/Camera/DotCamParams.hpp"
#include "Game/Util/MathUtil.hpp"
#include <cstring>

void CameraParamChunk_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

namespace {
    static const char* sFlagName[] = {"flag.noreset",     "flag.nofovy",       "flag.lofserpoff",
                                      "flag.antibluroff", "flag.collisionoff", "flag.subjectiveoff"};

    static CameraParamChunk::ExParam sUndoExParam = CameraParamChunk::ExParam();
    static CameraGeneralParam sUndoParam = CameraGeneralParam();
    // static const ___ sUndoCamera =
    // static const ___ sClipChunk =
    // static const ___ sLastUndoPtr =
};  // namespace

CameraGeneralParam& CameraGeneralParam::operator=(const CameraGeneralParam& rOther) {
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

CameraParamChunk::CameraParamChunk(CameraHolder* pHolder, const CameraParamChunkID& rChunk) {
    mParamChunkID = new CameraParamChunkID(rChunk);
    mCameraTypeIndex = pHolder->getIndexOfDefault();
    mGeneralParam = new CameraGeneralParam();
    _64 = false;
    mExParam.init();
}

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

void CameraParamChunk::getVPanAxis(TVec3f* pOut) const {
    pOut->set(mExParam.mVPanAxis);

    if (MR::isNearZero(*pOut)) {
        pOut->set< f32 >(0.0f, 1.0f, 0.0f);
    }

    MR::normalize(pOut);
}

void CameraParamChunk::setCameraType(const char* pType, const CameraHolder* pHolder) {
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

void CameraParamChunk::copy(const CameraParamChunk* pOther) {
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

void CameraParamChunk::load(DotCamReader* pReader, CameraHolder* pHolder) {
    const char* camType = "";
    pReader->getValueString("camtype", &camType);

    arrangeCamTypeName(pReader->getVersion(), &camType);

    bool isPlanet = strcmp(camType, "CAM_TYPE_PLANET") == 0;

    s32 index = pHolder->getIndexOf(camType);

    if (index == -1) {
        mCameraTypeIndex = pHolder->getIndexOfDefault();
    } else {
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

    for (u32 i = 0; i < 6; i++) {
        s32 flag;

        if (pReader->getValueInt(::sFlagName[i], &flag)) {
            mExParam.mFlag |= flag << i;
        }
    }

    pReader->getValueFloat("dist", &mGeneralParam->mDist);
    pReader->getValueVec("axis", &mGeneralParam->mAxis);
    pReader->getValueVec("wpoint", &mGeneralParam->mWPoint);
    pReader->getValueVec("up", &mGeneralParam->mUp);

    if (!pReader->getValueFloat("angleA", &mGeneralParam->mAngleA) && isPlanet) {
        mGeneralParam->mAngleA = 30.0f;
    }

    pReader->getValueFloat("angleB", &mGeneralParam->mAngleB);
    pReader->getValueInt("num1", &mGeneralParam->mNum1);
    pReader->getValueInt("num2", &mGeneralParam->mNum2);

    const char* stringParam = nullptr;

    if (pReader->getValueString("string", &stringParam)) {
        mGeneralParam->mString.setCharPtr(stringParam);
    }
}

void CameraParamChunk::initiate() {
    mExParam.init();
    CameraGeneralParam generalParam = CameraGeneralParam();
    *mGeneralParam = generalParam;
}

void CameraParamChunk::arrangeCamTypeName(u32 version, const char** ppType) {
    const char*& type = *ppType;

    if (version < 0x30004) {
        if (strcmp(type, "CAM_TYPE_DONKETSU_TEST") == 0) {
            type = "CAM_TYPE_BOSS_DONKETSU";
        }
    } else if (version < 0x30006) {
        if (strcmp(type, "CAM_TYPE_BEHIND_DEBUG") == 0) {
            type = "CAM_TYPE_SLIDER";
        } else if (strcmp(type, "CAM_TYPE_INWARD_TOWER_TEST") == 0) {
            type = "CAM_TYPE_INWARD_TOWER";
        } else if (strcmp(type, "CAM_TYPE_EYE_FIXED_THERE_TEST") == 0) {
            type = "CAM_TYPE_EYEPOS_FIX_THERE";
        }
    } else if (version < 0x30009) {
        if (strcmp(type, "CAM_TYPE_ICECUBE_PLANET") == 0) {
            type = "CAM_TYPE_CUBE_PLANET";
        }
    }
}

CameraParamChunkGame::CameraParamChunkGame(CameraHolder* pHolder, const CameraParamChunkID& rChunk)
    : CameraParamChunk(pHolder, rChunk), mThru(1), mEnableEndErpFrame(), mCamEndInt(120) {
}

void CameraParamChunkGame::load(DotCamReader* pReader, CameraHolder* pHolder) {
    CameraParamChunk::load(pReader, pHolder);

    s32 val;
    if (pReader->getValueInt("gflag.thru", &val)) {
        mThru = val;
    } else {
        mThru = 0;
    }

    if (pReader->getValueInt("gflag.enableEndErpFrame", &val)) {
        mEnableEndErpFrame = val;
    }

    if (pReader->getValueInt("gflag.camendint", &val)) {
        mCamEndInt = val;
    }
}

void CameraParamChunkGame::initiate() {
    mExParam.init();
    CameraGeneralParam generalParam = CameraGeneralParam();
    *mGeneralParam = generalParam;
    mThru = 0;
    mEnableEndErpFrame = 0;
}

void CameraParamChunkGame::copy(const CameraParamChunk* pOther) {
    CameraParamChunk::copy(pOther);
}

CameraParamChunkEvent::CameraParamChunkEvent(CameraHolder* pHolder, const CameraParamChunkID& rChunk)
    : CameraParamChunk(pHolder, rChunk), mEnableErpFrame(), mEvFrame(), mEnableEndErpFrame(), mCamEndInt(120), mEvPriority(1) {
}

void CameraParamChunkEvent::load(DotCamReader* pReader, CameraHolder* pHolder) {
    CameraParamChunk::load(pReader, pHolder);

    s32 val;

    if (pReader->getValueInt("eflag.enableErpFrame", &val)) {
        mEnableErpFrame = val;
    }

    if (pReader->getValueInt("eflag.enableEndErpFrame", &val)) {
        mEnableEndErpFrame = val;
    }

    if (pReader->getValueInt("camendint", &val)) {
        mCamEndInt = val;
    }

    if (pReader->getValueInt("evfrm", &val)) {
        mEvFrame = val;
    }

    if (pReader->getValueInt("evpriority", &val)) {
        mEvPriority = val;
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

void CameraParamChunkEvent::copy(const CameraParamChunk* pOther) {
    CameraParamChunk::copy(pOther);
}
