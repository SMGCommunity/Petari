#include "Game/Map/OceanBowl.hpp"
#include "Game/Map/OceanBowlPoint.hpp"
#include "Game/Map/OceanBowlBloomDrawer.hpp"
#include "Game/Map/WaterAreaHolder.hpp"
#include "Game/Map/WaterInfo.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util.hpp"
#include <JSystem/JGeometry.hpp>
#include <JSystem/JGeometry/TMatrix.hpp>
#include <JSystem/JMath/JMath.hpp>
#include <JSystem/JMath/JMATrigonometric.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <math_types.hpp>
#include <revolution/gd/GDBase.h>
#include <revolution/gx.h>
#include <revolution/gx/GXVert.h>
#include <revolution/mtx.h>
#include <revolution/os/OSCache.h>
#include <cmath>

// These are from the debug symbol map
const f32 sPointInterval = 200.0f;
//const f32 sPointWaveRateDistMax = 2607.5945f; // I couldn't figure out what sPointWaveRateDistMax ended up being in the final game
const f32 sTexRate0 = 0.05f;
const f32 sTexRate1 = 0.05f;
const f32 sTexRate2 = 0.1f;
const f32 sWaveSpeed0 = -0.04f;
const f32 sWaveSpeed1 = -0.04f;
const f32 sTexSpeedU0 = -0.0008f;
const f32 sTexSpeedV0 = -0.0008f;
const f32 sTexSpeedU1 = -0.001f;
const f32 sTexSpeedV1 = 0.0008f;
const f32 sTexSpeedU2 = -0.003f;
const f32 sTexSpeedV2 = -0.001f;
const f32 sIndirectScale = 0.1f;
const f32 sClippingDistance = 1000.0f;
const f32 sPointIntervalHalf = sPointInterval / 2.0f;

static GXColor sOceanBowlTevReg0 = {0x28, 0x28, 0x28, 0x14};
static GXColor sOceanBowlTevReg1 = {0xC8, 0xE6, 0xD2, 0xFF};
static u8 sOceanBowlBloomTevReg0V = 0x5F;
static u8 sOceanBowlBloomTevReg1V = 0x32;

OceanBowl::OceanBowl(const char* pName) : LiveActor(pName),
    mSide(1.0f, 0.0f, 0.0f), mUp(0.0f, 1.0f, 0.0f), mFront(0.0f, 0.0f, 1.0f), mRadius(0.0f), mWaveX(0.0f), mWaveZ(0.0f),
    mTexU0(0.0f), mTexV0(0.0f), mTexU1(0.0f), mTexV1(0.0f), mTexU2(0.0f), mTexV2(0.0f), _A98(0.041666667f) {
    mVtxTex0Array = nullptr;
    mVtxTex1Array = nullptr;
    mVtxTex2Array = nullptr;
    mVtxTex3Array = nullptr;
    mWaterTex = nullptr;
    mWaterIndirectTex = nullptr;
    mMaskTex = nullptr;
    mBloomDrawer = nullptr;
    mIsClipped = false;

    for (s32 x = 0; x < 25; x++) {
        for (s32 y = 0; y < 25; y++) {
            setPoint(x, y, nullptr);
        }
    }
}

void OceanBowl::init(const JMapInfoIter& rIter) {
    MR::connectToScene(this, MR::MovementType_MapObj, -1, -1, MR::DrawType_OceanBowl);
    MR::initDefaultPos(this, rIter);
    MR::calcActorAxis(&mSide, &mUp, &mFront, this);
    mRadius = 100.0f * mScale.x;
    initPoints();
    WaterAreaFunction::entryOceanBowl(this);
    mWaterTex = new JUTTexture(MR::loadTexFromArc("WaterWave.arc", "Water.bti"), 0);
    mWaterIndirectTex = new JUTTexture(MR::loadTexFromArc("WaterWave.arc", "WaterIndirect.bti"), 0);
    mMaskTex = new JUTTexture(MR::loadTexFromArc("WaterWave.arc", "Mask.bti"), 0);
    mBloomDrawer = new OceanBowlBloomDrawer(this);
    mBloomDrawer->initWithoutIter();
    MR::invalidateClipping(this);
    makeActorAppeared();
}

bool OceanBowl::isInWater(const TVec3f& rPos) const {
    if (PSVECDistance(&rPos, &mPosition) > mRadius) {
        return false;
    }

    return !(mUp.dot(rPos.subOperatorInLine(mPosition)) > 0.0f);
}

