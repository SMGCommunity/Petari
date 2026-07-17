#include "Game/Util/DirectDraw.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DrawUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "JSystem/JUtility/JUTTexture.hpp"
#include "math_types.hpp"
#include "revolution/gx/GXEnum.h"
#include "revolution/gx/GXVert.h"
#include <revolution/gx/GXGeometry.h>

namespace {
    static Mtx mViewMtx;

    static u8 byte_806B7048;
    static u8 byte_806B7049;
    static u8 byte_806B704A;

};  // namespace

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
        {
            sendPoint(a1, a3);
            sendPoint(a2, a3);
        }
        GXEnd();
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
        GXEnd();
    }

    void drawFillCircle(const TVec3f& a1, f32 a2, u32 a3, u32 a4, u32 a5) {
        GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, a5 + 2);
        {
            GXPosition3f32(a1.x, a1.y, a1.z);
            GXCmd1u32(a3);

            for (u32 i = 0; i <= a5; i++) {
                f32 v12 = a1.x - (a2 * MR::cos(2.0f * (i) / (a5 * PI)));
                f32 v13 = a1.y + (a2 * MR::sin(2.0f * (i) / (a5 * PI)));
                GXPosition3f32(v12, v13, a1.z);
                GXCmd1u32(a4);
            }
        }
        GXEnd();
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
        {
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
        GXEnd();
    }

    void drawFillFan(const TVec3f& a1, const TVec3f& a2, const TVec3f& a3, u32 a4, f32 a5, f32 a6, u32 a7) {
        Mtx v18;
        PSMTXRotAxisRad(v18, a2, a5);
        Mtx v17;
        PSMTXRotAxisRad(v17, a2, (a6 - a5) / (a7));
        TVec3f v16;
        PSMTXMultVec(v18, a3, v16);

        GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, a7 + 2);
        {
            GXPosition3f32(a1.x, a1.y, a1.z);
            GXCmd1u32(a4);

            for (u32 i = 0; i <= a7; i++) {
                TVec3f v15(v16);
                v15 += a1;
                GXPosition3f32(v15.x, v15.y, v15.z);
                GXCmd1u32(a4);
                PSMTXMultVec(v17, v16, v16);
            }
        }
        GXEnd();
    }

    void drawCylinder(const TVec3f& a1, const TVec3f& a2, f32 a3, u32 a4, u32 a5, u32 a6) {
        TVec3f v19;

        if (a2.x != 0.0f) {
            v19.set< f32 >(a2.y, -a2.x, 0.0f);
        } else {
            v19.set< f32 >(0.0f, -a2.z, a2.y);
        }

        if (MR::isNearZero(v19)) {
            return;
        }

        MR::normalizeOrZero(&v19);
        TVec3f v18(a1);
        v18 += a2;
        Mtx v20;
        PSMTXRotAxisRad(v20, a2, (TWO_PI / a6));

        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 2 * (a6 + 1));
        for (u32 i = 0; i <= a6; i++) {
            TVec3f v15(v19);
            v15 *= a3;
            TVec3f v17(a1);
            v17 += v15;
            TVec3f v14(v19);
            v14 *= a3;
            TVec3f v16(v18);
            v16 += v14;
            GXPosition3f32(v17.x, v17.y, v17.z);
            GXCmd1u32(a4);
            GXPosition3f32(v16.x, v16.y, v16.z);
            GXCmd1u32(a5);
            PSMTXMultVec(v20, v19, v19);
        }
        GXEnd();
    }

    void drawSpherePart(const TPos3f& a1, f32 a2, f32 a3, f32 a4, f32 a5, f32 a6, u32 a7, u32 a8, u32 a9) {
        TVec3f v47;
        v47.set< f32 >(a1(0, 0), a1(1, 0), a1(2, 0));
        TVec3f v46;
        v46.set< f32 >(a1(0, 1), a1(1, 1), a1(2, 1));
        TVec3f v45;
        v45.set< f32 >(a1(0, 2), a1(1, 2), a1(2, 2));
        TVec3f v44;
        a1.getTrans(v44);
        f32 v21 = MR::sin(a3);
        f32 v22 = MR::cos(a3);
        TVec3f v36(v46);
        v36 *= v21;
        TVec3f v37(v47);
        v37 *= v22;
        TVec3f v38(v37);
        v38 += v36;
        TVec3f v43(v38);
        v43 *= a2;
        f32 v23 = a6 - a5;

        for (u32 i = 1; i <= a8; i++) {
            f32 v25 = (a3 + (i / a8)) * (a4 - a3);
            f32 v26 = MR::sin(v25);
            f32 v27 = MR::cos(v25);
            TVec3f v33(v46);
            v33 *= v26;
            TVec3f v34(v47);
            v34 *= v27;
            TVec3f v35(v34);
            v34 += v33;
            TVec3f v42(v35);
            v42 *= a2;

            GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 2 * (a9 + 1));
            for (s32 j = 0; j <= a9; j++) {
                f32 v29 = (a5 + (j / a9) * v23);
                f32 v30 = MR::sin(v29);
                f32 v31 = MR::cos(v29);
                TVec3f v32(v45);
                v32 *= (v31 * a2);
                TVec3f v41(v44);
                v41 += v32;
                TVec3f v40(v43);
                v40 *= v30;
                TVec3f v39(v42);
                v39 *= v30;
                v40 += v41;
                v39 += v41;
                GXPosition3f32(v40.x, v40.y, v40.z);
                GXCmd1u32(a7);
                GXPosition3f32(v39.x, v39.y, v39.z);
                GXCmd1u32(a7);
            }
            GXEnd();

            v43 = v42;
        }
    }

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

    void drawTexture(const TVec2f& a1, JUTTexture* a2, const TVec2f& a3) {
        a2->load(GX_TEXMAP0);

        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        {
            GXPosition3f32(a1.x, a1.y, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);
            GXPosition3f32(a1.x + a3.x, a1.y, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);
            GXPosition3f32(a1.x + a3.x, a1.y + a3.y, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);
            GXPosition3f32(a1.x, a1.y + a3.y, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);
        }
        GXEnd();
    }

    // TDDraw::drawTexture3D

    void drawFillBox(const TVec3f& a1, const TVec3f& a2, u32 a3) {
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        {
            GXPosition3f32(a1.x, a1.y, a1.z);
            GXCmd1u32(a3);
            GXPosition3f32(a2.x, a1.y, a1.z);
            GXCmd1u32(a3);
            GXPosition3f32(a2.x, a2.y, a2.z);
            GXCmd1u32(a3);
            GXPosition3f32(a1.x, a2.y, a2.z);
            GXCmd1u32(a3);
        }
        GXEnd();
    }

    void drawFillBox(const TVec2f& a1, const TVec2f& a2, u32 a3) {
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        {
            GXPosition3f32(a1.x, a1.y, 0.0f);
            GXCmd1u32(a3);
            GXPosition3f32(a2.x, a1.y, 0.0f);
            GXCmd1u32(a3);
            GXPosition3f32(a2.x, a2.y, 0.0f);
            GXCmd1u32(a3);
            GXPosition3f32(a1.x, a2.y, 0.0f);
            GXCmd1u32(a3);
        }
        GXEnd();
    }

    //  TDDraw::drawFillBox3D

    void cameraInit3D() {
        MR::loadProjectionMtx();
    }

    // https://decomp.me/scratch/33EPL
    void cameraInit2D() {
        if (!byte_806B7048) {
            static TVec3f camLoc = TVec3f(MR::getScreenWidth() / 2.0f, MR::getScreenHeight() / 2.0f, -30.0f);
            byte_806B7048 = 1;
        }

        if (!byte_806B7049) {
            static TVec3f objPt = TVec3f(MR::getScreenWidth() / 2.0f, MR::getScreenHeight() / 2.0f, 0.0f);
            byte_806B7049 = 1;
        }

        if (!byte_806B704A) {
            static TVec3f up = TVec3f(0, -10, 0);
            byte_806B704A = 1;
        }

        f32 width = MR::getScreenWidth() / 2;
        f32 height = MR::getScreenHeight() / 2;

        Mtx proj;
        C_MTXOrtho(proj, height, -height, -width, width, 0.0f, -1.0f);
        GXSetProjection(proj, GX_ORTHOGRAPHIC);
        MR::setDefaultViewportAndScissor();
    }

    void mixFogColor(TVec3f a1, f32 a2, u32 a3) {
        f32 nearZ = MR::getNearZ();
        f32 farZ = MR::getFarZ();
        GXColor color;
        setGXColor(a3, &color);
        f32 v11;
        f32 v10;
        MR::calcFogStartEnd(a1, a2, &v11, &v10);
        GXSetFog(GX_FOG_PERSP_LIN, v11, v10, nearZ, farZ, color);
    }

    // TDDraw::tileConversion8
    // TDDraw::tileConversion16
    // TDDraw::getTexel32
    // TDDraw::getTexel32
    // TDDraw::setTexel32

    // https://decomp.me/scratch/WFf2R
    void setTexel32(u8* tex, u32 width, u32 x, u32 y, u32 color) {
        u32 offset = ((width << 4) & ~0x3F) * (y >> 2) + ((x << 4) & ~0x3F) + ((x & 3) << 1) + ((y & 3) << 3);
        u8* dst = tex + offset;
        dst[0x00] = color;
        dst[0x20] = color >> 8;
        dst[0x21] = color >> 16;
        dst[0x01] = color >> 24;
    }

    // TDDraw::invProject
    // TDDraw::project2D
    // TDDraw::project2D
    // TDDraw::fix2Dpos

    void setGXColor(u32 a1, GXColor* pColor) {
        pColor->r = (a1 >> 24) & 0xFF;
        pColor->g = (a1 >> 16) & 0xFF;
        pColor->b = (a1 >> 8) & 0xFF;
        pColor->a = a1 & 0xFF;
    }

};  // namespace TDDraw
