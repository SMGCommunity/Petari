#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include <JSystem/JMath/JMATrigonometric.hpp>
#include <JSystem/JMath/JMath.hpp>

static Mtx mtrans_org = {{1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}};

static Mtx tmpmtx_sc = {{1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}};

static Mtx tmpmtx_rx = {{1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}};

static Mtx tmpmtx_ry = {{1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}};

static Mtx tmpmtx_rz = {{1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}};

namespace MR {
    void makeMtxRotate(MtxPtr pMtx, s16 rx, s16 ry, s16 rz) {
        f32 sinX = JMASSin(rx);
        f32 sinY = JMASSin(ry);
        f32 sinZ = JMASSin(rz);
        f32 cosX = JMASCos(rx);
        f32 cosY = JMASCos(ry);
        f32 cosZ = JMASCos(rz);

        pMtx[0][0] = cosZ * cosY;
        pMtx[1][0] = sinZ * cosY;
        pMtx[2][0] = -sinY;

        pMtx[0][1] = cosZ * sinY * sinX - sinZ * cosX;
        pMtx[1][1] = sinZ * sinY * sinX + cosZ * cosX;
        pMtx[2][1] = cosY * sinX;

        pMtx[0][2] = cosZ * sinY * cosX + sinZ * sinX;
        pMtx[1][2] = sinZ * sinY * cosX - cosZ * sinX;
        pMtx[2][2] = cosY * cosX;

        pMtx[0][3] = 0.0f;
        pMtx[1][3] = 0.0f;
        pMtx[2][3] = 0.0f;
    }

    void makeMtxRotate(MtxPtr pMtx, f32 rx, f32 ry, f32 rz) {
        s16 angleX = (s16)(rx * DEGREE_TO_S16);
        s16 angleY = (s16)(ry * DEGREE_TO_S16);
        s16 angleZ = (s16)(rz * DEGREE_TO_S16);
        makeMtxRotate(pMtx, angleX, angleY, angleZ);
    }

    void makeMtxRotate(MtxPtr pMtx, const TVec3f& rVec) {
        makeMtxRotate(pMtx, rVec.x, rVec.y, rVec.z);
    }

    void makeMtxRotateY(MtxPtr pMtx, f32 ry) {
        s16 angle = (s16)(ry * DEGREE_TO_S16);
        f32 sinY = JMASSin(angle);
        f32 cosY = JMASCos(angle);

        pMtx[0][0] = cosY;
        pMtx[1][0] = 0.0f;
        pMtx[2][0] = -sinY;
        pMtx[0][1] = 0.0f;
        pMtx[1][1] = 1.0f;
        pMtx[2][1] = 0.0f;
        pMtx[0][2] = sinY;
        pMtx[1][2] = 0.0f;
        pMtx[2][2] = cosY;
        pMtx[0][3] = 0.0f;
        pMtx[1][3] = 0.0f;
        pMtx[2][3] = 0.0f;
    }

    void makeMtxTR(MtxPtr pMtx, f32 tx, f32 ty, f32 tz, f32 rx, f32 ry, f32 rz) {
        f32 sinX = JMASinDegree(rx);
        f32 sinY = JMASinDegree(ry);
        f32 sinZ = JMASinDegree(rz);
        f32 cosX = JMACosDegree(rx);
        f32 cosY = JMACosDegree(ry);
        f32 cosZ = JMACosDegree(rz);

        pMtx[0][0] = cosZ * cosY;
        pMtx[1][0] = sinZ * cosY;
        pMtx[2][0] = -sinY;

        pMtx[0][1] = cosZ * sinY * sinX - sinZ * cosX;
        pMtx[1][1] = sinZ * sinY * sinX + cosZ * cosX;
        pMtx[2][1] = cosY * sinX;

        pMtx[0][2] = cosZ * sinY * cosX + sinZ * sinX;
        pMtx[1][2] = sinZ * sinY * cosX - cosZ * sinX;
        pMtx[2][2] = cosY * cosX;

        pMtx[0][3] = tx;
        pMtx[1][3] = ty;
        pMtx[2][3] = tz;
    }

    void makeMtxTR(MtxPtr pMtx, const TVec3f& rTrans, const TVec3f& rRot) {
        makeMtxTR(pMtx, rTrans.x, rTrans.y, rTrans.z, rRot.x, rRot.y, rRot.z);
    }

    void makeMtxTransRotateY(MtxPtr pMtx, f32 tx, f32 ty, f32 tz, f32 ry) {
        makeMtxRotateY(pMtx, ry);
        pMtx[0][3] = tx;
        pMtx[1][3] = ty;
        pMtx[2][3] = tz;
    }