bool OceanBowl::calcWaterInfo(const TVec3f& rPos, const TVec3f& rGravity, WaterInfo* pInfo) const {
    const TVec3f* position = &mPosition;
    f32 B0 = mRadius;
    TVec3f v(rPos);
    v.sub(*position);
    f32 fa = MR::vecKillElement(v, mUp, &v);
    f32 fb = (PSVECMag(&v) / B0) * PI * 0.5f;

    pInfo->_4 = (JMath::sSinCosTable.cosLapRad(fb) * B0);
    pInfo->_4 += fa;

    pInfo->mCamWaterDepth = -fa;
    pInfo->mWaveHeight = OceanBowlPoint::calcHeightStatic(mWaveX, mWaveZ, rPos.x, rPos.z);

    TVec3f v2(rPos), v3;
    v2.sub(*position);
    MR::normalizeOrZero(&v2);

    pInfo->mSurfaceNormal.set(mUp);

    MR::vecKillElement(rPos.subOperatorInLine(*position), mUp, &v3);

    pInfo->mSurfacePos.set(position->addOperatorInLine(v3));

    TVec3f v5(rPos);
    v5.sub(mPosition);

    if (v5.y > 0.0f)
        v5.y = 0.0f;

    if (MR::isNearZero(v5)) {
        pInfo->mEdgeDistance = B0;
        TVec3f v6(rGravity);
        v6.scale(B0);
        v6.add(mPosition);
        pInfo->mEdgePos.set(v6);
    } else {
        MR::normalize(&v5);
        v5.scale(B0);
        v5.add(mPosition);
        pInfo->mEdgeDistance = PSVECDistance(&v5, &rPos);
        pInfo->mEdgePos.set(v5);
    }

    return true;
}

void OceanBowl::movement() {
    const TVec3f* position = &getPoint(12, 12)->_C;
    f32 distToPlayer = MR::calcDistanceToPlayer(mPosition);

    if ((MR::isCameraInWater() && WaterAreaFunction::getCameraWaterInfo()->mOceanBowl != this) || distToPlayer > sClippingDistance + mRadius) {
        mIsClipped = true;
        return;
    }

    mIsClipped = false;
    TVec3f playerpos(*MR::getPlayerPos());
    playerpos.sub(*position);
    f32 dotA = mSide.dot(playerpos);
    f32 dotB = mFront.dot(playerpos);

    if (dotA < -sPointIntervalHalf && dotA > -mRadius) {
        moveToLeft();
    } else if (dotA > sPointIntervalHalf && dotA < mRadius) {
        moveToRight();
    } else if (dotB < -sPointIntervalHalf && dotB > -mRadius) {
        moveToUpper();
    } else if (dotB > sPointIntervalHalf && dotB < mRadius) {
        moveToLower();
    }

    mWaveX += sWaveSpeed0;
    mWaveZ += sWaveSpeed1;
    updatePoints();

    mTexU0 = MR::repeat(mTexU0 + sTexSpeedU0, 0.0f, 1.0f);
    mTexV0 = MR::repeat(mTexV0 + sTexSpeedV0, 0.0f, 1.0f);
    mTexU1 = MR::repeat(mTexU1 + sTexSpeedU1, 0.0f, 1.0f);
    mTexV1 = MR::repeat(mTexV1 + sTexSpeedV1, 0.0f, 1.0f);
    mTexU2 = MR::repeat(mTexU2 + sTexSpeedU2, 0.0f, 1.0f);
    mTexV2 = MR::repeat(mTexV2 + sTexSpeedV2, 0.0f, 1.0f);
}

