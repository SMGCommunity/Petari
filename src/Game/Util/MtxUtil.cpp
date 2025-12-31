#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"

#define PI_180 0.017453292f

static Mtx mtrans_org = {
    { 1.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 1.0f, 0.0f }
};

static Mtx tmpmtx_sc = {
    { 1.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 1.0f, 0.0f }
};

static Mtx tmpmtx_rx = {
    { 1.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 1.0f, 0.0f }
};

static Mtx tmpmtx_ry = {
    { 1.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 1.0f, 0.0f }
};

static Mtx tmpmtx_rz = {
    { 1.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 1.0f, 0.0f }
};

namespace MR {
    void makeMtxRotate(MtxPtr mtx, s16 rx, s16 ry, s16 rz) {
        f32 sinY = JMASSin(ry);
        f32 cosZ = JMASCos(rz);
        f32 sinZ = JMASSin(rz);
        f32 cosX = JMASCos(rx);
        f32 sinX = JMASSin(rx);
        f32 cosY = JMASCos(ry);

        f32 sinZsinY = sinZ * sinY;
        f32 cosZsinY = cosZ * sinY;

        f32 sinXsinZsinY = sinX * sinZsinY;
        f32 cosXcosZ = cosX * cosZ;

        mtx[2][0] = -sinY;

        f32 cosZcosY = cosZ * cosY;
        f32 sinZcosY = sinZ * cosY;

        mtx[0][0] = cosZcosY;
        mtx[1][0] = sinZcosY;

        f32 cosXsinZ = cosX * sinZ;
        f32 sinXcosZ = sinX * cosZ;

        f32 sinXcosZsinY = sinX * cosZsinY;
        f32 cosXsinZsinY = cosX * sinZsinY;

        mtx[0][3] = 0.0f;
        mtx[0][1] = sinXcosZsinY - cosXsinZ;
        mtx[0][2] = cosXsinZsinY + sinXcosZ;

        f32 sinXcosY = sinX * cosY;
        f32 cosXcosY = cosX * cosY;

        mtx[2][1] = sinXcosY;
        mtx[1][1] = sinXsinZsinY + cosXcosZ;
        mtx[1][2] = cosXsinZsinY - sinXcosZ;

        mtx[2][2] = cosXcosY;
        mtx[1][3] = 0.0f;
        mtx[2][3] = 0.0f;
    }

    void makeMtxRotate(MtxPtr mtx, f32 rx, f32 ry, f32 rz) {
        s16 angleX = (s16)(rx * 182.04445f);
        s16 angleY = (s16)(ry * 182.04445f);
        s16 angleZ = (s16)(rz * 182.04445f);
        makeMtxRotate(mtx, angleX, angleY, angleZ);
    }

    void makeMtxRotate(MtxPtr mtx, const TVec3f& rVec) {
        makeMtxRotate(mtx, rVec.x, rVec.y, rVec.z);
    }

    void makeMtxRotateY(MtxPtr mtx, f32 ry) {
        s16 angle = (s16)(ry * 182.04445f);
        f32 sinY = JMASSin(angle);
        f32 cosY = JMASCos(angle);

        mtx[0][0] = cosY;
        mtx[1][0] = 0.0f;
        mtx[2][0] = -sinY;
        mtx[0][1] = 0.0f;
        mtx[1][1] = 1.0f;
        mtx[2][1] = 0.0f;
        mtx[0][2] = sinY;
        mtx[1][2] = 0.0f;
        mtx[2][2] = cosY;
        mtx[0][3] = 0.0f;
        mtx[1][3] = 0.0f;
        mtx[2][3] = 0.0f;
    }