    void rotateMtxLocalX(MtxPtr pMtx, f32 rad) {
        f32 cosAngle = JMACosRadian(rad);
        f32 sinAngle = JMASinRadian(rad);
        f32 a0 = cosAngle * pMtx[0][1] - sinAngle * pMtx[0][2];
        f32 a1 = cosAngle * pMtx[1][1] - sinAngle * pMtx[1][2];
        f32 a2 = cosAngle * pMtx[2][1] - sinAngle * pMtx[2][2];
        f32 b0 = sinAngle * pMtx[0][1] + cosAngle * pMtx[0][2];
        f32 b1 = sinAngle * pMtx[1][1] + cosAngle * pMtx[1][2];
        f32 b2 = sinAngle * pMtx[2][1] + cosAngle * pMtx[2][2];
        pMtx[0][1] = a0;
        pMtx[1][1] = a1;
        pMtx[2][1] = a2;
        pMtx[0][2] = b0;
        pMtx[1][2] = b1;
        pMtx[2][2] = b2;
    }

    void rotateMtxLocalY(MtxPtr pMtx, f32 rad) {
        f32 cosAngle = JMACosRadian(rad);
        f32 sinAngle = JMASinRadian(rad);
        f32 a0 = cosAngle * pMtx[0][0] - sinAngle * pMtx[0][2];
        f32 a1 = cosAngle * pMtx[1][0] - sinAngle * pMtx[1][2];
        f32 a2 = cosAngle * pMtx[2][0] - sinAngle * pMtx[2][2];
        f32 b0 = sinAngle * pMtx[0][0] + cosAngle * pMtx[0][2];
        f32 b1 = sinAngle * pMtx[1][0] + cosAngle * pMtx[1][2];
        f32 b2 = sinAngle * pMtx[2][0] + cosAngle * pMtx[2][2];
        pMtx[0][0] = a0;
        pMtx[1][0] = a1;
        pMtx[2][0] = a2;
        pMtx[0][2] = b0;
        pMtx[1][2] = b1;
        pMtx[2][2] = b2;
    }

    void rotateMtxLocalZ(MtxPtr pMtx, f32 rad) {
        f32 cosAngle = JMACosRadian(rad);
        f32 sinAngle = JMASinRadian(rad);
        f32 a0 = cosAngle * pMtx[0][0] - sinAngle * pMtx[0][1];
        f32 a1 = cosAngle * pMtx[1][0] - sinAngle * pMtx[1][1];
        f32 a2 = cosAngle * pMtx[2][0] - sinAngle * pMtx[2][1];
        f32 b0 = sinAngle * pMtx[0][0] + cosAngle * pMtx[0][1];
        f32 b1 = sinAngle * pMtx[1][0] + cosAngle * pMtx[1][1];
        f32 b2 = sinAngle * pMtx[2][0] + cosAngle * pMtx[2][1];
        pMtx[0][0] = a0;
        pMtx[1][0] = a1;
        pMtx[2][0] = a2;
        pMtx[0][1] = b0;
        pMtx[1][1] = b1;
        pMtx[2][1] = b2;
    }

    void rotateMtxLocalXDegree(MtxPtr pMtx, f32 deg) {
        rotateMtxLocalX(pMtx, deg * PI_180);
    }

    void rotateMtxLocalYDegree(MtxPtr pMtx, f32 deg) {
        rotateMtxLocalY(pMtx, deg * PI_180);
    }

    void rotateMtxLocalZDegree(MtxPtr pMtx, f32 deg) {
        rotateMtxLocalZ(pMtx, deg * PI_180);
    }

    void preScaleMtx(MtxPtr pMtx, f32 scale) {
        preScaleMtx(pMtx, scale, scale, scale);
    }

    void preScaleMtx(MtxPtr pMtx, const TVec3f& rScale) {
        preScaleMtx(pMtx, rScale.x, rScale.y, rScale.z);
    }

    void preScaleMtx(MtxPtr pMtx, f32 sx, f32 sy, f32 sz) {
        pMtx[0][0] *= sx;
        pMtx[1][0] *= sx;
        pMtx[2][0] *= sx;
        pMtx[0][1] *= sy;
        pMtx[1][1] *= sy;
        pMtx[2][1] *= sy;
        pMtx[0][2] *= sz;
        pMtx[1][2] *= sz;
        pMtx[2][2] *= sz;
    }

    void scaleMtxToLocalMtx(MtxPtr pDst, MtxPtr pSrc, MtxPtr pBase, const TVec3f& rScale) {
        TPos3f baseCopy;
        baseCopy.setInline(pBase);
        TVec3f srcTrans;
        baseCopy.setTrans(TVec3f(0.0f, 0.0f, 0.0f));

        TPos3f invMtx;
        invMtx.invert(baseCopy);
        preScaleMtx(baseCopy, rScale.x, rScale.y, rScale.z);

        extractMtxTrans(pSrc, &srcTrans);
        multMtx(pDst, pSrc, invMtx);
        multMtx(pDst, pDst, baseCopy);
        setMtxTrans(pDst, srcTrans.x, srcTrans.y, srcTrans.z);
    }

