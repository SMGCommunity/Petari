#include "Game/Map/OceanRingDrawer.hpp"
#include "Game/Map/OceanRing.hpp"
#include "Game/Map/WaterAreaHolder.hpp"
#include <revolution/gd/GDBase.h>

namespace {
    const f32 sPointNumInPart = 0.0f;
    const f32 sDistancePartDL = 13000.0f;
    const f32 sFlowSpeedTexRate = 0.0f;
    const f32 sFlowSpeedTexRateMin = 0.0f;
    const f32 sTexRate0 = 0.0f;
    const f32 sTexRate1 = 0.0f;
    const f32 sTexRate2 = 0.0f;
    const f32 sTexSpeed0U = -0.003f;
    const f32 sTexSpeed0V = -0.001f;
    const f32 sTexSpeed1U = -0.002f;
    const f32 sTexSpeed1V = 0.001f;
    const f32 sTexSpeed2V = 0.003f;
    const f32 sIndirectScale = 0.1f;
    const f32 sBloomCameraOffsetZ = 3000.0f;
    const f32 sDistancePartDrawBloom = 4000.0f;
    const f32 sBloomCameraUpMin = 0.0f;
    const f32 sBloomCameraUpMax = 0.0f;
    const f32 sBloomCameraDepthMin = 0.0f;
    const f32 sBloomCameraDepthMax = 0.0f;


    static GXColor color1 = {0x28, 0x28, 0x28, 0x14};
    static GXColor color2 = {0x76, 0xD7, 0xFF, 0xFF};
    static u8 unknownVal = 1;
}


OceanRingPartDrawer::OceanRingPartDrawer(const OceanRing* pRing, int a3, int a4, bool a5, f32* a6, f32* a7, f32* a8) : mPosition(0.0f, 0.0f, 0.0f) {
    mOceanRing = pRing;
    _10 = a3;
    _14 = a4;
    _18 = a5;
    _1C = *a6;
    _20 = *a7;
    _24 = *a8;
    mDispListLength = 0;
    mDispList = nullptr;

    initDisplayList(a6, a7, a8);

    for (int i = 0; i < a4; i++) {
        int v12 = a3 + i;

        if (i == a4 - 1 && _18) {
            v12 = 0;
        }

        mPosition.add(((mOceanRing->mWaterPoints[(v12 * mOceanRing->mStride - 1) + mOceanRing->mStride / 2])->mOrigPos));
    }

    mPosition.scale(1.0f / a4);
}

void OceanRingPartDrawer::initDisplayList(f32* a1, f32* a2, f32* a3) {
    MR::ProhibitSchedulerAndInterrupts prohibit(false);

    u32 x = _14 * 0x50;
    u32 size = (((x * mOceanRing->mStride + 3) >> 5) + 2) << 5;
    mDispList = new (0x20) u8[size];
    DCInvalidateRange(mDispList, size);
    GDLObj obj;
    GDInitGDLObj(&obj, mDispList, size);
    __GDCurrentDL = &obj;
    drawGD(a1, a2, a3);
    GDPadCurr32();
    mDispListLength = obj.ptr - obj.start;
    DCStoreRange(mDispList, size);
}

void OceanRingPartDrawer::draw() const {
    if (PSVECDistance(&mPosition, MR::getPlayerPos()) < sDistancePartDL) {
        drawDynamic();
    } else {
        GXCallDisplayList(mDispList, mDispListLength);
    }
}

