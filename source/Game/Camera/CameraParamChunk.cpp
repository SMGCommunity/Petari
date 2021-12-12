#include "Game/Camera/CameraParamChunk.h"
#include "Game/Util/MathUtil.h"
#include <string.h>

CameraGeneralParam::CameraGeneralParam() :
    _0(1200.0f), _4(0.0f, 1.0f, 0.0f), _10(0.0f, 0.0f, 0.0f),
    _1C(0.0f, 1.0f, 0.0f), _28(0.0f), _2C(0.3f), _30(0), _34(0), mParamString() {
    
}

CameraGeneralParam& CameraGeneralParam::operator=(const CameraGeneralParam &rOther) {
    _0 = rOther._0;
    _4 = rOther._4;
    _10 = rOther._10;
    _1C = rOther._1C;
    _28 = rOther._28;
    _2C = rOther._2C;
    _30 = rOther._30;
    _34 = rOther._34;
    mParamString = rOther.mParamString;

    return *this;
}


CameraParamChunk::ExParam::ExParam() {

}

void CameraParamChunk::ExParam::init() {
    _0.set(0.0f, 100.0f, 0.0f);
    _C = 0.0f;
    _10 = 0.0f;
    _14 = 0.0f;
    _18 = 45.0f;
    _1C = 120;
    mFlag = 0;
    _24 = 0.3f;
    _28 = 0.1f;
    _2C = 160;
    _30 = 300.0f;
    _34 = 800.0f;
    _38 = 120;
    _3C = 120;
    _40 = 120;
    _44 = 1;
    mVPanAxis.x = 0.0f;
    mVPanAxis.y = 1.0f;
    mVPanAxis.z = 0.0f;
}

CameraParamChunk::CameraParamChunk(CameraHolder *pHolder, const CameraParamChunkID &rChunk) {
    mParamChunkID = new CameraParamChunkID(rChunk);
    mCameraTypeIndex = pHolder->getIndexOfDefault();
    mGeneralParam = new CameraGeneralParam();
    _64 = 0;
    mExParam.init();
}

void CameraParamChunk::copy(const CameraParamChunk *pOther) {
    mCameraTypeIndex = pOther->mCameraTypeIndex;
    mExParam._0 = pOther->mExParam._0;
    mExParam._C = pOther->mExParam._C;
    mExParam._10 = pOther->mExParam._10;
    mExParam._14 = pOther->mExParam._14;
    mExParam._18 = pOther->mExParam._18;
    mExParam._1C = pOther->mExParam._1C;
    mExParam.mFlag = pOther->mExParam.mFlag;
    mExParam._24 = pOther->mExParam._24;
    mExParam._28 = pOther->mExParam._28;
    mExParam._2C = pOther->mExParam._2C;
    mExParam._30 = pOther->mExParam._30;
    mExParam._34 = pOther->mExParam._34;
    mExParam._38 = pOther->mExParam._38;
    mExParam._3C = pOther->mExParam._3C;
    mExParam._40 = pOther->mExParam._40;
    mExParam._44 = pOther->mExParam._44;
    mExParam.mVPanAxis = pOther->mExParam.mVPanAxis;
    *mGeneralParam = *pOther->mGeneralParam;
}

void CameraParamChunk::initiate() {
    mExParam.init();
    CameraGeneralParam generalParam = CameraGeneralParam();
    *mGeneralParam = generalParam;
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