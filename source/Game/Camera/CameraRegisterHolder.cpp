#include "Game/Camera/CameraRegisterHolder.h"
#include <cstring>

const char *sDummyMatrixName = "ダミーマトリクス";
const char *sDummyVectorName = "ダミーベクトル";
const char *sDummyFloatName = "ダミー浮動小数";

CameraRegisterHolder::CameraRegisterHolder(const char *pName) : NameObj(pName) {
    initMtx();
    initVec();
    initFloat();
    initDummyRegister();
}

void CameraRegisterHolder::declareMtxReg(const char *pName, MtxPtr pValue) {
    mMatrices[mNrMatrices] = pValue;
    mMatrixNames[mNrMatrices] = pName;

    mNrMatrices++;
}

MtxPtr CameraRegisterHolder::getMtx(const char *pName) const {
    s32 index = searchName(mMatrixNames, mNrMatrices, pName);

    if (index < 0) {
        index = 0;
    }

    return mMatrices[index];
}

void CameraRegisterHolder::declareVecReg(const char *pName, const TVec3f *pValue) {
    mVectors[mNrVectors] = pValue;
    mVectorNames[mNrVectors] = pName;
    mNrVectors++;
}

const TVec3f *CameraRegisterHolder::getVec(const char *pName) const {
    s32 index = searchName(mVectorNames, mNrVectors, pName);

    if (index < 0) {
        index = 0;
    }

    return mVectors[index];
}

void CameraRegisterHolder::declareFloatReg(const char *pName, const float *pValue) {
    mFloats[mNrFloats] = pValue;
    mFloatNames[mNrFloats] = pName;
    mNrFloats++;
}

const char *CameraRegisterHolder::getDummyVecRegName() const {
    return sDummyVectorName;
}

void CameraRegisterHolder::initMtx() {
    mNrMatrices = 0;

    for (u32 i = 0; i < NR_REGISTERS; i++) {
        mMatrices[i] = nullptr;
        mMatrixNames[i] = nullptr;
    }
}

void CameraRegisterHolder::initVec() {
    mNrVectors = 0;
    
    for (u32 i = 0; i < NR_REGISTERS; i++) {
        mVectors[i] = nullptr;
        mVectorNames[i] = nullptr;
    }
}

void CameraRegisterHolder::initFloat() {
    mNrFloats = 0;
    
    for (u32 i = 0; i < NR_REGISTERS; i++) {
        mFloats[i] = nullptr;
        mFloatNames[i] = nullptr;
    }
}

void CameraRegisterHolder::initDummyRegister() {
    mDummyMatrix.identity();
    declareMtxReg(sDummyMatrixName, reinterpret_cast<MtxPtr>(&mDummyMatrix));

    mDummyVector.z = 0.0f;
    mDummyVector.y = 0.0f;
    mDummyVector.x = 0.0f;
    declareVecReg(sDummyVectorName, &mDummyVector);

    mDummyFloat = 0.0f;
    declareFloatReg(sDummyFloatName, &mDummyFloat);
}

s32 CameraRegisterHolder::searchName(const char *const *pNames, unsigned long count, const char *pName) const {
    if (pName == nullptr) {
        return -1;
    }

    for (s32 i = 0; i < count; i++) {
        if (strcmp(pNames[i], pName) == 0) {
            return i;
        }
    }

    return -1;
}