void OceanRingPartDrawer::drawGD(f32* a1, f32* a2, f32* a3) const {
    f32 f30 = 0.05f;
    f32 f29;
    f32 f28;
    f32 f27;
    f32 f26;
    f32 f25;
    f32 f24;
    f32 f23;
    f32 f22;
    f32 f21 = 1.0f;

    f28 = f30 * f21;
    f29 = 0.1f * f21;
    f27 = _1C;
    f25 = _20;
    f23 = _24;
    f26 = _1C;
    f24 = _20;
    f22 = _24;
    for (s32 i = 0; i < _14 - 1; i++) {
        s32 index = _10 + i;
        s32 indexPlusOne = index + 1;
        if (i == _14 - 2 && _18) {
            indexPlusOne = 0;
        }

        mOceanRing->getPoint(0, index);
        mOceanRing->getPoint(0, indexPlusOne);

        f26 += f28;
        f24 += f28;
        f22 += f29;
        f32 f20 = 0.0f;
        f32 f19 = 0.0f;
        f32 f18 = 0.0f;
        f32 f17 = 0.0f;
        f32 f16 = 0.0f;
        f32 f15 = 0.0f;
        GDBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, (u16)mOceanRing->mStride << 1);

        for (s32 j = 0; j < mOceanRing->mStride; j++) {
            WaterPoint* pPoint = mOceanRing->getPoint(j, index);
            GDPosition3f32(pPoint->mPosition.x, pPoint->mPosition.y, pPoint->mPosition.z);
            GDColor4u8(0xFF, 0xFF, 0xFF, pPoint->mAlpha);
            GDWrite_f32(f27);
            GDWrite_f32(f17);
            GDWrite_f32(f25);
            GDWrite_f32(f16);
            GDWrite_f32(f23);
            GDWrite_f32(f15);

            pPoint = mOceanRing->getPoint(j, indexPlusOne);
            GDPosition3f32(pPoint->mPosition.x, pPoint->mPosition.y, pPoint->mPosition.z);
            GDColor4u8(0xFF, 0xFF, 0xFF, pPoint->mAlpha);
            GDWrite_f32(f26);
            GDWrite_f32(f20);
            GDWrite_f32(f24);
            GDWrite_f32(f19);
            GDWrite_f32(f22);
            GDWrite_f32(f18);

            f17 += f28;
            f16 += f28;
            f15 += f29;
            f20 += f28;
            f19 += f28;
            f18 += f29;
        }

        f27 = f26;
        f25 = f24;
        f23 = f22;
        if (i != _14 - 1) {
            *a1 += 0.05f * f21;
            *a2 += 0.05f * f21;
            *a3 += 0.1f * f21;
        }
    }
}

void OceanRingPartDrawer::drawDynamic() const {
    f32 f30 = 0.05f;
    f32 f29;
    f32 f28;
    f32 f27;
    f32 f26;
    f32 f25;
    f32 f24;
    f32 f23;
    f32 f22;
    f32 f21 = 1.0f;

    f28 = f30 * f21;
    f29 = 0.1f * f21;
    f27 = _1C;
    f25 = _20;
    f23 = _24;
    f26 = _1C;
    f24 = _20;
    f22 = _24;
    for (s32 i = 0; i < _14 - 1; i++) {
        s32 index = _10 + i;
        s32 indexPlusOne = index + 1;
        if (i == _14 - 2 && _18) {
            indexPlusOne = 0;
        }

        mOceanRing->getPoint(0, index);
        mOceanRing->getPoint(0, indexPlusOne);

        f26 += f28;
        f24 += f28;
        f22 += f29;
        f32 f20 = 0.0f;
        f32 f19 = 0.0f;
        f32 f18 = 0.0f;
        f32 f17 = 0.0f;
        f32 f16 = 0.0f;
        f32 f15 = 0.0f;
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, (u16)mOceanRing->mStride << 1);

        for (s32 j = 0; j < mOceanRing->mStride; j++) {
            WaterPoint* pPoint = mOceanRing->getPoint(j, index);
            GXPosition3f32(pPoint->mPosition.x, pPoint->mPosition.y, pPoint->mPosition.z);
            GXColor4u8(0xFF, 0xFF, 0xFF, pPoint->mAlpha);
            GXTexCoord2f32(f27, f17);
            GXTexCoord2f32(f25, f16);
            GXTexCoord2f32(f23, f15);

            pPoint = mOceanRing->getPoint(j, indexPlusOne);
            GXPosition3f32(pPoint->mPosition.x, pPoint->mPosition.y, pPoint->mPosition.z);
            GXColor4u8(0xFF, 0xFF, 0xFF, pPoint->mAlpha);
            GXTexCoord2f32(f26, f20);
            GXTexCoord2f32(f24, f19);
            GXTexCoord2f32(f22, f18);

            f17 += f28;
            f16 += f28;
            f15 += f29;
            f20 += f28;
            f19 += f28;
            f18 += f29;
        }

        f27 = f26;
        f25 = f24;
        f23 = f22;
    }
}

