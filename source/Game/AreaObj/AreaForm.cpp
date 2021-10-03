#include "Game/AreaObj/AreaForm.h"
#include "JSystem/JMath/JMath.h"
#include "Game/Util.h"

AreaFormCube::AreaFormCube(int a1) {
    _4 = 0;
    _8 = a1;
    mTranslation.x = 0.0f;
    mTranslation.y = 0.0f;
    mTranslation.z = 0.0f;
    mRotation.x = 0.0f;
    mRotation.y = 0.0f;
    mRotation.z = 0.0f;
    mScale.x = 1.0f;
    mScale.y = 1.0f;
    mScale.z = 1.0f;
    _30.x = 0.0f;
    _30.y = 0.0f;
    _30.z = 0.0f;
    _3C.x = 0.0f;
    _3C.y = 0.0f;
    _3C.z = 0.0f;
}

void AreaFormCube::init(const JMapInfoIter &rIter) {
    MR::getJMapInfoTrans(rIter, &mTranslation);
    MR::getJMapInfoRotate(rIter, &mRotation);
    MR::getJMapInfoScale(rIter, &mScale);
    updateBoxParam();
}

bool AreaFormCube::isInVolume(const TVec3f &rPos) const {
    TPos3f pos;
    calcWorldMtx(&pos);

    TVec3f transpose;
    pos.multTranspose(rPos, transpose);

    bool res = false;

    if (transpose.x >= _30.x && transpose.y >= _30.y && transpose.z >= _30.z && transpose.x < _3C.x && transpose.y < _3C.y && transpose.z < _3C.z) {
        res = true;
    }

    return res;
}

void AreaFormCube::calcWorldPos(TVec3f *pPos) const {
    if (_4) {
        _4->mult(mTranslation, *pPos);
        return;
    }

    pPos->set<f32>(mTranslation);
}

// AreaFormCube::calcWorldRotate

void AreaFormCube::calcWorldBox(TDirBox3f *pBox) const {
    TPos3f pos;
    calcWorldMtx(&pos);

    pBox->_0.x = pos.mMtx[0][0];
    pBox->_C.x = pos.mMtx[0][1];
    pBox->_18.x = pos.mMtx[0][2];
    pBox->_24.x = pos.mMtx[0][3];
    pBox->_0.y = pos.mMtx[1][0];
    pBox->_C.y = pos.mMtx[1][1];
    pBox->_18.y = pos.mMtx[1][2];
    pBox->_24.y = pos.mMtx[1][3];
    pBox->_0.z = pos.mMtx[2][0];
    pBox->_C.z = pos.mMtx[2][1];
    pBox->_18.z = pos.mMtx[2][2];
    pBox->_24.z = pos.mMtx[2][3];

    JMathInlineVEC::PSVECSubtract((const Vec*)&_3C, (const Vec*)&_30, (Vec*)&pBox->_30);
    pos.mult(_30, pBox->_24);
}

void AreaFormCube::calcLocalPos(TVec3f *pPos, const TVec3f &a2) const {
    TPos3f worldMtx;
    calcWorldMtx(&worldMtx);
    worldMtx.multTranspose(a2, *pPos);
}

f32 AreaFormCube::getBaseSize() {
    return 1000.0f;
}


#ifdef NON_MATCHING
// issues with inlining vector constructors
void AreaFormCube::updateBoxParam() {
    MR::makeMtxRotate(&_48, mRotation.x, mRotation.y, mRotation.z);

    f32 v3 = (0.5f * (-mScale.x * 1000.0f));
    f32 v5 = (0.5f * (-mScale.y * 1000.0f));
    f32 v6 = (0.5f * (-mScale.z * 1000.0f));

    _48[0][3] = mTranslation.x;
    _48[1][3] = mTranslation.y;
    _48[2][3] = mTranslation.z;

    TVec3f v10(v3, v5, v6);

    f32 v20 = (0.5f * (mScale.x * 1000.0f));
    f32 v21 = (0.5f * (mScale.y * 1000.0f));
    f32 v22 = (0.5f * (mScale.z * 1000.0f));

    TVec3f v9(v20, v21, v22);

    _30.set<f32>(v10);
    _3C.set<f32>(v9);

    if (_8 == 1) {
        TVec3f temp;
        
        f32 v7 = mScale.y * 1000.0f;
        temp.x = 0.0f;
        temp.z = 0.0f;
        temp.y = 0.5f * v7;

        _30.add(temp);
        _3C.add(temp);
    }
}
#endif

void AreaFormCube::calcWorldMtx(register TPos3f *pPos) const {
    register const AreaFormCube* cube = this;
    
    if (_4) {
        return pPos->concat(*_4, (const TSMtxf&)*_48);
    }

    __asm {
        psq_l f0, 0x48(cube), 0, 0
        psq_l f1, 0x50(cube), 0, 0
        psq_l f2, 0x58(cube), 0, 0
        psq_l  f3, 0x60(cube), 0, 0
        psq_l  f4, 0x68(cube), 0, 0
        psq_l  f5, 0x70(cube), 0, 0
        psq_st f0, 0(pPos), 0, 0
        psq_st f1, 8(pPos), 0, 0
        psq_st f2, 0x10(pPos), 0, 0
        psq_st f3, 0x18(pPos), 0, 0
        psq_st f4, 0x20(pPos), 0, 0
        psq_st f5, 0x28(pPos), 0, 0
    };
}

AreaFormSphere::AreaFormSphere() {
    _4 = 0;
    mTranslation.x = 0.0f;
    mTranslation.y = 0.0f;
    mTranslation.z = 0.0f;
    _14 = 0.0f;
}

void AreaFormSphere::calcUpVec(TVec3f *pOut) const {
    if (_4) {
        _4->mult33(mUp, *pOut);
        MR::normalize(pOut);
    }
    else {
        pOut->set<f32>(mUp);
    }
}

void AreaFormSphere::calcPos(TVec3f *pOut) const {
    if (_4) {
        _4->mult(mTranslation, *pOut);
    }
    else {
        pOut->set<f32>(mTranslation);
    }
}


#ifndef NONMATCHING
// vector inlining
bool AreaFormSphere::isInVolume(const TVec3f &rVector) const {
    TVec3f pos;
    calcPos(&pos);

    TVec3f thing(rVector);

    JMathInlineVEC::PSVECSubtract((const Vec*)&thing, (const Vec*)&pos, (Vec*)&thing);

    return PSVECMag((const Vec*)&thing) < _14;
}