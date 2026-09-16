#include "Game/Util/DirectDraw.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DrawUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SystemUtil.hpp"
#include "JSystem/JUtility/JUTTexture.hpp"
#include "math_types.hpp"
#include "revolution/gx/GXEnum.h"
#include "revolution/gx/GXVert.h"
#include <revolution/gx/GXGeometry.h>

namespace {
    static Mtx mViewMtx;

}  // namespace

namespace TDDraw {
    void setViewMtx(MtxPtr pMtx) {
        PSMTXCopy(pMtx, mViewMtx);
    }

    void loadViewMtx(MtxPtr pMtx) {
        GXLoadPosMtxImm(pMtx, 0);
    }

    void setModelMtx(MtxPtr pMtx) {
        MR::multMtx(mViewMtx, pMtx, MR::getCameraViewMtx());
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

    void setup(u32 drawMode, u32 blendMode, u8 viewMode) {
        GXColor color = {0, 0, 0, 0};

        GXSetCullMode(GX_CULL_NONE);
        GXSetClipMode(GX_CLIP_ENABLE);
        GXSetCoPlanar(0);
        GXSetZMode(0, GX_ALWAYS, 0);
        GXSetZCompLoc(0);
        GXSetColorUpdate(GX_TRUE);
        GXSetAlphaUpdate(0);
        GXSetLineWidth(6, GX_TO_ZERO);
        GXSetChanMatColor(GX_COLOR0A0, color);
        GXSetChanAmbColor(GX_COLOR0A0, color);
        GXSetChanMatColor(GX_COLOR1A1, color);
        GXSetChanAmbColor(GX_COLOR1A1, color);
        GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
        GXSetChanCtrl(GX_COLOR1A1, 0, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
        GXSetNumChans(1);

        Mtx mtx;
        PSMTXIdentity(mtx);

        switch (viewMode) {
        case 0:
            PSMTXCopy(MR::getCameraViewMtx(), mViewMtx);
            GXLoadPosMtxImm(mViewMtx, 0);
            PSMTXCopy(mViewMtx, mtx);
            break;
        case 1:
            MR::loadProjectionMtx();
            GXLoadPosMtxImm(mViewMtx, 0);
            break;
        case 2:
            cameraInit2D();
            PSMTXTrans(mtx, -MR::getScreenWidth() / 2.0f, MR::getScreenHeight() / 2.0f, 0.0f);
            mtx[1][1] = -1.0f;
            GXLoadPosMtxImm(mtx, 0);
            break;
        }

        GXLoadNrmMtxImm(mtx, 0);
        GXSetCurrentMtx(0);
        GXSetTevDirect(GX_TEVSTAGE0);
        GXSetNumIndStages(0);

        switch (drawMode) {
        case 0:
            GXSetNumTexGens(0);
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
            GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
            GXClearVtxDesc();
            GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
            GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
            break;
        case 1:
            GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
            GXSetNumTexGens(1);
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
            GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
            GXClearVtxDesc();
            GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
            GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
            break;
        case 2:
            GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
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
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
            break;
        }

        switch (blendMode) {
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

        GXSetFog(GX_FOG_NONE, 0.0, 0.0, 0.0, 0.0, color);
        GXSetFogRangeAdj(GX_FALSE, 0, nullptr);
    }

    void sendPoint(const TVec3f& rPoint, u32 color) NO_INLINE {
        GXPosition3f32(rPoint.x, rPoint.y, rPoint.z);
        GXColor1u32(color);
    }

    void sendPoint(const TVec3f& rVec) NO_INLINE {
        GXPosition3f32(rVec.x, rVec.y, rVec.z);
    }

    void drawLine(const TVec3f& rStart, const TVec3f& rEnd, u32 color) {
        GXBegin(GX_LINES, GX_VTXFMT0, 2);
        {
            GXPosition3f32(rStart.x, rStart.y, rStart.z);
            GXColor1u32(color);
            GXPosition3f32(rEnd.x, rEnd.y, rEnd.z);
            GXColor1u32(color);
        }

        GXEnd();
    }

    void drawCircle(const TVec3f& rCenter, const TVec3f& rNormal, f32 radius, u32 color, u32 segments) {
        TVec3f axis;

        if (rNormal.x != 0.0f) {
            axis.set< f32 >(rNormal.y, -rNormal.x, 0.0f);
        } else {
            axis.set< f32 >(0.0f, -rNormal.z, rNormal.y);
        }

        MR::normalizeOrZero(&axis);
        drawCircle(rCenter, rNormal, axis, radius, color, segments);
    }

    void drawCircle(const TVec3f& rCenter, const TVec3f& rNormal, const TVec3f& rAxis, f32 radius, u32 color, u32 segments) {
        TVec3f axis(rAxis);
        Mtx rotation;
        PSMTXRotAxisRad(rotation, rNormal, 6.2831855f / segments);

        GXBegin(GX_LINESTRIP, GX_VTXFMT0, segments + 1);
        for (u32 i = 0; i <= segments; i++) {
            TVec3f point(rCenter + axis * radius);
            GXPosition3f32(point.x, point.y, point.z);
            GXColor1u32(color);
            PSMTXMultVec(rotation, axis, axis);
        }

        GXEnd();
    }

    void drawFillCircle(const TVec3f& rCenter, f32 radius, u32 centerColor, u32 edgeColor, u32 segments) {
        TVec3f point;
        point.z = rCenter.z;
        GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, segments + 2);
        GXPosition3f32(rCenter.x, rCenter.y, rCenter.z);
        GXColor1u32(centerColor);

        for (u32 i = 0; i <= segments; i++) {
            point.x = rCenter.x - radius * MR::cos(2.0f * ((static_cast< f32 >(i) / segments) * PI));
            point.y = rCenter.y + radius * MR::sin(2.0f * ((static_cast< f32 >(i) / segments) * PI));
            GXPosition3f32(point.x, point.y, point.z);
            GXColor1u32(edgeColor);
        }

        GXEnd();
    }

    void drawFillCircle(const TVec3f& rCenter, const TVec3f& rNormal, f32 radius, u32 color, u32 segments) {
        TVec3f axis;
        if (rNormal.x != 0.0f) {
            axis.set< f32 >(rNormal.y, -rNormal.x, 0.0f);
        } else {
            axis.set< f32 >(0.0f, -rNormal.z, rNormal.y);
        }

        MR::normalizeOrZero(&axis);
        Mtx rotation;
        PSMTXRotAxisRad(rotation, rNormal, (6.2831855f / segments));

        GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, segments + 2);
        {
            GXPosition3f32(rCenter.x, rCenter.y, rCenter.z);
            GXColor1u32(color);

            for (u32 i = 0; i <= segments; i++) {
                TVec3f point(rCenter + axis * radius);
                PSMTXMultVec(rotation, axis, axis);
                GXPosition3f32(point.x, point.y, point.z);
                GXColor1u32(color);
            }
        }

        GXEnd();
    }

    void drawFillFan(const TVec3f& rCenter, const TVec3f& rNormal, const TVec3f& rAxis, u32 color, f32 startAngle, f32 endAngle, u32 segments) {
        Mtx startRotation;
        PSMTXRotAxisRad(startRotation, rNormal, startAngle);
        Mtx stepRotation;
        PSMTXRotAxisRad(stepRotation, rNormal, (endAngle - startAngle) / (segments));
        TVec3f axis;
        PSMTXMultVec(startRotation, rAxis, axis);

        GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, segments + 2);
        {
            GXPosition3f32(rCenter.x, rCenter.y, rCenter.z);
            GXColor1u32(color);

            for (u32 i = 0; i <= segments; i++) {
                TVec3f point(axis);
                point += rCenter;
                GXPosition3f32(point.x, point.y, point.z);
                GXColor1u32(color);
                PSMTXMultVec(stepRotation, axis, axis);
            }
        }

        GXEnd();
    }