    void makeMtxTR(MtxPtr mtx, f32 tx, f32 ty, f32 tz, f32 rx, f32 ry, f32 rz) {
        f32 sinX = JMASinDegree(rx);
        f32 sinY = JMASinDegree(ry);
        f32 sinZ = JMASinDegree(rz);
        f32 cosX = JMACosDegree(rx);
        f32 cosY = JMACosDegree(ry);
        f32 cosZ = JMACosDegree(rz);
        f32 sinZsinY = sinZ * sinY;
        f32 cosZsinY = cosZ * sinY;
        mtx[0][0] = cosZ * cosY;
        mtx[0][1] = sinX * cosZsinY - cosX * sinZ;
        mtx[0][2] = cosX * cosZsinY + sinX * sinZ;
        mtx[0][3] = tx;

        mtx[1][0] = sinZ * cosY;
        mtx[1][1] = sinX * sinZsinY + cosX * cosZ;
        mtx[1][2] = cosX * sinZsinY - sinX * cosZ;
        mtx[1][3] = ty;

        mtx[2][0] = -sinY;
        mtx[2][1] = sinX * cosY;
        mtx[2][2] = cosX * cosY;
        mtx[2][3] = tz;
    }

    void makeMtxTR(MtxPtr mtx, const TVec3f& rTrans, const TVec3f& rRot) {
        makeMtxTR(mtx, rTrans.x, rTrans.y, rTrans.z, rRot.x, rRot.y, rRot.z);
    }

    void makeMtxTransRotateY(MtxPtr mtx, f32 tx, f32 ty, f32 tz, f32 ry) {
        makeMtxRotateY(mtx, ry);
        mtx[0][3] = tx;
        mtx[1][3] = ty;
        mtx[2][3] = tz;
    }

    void rotateMtxLocalX(MtxPtr mtx, f32 rad) {
        f32 cosX = JMACosRadian(rad);
        f32 sinX = JMASinRadian(rad);

        f32 y0 = mtx[0][1];
        f32 z0 = mtx[0][2];
        f32 y1 = mtx[1][1];
        f32 z1 = mtx[1][2];
        f32 y2 = mtx[2][1];
        f32 z2 = mtx[2][2];

        mtx[0][1] = cosX * y0 - sinX * z0;
        mtx[1][1] = cosX * y1 - sinX * z1;
        mtx[2][1] = cosX * y2 - sinX * z2;
        mtx[0][2] = sinX * y0 + cosX * z0;
        mtx[1][2] = sinX * y1 + cosX * z1;
        mtx[2][2] = sinX * y2 + cosX * z2;
    }

    void rotateMtxLocalY(MtxPtr mtx, f32 rad) {
        f32 cosY = JMACosRadian(rad);
        f32 sinY = JMASinRadian(rad);

        f32 x0 = mtx[0][0];
        f32 z0 = mtx[0][2];
        f32 x1 = mtx[1][0];
        f32 z1 = mtx[1][2];
        f32 x2 = mtx[2][0];
        f32 z2 = mtx[2][2];

        mtx[0][0] = cosY * x0 - sinY * z0;
        mtx[1][0] = cosY * x1 - sinY * z1;
        mtx[2][0] = cosY * x2 - sinY * z2;
        mtx[0][2] = sinY * x0 + cosY * z0;
        mtx[1][2] = sinY * x1 + cosY * z1;
        mtx[2][2] = sinY * x2 + cosY * z2;
    }

    void rotateMtxLocalZ(MtxPtr mtx, f32 rad) {
        f32 cosZ = JMACosRadian(rad);
        f32 sinZ = JMASinRadian(rad);

        f32 x0 = mtx[0][0];
        f32 y0 = mtx[0][1];
        f32 x1 = mtx[1][0];
        f32 y1 = mtx[1][1];
        f32 x2 = mtx[2][0];
        f32 y2 = mtx[2][1];

        mtx[0][0] = cosZ * x0 - sinZ * y0;
        mtx[1][0] = cosZ * x1 - sinZ * y1;
        mtx[2][0] = cosZ * x2 - sinZ * y2;
        mtx[0][1] = sinZ * x0 + cosZ * y0;
        mtx[1][1] = sinZ * x1 + cosZ * y1;
        mtx[2][1] = sinZ * x2 + cosZ * y2;
    }

    void rotateMtxLocalXDegree(MtxPtr mtx, f32 deg) {
        rotateMtxLocalX(mtx, deg * PI_180);
    }

    void rotateMtxLocalYDegree(MtxPtr mtx, f32 deg) {
        rotateMtxLocalY(mtx, deg * PI_180);
    }

    void rotateMtxLocalZDegree(MtxPtr mtx, f32 deg) {
        rotateMtxLocalZ(mtx, deg * PI_180);
    }

