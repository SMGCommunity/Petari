#include "Game/Util/DirectDraw.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "math_types.hpp"
#include "revolution/gx/GXEnum.h"
#include "revolution/gx/GXVert.h"
#include <revolution/gx/GXGeometry.h>

namespace {
    static Mtx mViewMtx;
};

namespace TDDraw {
    void setViewMtx(MtxPtr mtx) {
        PSMTXCopy(mtx, mViewMtx);
    }

    void loadViewMtx(MtxPtr mtx) {
        GXLoadPosMtxImm(mtx, 0);
    }

    void setModelMtx(MtxPtr mtx) {
        PSMTXConcat(MR::getCameraViewMtx(), mtx, mViewMtx);
        GXLoadPosMtxImm(mViewMtx, 0);
    }

    void resetViewMtx() {
        PSMTXCopy(MR::getCameraViewMtx(), mViewMtx);
        GXLoadPosMtxImm(mViewMtx, 0);
    }

    void close() {
        MR::loadProjectionMtx();
        MR::loadViewMtx();
        GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    }

    void setup(u32 a1, u32 a2, u8 a3) {
        GXColor v15 = {0, 0, 0, 0};

        GXSetCullMode(GX_CULL_NONE);
        GXSetClipMode(GX_CLIP_ENABLE);
        GXSetCoPlanar(0);
        GXSetZMode(0, GX_ALWAYS, 0);
        GXSetZCompLoc(0);
        GXSetColorUpdate(1u);
        GXSetAlphaUpdate(0);
        GXSetLineWidth(6, GX_TO_ZERO);
        GXSetChanMatColor(GX_COLOR0A0, v15);
        GXSetChanAmbColor(GX_COLOR0A0, v15);
        GXSetChanMatColor(GX_COLOR1A1, v15);
        GXSetChanAmbColor(GX_COLOR1A1, v15);
        GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
        GXSetChanCtrl(GX_COLOR1A1, 0, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
        GXSetNumChans(1);

        Mtx v16;
        PSMTXIdentity(v16);

        switch (a3) {
        case 0:
            PSMTXCopy(MR::getCameraViewMtx(), mViewMtx);
            GXLoadPosMtxImm(mViewMtx, 0);
            PSMTXCopy(mViewMtx, v16);
            break;
        case 1:
            MR::loadProjectionMtx();
            GXLoadPosMtxImm(mViewMtx, 0);
            break;
        case 2:
            cameraInit2D();
            PSMTXTrans(v16, -MR::getScreenWidth() / 2.0f, MR::getScreenHeight() / 2.0f, 0.0f);
            v16[1][1] = -1.0f;
            GXLoadPosMtxImm(v16, 0);
            break;
        }

        GXLoadNrmMtxImm(v16, 0);
        GXSetCurrentMtx(0);
        GXSetTevDirect(GX_TEVSTAGE0);
        GXSetNumIndStages(0);

        switch (a1) {
        case 0:
            GXSetNumTexGens(0);
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
            GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
            GXClearVtxDesc();
            GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
            GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_POS_XYZ, GX_RGBA8, 0);
            break;
        case 1:
            GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
            GXSetNumTexGens(1);
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
            GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
            GXClearVtxDesc();
            GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
            GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_F32, 0);
            break;
        case 2:
            GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
            GXSetNumTexGens(1);
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
            GXClearVtxDesc();
            GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
            GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_F32, 0);
            break;
        }

        switch (a2) {
        case 2:
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_CLEAR);
            GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
            break;
        case 1:
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
            GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
            break;
        case 0:
            GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_CLEAR);
            GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
            break;
        }

        GXSetFog(GX_FOG_NONE, 0.0, 0.0, 0.0, 0.0, v15);
        GXSetFogRangeAdj(0, 0, 0);
    }

    void sendPoint(const TVec3f& a1, u32 a2) {
        GXPosition3f32(a1.x, a1.y, a1.z);
        GXCmd1u32(a2);
    }

    void sendPoint(const TVec3f& rVec) {
        GXPosition3f32(rVec.x, rVec.y, rVec.z);
    }

    void drawLine(const TVec3f& a1, const TVec3f& a2, u32 a3) {
        GXBegin(GX_LINES, GX_VTXFMT0, 2);
        GXPosition3f32(a1.x, a1.y, a1.z);
        GXCmd1u32(a3);
        GXPosition3f32(a2.x, a2.y, a2.z);
        GXCmd1u32(a3);
    }

    void drawCircle(const TVec3f& a1, const TVec3f& a2, f32 a3, u32 a4, u32 a5) {
        TVec3f v11;

        if (a2.x != 0.0f) {
            v11.set< f32 >(a2.y, -a2.x, 0.0f);
        } else {
            v11.set< f32 >(0.0f, -a2.z, a2.y);
        }

        MR::normalizeOrZero(&v11);
        drawCircle(a1, a2, v11, a3, a4, a5);
    }

    // tvec inlines
    void drawCircle(const TVec3f& a1, const TVec3f& a2, const TVec3f& a3, f32 a4, u32 a5, u32 a6) {
        TVec3f v14(a3);
        Mtx v15;
        PSMTXRotAxisRad(v15, a2, 6.2831855f / a6);
        GXBegin(GX_LINESTRIP, GX_VTXFMT0, a6 + 1);

        for (u32 i = 0; i <= a6; i++) {
            TVec3f v12(v14);
            v12 *= a4;
            TVec3f v13(a1);
            v13 += v12;
            GXPosition3f32(v13.x, v13.y, v13.z);
            GXCmd1u32(a5);
            PSMTXMultVec(v15, v14, v14);
        }
    }

    void drawFillCircle(const TVec3f& a1, f32 a2, u32 a3, u32 a4, u32 a5) {
        GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, a5 + 2);
        GXPosition3f32(a1.x, a1.y, a1.z);
        GXCmd1u32(a3);

        for (u32 i = 0; i <= a5; i++) {
            f32 v12 = a1.x - (a2 * MR::cos(2.0f * (i) / (a5 * PI)));
            f32 v13 = a1.y + (a2 * MR::sin(2.0f * (i) / (a5 * PI)));
            GXPosition3f32(v12, v13, a1.z);
            GXCmd1u32(a4);
        }
    }

    // tvec inlines
    void drawFillCircle(const TVec3f& a1, const TVec3f& a2, f32 a3, u32 a4, u32 a5) {
        TVec3f v14;
        if (a2.x != 0.0f) {
            v14.set< f32 >(a2.y, -a2.x, 0.0f);
        } else {
            v14.set< f32 >(0.0f, -a2.z, a2.y);
        }

        MR::normalizeOrZero(&v14);
        Mtx v15;
        PSMTXRotAxisRad(v15, a2, (6.2831855f / a5));
        GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, a5 + 2);
        GXPosition3f32(a1.x, a1.y, a1.z);
        GXCmd1u32(a4);

        for (u32 i = 0; i <= a5; i++) {
            TVec3f v12(v14);
            v12 *= a3;
            TVec3f v13(a1);
            v13 += v12;
            PSMTXMultVec(v15, v14, v14);
            GXPosition3f32(v13.x, v13.y, v13.z);
            GXCmd1u32(a4);
        }
    }

    // TDDraw::drawFillFan
    // TDDraw::drawCylinder
    // TDDraw::drawSpherePart

    void drawSphere(const TVec3f& a1, f32 a2, u32 a3, u32 a4) {
        TVec3f v9 = MR::getCamZdir();
        drawFillCircle(a1, v9, a2, a3, a4);
    }

    void drawSphere3D(TVec3f a1, f32 a2, u32 a3, u32 a4) {
        TPos3f v8;
        v8.identity();
        v8.setTrans(a1);
        drawSpherePart(v8, a2, a3, a3, a4, 0.0f, TWO_PI, 0.0f, PI);
    }

};  // namespace TDDraw