    void scaleMtxToLocalMtx(MtxPtr pDst, MtxPtr pSrc, const TVec3f& rScale) {
        scaleMtxToLocalMtx(pDst, pDst, pSrc, rScale);
    }

    void scaleMtxToDir(TPos3f* pDst, const TVec3f& rDir, const TVec3f& rScale) {
        TVec3f axisX, axisY;
        MR::makeAxisCrossPlane(&axisX, &axisY, rDir);

        TPos3f tmp1;
        tmp1.identity();
        TPos3f tmp2;
        tmp2.identity();

        tmp1.setXYZDir(axisX, rDir, axisY);

        tmp2.mMtx[0][0] = axisX.x;
        tmp2.mMtx[0][1] = axisX.y;
        tmp2.mMtx[0][2] = axisX.z;
        tmp2.mMtx[1][0] = rDir.x;
        tmp2.mMtx[1][1] = rDir.y;
        tmp2.mMtx[1][2] = rDir.z;
        tmp2.mMtx[2][0] = axisY.x;
        tmp2.mMtx[2][1] = axisY.y;
        tmp2.mMtx[2][2] = axisY.z;

        pDst->zeroTrans();
        pDst->setScale(rScale.x, rScale.y, rScale.z);
        pDst->concat(*pDst, tmp2);
        pDst->concat(tmp1, *pDst);
    }

    void makeMtxTRS(MtxPtr pMtx, f32 tx, f32 ty, f32 tz, f32 rx, f32 ry, f32 rz, f32 sx, f32 sy, f32 sz) {
        f32 sinX = JMASinDegree(rx);
        f32 sinY = JMASinDegree(ry);
        f32 sinZ = JMASinDegree(rz);
        f32 cosX = JMACosDegree(rx);
        f32 cosY = JMACosDegree(ry);
        f32 cosZ = JMACosDegree(rz);

        pMtx[0][0] = sx * (cosZ * cosY);
        pMtx[1][0] = sx * (sinZ * cosY);
        pMtx[2][0] = -sinY * sx;

        pMtx[0][1] = sy * (cosZ * sinY * sinX - sinZ * cosX);
        pMtx[1][1] = sy * (sinZ * sinY * sinX + cosZ * cosX);
        pMtx[2][1] = sy * (cosY * sinX);

        pMtx[0][2] = sz * (cosZ * sinY * cosX + sinZ * sinX);
        pMtx[1][2] = sz * (sinZ * sinY * cosX - cosZ * sinX);
        pMtx[2][2] = sz * (cosY * cosX);

        pMtx[0][3] = tx;
        pMtx[1][3] = ty;
        pMtx[2][3] = tz;
    }

    void makeMtxTRS(MtxPtr pMtx, const TVec3f& rTrans, const TVec3f& rRot, const TVec3f& rScale) {
        makeMtxTRS(pMtx, rTrans.x, rTrans.y, rTrans.z, rRot.x, rRot.y, rRot.z, rScale.x, rScale.y, rScale.z);
    }

    void setMtxAxisXYZ(MtxPtr pMtx, const TVec3f& rAxisX, const TVec3f& rAxisY, const TVec3f& rAxisZ) {
        pMtx[0][0] = rAxisX.x;
        pMtx[1][0] = rAxisX.y;
        pMtx[2][0] = rAxisX.z;
        pMtx[0][1] = rAxisY.x;
        pMtx[1][1] = rAxisY.y;
        pMtx[2][1] = rAxisY.z;
        pMtx[0][2] = rAxisZ.x;
        pMtx[1][2] = rAxisZ.y;
        pMtx[2][2] = rAxisZ.z;
    }

    void flattenMtx(MtxPtr pDst, MtxPtr pSrc, const TVec3f& rNormal) {
        TVec3f axisX, axisY, axisZ;
        MR::extractMtxXDir(pSrc, &axisX);
        MR::extractMtxYDir(pSrc, &axisY);
        MR::extractMtxZDir(pSrc, &axisZ);

        axisX.orthogonalize(rNormal);
        axisY.orthogonalize(rNormal);
        axisZ.orthogonalize(rNormal);

        MR::setMtxAxisXYZ(pDst, axisX, axisY, axisZ);
    }

    void flattenMtx(MtxPtr pMtx, const TVec3f& rNormal) {
        flattenMtx(pMtx, pMtx, rNormal);
    }

    void blendMtxRotate(MtxPtr pMtxA, MtxPtr pMtxB, f32 blend, MtxPtr pDst) {
        Quaternion quatA, quatB, quatR;
        C_QUATMtx(&quatA, pMtxA);
        C_QUATMtx(&quatB, pMtxB);
        JMAQuatLerp(&quatA, &quatB, blend, &quatR);
        PSMTXQuat(pDst, &quatR);
    }