void OceanRingPartDrawer::drawDynamicBloom() const {
    f32 f30 = 0.05f;
    f32 f29;
    f32 f28;
    f32 f27;
    f32 f26;
    f32 f25;
    f32 f24;
    f32 f21 = 1.0f;

    f28 = f30 * f21;
    f29 = 0.1f * f21;
    f27 = _1C;
    f25 = _20;
    f26 = _1C;
    f24 = _20;
    for (s32 i = 0; i < _14 - 1; i++) {
        s32 index = _10 + i;
        s32 indexPlusOne = index + 1;
        if (i == _14 - 2 && _18) {
            indexPlusOne = 0;
        }

        mOceanRing->getPoint(0, index);
        mOceanRing->getPoint(0, indexPlusOne);

        f26 += f28;
        f24 += f28;
        f32 f20 = 0.0f;
        f32 f19 = 0.0f;
        f32 f17 = 0.0f;
        f32 f16 = 0.0f;
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, (u16)mOceanRing->mStride << 1);

        for (s32 j = 0; j < mOceanRing->mStride; j++) {
            WaterPoint* pPoint = mOceanRing->getPoint(j, index);
            WaterPoint* pPoint2 = mOceanRing->getPoint(j, indexPlusOne);
            GXPosition3f32(pPoint->mPosition.x, pPoint->mPosition.y, pPoint->mPosition.z);
            GXTexCoord2f32(f27, f17);
            GXTexCoord2f32(f25, f16);

            GXPosition3f32(pPoint2->mPosition.x, pPoint2->mPosition.y, pPoint2->mPosition.z);
            GXTexCoord2f32(f26, f20);
            GXTexCoord2f32(f24, f19);

            f17 += f28;
            f16 += f28;
            f20 += f28;
            f19 += f28;
        }

        f27 = f26;
        f25 = f24;
    }
}


OceanRingDrawer::OceanRingDrawer(const OceanRing* pOceanRing) {
    mRing = pOceanRing;
    mDrawerCount = 0;
    mPartDrawers = nullptr;
    _C = 0.0f;
    _10 = 0.0f;
    _14 = 0.0f;
    _18 = 0.0f;
    _1C = 0.0f;
    _20 = 0.0f;
    mWaterTex = nullptr;
    mWaterIndTex = nullptr;
    _2C = 0;
    mDispList = nullptr;

    mWaterTex = new JUTTexture(MR::loadTexFromArc("WaterWave.arc", "Water.bti"), 0);
    mWaterIndTex = new JUTTexture(MR::loadTexFromArc("WaterWave.arc", "WaterIndirect.bti"), 0);
    initParts();
    initDisplayList();
}

void OceanRingDrawer::update() {
    _C  = MR::repeat(_C  + sTexSpeed0U, 0.0f, 1.0f);
    _10 = MR::repeat(_10 + sTexSpeed0V, 0.0f, 1.0f);
    _14 = MR::repeat(_14 + sTexSpeed1U, 0.0f, 1.0f);
    _18 = MR::repeat(_18 + sTexSpeed1V, 0.0f, 1.0f);
    _1C = MR::repeat(_1C + 0.0f, 0.0f, 1.0f);
    _20 = MR::repeat(_20 + sTexSpeed2V, 0.0f, 1.0f);
}

void OceanRingDrawer::draw() const {
    loadMaterial();

    if (mRing->mIsClipped) {
        GXCallDisplayList(mDispList, _2C);
    } else {
        for (s32 i = 0; i < mDrawerCount; i++) {
            getDrawer(i)->draw();
        }
    }
}

void OceanRingDrawer::drawBloom() const {
    loadMaterialBloom();
    TVec3f zDir = MR::getCamZdir();
    zDir.scale(sBloomCameraOffsetZ);
    TVec3f camPos = MR::getCamPos();
    zDir.add(camPos);

    if (!mRing->mIsClipped) {
        for (s32 i = 0; i < mDrawerCount; i++) {
            OceanRingPartDrawer* drwr = getDrawer(i);

            if (PSVECDistance(&drwr->mPosition, &zDir) < sDistancePartDrawBloom) {
                drwr->drawDynamicBloom();
            }
        }
    }
}

void OceanRingDrawer::initParts() {
    mDrawerCount = (mRing->mSegCount / 10) + 1;
    s32 count2 = mRing->mSegCount / mDrawerCount;
    f32 a = 0.0f, b = 0.0f, c = 0.0f;
    mPartDrawers = new OceanRingPartDrawer*[mDrawerCount];
    for (s32 i = 0; i < mDrawerCount - 1; i++) {
        mPartDrawers[i] = new OceanRingPartDrawer(mRing, count2 * i, count2 + 1, false, &a, &b, &c);
    }
    s32 v;
    bool flag;
    s32 w;

    s32 last = mDrawerCount - 1;
    w = count2 * last;
    v = mRing->mSegCount - w;
    flag = false;

    if (MR::isLoopRail(mRing)) {
        flag = true;
        v += 1;
    }
    mPartDrawers[last] = new OceanRingPartDrawer(mRing, w, v, flag, &a, &b, &c);
}