void OceanBowl::initPoints() {
    for (s32 x = 0; x < 25; x++) {
        for (s32 y = 0; y < 25; y++) {
            TVec3f vecA(mSide);
            TVec3f vecB(mFront);
            vecA.scale(((static_cast<f32>(y) * sPointInterval) - 2500.0f) + sPointIntervalHalf);
            vecB.scale(((static_cast<f32>(x) * sPointInterval) - 2500.0f) + sPointIntervalHalf);
            TVec3f finalpos(mPosition);
            finalpos.add(vecA);
            finalpos.add(vecB);

            setPoint(x, y, new OceanBowlPoint(finalpos));
        }
    }

    mVtxTex0Array = new (0x20) f32[24 * 25 * 4];
    mVtxTex1Array = new (0x20) f32[24 * 25 * 4];
    mVtxTex2Array = new (0x20) f32[24 * 25 * 4];
    mVtxTex3Array = new (0x20) f32[24 * 25 * 4];

    s32 index = 0;

    f32 f5 = sTexRate0;
    f32 f6 = 0.0f;

    f32 f7 = sTexRate1;
    f32 f8 = 0.0f;

    f32 f9 = sTexRate2;
    f32 f10 = 0.0f;

    f32 f11 = 0.0f;
    f32 f12 = _A98;

    for (s32 x = 0; x < 24; x++) {
        f32 r13 = 0.0f;
        f32 r28 = 0.0f;
        f32 r29 = 0.0f;
        f32 r30 = 0.0f;
        for (s32 y = 0; y < 25; y++) {
            mVtxTex0Array[index + 0] = f5;
            mVtxTex0Array[index + 1] = r13;
            mVtxTex0Array[index + 2] = f6;
            mVtxTex0Array[index + 3] = r13;
            r13 += sTexRate0;

            mVtxTex1Array[index + 0] = f7;
            mVtxTex1Array[index + 1] = r28;
            mVtxTex1Array[index + 2] = f8;
            mVtxTex1Array[index + 3] = r28;
            r28 += sTexRate1;

            mVtxTex2Array[index + 0] = f9;
            mVtxTex2Array[index + 1] = r29;
            mVtxTex2Array[index + 2] = f10;
            mVtxTex2Array[index + 3] = r29;
            r29 += sTexRate2;

            mVtxTex3Array[index + 0] = f12;
            mVtxTex3Array[index + 1] = r30;
            mVtxTex3Array[index + 2] = f11;
            mVtxTex3Array[index + 3] = r30;
            r30 += _A98;

            index += 4;
        }
        f6 = f5;
        f8 = f7;
        f10 = f9;
        f11 = f12;
        f5 += sTexRate0;
        f7 += sTexRate1;
        f9 += sTexRate2;
        f12 += _A98;
    }
}

void OceanBowl::updatePoints() {
    for (s32 x = 0; x < 25; x++) {
        for (s32 y = 0; y < 25; y++) {
            getPoint(x, y)->updatePos(mWaveX, mWaveZ);
        }
    }
}

void OceanBowl::moveToLeft() {
    const TVec3f* position = &mPosition;
    OceanBowlPoint* mLastPoint;

    for (s32 x = 0; x < 25; x++) {
        mLastPoint = getPoint(x, 24);

        for (s32 y = 24; y > 0; y--) {
            setPoint(x, y, getPoint(x, y - 1));
        }
        setPoint(x, 0, mLastPoint);

        TVec3f resetvec(mSide);
        resetvec.scale(-200.0f);
        resetvec.add(getPoint(x, 1)->_C);
        mLastPoint->reset(resetvec, MR::clamp((mRadius - PSVECDistance(position, &resetvec)) / 500.0f, 0.0f, 1.0f));
    }
    mTexV0 -= sTexRate0;
    mTexV1 -= sTexRate1;
    mTexV2 -= sTexRate2;
}

void OceanBowl::moveToRight() {
    const TVec3f* position = &mPosition;
    OceanBowlPoint* mLastPoint;

    for (s32 x = 0; x < 25; x++) {
        mLastPoint = getPoint(x, 0);

        for (s32 y = 0; y < 24; y++) {
            setPoint(x, y, getPoint(x, y + 1));
        }
        setPoint(x, 24, mLastPoint);

        TVec3f resetvec(mSide);
        resetvec.scale(sPointInterval);
        resetvec.add(getPoint(x, 23)->_C);
        mLastPoint->reset(resetvec, MR::clamp((mRadius - PSVECDistance(position, &resetvec)) / 500.0f, 0.0f, 1.0f));
    }
    mTexV0 += sTexRate0;
    mTexV1 += sTexRate1;
    mTexV2 += sTexRate2;
}