    void blendMtxRotateSlerp(MtxPtr pMtxA, MtxPtr pMtxB, f32 blend, MtxPtr pDst) {
        Quaternion quatA, quatB, quatR;
        C_QUATMtx(&quatA, pMtxA);
        C_QUATMtx(&quatB, pMtxB);
        C_QUATSlerp(&quatA, &quatB, &quatR, blend);
        PSMTXQuat(pDst, &quatR);
    }

    void blendMtx(MtxPtr pMtxA, MtxPtr pMtxB, f32 blend, MtxPtr pDst) {
        TVec3f transA, transB, transR;
        extractMtxTrans(pMtxA, &transA);
        extractMtxTrans(pMtxB, &transB);

        transR = transA * (1.0f - blend) + transB * blend;

        Quaternion quatR, quatB, quatA;
        C_QUATMtx(&quatA, pMtxA);
        C_QUATMtx(&quatB, pMtxB);

        JMAQuatLerp(&quatA, &quatB, blend, &quatR);

        PSMTXQuat(pDst, &quatR);
        setMtxTrans(pDst, transR.x, transR.y, transR.z);
    }

    void makeMtxWithoutScale(TPos3f* pDst, const TPos3f& rSrc) {
        TVec3f trans;
        rSrc.getTrans(trans);

        TVec3f axisX, axisY, axisZ;
        rSrc.getXYZDir(axisX, axisY, axisZ);

        pDst->setTrans(trans);

        MR::normalize(&axisX);
        MR::normalize(&axisY);
        MR::normalize(&axisZ);

        pDst->setXYZDir(axisX, axisY, axisZ);
    }

    void makeRTFromMtxPtr(TVec3f* pOutTrans, TVec3f* pOutRot, MtxPtr pSrc, bool toDegree) {
        if (pOutTrans != nullptr) {
            ((TPos3f*)pSrc)->getTrans(*pOutTrans);
        }

        if (pOutRot != nullptr) {
            ((TRot3f*)pSrc)->getEuler(*pOutRot);

            if (toDegree) {
                pOutRot->set(*pOutRot * _180_PI);
            }
        }
    }

    void makeMtxSideUp(TPos3f* pDst, const TVec3f& rSide, const TVec3f& rUp) {
        TVec3f axisZ, axisY, axisX;
        MR::normalize(rSide, &axisX);

        axisZ.cross(axisX, rUp);
        MR::normalize(&axisZ);

        axisY.cross(axisZ, axisX);

        pDst->setXYZDir(axisX, axisY, axisZ);
    }

    void makeMtxSideUpPos(TPos3f* pDst, const TVec3f& rSide, const TVec3f& rUp, const TVec3f& rPos) {
        makeMtxSideUp(pDst, rSide, rUp);
        pDst->setTrans(rPos);
    }

    void makeMtxSideFront(TPos3f* pDst, const TVec3f& rSide, const TVec3f& rFront) {
        TVec3f axisZ, axisY, axisX;
        MR::normalize(rSide, &axisX);

        axisY.cross(rFront, axisX);
        MR::normalize(&axisY);

        axisZ.cross(axisX, axisY);

        pDst->setXYZDir(axisX, axisY, axisZ);
    }

    void makeMtxUpSide(TPos3f* pDst, const TVec3f& rUp, const TVec3f& rSide) {
        TVec3f axisZ, axisY, axisX;
        MR::normalize(rUp, &axisY);

        axisZ.cross(rSide, axisY);
        MR::normalize(&axisZ);

        axisX.cross(axisY, axisZ);

        pDst->setXYZDir(axisX, axisY, axisZ);
    }

    void makeMtxUpSidePos(TPos3f* pDst, const TVec3f& rUp, const TVec3f& rSide, const TVec3f& rPos) {
        makeMtxUpSide(pDst, rUp, rSide);
        pDst->setTrans(rPos);
    }

    void makeMtxUpFront(TPos3f* pDst, const TVec3f& rUp, const TVec3f& rFront) {
        TVec3f axisZ, axisY, axisX;
        MR::normalize(rUp, &axisY);

        axisX.cross(axisY, rFront);
        MR::normalize(&axisX);

        axisZ.cross(axisX, axisY);

        pDst->setXYZDir(axisX, axisY, axisZ);
    }

    void makeMtxUpFrontPos(TPos3f* pDst, const TVec3f& rUp, const TVec3f& rFront, const TVec3f& rPos) {
        makeMtxUpFront(pDst, rUp, rFront);
        pDst->setTrans(rPos);
    }

