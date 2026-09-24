#include "Game/MapObj/ClipAreaShape.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"
#include "Game/Util/MtxUtil.hpp"

void ClipAreaShape_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)2.0f;
    (void)0.001f;
    (void)500.0f;
    (void)0.01f;
    (void)1000.0f;
}

ClipAreaShape::ClipAreaShape(const char* pName) : mModelData() {
    mModelData = MR::getJ3DModelData(pName);
}

bool ClipAreaShape::isInArea(const TVec3f& rPosition, f32 radius, const TPos3f& rMtx, const TVec3f& rScale) const {
    if (MR::isNearZero(rScale.x) || (MR::isNearZero(rScale.y) || MR::isNearZero(rScale.z))) {
        return false;
    }

    TPos3f mtx;
    mtx.identity();
    mtx.invert(rMtx);
    TVec3f srcVec;
    mtx.mult(rPosition, srcVec);

    srcVec.x = (srcVec.x / rScale.x);
    srcVec.y = (srcVec.y / rScale.y);
    srcVec.z = (srcVec.z / rScale.z);
    return isInArea(srcVec);
}

void ClipAreaShape::calcVolumeMatrix(TPos3f* pVolMtx, const TPos3f& rSrcMtx, const TVec3f& rScale) const {
    pVolMtx->set(rSrcMtx);
    MR::preScaleMtx(pVolMtx->toMtxPtr(), rScale);
}

void ClipAreaShape::drawVolumeShape(const TPos3f& rMtx, const TVec3f& rPos) const {
    TPos3f volMtx;
    volMtx.identity();
    calcVolumeMatrix(&volMtx, rMtx, rPos);
    MR::multMtx(volMtx, volMtx, MR::getCameraViewMtx());
    GXLoadPosMtxImm(volMtx, 0);
    MR::drawSimpleModel(mModelData);
}

bool ClipAreaShapeSphere::isInArea(const TVec3f& rVec) const {
    return rVec.squared() < mRadius * mRadius;
}

void ClipAreaShapeSphere::calcVolumeMatrix(TPos3f* pPos, const TPos3f& rPos, const TVec3f& rVec) const {
    pPos->set(rPos);

    TVec3f stack_14 = (TVec3f(rVec.scaleInline(mRadius)));
    stack_14.scale(0.0099999998f);
    MR::preScaleMtx(pPos->toMtxPtr(), stack_14);
}

bool ClipAreaShapeBox::isInArea(const TVec3f& rVec) const {
    switch (_C) {
    case 0:
        return (MR::isInRange(rVec.x, -mRadius, mRadius) && MR::isInRange(rVec.y, -mRadius, mRadius) && MR::isInRange(rVec.z, -mRadius, mRadius));
    case 1:
        return (MR::isInRange(rVec.x, -mRadius, mRadius) && MR::isInRange(rVec.y, 0.0f, 2.0f * mRadius) && MR::isInRange(rVec.z, -mRadius, mRadius));
    }

    return false;
}

void ClipAreaShapeBox::calcVolumeMatrix(TPos3f* pPos, const TPos3f& rPos, const TVec3f& rVec) const {
    pPos->set(rPos);

    if (_C == 1) {
        MR::addTransMtxLocalY(pPos->toMtxPtr(), mRadius * rVec.y);
    }

    MR::preScaleMtx(pPos->toMtxPtr(), rVec);
}

ClipAreaShapeCone::ClipAreaShapeCone(s32 a1) : ClipAreaShape("ClipVolumeSphere") {
    _8 = 500.0f;
    _C = 1000.0f;
    _10 = a1;
}

bool ClipAreaShapeCone::isInArea(const TVec3f& rVec) const {
    f32 v3 = (rVec.y / _C);

    if (!MR::isInRange(v3, 0.0f, 1.0f)) {
        return false;
    }

    if (_10 == 1) {
        v3 = (1.0f - v3);
    }

    f32 v23 = ((rVec.x * rVec.x) + (rVec.z * rVec.z));
    f32 v24 = (v3 * _8) * (v3 * _8);
    return v23 < v24;
}

bool ClipAreaShape::isInArea(const TVec3f&) const {
    return false;
}

ClipAreaShapeSphere::ClipAreaShapeSphere() : ClipAreaShape("ClipVolumeSphere"), mRadius(500.0f) {
}

ClipAreaShapeBox::ClipAreaShapeBox(s32 u1) : ClipAreaShape("ClipVolumeBox") {
    mRadius = 500.0f;
    _C = u1;
}