void OceanBowl::moveToUpper() {
    const TVec3f* position = &mPosition;
    OceanBowlPoint* mLastPoint;

    for (s32 y = 0; y < 25; y++) {
        mLastPoint = getPoint(24, y);

        for (s32 x = 24; x > 0; x--) {
            setPoint(x, y, getPoint(x - 1, y));
        }
        setPoint(0, y, mLastPoint);

        TVec3f resetvec(mFront);
        resetvec.scale(-sPointInterval);
        resetvec.add(getPoint(1, y)->_C);
        mLastPoint->reset(resetvec, MR::clamp((mRadius - PSVECDistance(position, &resetvec)) / 500.0f, 0.0f, 1.0f));
    }
    mTexU0 -= sTexRate0;
    mTexU1 -= sTexRate1;
    mTexU2 -= sTexRate2;
}

void OceanBowl::moveToLower() {
    const TVec3f* position = &mPosition;
    OceanBowlPoint* mLastPoint;

    for (s32 y = 0; y < 25; y++) {
        mLastPoint = getPoint(0, y);

        for (s32 x = 0; x < 24; x++) {
            setPoint(x, y, getPoint(x + 1, y));
        }
        setPoint(24, y, mLastPoint);

        TVec3f resetvec(mFront);
        resetvec.scale(sPointInterval);
        resetvec.add(getPoint(23, y)->_C);
        mLastPoint->reset(resetvec, MR::clamp((mRadius - PSVECDistance(position, &resetvec)) / 500.0f, 0.0f, 1.0f));
    }
    mTexU0 += sTexRate0;
    mTexU1 += sTexRate1;
    mTexU2 += sTexRate2;
}

void OceanBowl::draw() const {
    if (mIsClipped || !MR::isValidDraw(this))
        return;

    loadMaterial();

    OceanBowlPoint* pPoint2;
    u16 zero = 0;
    u16 one = 1;
    for (s32 x = 0; x < 24; x++) {
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 0x32);
        for (s32 y = 0; y < 25; y++) {
            pPoint2 = getPoint(x, y);
            GXPosition3f32(getPoint(x + 1, y)->_0.x, getPoint(x + 1, y)->_0.y, getPoint(x + 1, y)->_0.z);
            GXColor4u8(0xFF, 0xFF, 0xFF, getPoint(x + 1, y)->_1C);
            GXTexCoord2s16(zero, zero);
            GXTexCoord2s16(zero, zero);

            GXPosition3f32(pPoint2->_0.x, pPoint2->_0.y, pPoint2->_0.z);
            GXColor4u8(0xFF, 0xFF, 0xFF, pPoint2->_1C);
            GXTexCoord2s16(one, one);
            GXTexCoord2s16(one, one);

            zero += 2;
            one += 2;
        }
    }
}