    void makeMtxFrontSidePos(TPos3f* pDst, const TVec3f& rFront, const TVec3f& rSide, const TVec3f& rPos) {
        TVec3f axisX, axisY, axisZ;
        MR::normalize(rFront, &axisZ);

        axisY.cross(axisZ, rSide);
        MR::normalize(&axisY);

        axisX.cross(axisY, axisZ);

        pDst->setXYZDir(axisX, axisY, axisZ);
        pDst->setTrans(rPos);
    }

    void makeMtxFrontUp(TPos3f* pDst, const TVec3f& rFront, const TVec3f& rUp) {
        TVec3f axisZ, axisY, axisX;
        MR::normalize(rFront, &axisZ);

        axisX.cross(rUp, axisZ);
        MR::normalize(&axisX);

        axisY.cross(axisZ, axisX);

        pDst->setXYZDir(axisX, axisY, axisZ);
    }

    void makeMtxFrontUpPos(TPos3f* pDst, const TVec3f& rFront, const TVec3f& rUp, const TVec3f& rPos) {
        makeMtxFrontUp(pDst, rFront, rUp);
        pDst->setTrans(rPos);
    }

    void makeMtxUpNoSupport(TPos3f* pDst, const TVec3f& rUp) {
        TVec3f support;
        if (MR::getMaxAbsElementIndex(rUp) == 2) {
            support.set(0.0f, 1.0f, 0.0f);
        } else {
            support.set(0.0f, 0.0f, 1.0f);
        }

        TVec3f axisX, axisY, axisZ;
        MR::normalize(rUp, &axisY);

        axisX.cross(axisY, support);
        MR::normalize(&axisX);

        axisZ.cross(axisX, axisY);

        pDst->setXYZDir(axisX, axisY, axisZ);
    }

    void makeMtxUpNoSupportPos(TPos3f* pDst, const TVec3f& rUp, const TVec3f& rPos) {
        TVec3f support;
        if (MR::getMaxAbsElementIndex(rUp) == 2) {
            support.set< f32 >(0.0f, 1.0f, 0.0f);
        } else {
            support.set< f32 >(0.0f, 0.0f, 1.0f);
        }

        MR::makeMtxUpFrontPos(pDst, rUp, support, rPos);
    }

    void makeMtxFrontNoSupport(TPos3f* pDst, const TVec3f& rFront) {
        TVec3f support;
        if (MR::getMaxAbsElementIndex(rFront) == 1) {
            support.set(1.0f, 0.0f, 0.0f);
        } else {
            support.set(0.0f, 1.0f, 0.0f);
        }

        TVec3f axisX, axisY, axisZ;
        MR::normalize(rFront, &axisZ);

        axisX.cross(support, axisZ);
        MR::normalize(&axisX);

        axisY.cross(axisZ, axisX);

        pDst->setXYZDir(axisX, axisY, axisZ);
    }

    void makeMtxFrontNoSupportPos(TPos3f* pDst, const TVec3f& rFront, const TVec3f& rPos) {
        TVec3f support;
        if (MR::getMaxAbsElementIndex(rFront) == 1) {
            support.set< f32 >(1.0f, 0.0f, 0.0f);
        } else {
            support.set< f32 >(0.0f, 1.0f, 0.0f);
        }

        MR::makeMtxFrontUpPos(pDst, rFront, support, rPos);
    }