void OceanRingDrawer::initDisplayList() {
    MR::ProhibitSchedulerAndInterrupts prohibit(false);

    s32 flag = 0;

    if (MR::isLoopRail(mRing)) {
        flag = 1;
    }

    u32 x = 0x50 * (flag + mRing->mSegCount);
    u32 length = (((x + 3) >> 5) + 2) << 5;
    mDispList = new (0x20) u8[length];
    DCInvalidateRange(mDispList, length);
    GDLObj obj;
    GDInitGDLObj(&obj, mDispList, length);
    __GDCurrentDL = &obj;
    drawGD();
    GDPadCurr32();
    _2C = obj.ptr - obj.start;
    DCStoreRange(mDispList, length);
}

void OceanRingDrawer::drawGD() const {
    s32 flag = 0;

    if (MR::isLoopRail(mRing)) {
        flag = 1;
    }

    f32 scalex;
    f32 U0 = 0.0f;
    f32 U1 = 0.0f;
    f32 U2 = 0.0f;
    f32 V01 = 0.0f;
    f32 V11 = 0.0f;
    f32 V21 = 0.0f;
    s32 m = mRing->mStride;
    m -= 1;
    f32 V00 = 0.05f * m;
    f32 V10 = 0.05f * m;
    f32 V20 = 0.1f * m;

    GDBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, (u16)(flag + mRing->mSegCount) << 1);

    scalex = 1.0f;
    f32 V0 = V00 * scalex;
    f32 V1 = V10 * scalex;
    f32 V2 = V20 * scalex;

    WaterPoint* pPoint;
    WaterPoint* pPoint2;
    for (u32 i = 0; i < flag + mRing->mSegCount; i++) {
        s32 index = i;
        if (index == mRing->mSegCount) {
            index = 0;
        }

        s32 ddd = index * mRing->mStride;
        pPoint = mRing->mWaterPoints[mRing->mStride + ddd - 1];
        pPoint2 = mRing->mWaterPoints[ddd];
        GDPosition3f32(pPoint->mOrigPos.x, pPoint->mOrigPos.y, pPoint->mOrigPos.z);
        GDColor4u8(0xFF, 0xFF, 0xFF, pPoint->mAlpha);
        GDWrite_f32(U0);
        GDWrite_f32(V0);
        GDWrite_f32(U1);
        GDWrite_f32(V1);
        GDWrite_f32(U2);
        GDWrite_f32(V2);

        GDPosition3f32(pPoint2->mOrigPos.x, pPoint2->mOrigPos.y, pPoint2->mOrigPos.z);
        GDColor4u8(0xFF, 0xFF, 0xFF, pPoint2->mAlpha);
        GDWrite_f32(U0);
        GDWrite_f32(V01);
        GDWrite_f32(U1);
        GDWrite_f32(V11);
        GDWrite_f32(U2);
        GDWrite_f32(V21);

        U0 += (0.05f * scalex);
        U1 += (0.05f * scalex);
        U2 += (0.1f * scalex);
    }
}