    void preScaleMtx(MtxPtr mtx, f32 scale) {
        preScaleMtx(mtx, scale, scale, scale);
    }

    void preScaleMtx(MtxPtr mtx, const TVec3f& rScale) {
        preScaleMtx(mtx, rScale.x, rScale.y, rScale.z);
    }

    void preScaleMtx(MtxPtr mtx, f32 sx, f32 sy, f32 sz) {
        mtx[0][0] *= sx;
        mtx[1][0] *= sx;
        mtx[2][0] *= sx;
        mtx[0][1] *= sy;
        mtx[1][1] *= sy;
        mtx[2][1] *= sy;
        mtx[0][2] *= sz;
        mtx[1][2] *= sz;
        mtx[2][2] *= sz;
    }

    void scaleMtxToLocalMtx(MtxPtr dst, MtxPtr src, MtxPtr base, const TVec3f& rScale) {
        TPos3f baseCopy;
        baseCopy.setInline(base);
        TVec3f srcTrans;
        baseCopy.setTrans(TVec3f(0.0f, 0.0f, 0.0f));
        
        TPos3f invMtx;
        invMtx.invert(baseCopy);
        preScaleMtx(baseCopy.toMtxPtr(), rScale.x, rScale.y, rScale.z);
        
        extractMtxTrans(src, &srcTrans);
        PSMTXConcat(invMtx.toMtxPtr(), src, dst);
        PSMTXConcat(baseCopy.toMtxPtr(), dst, dst);
        setMtxTrans(dst, srcTrans.x, srcTrans.y, srcTrans.z);
    }

    void scaleMtxToLocalMtx(MtxPtr dst, MtxPtr src, const TVec3f& rScale) {
        scaleMtxToLocalMtx(dst, dst, src, rScale);
    }