    void orthogonalize(TPos3f* pMtx) {
        TVec3f axisZ, axisY, axisX;

        pMtx->getXYZDir(axisX, axisY, axisZ);

        axisX.cross(axisY, axisZ);
        axisY.cross(axisZ, axisX);

        axisX.normalize();
        axisY.normalize();

        pMtx->setXYZDir(axisX, axisY, axisZ);

        TVec3f zDir;
        pMtx->getZDir(zDir);

        f32 magAll = pMtx->mMtx[0][0] * pMtx->mMtx[0][0] + pMtx->mMtx[1][0] * pMtx->mMtx[1][0] + pMtx->mMtx[2][0] * pMtx->mMtx[2][0] +
                     pMtx->mMtx[0][1] * pMtx->mMtx[0][1] + pMtx->mMtx[1][1] * pMtx->mMtx[1][1] + pMtx->mMtx[2][1] * pMtx->mMtx[2][1] +
                     pMtx->mMtx[0][2] * pMtx->mMtx[0][2] + pMtx->mMtx[1][2] * pMtx->mMtx[1][2] + pMtx->mMtx[2][2] * pMtx->mMtx[2][2];

        JGeometry::TUtil< f32 >::sqrt(magAll);

        if (pMtx != nullptr) {
            f32 magX = pMtx->mMtx[0][0] * pMtx->mMtx[0][0] + pMtx->mMtx[1][0] * pMtx->mMtx[1][0] + pMtx->mMtx[2][0] * pMtx->mMtx[2][0];
            f32 invSqrtX = JGeometry::TUtil< f32 >::inv_sqrt(magX);
            pMtx->mMtx[0][0] = invSqrtX * pMtx->mMtx[0][0];
            pMtx->mMtx[1][0] = invSqrtX * pMtx->mMtx[1][0];
            pMtx->mMtx[2][0] = invSqrtX * pMtx->mMtx[2][0];

            f32 magY = pMtx->mMtx[0][1] * pMtx->mMtx[0][1] + pMtx->mMtx[1][1] * pMtx->mMtx[1][1] + pMtx->mMtx[2][1] * pMtx->mMtx[2][1];
            f32 invSqrtY = JGeometry::TUtil< f32 >::inv_sqrt(magY);
            pMtx->mMtx[0][1] = invSqrtY * pMtx->mMtx[0][1];
            pMtx->mMtx[1][1] = invSqrtY * pMtx->mMtx[1][1];
            pMtx->mMtx[2][1] = invSqrtY * pMtx->mMtx[2][1];

            f32 magZ = pMtx->mMtx[0][2] * pMtx->mMtx[0][2] + pMtx->mMtx[1][2] * pMtx->mMtx[1][2] + pMtx->mMtx[2][2] * pMtx->mMtx[2][2];
            f32 invSqrtZ = JGeometry::TUtil< f32 >::inv_sqrt(magZ);
            pMtx->mMtx[0][2] = invSqrtZ * pMtx->mMtx[0][2];
            pMtx->mMtx[1][2] = invSqrtZ * pMtx->mMtx[1][2];
            pMtx->mMtx[2][2] = invSqrtZ * pMtx->mMtx[2][2];
        }

        pMtx->mMtx[0][2] = zDir.x;
        pMtx->mMtx[1][2] = zDir.y;
        pMtx->mMtx[2][2] = zDir.z;
    }

    void turnMtxToXDirRate(TPos3f* pMtx, const TVec3f& rDir, f32 rate) {
        TQuat4f quat;
        pMtx->getQuat(quat);
        MR::turnQuatXDirRate(&quat, quat, rDir, rate);
        pMtx->setQuat(quat);
    }

    void turnMtxToXDirDegree(TPos3f* pMtx, const TVec3f& rDir, f32 degree) {
        TQuat4f quat;
        pMtx->getQuat(quat);
        MR::turnQuatXDirRad(&quat, quat, rDir, PI_180 * degree);
        pMtx->setQuat(quat);
    }

    void turnMtxToYDirRate(TPos3f* pMtx, const TVec3f& rDir, f32 rate) {
        TQuat4f quat;
        pMtx->getQuat(quat);
        MR::turnQuatYDirRate(&quat, quat, rDir, rate);
        pMtx->setQuat(quat);
    }

    void turnMtxToYDirDegree(TPos3f* pMtx, const TVec3f& rDir, f32 degree) {
        TQuat4f quat;
        pMtx->getQuat(quat);
        MR::turnQuatYDirRad(&quat, quat, rDir, PI_180 * degree);
        pMtx->setQuat(quat);
    }

    void turnMtxToLocalDirDegree(TPos3f* pMtx, const TVec3f& rLocalAxis, const TVec3f& rDir, f32 degree) {
        TVec3f worldAxis;
        pMtx->mult33(rLocalAxis, worldAxis);

        TQuat4f quat;
        pMtx->getQuat(quat);
        MR::turnQuat(&quat, quat, worldAxis, rDir, PI_180 * degree);
        pMtx->setQuat(quat);
    }

    void makeMtxMoment(TPos3f* pMtx, const TVec3f& rMoment) {
        pMtx->identity();

        if (!rMoment.isZero()) {
            TVec3f dir(rMoment);
            f32 mag;
            MR::separateScalarAndDirection(&mag, &dir, dir);
            pMtx->makeRotate(dir, mag);
        }
    }

    void rotateMtxMoment(TPos3f* pDst, const TPos3f& rSrc, const TVec3f& rMoment) {
        if (rMoment.isZero()) {
            *pDst = rSrc;
        } else {
            TVec3f dir(rMoment);
            f32 mag;
            MR::separateScalarAndDirection(&mag, &dir, dir);

            TVec3f trans;
            rSrc.getTrans(trans);

            TPos3f rotMtx;
            rotMtx.makeRotate(dir, mag);
            rotMtx.concat(rotMtx, rSrc);
            orthogonalize(&rotMtx);
            rotMtx.setTrans(trans);
            *pDst = rotMtx;
        }
    }

    void rotateMtxMoment(TPos3f* pMtx, const TVec3f& rMoment) {
        rotateMtxMoment(pMtx, *pMtx, rMoment);
    }

