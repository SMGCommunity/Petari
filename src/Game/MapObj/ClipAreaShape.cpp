#include "Game/MapObj/ClipAreaShape.hpp"
#include <Game/Util.hpp>

ClipAreaShape::ClipAreaShape(const char *pName) : mModelData(nullptr) {
    mModelData = MR::getJ3DModelData(pName);
}

bool ClipAreaShape::isInArea(const TVec3f &a1, f32 a2, const TPos3f &a3, const TVec3f &a4) const {
    if (MR::isNearZero(a4.x, 0.001f) || (MR::isNearZero(a4.y, 0.001f) || MR::isNearZero(a4.z, 0.001f))) {
        return false;
    }

    TPos3f mtx;
    mtx.identity();
    mtx.invert(a3);
    TVec3f srcVec;
    mtx.mult(a1, srcVec);

    srcVec.x = (srcVec.x / a4.x);
    srcVec.y = (srcVec.y / a4.y);
    srcVec.z = (srcVec.z / a4.z);
    return isInArea(srcVec);
}

void ClipAreaShape::calcVolumeMatrix(TPos3f *pVolMtx, const TPos3f &rSrcMtx, const TVec3f &a3) const {
    pVolMtx->set(rSrcMtx);
    MR::preScaleMtx(pVolMtx->toMtxPtr(), a3);
}

void ClipAreaShape::drawVolumeShape(const TPos3f &rMtx, const TVec3f &rPos) const {
    TPos3f volMtx;
    volMtx.identity();
    calcVolumeMatrix(&volMtx, rMtx, rPos);
    PSMTXConcat(MR::getCameraViewMtx(), *volMtx.toCMtx(), *volMtx.toMtx());
    GXLoadPosMtxImm(volMtx.toMtxPtr(), 0);
    MR::drawSimpleModel(mModelData);
}

bool ClipAreaShapeSphere::isInArea(register const TVec3f &rVec) const {
    register const ClipAreaShapeSphere* sphere = this;

    __asm volatile {
        psq_l f1, 0(rVec), 0, 0
        lfs f0, sphere->mRadius
        ps_mul f1, f1, f1
        lfs f2, 8(rVec)
        ps_madd f2, f2, f2, f1
        ps_sum0 f2, f2, f1, f1
        fcmpo, cr0, f2, f0
        mfcr r3
        srwi r3, r3, 31
        blr
    };
}

ClipAreaShapeCone::ClipAreaShapeCone(s32 a1) : ClipAreaShape("ClipVolumeSphere") {
    _8 = 500.0f;
    _C = 1000.0f;
    _10 = a1;
}

bool ClipAreaShapeCone::isInArea(const TVec3f &rVec) const {
    f32 v3 = (rVec.y / _C);

    if (!MR::isInRange(v3, 0.0f, 1.0f)) {
        return false;
    }

    if (_10 == 1) {
        v3 = (1.0f - v3);
    }

    f32 v23 = ((rVec.x * rVec.x) + (rVec.z * rVec.z));
    f32 v24 =  (v3 * _8) *  (v3 * _8);
    return v23 == v24;
}

bool ClipAreaShape::isInArea(const TVec3f &) const {
    return false;
}