#pragma once

#include "JSystem/JGeometry.hpp"
#include <revolution.h>

class LiveActor;

namespace MR {
    void makeMtxRotate(MtxPtr, s16, s16, s16);
    void makeMtxRotate(MtxPtr, f32, f32, f32);
    void makeMtxRotate(MtxPtr, const TVec3f&);
    void makeMtxRotateY(MtxPtr, f32);
    void makeMtxTR(MtxPtr, f32, f32, f32, f32, f32, f32);
    void makeMtxTR(MtxPtr, const TVec3f&, const TVec3f&);
    void makeMtxTransRotateY(MtxPtr, f32, f32, f32, f32);
    void rotateMtxLocalX(MtxPtr, f32);
    void rotateMtxLocalY(MtxPtr, f32);
    void rotateMtxLocalZ(MtxPtr, f32);
    void rotateMtxLocalXDegree(MtxPtr, f32);
    void rotateMtxLocalYDegree(MtxPtr, f32);
    void rotateMtxLocalZDegree(MtxPtr, f32);
    void preScaleMtx(MtxPtr, f32);
    void preScaleMtx(MtxPtr, const TVec3f&);

    void preScaleMtx(MtxPtr, f32, f32, f32);

    void scaleMtxToLocalMtx(MtxPtr, MtxPtr, MtxPtr, const TVec3f&);
    void scaleMtxToLocalMtx(MtxPtr, MtxPtr, const TVec3f&);
    void scaleMtxToDir(TPos3f*, const TVec3f&, const TVec3f&);
    void makeMtxTRS(MtxPtr, f32, f32, f32, f32, f32, f32, f32, f32, f32);
    void makeMtxTRS(MtxPtr, const TVec3f&, const TVec3f&, const TVec3f&);
    void setMtxAxisXYZ(MtxPtr, const TVec3f&, const TVec3f&, const TVec3f&);
    void flattenMtx(MtxPtr, MtxPtr, const TVec3f&);
    void flattenMtx(MtxPtr, const TVec3f&);
    void blendMtxRotate(MtxPtr, MtxPtr, f32, MtxPtr);
    void blendMtxRotateSlerp(MtxPtr, MtxPtr, f32, MtxPtr);
    void blendMtx(MtxPtr, MtxPtr, f32, MtxPtr);
    void makeMtxWithoutScale(TPos3f*, const TPos3f&);
    void makeRTFromMtxPtr(TVec3f*, TVec3f*, MtxPtr, bool);
    void makeMtxSideUp(TPos3f*, const TVec3f&, const TVec3f&);
    void makeMtxSideUpPos(TPos3f*, const TVec3f&, const TVec3f&, const TVec3f&);
    void makeMtxSideFront(TPos3f*, const TVec3f&, const TVec3f&);
    void makeMtxUpSide(TPos3f*, const TVec3f&, const TVec3f&);
    void makeMtxUpSidePos(TPos3f*, const TVec3f&, const TVec3f&, const TVec3f&);
    void makeMtxUpFront(TPos3f*, const TVec3f&, const TVec3f&);
    void makeMtxUpFrontPos(TPos3f*, const TVec3f&, const TVec3f&, const TVec3f&);
    void makeMtxFrontSidePos(TPos3f*, const TVec3f&, const TVec3f&, const TVec3f&);
    void makeMtxFrontUp(TPos3f*, const TVec3f&, const TVec3f&);
    void makeMtxFrontUpPos(TPos3f*, const TVec3f&, const TVec3f&, const TVec3f&);
    void makeMtxUpNoSupport(TPos3f*, const TVec3f&);
    void makeMtxUpNoSupportPos(TPos3f*, const TVec3f&, const TVec3f&);
    void makeMtxFrontNoSupport(TPos3f*, const TVec3f&);
    void makeMtxFrontNoSupportPos(TPos3f*, const TVec3f&, const TVec3f&);
    void orthogonalize(TPos3f*);
    void turnMtxToXDirRate(TPos3f*, const TVec3f&, f32);
    void turnMtxToXDirDegree(TPos3f*, const TVec3f&, f32);
    void turnMtxToYDirRate(TPos3f*, const TVec3f&, f32);
    void turnMtxToYDirDegree(TPos3f*, const TVec3f&, f32);
    void turnMtxToLocalDirDegree(TPos3f*, const TVec3f&, const TVec3f&, f32);
    void makeMtxMoment(TPos3f*, const TVec3f&);
    void rotateMtxMoment(TPos3f*, const TPos3f&, const TVec3f&);
    void rotateMtxMoment(TPos3f*, const TVec3f&);
    void setMtxTrans(MtxPtr, f32, f32, f32);
    void addTransMtx(MtxPtr, const TVec3f&);
    void addTransMtxLocal(MtxPtr, const TVec3f&);
    void addTransMtxLocalX(MtxPtr, f32);
    void addTransMtxLocalY(MtxPtr, f32);
    void addTransMtxLocalZ(MtxPtr, f32);
    void extractMtxXDir(MtxPtr, TVec3f*);
    void extractMtxYDir(MtxPtr, TVec3f*);
    void extractMtxZDir(MtxPtr, TVec3f*);
    void extractMtxXYZDir(MtxPtr, TVec3f*, TVec3f*, TVec3f*);
    void extractMtxTrans(MtxPtr, TVec3f*);
    bool isSameMtx(MtxPtr, MtxPtr);
    bool isSameMtxRot(MtxPtr, MtxPtr);
    bool isRotAxisY(MtxPtr, MtxPtr);
    void calcMtxRotAxis(TVec3f*, MtxPtr, MtxPtr);
    MtxPtr tmpMtxTrans(const TVec3f&);
    MtxPtr tmpMtxScale(f32, f32, f32);
    MtxPtr tmpMtxRotXRad(f32);
    MtxPtr tmpMtxRotYRad(f32);
    MtxPtr tmpMtxRotZRad(f32);
    MtxPtr tmpMtxRotXDeg(f32);
    MtxPtr tmpMtxRotYDeg(f32);
    MtxPtr tmpMtxRotZDeg(f32);
    void orderRotateMtx(s16, const TVec3f&, MtxPtr);
    void rotAxisVecRad(const TVec3f&, const TVec3f&, TVec3f*, f32);

    void makeMtxTR(MtxPtr, const LiveActor*);

    void setMtxTrans(MtxPtr mtx, const TVec3f& rVec); /*{
        MR::setMtxTrans(mtx, rVec.x, rVec.y, rVec.z);
    }*/
};  // namespace MR