    void setMtxTrans(MtxPtr pMtx, f32 x, f32 y, f32 z) {
        pMtx[0][3] = x;
        pMtx[1][3] = y;
        pMtx[2][3] = z;
    }

    void addTransMtx(MtxPtr pMtx, const TVec3f& rVec) {
        pMtx[0][3] += rVec.x;
        pMtx[1][3] += rVec.y;
        pMtx[2][3] += rVec.z;
    }

    void addTransMtxLocal(MtxPtr pMtx, const TVec3f& rVec) {
        addTransMtxLocalX(pMtx, rVec.x);
        addTransMtxLocalY(pMtx, rVec.y);
        addTransMtxLocalZ(pMtx, rVec.z);
    }

    void addTransMtxLocalX(MtxPtr pMtx, f32 x_coord) {
        pMtx[0][3] = pMtx[0][3] + (pMtx[0][0] * x_coord);
        pMtx[1][3] = pMtx[1][3] + (pMtx[1][0] * x_coord);
        pMtx[2][3] = pMtx[2][3] + (pMtx[2][0] * x_coord);
    }

    void addTransMtxLocalY(MtxPtr pMtx, f32 y_coord) {
        pMtx[0][3] = pMtx[0][3] + (pMtx[0][1] * y_coord);
        pMtx[1][3] = pMtx[1][3] + (pMtx[1][1] * y_coord);
        pMtx[2][3] = pMtx[2][3] + (pMtx[2][1] * y_coord);
    }

    void addTransMtxLocalZ(MtxPtr pMtx, f32 z_coord) {
        pMtx[0][3] = pMtx[0][3] + (pMtx[0][2] * z_coord);
        pMtx[1][3] = pMtx[1][3] + (pMtx[1][2] * z_coord);
        pMtx[2][3] = pMtx[2][3] + (pMtx[2][2] * z_coord);
    }

    void extractMtxXDir(MtxPtr pMtx, TVec3f* pOut) {
        pOut->x = pMtx[0][0];
        pOut->y = pMtx[1][0];
        pOut->z = pMtx[2][0];
    }

    void extractMtxYDir(MtxPtr pMtx, TVec3f* pOut) {
        pOut->x = pMtx[0][1];
        pOut->y = pMtx[1][1];
        pOut->z = pMtx[2][1];
    }

    void extractMtxZDir(MtxPtr pMtx, TVec3f* pOut) {
        pOut->x = pMtx[0][2];
        pOut->y = pMtx[1][2];
        pOut->z = pMtx[2][2];
    }

    void extractMtxXYZDir(MtxPtr pMtx, TVec3f* pOutX, TVec3f* pOutY, TVec3f* pOutZ) {
        MR::extractMtxXDir(pMtx, pOutX);
        MR::extractMtxYDir(pMtx, pOutY);
        MR::extractMtxZDir(pMtx, pOutZ);
    }

    void extractMtxTrans(MtxPtr pMtx, TVec3f* pOut) {
        pOut->x = pMtx[0][3];
        pOut->y = pMtx[1][3];
        pOut->z = pMtx[2][3];
    }

    bool isSameMtx(MtxPtr pMtxA, MtxPtr pMtxB) {
        f32* pA = (f32*)pMtxA;
        f32* pB = (f32*)pMtxB;
        for (int i = 0; i < 12; i++) {
            if (*pA != *pB) {
                return false;
            }

            pA++;
            pB++;
        }

        return true;
    }

    bool isSameMtxRot(MtxPtr pMtxA, MtxPtr pMtxB) {
        f32* pA = (f32*)pMtxA;
        f32* pB = (f32*)pMtxB;
        for (u32 i = 0; i < 12; i++) {
            if ((i & 3) != 3) {
                if (*pA != *pB) {
                    return false;
                }
            }
            pA++;
            pB++;
        }

        return true;
    }

    bool isRotAxisY(MtxPtr pMtxA, MtxPtr pMtxB) {
        TVec3f yDirA, yDirB;
        ((TRot3f*)pMtxA)->getYDir(yDirA);
        ((TRot3f*)pMtxB)->getYDir(yDirB);
        bool result = false;

        if (JGeometry::TUtil< f32 >::epsilonEquals(yDirA.x, yDirB.x, 0.001f) && JGeometry::TUtil< f32 >::epsilonEquals(yDirA.y, yDirB.y, 0.001f) &&
            JGeometry::TUtil< f32 >::epsilonEquals(yDirA.z, yDirB.z, 0.001f)) {
            result = true;
        }

        return result;
    }

    void calcMtxRotAxis(TVec3f* pOut, MtxPtr pMtxA, MtxPtr pMtxB) {
        TVec3f localY(0.0f, 1.0f, 0.0f);

        Mtx invA;
        PSMTXInverse(pMtxA, invA);

        TVec3f axisYA, axisYB;
        PSMTXMultVecSR(invA, &localY, &axisYA);
        PSMTXMultVecSR(pMtxB, &axisYA, &axisYB);

        TVec3f cross = localY.cross(axisYB);

        if (MR::normalizeOrZero(&cross)) {
            *pOut = localY;
        } else {
            *pOut = cross;
        }
    }

