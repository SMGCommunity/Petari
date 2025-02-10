#include "Game/AreaObj/AreaForm.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "Game/Util.hpp"

template<>
void TRot3f::mult33(const TVec3f &rSrc, TVec3f &rDst) const {
    f32 a32, a22, a12, a31, a21, a11, a13, a33, a23;
    a32 = mMtx[2][1];
    
    a22 = mMtx[1][1];
    a12 = mMtx[0][1];
    a31 = mMtx[2][0];
    a21 = mMtx[1][0];
    a11 = mMtx[0][0];
    
    a33 = mMtx[2][2];
    a13 = mMtx[0][2];
    a23 = mMtx[1][2];
    rDst.set(rSrc.z * a13 + (rSrc.x * a11 + rSrc.y * a12), rSrc.z * a23 + (rSrc.x * a21 + rSrc.y * a22), rSrc.z * a33 + (rSrc.x * a31 + rSrc.y * a32));
    
}

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
    mBounding.mMin.x = 0.0f;
    mBounding.mMin.y = 0.0f;
    mBounding.mMin.z = 0.0f;
    mBounding.mMax.x = 0.0f;
    mBounding.mMax.y = 0.0f;
    mBounding.mMax.z = 0.0f;
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

    if (transpose.x >= mBounding.mMin.x && transpose.y >= mBounding.mMin.y && transpose.z >= mBounding.mMin.z && transpose.x < mBounding.mMax.x && transpose.y < mBounding.mMax.y && transpose.z < mBounding.mMax.z) {
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

    JMathInlineVEC::PSVECSubtract(&mBounding.mMax, &mBounding.mMin, &pBox->_30);
    pos.mult(mBounding.mMin, pBox->_24);
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
// issues with floating point math order
void AreaFormCube::updateBoxParam() {
    MR::makeMtxRotate(_48, mRotation.x, mRotation.y, mRotation.z);

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

    _30.set(v10);
    _3C.set(v9);

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
    #ifdef __MWERKS__
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
    #endif
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

bool AreaFormSphere::isInVolume(const TVec3f &rVector) const {
    TVec3f pos;
    calcPos(&pos);

    TVec3f thing(rVector);
    JMathInlineVEC::PSVECSubtract(&thing, &pos, &thing);
    return PSVECMag(&thing) < _14;
}

AreaFormBowl::AreaFormBowl() {
    _4 = 0;
    mTranslation.x = 0.0f;
    mTranslation.y = 0.0f;
    mTranslation.z = 0.0f;
    _20 = 0.0f;
}

void AreaFormBowl::init(const JMapInfoIter &rIter) {
    MR::getJMapInfoTrans(rIter, &mTranslation);

    f32 scale_x;
    rIter.getValue<f32>("scale_x", &scale_x);
    _20 = scale_x;
    _20 *= 500.0f;

    TVec3f rotation;    
    MR::getJMapInfoRotate(rIter, &rotation);
    calcUpVec(rotation);
}

bool AreaFormBowl::isInVolume(const TVec3f &rPos) const {
    TVec3f pos(rPos);
    JMathInlineVEC::PSVECSubtract(&pos, &mTranslation, &pos);
    
    if (PSVECMag((const Vec*)&pos) > _20) {
        return false;
    }

    TVec3f other_pos(rPos);
    PSVECSubtract((const Vec*)&other_pos, (const Vec*)&mTranslation, (Vec*)&other_pos);
    return other_pos.dot(mUp) < 0.0f;
}

void AreaFormBowl::calcUpVec(const TVec3f &rPos) {
    TVec3f up(0.0f, 1.0f, 0.0f);

    Mtx mtx;
    MR::makeMtxTR(mtx, 0.0f, 0.0f, 0.0f, rPos.x, rPos.y, rPos.z);
    PSMTXMultVec(mtx, (const Vec*)&up, (Vec*)&up);
    mUp = up;
}

AreaFormCylinder::AreaFormCylinder() {
    _4 = 0;
    mTranslation.x = 0.0f;
    mTranslation.y = 0.0f;
    mTranslation.z = 0.0f;
    mRotation.x = 0.0f;
    mRotation.y = 0.0f;
    mRotation.z = 0.0f;
    _20 = 0.0f;
    _24 = 0.0f;
}

void AreaFormCylinder::calcPos(TVec3f *pPos) const {
    if (_4) {
        _4->mult(mTranslation, *pPos);
        return;
    }

    pPos->set<f32>(mTranslation);
}

void AreaFormCylinder::calcCenterPos(TVec3f *pCenterPos) const {
    calcPos(pCenterPos);
    TVec3f upVec;
    calcUpVec(&upVec);

    pCenterPos->add((upVec * _24) * 0.5f);
}

void AreaFormCylinder::calcUpVec(TVec3f *pUpVec) const {
    if (_4) {
        _4->mult33(mRotation, *pUpVec);
        MR::normalize(pUpVec);
    }
    else {
        pUpVec->set<f32>(mRotation);
    }
}

bool AreaFormCylinder::isInVolume(const TVec3f &rVec) const {
    TVec3f pos;
    calcPos(&pos);

    TVec3f up;
    calcUpVec(&up);

    TVec3f stack_8(rVec);
    JMathInlineVEC::PSVECSubtract((const Vec*)&stack_8, (const Vec*)&pos, (Vec*)&stack_8);
    f32 v6 = MR::vecKillElement(stack_8, up, &stack_8);
    f32 v7 = PSVECMag((const Vec*)&stack_8);

    bool ret;

    if (MR::isInRange(v6, 0.0f, _24) && (v7 < _20)) {
        ret = true;
    }
    else {
        ret = false;
    }

    return ret;
}

void AreaFormCylinder::calcDir(const TVec3f &a1) {
    mRotation.x = 0.0f;
    mRotation.y = 1.0f;
    mRotation.z = 0.0f;

    MtxPtr mtx_x = MR::tmpMtxRotXDeg(a1.x);
    PSMTXMultVec(mtx_x, (const Vec*)&mRotation, (Vec*)&mRotation);
    MtxPtr mtx_y = MR::tmpMtxRotYDeg(a1.y);
    PSMTXMultVec(mtx_y, (const Vec*)&mRotation, (Vec*)&mRotation);
    MtxPtr mtx_z = MR::tmpMtxRotYDeg(a1.z);
    PSMTXMultVec(mtx_z, (const Vec*)&mRotation, (Vec*)&mRotation);
    PSVECMag((const Vec*)&mRotation);
    PSVECNormalize((const Vec*)&mRotation, (Vec*)&mRotation);
}

void AreaFormSphere::init(const JMapInfoIter &rIter) {
    MR::getJMapInfoTrans(rIter, &mTranslation);

    f32 scale_x;
    rIter.getValue<f32>("scale_x", &scale_x);

    _14 = scale_x;
    _14 *= 500.0f;
    
    TVec3f rotation;
    MR::getJMapInfoRotate(rIter, &rotation);

    TVec3f butts(0.0f, 1.0f, 0.0f);

    Mtx mtx;
    MR::makeMtxTR(mtx, 0.0f, 0.0f, 0.0f, rotation.x, rotation.y, rotation.z);
    PSMTXMultVec(mtx, (const Vec*)&butts, (Vec*)&butts);
    mUp = butts;
}

void AreaFormCylinder::init(const JMapInfoIter &rIter) {
    MR::getJMapInfoTrans(rIter, &mTranslation);
    TVec3f rotate;
    
    MR::getJMapInfoRotate(rIter, &rotate);
    calcDir(rotate);

    f32 temp;
    rIter.getValue<f32>("scale_x", &temp);

    _20 = temp;
    _20 *= 500.0f;

    rIter.getValue<f32>("scale_y", &temp);
    _24 = temp;
    _24 *= 500.0f;
}