    void drawCylinder(const TVec3f& rBase, const TVec3f& rHeight, f32 radius, u32 baseColor, u32 topColor, u32 segments) {
        TVec3f axis;

        if (rHeight.x != 0.0f) {
            axis.set< f32 >(rHeight.y, -rHeight.x, 0.0f);
        } else {
            axis.set< f32 >(0.0f, -rHeight.z, rHeight.y);
        }

        if (MR::isNearZero(axis)) {
            return;
        }

        MR::normalizeOrZero(&axis);
        TVec3f top(rBase);
        top += rHeight;
        Mtx rotation;
        PSMTXRotAxisRad(rotation, rHeight, (TWO_PI / segments));

        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 2 * (segments + 1));
        for (u32 i = 0; i <= segments; i++) {
            TVec3f basePoint(rBase + axis * radius);
            TVec3f topPoint(top + axis * radius);
            GXPosition3f32(basePoint.x, basePoint.y, basePoint.z);
            GXColor1u32(baseColor);
            GXPosition3f32(topPoint.x, topPoint.y, topPoint.z);
            GXColor1u32(topColor);
            PSMTXMultVec(rotation, axis, axis);
        }

        GXEnd();
    }

    void drawSpherePart(const TPos3f& rMtx, f32 radius, f32 startAzimuth, f32 endAzimuth, f32 startPolar, f32 endPolar, u32 color,
                        u32 azimuthSegments, u32 polarSegments) {
        TVec3f side;
        side.set< f32 >(rMtx(0, 0), rMtx(1, 0), rMtx(2, 0));
        TVec3f up;
        up.set< f32 >(rMtx(0, 1), rMtx(1, 1), rMtx(2, 1));
        TVec3f front;
        front.set< f32 >(rMtx(0, 2), rMtx(1, 2), rMtx(2, 2));
        TVec3f center;
        rMtx.getTrans(center);
        f32 startSin = MR::sin(startAzimuth);
        f32 startCos = MR::cos(startAzimuth);
        TVec3f previousRadial((side * startCos + up * startSin) * radius);
        f32 polarSpan = endPolar - startPolar;

        for (u32 i = 1; i <= azimuthSegments; i++) {
            f32 azimuth = startAzimuth + (static_cast< f32 >(i) / azimuthSegments) * (endAzimuth - startAzimuth);
            f32 azimuthSin = MR::sin(azimuth);
            f32 azimuthCos = MR::cos(azimuth);
            TVec3f radial((side * azimuthCos + up * azimuthSin) * radius);

            GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 2 * (polarSegments + 1));
            for (u32 j = 0; j <= polarSegments; j++) {
                f32 polar = (startPolar + (static_cast< f32 >(j) / polarSegments) * polarSpan);
                f32 polarSin = MR::sin(polar);
                f32 polarCos = MR::cos(polar);
                TVec3f ringCenter(center + front * (polarCos * radius));
                TVec3f previousPoint(previousRadial);
                previousPoint *= polarSin;
                TVec3f point(radial);
                point *= polarSin;
                previousPoint += ringCenter;
                point += ringCenter;
                GXPosition3f32(previousPoint.x, previousPoint.y, previousPoint.z);
                GXColor1u32(color);
                GXPosition3f32(point.x, point.y, point.z);
                GXColor1u32(color);
            }

            GXEnd();

            previousRadial = radial;
        }
    }

    void drawSphere(const TVec3f& rCenter, f32 radius, u32 color, u32 segments) {
        TVec3f normal;
        normal = MR::getCamZdir();
        drawFillCircle(rCenter, normal, radius, color, segments);
    }

    void drawSphere3D(TVec3f center, f32 radius, u32 color, u32 segments) {
        TPos3f mtx;
        mtx.identity();
        mtx.setTrans(center);
        drawSpherePart(mtx, radius, 0.0f, TWO_PI, 0.0f, PI, color, segments, segments);
    }

    void drawTexture(const TVec2f& rPosition, JUTTexture* pTexture, const TVec2f& rSize) {
        pTexture->load(GX_TEXMAP0);

        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        {
            GXPosition3f32(rPosition.x, rPosition.y, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);
            GXPosition3f32(rPosition.x + rSize.x, rPosition.y, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);
            GXPosition3f32(rPosition.x + rSize.x, rPosition.y + rSize.y, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);
            GXPosition3f32(rPosition.x, rPosition.y + rSize.y, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);
        }

        GXEnd();
    }

    void drawTexture3D(const TVec3f& rCenter, const TVec3f& rNormal, const TVec3f& rUp, f32 width, f32 height, JUTTexture* pTexture, bool flipX,
                       bool flipY) {
        TVec3f p3;
        TVec3f p2;
        TVec3f p1;
        TVec3f p0;
        TVec3f right;
        TVec3f up;

        if (pTexture != nullptr) {
            pTexture->load(GX_TEXMAP0);
        }

        right.cross(rNormal, rUp);
        MR::normalizeOrZero(&right);
        up.cross(right, rNormal);
        MR::normalizeOrZero(&up);
        p0 = rCenter - right * height + up * width;
        p1 = rCenter + right * height + up * width;
        p2 = rCenter + right * height - up * width;
        p3 = rCenter - right * height - up * width;

        f32 left = flipX ? 1.0f : 0.0f;
        f32 rightTex = flipX ? 0.0f : 1.0f;
        f32 top = flipY ? 1.0f : 0.0f;
        f32 bottom = flipY ? 0.0f : 1.0f;
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(p0.x, p0.y, p0.z);
        GXTexCoord2f32(left, top);
        sendPoint(p1);
        GXTexCoord2f32(rightTex, top);
        sendPoint(p2);
        GXTexCoord2f32(rightTex, bottom);
        sendPoint(p3);
        GXTexCoord2f32(left, bottom);
        GXEnd();
    }

    void drawFillBox(const TVec3f& rMin, const TVec3f& rMax, u32 color) {
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        {
            GXPosition3f32(rMin.x, rMin.y, rMin.z);
            GXColor1u32(color);
            GXPosition3f32(rMax.x, rMin.y, rMin.z);
            GXColor1u32(color);
            GXPosition3f32(rMax.x, rMax.y, rMax.z);
            GXColor1u32(color);
            GXPosition3f32(rMin.x, rMax.y, rMax.z);
            GXColor1u32(color);
        }

        GXEnd();
    }

    void drawFillBox(const TVec2f& rMin, const TVec2f& rMax, u32 color) {
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        {
            GXPosition3f32(rMin.x, rMin.y, 0.0f);
            GXColor1u32(color);
            GXPosition3f32(rMax.x, rMin.y, 0.0f);
            GXColor1u32(color);
            GXPosition3f32(rMax.x, rMax.y, 0.0f);
            GXColor1u32(color);
            GXPosition3f32(rMin.x, rMax.y, 0.0f);
            GXColor1u32(color);
        }

        GXEnd();
    }

    void drawFillBox3D(const TVec3f& rCenter, const TVec3f& rHeight, const TVec3f& rSide, const TVec3f& rFront, u32 color) {
        TVec3f p7, p6, p5, p4, p3, p2, p1, p0;
        p0 = rCenter + rSide - rFront;
        p1 = rCenter - rSide - rFront;
        p2 = rCenter - rSide + rFront;
        p3 = rCenter + rSide + rFront;
        p4 = p0 + rHeight;
        p5 = p1 + rHeight;
        p6 = p2 + rHeight;
        p7 = p3 + rHeight;

        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(p0.x, p0.y, p0.z);
        GXColor1u32(color);
        sendPoint(p1, color);
        sendPoint(p2, color);
        sendPoint(p3, color);
        GXEnd();

        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        sendPoint(p7, color);
        sendPoint(p4, color);
        GXPosition3f32(p0.x, p0.y, p0.z);
        GXColor1u32(color);
        sendPoint(p3, color);
        GXEnd();

        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        sendPoint(p6, color);
        sendPoint(p2, color);
        sendPoint(p1, color);
        sendPoint(p5, color);
        GXEnd();

        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(p0.x, p0.y, p0.z);
        GXColor1u32(color);
        sendPoint(p4, color);
        sendPoint(p5, color);
        sendPoint(p1, color);
        GXEnd();

        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        sendPoint(p6, color);
        sendPoint(p7, color);
        sendPoint(p3, color);
        sendPoint(p2, color);
        GXEnd();

        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        sendPoint(p6, color);
        sendPoint(p5, color);
        sendPoint(p4, color);
        sendPoint(p7, color);
        GXEnd();
    }

    void cameraInit3D() {
        MR::loadProjectionMtx();
    }

    void cameraInit2D() {
        static TVec3f camLoc(MR::getScreenWidth() / 2.0f, MR::getScreenHeight() / 2.0f, -30.0f);
        static TVec3f objPt(MR::getScreenWidth() / 2.0f, MR::getScreenHeight() / 2.0f, 0.0f);
        static TVec3f up(0, -10, 0);

        f32 nearZ = 0.0f;
        f32 farZ = 1.0f;
        f32 width = MR::getScreenWidth() / 2;
        f32 height = MR::getScreenHeight() / 2;
        Mtx44 projection;
        C_MTXOrtho(projection, height, -height, -width, width, nearZ, -farZ);
        GXSetProjection(projection, GX_ORTHOGRAPHIC);
        MR::setDefaultViewportAndScissor();
    }

    void mixFogColor(TVec3f position, f32 radius, u32 packedColor) {
        f32 nearZ = MR::getNearZ();
        f32 farZ = MR::getFarZ();
        GXColor color;
        setGXColor(packedColor, &color);
        f32 start;
        f32 end;
        MR::calcFogStartEnd(position, radius, &start, &end);
        GXSetFog(GX_FOG_PERSP_LIN, start, end, nearZ, farZ, color);
    }

    namespace {
        u8 tileBuffer[0x800] ATTRIBUTE_ALIGN(32);
    }

    void tileConversion8(u8* pTexture, u32 width, u32 height) {
        u8* pDest = pTexture;
        u32 offset = 0;
        for (u32 y = 0; y < height; y += 4) {
            for (u32 x = 0; x < width; x += 8) {
                for (u32 row = 0; row < 4; row++) {
                    for (u32 column = 0; column < 8; column++) {
                        tileBuffer[offset++] = pTexture[(y + row) * width + x + column];
                    }
                }
            }

            MR::copyMemory(pDest, tileBuffer, width * 4);
            pDest += width * 4;
            offset = 0;
        }
    }

    void tileConversion16(u16* pTexture, u32 width, u32 height) {
        u8* pDest = reinterpret_cast< u8* >(pTexture);
        u32 offset = 0;
        for (u32 y = 0; y < height; y += 4) {
            for (u32 x = 0; x < width; x += 4) {
                for (u32 row = 0; row < 4; row++) {
                    for (u32 column = 0; column < 4; column++) {
                        tileBuffer[offset++] = pTexture[(y + row) * width + x + column];
                    }
                }
            }

            MR::copyMemory(pDest, tileBuffer, 32);
            offset = 0;
            pDest += 32;
        }
    }

    u32 getTexel32(const JUTTexture* pTexture, u32 x, u32 y) {
        return getTexel32(pTexture->mImage, pTexture->getWidth(), x, y);
    }

    u32 getTexel32(const u8* pTexture, u32 width, u32 x, u32 y) {
        u32 offset = ((width << 4) & ~0x3F) * (y >> 2) + (((x & 3) << 1) + (((y & 3) << 3) + ((x << 4) & ~0x3F)));
        return (pTexture[offset] | (pTexture[offset + 0x21] << 8)) | ((pTexture[offset + 1] << 24) | (pTexture[offset + 0x20] << 16));
    }

    void setTexel32(JUTTexture* pTexture, u32 x, u32 y, u32 color) {
        setTexel32(pTexture->mImage, pTexture->getWidth(), x, y, color);
    }

    void setTexel32(u8* pTexture, u32 width, u32 x, u32 y, u32 color) {
        u32 offset = ((width << 4) & ~0x3F) * (y >> 2) + (((x & 3) << 1) + (((y & 3) << 3) + ((x << 4) & ~0x3F)));
        pTexture[offset] = color;
        pTexture[offset + 1] = color >> 24;
        pTexture[offset + 0x20] = color >> 16;
        pTexture[offset + 0x21] = color >> 8;
    }

    void invProject(TVec3f* pDest, const TVec3f& rScreen, MtxPtr pView, const f32* pProjection, const f32* pViewport, bool normalizedZ) {
        f32 screenZ;
        if (normalizedZ == false) {
            screenZ = rScreen.z / 16777215.0f;
        } else {
            screenZ = rScreen.z;
        }

        f32 depth = screenZ - pViewport[5];
        f32 w;
        f32 projectedZ = (depth * pProjection[6]) / (depth + pProjection[5] * (pViewport[5] - pViewport[4]));
        w = pProjection[5] / (pProjection[6] - projectedZ);
        f32 x;
        if (MR::isScreen16Per9()) {
            x = ((rScreen.x * MR::getFrameBufferWidth() / MR::getScreenWidth() - (pViewport[0] + pViewport[2] / 2.0f)) / w) / (pViewport[2] / 2.0f);
        } else {
            x = ((rScreen.x - (pViewport[0] + pViewport[2] / 2.0f)) / w) / (pViewport[2] / 2.0f);
        }

        f32 y = (-(rScreen.y - (pViewport[1] + pViewport[3] / 2.0f)) / w) / (pViewport[3] / 2.0f);
        TVec3f viewPos;
        if (pProjection[0] == 0.0f) {
            viewPos.z = (projectedZ - pProjection[6]) / pProjection[5];
            viewPos.x = (x - viewPos.z * pProjection[2]) / pProjection[1];
            viewPos.y = (y - viewPos.z * pProjection[4]) / pProjection[3];
        } else {
            viewPos.z = (projectedZ - pProjection[6]) / pProjection[5];
            viewPos.x = (x - pProjection[2]) / pProjection[1];
            viewPos.y = (y - pProjection[4]) / pProjection[3];
        }

        Mtx inverseView;
        PSMTXInverse(pView, inverseView);
        PSMTXMultVec(inverseView, viewPos, pDest);
    }

    void project2D(TVec3f* pDest, const TVec3f& rPosition) {
        f32 projection[7];
        f32 viewport[6];
        GXGetViewportv(viewport);
        const TProj3f& rProjection = MR::getCameraProjectionMtx();
        projection[0] = 0.0f;
        projection[1] = rProjection.mMtx[0][0];
        projection[2] = rProjection.mMtx[0][2];
        projection[3] = rProjection.mMtx[1][1];
        projection[4] = rProjection.mMtx[1][2];
        projection[5] = rProjection.mMtx[2][2];
        projection[6] = rProjection.mMtx[2][3];
        GXProject(rPosition.x, rPosition.y, rPosition.z, MR::getCameraViewMtx(), projection, viewport, &pDest->x, &pDest->y, &pDest->z);
        if (MR::isScreen16Per9()) {
            pDest->x *= static_cast< f32 >(MR::getScreenWidth()) / MR::getFrameBufferWidth();
        }
    }

    void project2D(TVec2f* pDest, const TVec3f& rPosition) {
        TVec3f screen;
        project2D(&screen, rPosition);
        pDest->x = screen.x;
        pDest->y = screen.y;
    }

    void fix2Dpos(TVec3f* pPosition) {
        if (MR::isScreen16Per9()) {
            pPosition->x *= static_cast< f32 >(MR::getScreenWidth()) / MR::getFrameBufferWidth();
        }
    }

    void setGXColor(u32 color, GXColor* pColor) {
        pColor->r = (color >> 24) & 0xFF;
        pColor->g = (color >> 16) & 0xFF;
        pColor->b = (color >> 8) & 0xFF;
        pColor->a = color & 0xFF;
    }

}  // namespace TDDraw