    MtxPtr tmpMtxTrans(const TVec3f& rVec) {
        setMtxTrans(mtrans_org, rVec.x, rVec.y, rVec.z);
        return mtrans_org;
    }

    MtxPtr tmpMtxScale(f32 sx, f32 sy, f32 sz) {
        tmpmtx_sc[0][0] = sx;
        tmpmtx_sc[1][1] = sy;
        tmpmtx_sc[2][2] = sz;
        return tmpmtx_sc;
    }

    MtxPtr tmpMtxRotXRad(f32 rad) {
        f32 cosX = JMACosRadian(rad);
        f32 sinX = JMASinRadian(rad);
        tmpmtx_rx[1][1] = cosX;
        tmpmtx_rx[2][1] = sinX;
        tmpmtx_rx[1][2] = -sinX;
        tmpmtx_rx[2][2] = cosX;
        return tmpmtx_rx;
    }

    MtxPtr tmpMtxRotYRad(f32 rad) {
        f32 cosY = JMACosRadian(rad);
        f32 sinY = JMASinRadian(rad);
        tmpmtx_ry[0][0] = cosY;
        tmpmtx_ry[0][2] = sinY;
        tmpmtx_ry[2][0] = -sinY;
        tmpmtx_ry[2][2] = cosY;
        return tmpmtx_ry;
    }

    MtxPtr tmpMtxRotZRad(f32 rad) {
        f32 cosZ = JMACosRadian(rad);
        f32 sinZ = JMASinRadian(rad);
        tmpmtx_rz[0][0] = cosZ;
        tmpmtx_rz[1][0] = sinZ;
        tmpmtx_rz[0][1] = -sinZ;
        tmpmtx_rz[1][1] = cosZ;
        return tmpmtx_rz;
    }

    MtxPtr tmpMtxRotXDeg(f32 deg) {
        f32 cosX = JMACosDegree(deg);
        f32 sinX = JMASinDegree(deg);
        tmpmtx_rx[1][1] = cosX;
        tmpmtx_rx[2][1] = sinX;
        tmpmtx_rx[1][2] = -sinX;
        tmpmtx_rx[2][2] = cosX;
        return tmpmtx_rx;
    }

    MtxPtr tmpMtxRotYDeg(f32 deg) {
        f32 cosY = JMACosDegree(deg);
        f32 sinY = JMASinDegree(deg);
        tmpmtx_ry[0][0] = cosY;
        tmpmtx_ry[0][2] = sinY;
        tmpmtx_ry[2][0] = -sinY;
        tmpmtx_ry[2][2] = cosY;
        return tmpmtx_ry;
    }

    MtxPtr tmpMtxRotZDeg(f32 deg) {
        f32 cosZ = JMACosDegree(deg);
        f32 sinZ = JMASinDegree(deg);
        tmpmtx_rz[0][0] = cosZ;
        tmpmtx_rz[1][0] = sinZ;
        tmpmtx_rz[0][1] = -sinZ;
        tmpmtx_rz[1][1] = cosZ;
        return tmpmtx_rz;
    }

    void orderRotateMtx(s16 order, const TVec3f& rRad, MtxPtr pDst) {
        const TVec3f* pRad = &rRad;
        MtxPtr mtxX = tmpMtxRotXRad(pRad->x);
        MtxPtr mtxY = tmpMtxRotYRad(pRad->y);
        MtxPtr mtxZ = tmpMtxRotZRad(pRad->z);

        MtxPtr first, second, third;

        switch (order) {
        case 0:
            first = mtxY;
            second = mtxX;
            third = mtxZ;
            break;
        case 1:
            second = mtxX;
            first = mtxZ;
            third = mtxY;
            break;
        case 2:
            first = mtxX;
            second = mtxY;
            third = mtxZ;
            break;
        case 3:
            first = mtxZ;
            second = mtxY;
            third = mtxX;
            break;
        case 4:
            second = mtxZ;
            first = mtxX;
            third = mtxZ;
            break;
        case 5:
            first = mtxY;
            second = mtxZ;
            third = mtxX;
            break;
        }

        MR::multMtx(pDst, second, first);
        MR::multMtx(pDst, pDst, third);
    }

    void rotAxisVecRad(const TVec3f& rAxis, const TVec3f& rVec, TVec3f* pOut, f32 rad) {
        Mtx rot;
        PSMTXRotAxisRad(rot, &rVec, rad);
        PSMTXMultVec(rot, &rAxis, pOut);
    }
};  // namespace MR