void OceanRingDrawer::loadMaterial() const {
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_POS_XYZ, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX2, GX_POS_XYZ, GX_F32, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX2, GX_DIRECT);
    GXLoadPosMtxImm(MR::getCameraViewMtx(), 0);
    GXSetCurrentMtx(0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(4);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, 0x21, 0, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX2, 0x24, 0, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD3, GX_TG_MTX3x4, GX_TG_POS, 0x27, 0, 0x7D);

    TMtx34f mtx;
    mtx.identity();
    mtx.mMtx[0][2] = _C;
    mtx.mMtx[1][2] = _10;
    GXLoadTexMtxImm(mtx.toMtxPtr(), 0x1E, GX_MTX2x4);
    mtx.mMtx[0][2] = _14;
    mtx.mMtx[1][2] = _18;
    GXLoadTexMtxImm(mtx.toMtxPtr(), 0x21, GX_MTX2x4);
    mtx.mMtx[0][2] = _C;
    mtx.mMtx[1][2] = _10;
    GXLoadTexMtxImm(mtx.toMtxPtr(), 0x24, GX_MTX2x4);
    MR::loadTexProjectionMtx(39);

    mWaterTex->load(GX_TEXMAP0);
    JUTTexture tex(MR::getScreenResTIMG(), 0);
    tex.load(GX_TEXMAP1);
    mWaterIndTex->load(GX_TEXMAP2);

    if (unknownVal != 0 && MR::isCameraInWater()) {
        GXSetNumIndStages(0);
        GXSetTevDirect(GX_TEVSTAGE0);
        GXSetTevDirect(GX_TEVSTAGE1);
        GXSetTevDirect(GX_TEVSTAGE2);
        GXSetTevDirect(GX_TEVSTAGE3);
    } else {
        GXSetNumIndStages(1);
        GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD2, GX_TEXMAP2);
        GXSetTevIndWarp(GX_TEVSTAGE3, GX_INDTEXSTAGE0, 1, 0, GX_ITM_0);

        Mtx23 new_mtx;
        new_mtx[0][0] = sIndirectScale;
        new_mtx[0][1] = 0.0f;
        new_mtx[0][2] = 0.0f;
        new_mtx[1][0] = 0.0f;
        new_mtx[1][1] = sIndirectScale;
        new_mtx[1][2] = 0.0f;

        GXSetIndTexMtx(GX_ITM_0, new_mtx, 0);
    }

    GXSetNumTevStages(4);
    GXSetTevColor(GX_TEVREG0, (GXColor&)color1);
    GXSetTevColor(GX_TEVREG1, (GXColor&)color2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 0, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 0, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_TEXC, GX_CC_CPREV, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_DIVIDE_2, 0, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, 0, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_CPREV, GX_CC_A0, GX_CC_C0, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_COMP_R8_EQ, GX_TB_ZERO, GX_CS_SCALE_1, 0, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_KONST, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 0, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD3, GX_TEXMAP1, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE3, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C1, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE3, GX_CA_RASA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 0, GX_TEVPREV);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
    GXSetZMode(1, GX_LEQUAL, 0);
    GXSetZCompLoc(1);
    GXSetCullMode(GX_CULL_NONE);
    GXSetClipMode(GX_CLIP_ENABLE);
}

void OceanRingDrawer::loadMaterialBloom() const {
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_POS_XYZ, GX_F32, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
    GXLoadPosMtxImm(MR::getCameraViewMtx(), 0);
    GXSetCurrentMtx(0);
    GXSetNumChans(0);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, 0x21, 0, 0x7D);

    TMtx34f mtx;
    mtx.identity();
    mtx.mMtx[0][2] = _C;
    mtx.mMtx[1][2] = _10;
    GXLoadTexMtxImm(mtx.toMtxPtr(), 0x1E, GX_MTX2x4);
    mtx.mMtx[0][2] = _14;
    mtx.mMtx[1][2] = _18;
    GXLoadTexMtxImm(mtx.toMtxPtr(), 0x21, GX_MTX2x4);
    mWaterTex->load(GX_TEXMAP0);
    GXSetNumIndStages(0);
    TVec3f zDir = MR::getCamZdir();
    f32 v6 = ((MR::clamp(zDir.y, -0.2f, 0.1f) - -0.2f) / 0.3f);
    f32 v8 = ((MR::clamp(WaterAreaFunction::getCameraWaterDepth(), 150.0f, 300.0f) - 150.0f) / 150.0f);

    if (v6 >= v8) {
        v8 = v8;
    } else {
        v8 = v6;
    }

    f32 v18 = (150.0f + (105.0f * v8));
    u8 v19 = v18;
    GXSetNumTevStages(2);
    Color8 v14(80, 80, 80, v19);
    Color8 v13(80, 80, 80, 255);
    GXSetTevColor(GX_TEVREG0, (GXColor&)v14);
    GXSetTevColor(GX_TEVREG1, (GXColor&)v13);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C0, GX_CC_C1);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1u, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1u, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C0, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1u, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_APREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1u, GX_TEVPREV);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
    GXSetZMode(1u, GX_LEQUAL, 1u);
    GXSetZCompLoc(1u);
    GXSetCullMode(GX_CULL_NONE);
    GXSetClipMode(GX_CLIP_ENABLE);
}