    void scaleMtxToDir(TPos3f* pDst, const TVec3f& rDir, const TVec3f& rScale) {
        TVec3f axisX, axisY;
        MR::makeAxisCrossPlane(&axisX, &axisY, rDir);

        TPos3f tmp1;
        tmp1.identity();
        TPos3f tmp2;
        tmp2.identity();

        tmp1.setXYZDir(axisX, axisY, rDir);

        // Copy rotation part from tmp1 to tmp2
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

    void makeMtxTRS(MtxPtr mtx, f32 tx, f32 ty, f32 tz, f32 rx, f32 ry, f32 rz, f32 sx, f32 sy, f32 sz) {
        f32 sinX = JMASinDegree(rx);
        f32 sinY = JMASinDegree(ry);
        f32 sinZ = JMASinDegree(rz);
        f32 cosX = JMACosDegree(rx);
        f32 cosY = JMACosDegree(ry);
        f32 cosZ = JMACosDegree(rz);
        f32 cosZsinY = cosZ * sinY;
        mtx[0][3] = tx;

        f32 m00 = cosZ * cosY;
        f32 cosXsinZ = cosX * sinZ;
        mtx[1][3] = ty;

        f32 sinXcosZsinY = sinX * cosZsinY;
        f32 cosXcosZsinY = cosX * cosZsinY;
        mtx[0][0] = sx * m00;

        f32 sinXsinZ = sinX * sinZ;
        f32 m01 = sinXcosZsinY - cosXsinZ;
        mtx[2][3] = tz;

        f32 m10 = sinZ * cosY;
        f32 m02 = cosXcosZsinY + sinXsinZ;
        f32 m20 = -sinY;
        mtx[0][1] = sy * m01;

        f32 sinZsinY = sinZ * sinY;
        mtx[0][2] = sz * m02;
        mtx[1][0] = sx * m10;

        f32 cosXcosZ = cosX * cosZ;
        f32 sinXcosY = sinX * cosY;
        f32 cosXcosY = cosX * cosY;
        mtx[2][0] = sx * m20;

        f32 sinXsinZsinY = sinX * sinZsinY;
        f32 cosXsinZsinY = cosX * sinZsinY;
        f32 sinXcosZ = sinX * cosZ;
        mtx[2][1] = sy * sinXcosY;
        mtx[2][2] = sz * cosXcosY;

        f32 m11 = sinXsinZsinY + cosXcosZ;
        f32 m12 = cosXsinZsinY - sinXcosZ;
        mtx[1][1] = sy * m11;
        mtx[1][2] = sz * m12;
    }

    void makeMtxTRS(MtxPtr mtx, const TVec3f& rTrans, const TVec3f& rRot, const TVec3f& rScale) {
        makeMtxTRS(mtx, rTrans.x, rTrans.y, rTrans.z, rRot.x, rRot.y, rRot.z, rScale.x, rScale.y, rScale.z);
    }

    void setMtxAxisXYZ(MtxPtr mtx, const TVec3f& rAxisX, const TVec3f& rAxisY, const TVec3f& rAxisZ) {
        mtx[0][0] = rAxisX.x;
        mtx[1][0] = rAxisX.y;
        mtx[2][0] = rAxisX.z;
        mtx[0][1] = rAxisY.x;
        mtx[1][1] = rAxisY.y;
        mtx[2][1] = rAxisY.z;
        mtx[0][2] = rAxisZ.x;
        mtx[1][2] = rAxisZ.y;
        mtx[2][2] = rAxisZ.z;
    }

    void flattenMtx(MtxPtr dst, MtxPtr src, const TVec3f& rNormal) {
        TVec3f axisX, axisY, axisZ;
        ((TRot3f*)src)->getXDir(axisX);
        ((TRot3f*)src)->getYDir(axisY);
        ((TRot3f*)src)->getZDir(axisZ);

        JMAVECScaleAdd(&rNormal, &axisX, &axisX, -rNormal.dot(axisX));
        JMAVECScaleAdd(&rNormal, &axisY, &axisY, -rNormal.dot(axisY));
        JMAVECScaleAdd(&rNormal, &axisZ, &axisZ, -rNormal.dot(axisZ));

        ((TRot3f*)dst)->setXYZDir(axisX, axisY, axisZ);
    }

    void flattenMtx(MtxPtr mtx, const TVec3f& rNormal) {
        flattenMtx(mtx, mtx, rNormal);
    }

    void blendMtxRotate(MtxPtr mtxA, MtxPtr mtxB, f32 blend, MtxPtr dst) {
        Quaternion quatA, quatB, quatR;
        C_QUATMtx(&quatA, mtxA);
        C_QUATMtx(&quatB, mtxB);
        JMAQuatLerp(&quatA, &quatB, blend, &quatR);
        PSMTXQuat(dst, &quatR);
    }

    void blendMtxRotateSlerp(MtxPtr mtxA, MtxPtr mtxB, f32 blend, MtxPtr dst) {
        Quaternion quatA, quatB, quatR;
        C_QUATMtx(&quatA, mtxA);
        C_QUATMtx(&quatB, mtxB);
        C_QUATSlerp(&quatA, &quatB, &quatR, blend);
        PSMTXQuat(dst, &quatR);
    }

    void blendMtx(MtxPtr mtxA, MtxPtr mtxB, f32 blend, MtxPtr dst) {
        TVec3f transA, transB;
        extractMtxTrans(mtxA, &transA);
        extractMtxTrans(mtxB, &transB);

        TVec3f scaledB = transB * blend;
        TVec3f scaledA = transA * (1.0f - blend);
        TVec3f transR = scaledA + scaledB;

        Quaternion quatA, quatB, quatR;
        C_QUATMtx(&quatA, mtxA);
        C_QUATMtx(&quatB, mtxB);

        JMAQuatLerp(&quatA, &quatB, blend, &quatR);

        PSMTXQuat(dst, &quatR);
        setMtxTrans(dst, transR.x, transR.y, transR.z);
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

    void makeRTFromMtxPtr(TVec3f* pOutTrans, TVec3f* pOutRot, MtxPtr src, bool toDegree) {
        if (pOutTrans) {
            ((TPos3f*)src)->getTrans(*pOutTrans);
        }

        if (pOutRot) {
            ((TRot3f*)src)->getEuler(*pOutRot);

            if (toDegree) {
                TVec3f scaled;
                pOutRot->scale(57.29578f, scaled);
                *pOutRot = scaled;
            }
        }
    }

    void makeMtxSideUp(TPos3f* pDst, const TVec3f& rSide, const TVec3f& rUp) {
        TVec3f axisX;
        MR::normalize(rSide, &axisX);

        TVec3f axisZ;
        PSVECCrossProduct(&axisX, &rUp, &axisZ);
        MR::normalize(&axisZ);

        TVec3f axisY;
        PSVECCrossProduct(&axisZ, &axisX, &axisY);

        pDst->setXYZDir(axisX, axisY, axisZ);
    }

    void makeMtxSideUpPos(TPos3f* pDst, const TVec3f& rSide, const TVec3f& rUp, const TVec3f& rPos) {
        makeMtxSideUp(pDst, rSide, rUp);
        pDst->setTrans(rPos);
    }

    void makeMtxSideFront(TPos3f* pDst, const TVec3f& rSide, const TVec3f& rFront) {
        TVec3f axisX;
        MR::normalize(rSide, &axisX);

        TVec3f axisY;
        PSVECCrossProduct(&axisX, &rFront, &axisY);
        MR::normalize(&axisY);

        TVec3f axisZ;
        PSVECCrossProduct(&axisY, &axisX, &axisZ);

        pDst->setXYZDir(axisX, axisY, axisZ);
    }

    void makeMtxUpSide(TPos3f* pDst, const TVec3f& rUp, const TVec3f& rSide) {
        TVec3f axisY;
        MR::normalize(rUp, &axisY);

        TVec3f axisZ;
        PSVECCrossProduct(&axisY, &rSide, &axisZ);
        MR::normalize(&axisZ);

        TVec3f axisX;
        PSVECCrossProduct(&axisZ, &axisY, &axisX);

        pDst->setXYZDir(axisX, axisY, axisZ);
    }

    void makeMtxUpSidePos(TPos3f* pDst, const TVec3f& rUp, const TVec3f& rSide, const TVec3f& rPos) {
        makeMtxUpSide(pDst, rUp, rSide);
        pDst->setTrans(rPos);
    }

    void makeMtxUpFront(TPos3f* pDst, const TVec3f& rUp, const TVec3f& rFront) {
        TVec3f axisY;
        MR::normalize(rUp, &axisY);

        TVec3f axisX;
        PSVECCrossProduct(&axisY, &rFront, &axisX);
        MR::normalize(&axisX);

        TVec3f axisZ;
        PSVECCrossProduct(&axisX, &axisY, &axisZ);

        pDst->setXYZDir(axisX, axisY, axisZ);
    }

    void makeMtxUpFrontPos(TPos3f* pDst, const TVec3f& rUp, const TVec3f& rFront, const TVec3f& rPos) {
        makeMtxUpFront(pDst, rUp, rFront);
        pDst->setTrans(rPos);
    }

    void makeMtxFrontSidePos(TPos3f* pDst, const TVec3f& rFront, const TVec3f& rSide, const TVec3f& rPos) {
        TVec3f axisZ;
        MR::normalize(rFront, &axisZ);

        TVec3f axisY;
        PSVECCrossProduct(&axisZ, &rSide, &axisY);
        MR::normalize(&axisY);

        TVec3f axisX;
        PSVECCrossProduct(&axisY, &axisZ, &axisX);

        pDst->setXYZDir(axisX, axisY, axisZ);
        pDst->setTrans(rPos);
    }

    void makeMtxFrontUp(TPos3f* pDst, const TVec3f& rFront, const TVec3f& rUp) {
        TVec3f axisZ;
        MR::normalize(rFront, &axisZ);

        TVec3f axisX;
        PSVECCrossProduct(&rUp, &axisZ, &axisX);
        MR::normalize(&axisX);

        TVec3f axisY;
        PSVECCrossProduct(&axisZ, &axisX, &axisY);

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

        TVec3f axisY;
        MR::normalize(rUp, &axisY);

        TVec3f axisX;
        PSVECCrossProduct(&axisY, &support, &axisX);
        MR::normalize(&axisX);

        TVec3f axisZ;
        PSVECCrossProduct(&axisX, &axisY, &axisZ);

        pDst->setXYZDir(axisX, axisY, axisZ);
    }

#pragma dont_inline on
    void makeMtxUpNoSupportPos(TPos3f* pDst, const TVec3f& rUp, const TVec3f& rPos) {
        TVec3f support;
        if (MR::getMaxAbsElementIndex(rUp) == 2) {
            support.set<f32>(0.0f, 1.0f, 0.0f);
        } else {
            support.set<f32>(0.0f, 0.0f, 1.0f);
        }

        MR::makeMtxUpFrontPos(pDst, rUp, support, rPos);
    }
#pragma dont_inline reset

    void makeMtxFrontNoSupport(TPos3f* pDst, const TVec3f& rFront) {
        TVec3f support;
        if (MR::getMaxAbsElementIndex(rFront) == 1) {
            support.set(1.0f, 0.0f, 0.0f);
        } else {
            support.set(0.0f, 1.0f, 0.0f);
        }

        TVec3f axisZ;
        MR::normalize(rFront, &axisZ);

        TVec3f axisX;
        PSVECCrossProduct(&support, &axisZ, &axisX);
        MR::normalize(&axisX);

        TVec3f axisY;
        PSVECCrossProduct(&axisZ, &axisX, &axisY);

        pDst->setXYZDir(axisX, axisY, axisZ);
    }

#pragma dont_inline on
    void makeMtxFrontNoSupportPos(TPos3f* pDst, const TVec3f& rFront, const TVec3f& rPos) {
        TVec3f support;
        if (MR::getMaxAbsElementIndex(rFront) == 1) {
            support.set<f32>(1.0f, 0.0f, 0.0f);
        } else {
            support.set<f32>(0.0f, 1.0f, 0.0f);
        }

        MR::makeMtxFrontUpPos(pDst, rFront, support, rPos);
    }
#pragma dont_inline reset

    void orthogonalize(TPos3f* pMtx) {
        TVec3f axisX, axisY, axisZ;

        pMtx->getXYZDir(axisX, axisY, axisZ);

        PSVECCrossProduct(&axisY, &axisZ, &axisX);
        PSVECCrossProduct(&axisZ, &axisX, &axisY);

        PSVECMag(&axisX);
        PSVECNormalize(&axisX, &axisX);

        PSVECMag(&axisY);
        PSVECNormalize(&axisY, &axisY);

        pMtx->setXYZDir(axisX, axisY, axisZ);

        TVec3f zDir;
        zDir.set<f32>(pMtx->mMtx[0][2], pMtx->mMtx[1][2], pMtx->mMtx[2][2]);

        f32 magAll = pMtx->mMtx[1][0] * pMtx->mMtx[1][0] + pMtx->mMtx[0][0] * pMtx->mMtx[0][0] + pMtx->mMtx[2][0] * pMtx->mMtx[2][0] +
                     pMtx->mMtx[0][1] * pMtx->mMtx[0][1] + pMtx->mMtx[1][1] * pMtx->mMtx[1][1] + pMtx->mMtx[2][1] * pMtx->mMtx[2][1] +
                     pMtx->mMtx[0][2] * pMtx->mMtx[0][2] + pMtx->mMtx[1][2] * pMtx->mMtx[1][2] + pMtx->mMtx[2][2] * pMtx->mMtx[2][2];

        JGeometry::TUtil<f32>::sqrt(magAll);

        if (pMtx) {
            f32 magX = pMtx->mMtx[0][0] * pMtx->mMtx[0][0] + pMtx->mMtx[1][0] * pMtx->mMtx[1][0] + pMtx->mMtx[2][0] * pMtx->mMtx[2][0];
            f32 invSqrtX = JGeometry::TUtil<f32>::inv_sqrt(magX);
            pMtx->mMtx[0][0] = invSqrtX * pMtx->mMtx[0][0];
            pMtx->mMtx[1][0] = invSqrtX * pMtx->mMtx[1][0];
            pMtx->mMtx[2][0] = invSqrtX * pMtx->mMtx[2][0];

            f32 magY = pMtx->mMtx[0][1] * pMtx->mMtx[0][1] + pMtx->mMtx[1][1] * pMtx->mMtx[1][1] + pMtx->mMtx[2][1] * pMtx->mMtx[2][1];
            f32 invSqrtY = JGeometry::TUtil<f32>::inv_sqrt(magY);
            pMtx->mMtx[0][1] = invSqrtY * pMtx->mMtx[0][1];
            pMtx->mMtx[1][1] = invSqrtY * pMtx->mMtx[1][1];
            pMtx->mMtx[2][1] = invSqrtY * pMtx->mMtx[2][1];

            f32 magZ = pMtx->mMtx[0][2] * pMtx->mMtx[0][2] + pMtx->mMtx[1][2] * pMtx->mMtx[1][2] + pMtx->mMtx[2][2] * pMtx->mMtx[2][2];
            f32 invSqrtZ = JGeometry::TUtil<f32>::inv_sqrt(magZ);
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
        f32 axisX = rLocalAxis.x;
        f32 axisY = rLocalAxis.y;
        f32 axisZ = rLocalAxis.z;

        f32 worldX = axisX * pMtx->mMtx[0][0] + axisY * pMtx->mMtx[0][1] + axisZ * pMtx->mMtx[0][2];
        f32 worldY = axisX * pMtx->mMtx[1][0] + axisY * pMtx->mMtx[1][1] + axisZ * pMtx->mMtx[1][2];
        f32 worldZ = axisX * pMtx->mMtx[2][0] + axisY * pMtx->mMtx[2][1] + axisZ * pMtx->mMtx[2][2];

        TVec3f worldAxis;
        worldAxis.set<f32>(worldX, worldY, worldZ);

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

    void setMtxTrans(MtxPtr mtx, f32 x, f32 y, f32 z) {
        mtx[0][3] = x;
        mtx[1][3] = y;
        mtx[2][3] = z;
    }

    void addTransMtx(MtxPtr mtx, const TVec3f& rVec) {
        mtx[0][3] += rVec.x;
        mtx[1][3] += rVec.y;
        mtx[2][3] += rVec.z;
    }

    void addTransMtxLocal(MtxPtr mtx, const TVec3f& rVec) {
        addTransMtxLocalX(mtx, rVec.x);
        addTransMtxLocalY(mtx, rVec.y);
        addTransMtxLocalZ(mtx, rVec.z);
    }

    void addTransMtxLocalX(MtxPtr mtx, f32 x_coord) {
        mtx[0][3] = mtx[0][3] + (mtx[0][0] * x_coord);
        mtx[1][3] = mtx[1][3] + (mtx[1][0] * x_coord);
        mtx[2][3] = mtx[2][3] + (mtx[2][0] * x_coord);
    }

    void addTransMtxLocalY(MtxPtr mtx, f32 y_coord) {
        mtx[0][3] = mtx[0][3] + (mtx[0][1] * y_coord);
        mtx[1][3] = mtx[1][3] + (mtx[1][1] * y_coord);
        mtx[2][3] = mtx[2][3] + (mtx[2][1] * y_coord);
    }

    void addTransMtxLocalZ(MtxPtr mtx, f32 z_coord) {
        mtx[0][3] = mtx[0][3] + (mtx[0][2] * z_coord);
        mtx[1][3] = mtx[1][3] + (mtx[1][2] * z_coord);
        mtx[2][3] = mtx[2][3] + (mtx[2][2] * z_coord);
    }

    void extractMtxXDir(MtxPtr mtx, TVec3f* pOut) {
        pOut->x = mtx[0][0];
        pOut->y = mtx[1][0];
        pOut->z = mtx[2][0];
    }

    void extractMtxYDir(MtxPtr mtx, TVec3f* pOut) {
        pOut->x = mtx[0][1];
        pOut->y = mtx[1][1];
        pOut->z = mtx[2][1];
    }

    void extractMtxZDir(MtxPtr mtx, TVec3f* pOut) {
        pOut->x = mtx[0][2];
        pOut->y = mtx[1][2];
        pOut->z = mtx[2][2];
    }

    void extractMtxXYZDir(MtxPtr mtx, TVec3f* pOutX, TVec3f* pOutY, TVec3f* pOutZ) {
        MR::extractMtxXDir(mtx, pOutX);
        MR::extractMtxYDir(mtx, pOutY);
        MR::extractMtxZDir(mtx, pOutZ);
    }

    void extractMtxTrans(MtxPtr mtx, TVec3f* pOut) {
        pOut->x = mtx[0][3];
        pOut->y = mtx[1][3];
        pOut->z = mtx[2][3];
    }

    bool isSameMtx(MtxPtr a, MtxPtr b) {
        f32* pA = (f32*)a;
        f32* pB = (f32*)b;
        for (int i = 0; i < 12; i++) {
            if (*pA != *pB) {
                return false;
            }
            pA++;
            pB++;
        }
        return true;
    }

    bool isSameMtxRot(MtxPtr a, MtxPtr b) {
        f32* pA = (f32*)a;
        f32* pB = (f32*)b;
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

    bool isRotAxisY(MtxPtr a, MtxPtr b) {
        TVec3f yDirA, yDirB;
        ((TRot3f*)a)->getYDir(yDirA);
        ((TRot3f*)b)->getYDir(yDirB);
        bool result = false;

        if (JGeometry::TUtil<f32>::epsilonEquals(yDirA.x, yDirB.x, 0.001f) &&
            JGeometry::TUtil<f32>::epsilonEquals(yDirA.y, yDirB.y, 0.001f) &&
            JGeometry::TUtil<f32>::epsilonEquals(yDirA.z, yDirB.z, 0.001f)) {
            result = true;
        }
        return result;
    }

    void calcMtxRotAxis(TVec3f* pOut, MtxPtr a, MtxPtr b) {
        TVec3f localZ(0.0f, 0.0f, 1.0f);

        Mtx invA;
        PSMTXInverse(a, invA);

        TVec3f axisZA, axisZB;
        PSMTXMultVecSR(invA, &localZ, &axisZA);
        PSMTXMultVecSR(b, &localZ, &axisZB);

        TVec3f cross;
        PSVECCrossProduct(&axisZA, &axisZB, &cross);

        if (MR::normalizeOrZero(&cross)) {
            *pOut = localZ;
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
        tmpmtx_rx[1][2] = sinX;
        tmpmtx_rx[2][1] = -sinX;
        tmpmtx_rx[2][2] = cosX;
        return tmpmtx_rx;
    }

    MtxPtr tmpMtxRotYDeg(f32 deg) {
        f32 cosY = JMACosDegree(deg);
        f32 sinY = JMASinDegree(deg);
        tmpmtx_ry[0][0] = cosY;
        tmpmtx_ry[0][2] = -sinY;
        tmpmtx_ry[2][0] = sinY;
        tmpmtx_ry[2][2] = cosY;
        return tmpmtx_ry;
    }

    MtxPtr tmpMtxRotZDeg(f32 deg) {
        f32 cosZ = JMACosDegree(deg);
        f32 sinZ = JMASinDegree(deg);
        tmpmtx_rz[0][0] = cosZ;
        tmpmtx_rz[0][1] = sinZ;
        tmpmtx_rz[1][0] = -sinZ;
        tmpmtx_rz[1][1] = cosZ;
        return tmpmtx_rz;
    }

    void orderRotateMtx(s16 order, const TVec3f& rRad, MtxPtr dst) {
        const TVec3f* rad = &rRad;
        MtxPtr mtxX = tmpMtxRotXRad(rad->x);
        MtxPtr mtxY = tmpMtxRotYRad(rad->y);
        MtxPtr mtxZ = tmpMtxRotZRad(rad->z);

        MtxPtr first, second, third;

        switch (order) {
            case 0:
                first = mtxY;
                second = mtxX;
                third = mtxZ;
                break;
            case 1:
                first = mtxZ;
                second = mtxX;
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
                first = mtxX;
                second = mtxZ;
                third = mtxZ;
                break;
            case 5:
            default:
                first = mtxY;
                second = mtxZ;
                third = mtxX;
                break;
        }

        PSMTXConcat(first, second, dst);
        PSMTXConcat(third, dst, dst);
    }

    void rotAxisVecRad(const TVec3f& rAxis, const TVec3f& rVec, TVec3f* pOut, f32 rad) {
        Mtx rot;
        PSMTXRotAxisRad(rot, &rVec, rad);
        PSMTXMultVec(rot, &rAxis, pOut);
    }
};  // namespace MR