void OceanBowl::loadMaterial() const {
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_TEX_ST, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX2, GX_TEX_ST, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX3, GX_TEX_ST, GX_F32, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
    GXSetVtxDesc(GX_VA_TEX1, GX_INDEX16);
    GXSetVtxDesc(GX_VA_TEX2, GX_INDEX16);
    GXSetVtxDesc(GX_VA_TEX3, GX_INDEX16);
    GXSetArray(GX_VA_TEX0, mVtxTex0Array, 8);
    GXSetArray(GX_VA_TEX1, mVtxTex1Array, 8);
    GXSetArray(GX_VA_TEX2, mVtxTex2Array, 8);
    GXSetArray(GX_VA_TEX3, mVtxTex3Array, 8);
    GXLoadPosMtxImm(MR::getCameraViewMtx(), 0);
    GXSetCurrentMtx(0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(5);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX2, GX_TEXMTX2, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD3, GX_TG_MTX3x4, GX_TG_POS, GX_TEXMTX3, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD4, GX_TG_MTX3x4, GX_TG_TEX3, GX_TEXMTX4, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD4, GX_TG_MTX2x4, GX_TG_TEX3, GX_TEXMTX4, GX_TRUE, GX_PTIDENTITY);

    TMtx34f mat;
    mat.identity();
    mat[0][2] = mTexU0;
    mat[1][2] = mTexV0;
    GXLoadTexMtxImm(mat, GX_TEXMTX0, GX_MTX2x4);
    mat[0][2] = mTexU1;
    mat[1][2] = mTexV1;
    GXLoadTexMtxImm(mat, GX_TEXMTX1, GX_MTX2x4);
    mat[0][2] = mTexU2;
    mat[1][2] = mTexV2;
    GXLoadTexMtxImm(mat, GX_TEXMTX2, GX_MTX2x4);
    f32 a = (getPoint(0, 0)->_0.x - MR::getPlayerPos()->x) + 2400.0f;
    f32 b = (getPoint(0, 0)->_0.z - MR::getPlayerPos()->z) + 2400.0f;
    mat[0][2] = b / 4800.0f;
    mat[1][2] = a / 4800.0f;
    GXLoadTexMtxImm(mat, GX_TEXMTX4, GX_MTX2x4);
    MR::loadTexProjectionMtx(0x27);
    mWaterTex->load(GX_TEXMAP0);
    JUTTexture screentex(MR::getScreenResTIMG(), 0);
    screentex.load(GX_TEXMAP1);
    mWaterIndirectTex->load(GX_TEXMAP2);
    mMaskTex->load(GX_TEXMAP3);
    MR::resetTextureCacheSize();
    if (MR::isCameraInWater()) {
        GXSetNumIndStages(0);
    } else {
        GXSetNumIndStages(1);
        GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD2, GX_TEXMAP2);
        GXSetTevIndWarp(GX_TEVSTAGE3, GX_INDTEXSTAGE0, 1, 0, GX_ITM_0);
        f32 indmtx[2][3];
        indmtx[0][0] = sIndirectScale;
        indmtx[0][1] = 0.0f;
        indmtx[0][2] = 0.0f;
        indmtx[1][0] = 0.0f;
        indmtx[1][1] = sIndirectScale;
        indmtx[1][2] = 0.0f;

        GXSetIndTexMtx(GX_ITM_0, indmtx, 0);
    }

    if (MR::isCameraInWater()) {
        GXSetNumTevStages(3);
    } else {
        GXSetNumTevStages(4);
    }

    GXSetTevColor(GX_TEVREG0, sOceanBowlTevReg0);
    GXSetTevColor(GX_TEVREG1, sOceanBowlTevReg1);

    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);

    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_TEXC, GX_CC_CPREV, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_DIVIDE_2, GX_FALSE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_TEXA, GX_CA_APREV, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, GX_FALSE, GX_TEVPREV);

    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD4, GX_TEXMAP3, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_CPREV, GX_CC_A0, GX_CC_C0, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_COMP_R8_EQ, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);

    if (!MR::isCameraInWater()) {
        GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD3, GX_TEXMAP1, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE3, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C1, GX_CC_CPREV);
        GXSetTevColorOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE3, GX_CA_APREV, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    }

    if (MR::isCameraInWater()) {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
    } else {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    }
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetZCompLoc(GX_TRUE);
    GXSetCullMode(GX_CULL_NONE);
    GXSetClipMode(GX_CLIP_ENABLE);
}

void OceanBowl::loadMaterialBloom() const {
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_TEX_ST, GX_F32, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
    GXSetVtxDesc(GX_VA_TEX1, GX_INDEX16);
    GXSetArray(GX_VA_TEX0, mVtxTex0Array, 8);
    GXSetArray(GX_VA_TEX1, mVtxTex1Array, 8);
    GXLoadPosMtxImm(MR::getCameraViewMtx(), 0);
    GXSetCurrentMtx(0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);

    TMtx34f mat;
    mat.identity();
    mat[0][2] = mTexU0;
    mat[1][2] = mTexV0;
    GXLoadTexMtxImm(mat, GX_TEXMTX0, GX_MTX2x4);
    mat[0][2] = mTexU1;
    mat[1][2] = mTexV1;
    GXLoadTexMtxImm(mat, GX_TEXMTX1, GX_MTX2x4);
    mWaterTex->load(GX_TEXMAP0);
    GXSetNumIndStages(0);
    GXSetNumTevStages(2);
    Color8 c0 = Color8(sOceanBowlBloomTevReg0V, sOceanBowlBloomTevReg0V, sOceanBowlBloomTevReg0V, 0xFF);
    Color8 c1 = Color8(sOceanBowlBloomTevReg1V, sOceanBowlBloomTevReg1V, sOceanBowlBloomTevReg1V, 0xFF);
    GXSetTevColor(GX_TEVREG0, c0);
    GXSetTevColor(GX_TEVREG1, c1);

    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C0, GX_CC_C1);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);

    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C0, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_APREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);

    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetZCompLoc(GX_TRUE);
    GXSetCullMode(GX_CULL_NONE);
    GXSetClipMode(GX_CLIP_ENABLE);
}