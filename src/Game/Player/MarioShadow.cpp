#include "Game/Player/MarioShadow.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Player/J3DModelX.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DirectDraw.hpp"
#include "Game/Util/DrawUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/SchedulerUtil.hpp"
#include <JSystem/J3DGraphBase/J3DShape.hpp>
#include <JSystem/J3DGraphBase/J3DSys.hpp>
#include <JSystem/JKernel/JKRSolidHeap.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <JSystem/JUtility/JUTVideo.hpp>
#include <cstring>
#include <revolution/gd.h>

namespace {
    f32 cCheckOffset0 = 5.0f;
    f32 cDrawOffset0 = -5.0f;
}  // namespace

CollisionShadow::~CollisionShadow() {
}

void CollisionShadow::setMode(u32 mode) {
    if (_C == mode) {
        return;
    }
    switch (mode) {
    case 0:
        _30C = cCheckOffset0;
        _310 = cDrawOffset0;
        break;
    case 1:
        _30C = 50.0f;
        _310 = -150.0f;
        break;
    case 2:
        _30C = 18.0f;
        _310 = 0.0f;
        break;
    }
    _C = mode;
}

CollisionShadow::CollisionShadow(f32 radius, f32 length) : NameObj("投影シャドウ") {
    _2F0.zero();
    _2FC = 0;
    _2FE = 0;
    _300 = nullptr;
    _305 = false;
    _30C = 0.0f;
    _310 = 0.0f;
    _314.zero();
    _320.zero();
    _32C = nullptr;
    _330 = nullptr;
    _334 = nullptr;
    _338 = 0;
    _33A = 0;
    _33C = 0;
    _340 = nullptr;
    _344 = nullptr;
    _348 = 0;
    _308 = 0;
    _E = 0;
    _14 = radius;
    _18 = 0.0f;
    _1C = 0.0f;
    _24.zero();
    _30.zero();
    _3C.zero();
    _48.zero();
    _10 = radius;
    _20 = length;
    _54 = 0;
    _C = 255;
    setMode(0);
    initCaptureTex();
    _58 = 128;
    _5A = 3;
    _60 = new (32) TVec3f[_58 * _5A];
    _64 = new (32) TVec2f[_58 * _5A];
    _68 = new u16[_58 * _5A];
    _5C = new u8[_58];
    _6C = 0;
    _32C = new Triangle[_58];
    _330 = new Triangle[2];
    _334 = new Triangle[2];
    _338 = 0;
    _33A = 0;
    _33C = 0;
    _6E = 0;
    _306 = 0;
    _304 = 0;
    _307 = 1;
    for (u32 i = 0; i < 128; i++) {
        _70[i] = 0;
    }
    for (u32 i = 0; i < 128; i++) {
        _F0[i] = 0;
    }
    createDL();
}

void CollisionShadow::create(const TVec3f& position, const TVec3f& direction, const TVec3f& up) {
    TVec3f corners[8];
    TVec3f blended;
    TVec3f vertical(up);
    bool success = MR::vecBlendSphere(_24, direction, &blended, 0.1f);
    if (MR::isNearZero(_24) || !success || _305) {
        blended = direction;
    }
    _305 = false;
    if (!MR::isNearZero(blended)) {
        _24 = blended;
        MR::vecKillElement(vertical, blended, &vertical);
        MR::normalizeOrZero(&vertical);
        if (!MR::isNearZero(vertical)) {
            _30 = vertical;
        }
    }
    _2F0 = position;
    if (_C == 3) {
        return;
    }
    _E++;
    TVec3f side;
    side.cross(vertical, blended);
    if (MR::isNearZero(side)) {
        return;
    }
    MR::normalize(&side);
    for (u32 i = 0; i < 8; i++) {
        corners[i] = position;
        if (i & 1) {
            corners[i] += vertical * _10;
        } else {
            corners[i] -= vertical * _10;
        }
        if (i & 2) {
            corners[i] += side * _10;
        } else {
            corners[i] -= side * _10;
        }
        if (i & 4) {
            corners[i] -= blended * _30C;
        } else {
            corners[i] += blended * _20;
        }
    }
    TVec3f minimum;
    TVec3f maximum;
    MR::createBoundingBox(corners, 8, &minimum, &maximum);
    _314 = minimum;
    _320 = maximum;
    if (MR::isInitializeStatePlacementSomething()) {
        return;
    }
    u32 count = MR::createAreaPolygonListArray(_32C, _58, corners, 8);
    for (u32 i = 0; i < _338; i++) {
        _32C[count] = _330[i];
        count++;
    }
    _338 = 0;
    for (u32 i = 0; i < _33A; i++) {
        _32C[count] = _334[i];
        count++;
    }
    _33A = 0;
    _33C = count;
    _54 = 0;
    if (!count) {
        _10 = _14;
        return;
    }
    _6E = 0;
    u32 polygonCount = 0;
    for (u32 i = 0; i < count; i++) {
        TVec3f normal(*_32C[i].getNormal(0));
        f32 facing = normal.dot(_24);
        if (facing > 0.0f) {
            continue;
        }
        if (normal.dot(MR::getCamZdir()) > 0.707f) {
            continue;
        }
        if (MR::isNearZero(facing, 0.15f)) {
            _70[_6E] = i;
            _6E++;
        } else {
            const char* code = MR::getFloorCodeString(&_32C[i]);
            if (!code || (strcmp(code, "PullBack") != 0 && strcmp(code, "Glass") != 0)) {
                _5C[polygonCount] = i;
                polygonCount++;
            }
        }
    }
    _54 = polygonCount;
    f32 tolerance = 2.0f + _310;
    _6C = 0;
    s32 index = 0;
    for (u32 i = 0; i < _54; i++) {
        for (u32 j = 0; j < _5A; j++) {
            const TVec3f& point = *_32C[_5C[i]].getPos(j);
            u32 k;
            for (k = 0; k < _6C; k++) {
                if (__fabsf(_60[k].x - point.x) < tolerance && __fabsf(_60[k].y - point.y) < tolerance && __fabsf(_60[k].z - point.z) < tolerance) {
                    break;
                }
            }
            if (k == _6C) {
                const TVec3f& normal = *_32C[_5C[i]].getNormal(0);
                _60[_6C] = point + normal * _310;
                _6C++;
            }
            _68[index] = k;
            index++;
        }
    }
    for (u32 i = 0; i < _6C; i++) {
        TVec3f relative(_60[i] - position);
        f32 s = relative.dot(side);
        f32 t = relative.dot(_30);
        _64[i].x = 0.5f + s * 0.0033333334f;
        _64[i].y = 0.5f + t * 0.0033333334f;
    }
    if (_33C > 40) {
        _10 -= 10.0f;
    } else if (_33C > 10) {
        _10 -= 1.0f;
    } else if (_33C > 20) {
        _10 -= 4.0f;
    } else {
        _10 += 1.0f;
    }
    _10 = MR::clamp(_10, 1.0f, _14);
}

void CollisionShadow::draw1() const {
    if (_308 & 8) {
        return;
    }
    TDDraw::setup(1, 1, 0);
    GXSetZMode(GX_TRUE, GX_GEQUAL, GX_FALSE);
    _300->load(GX_TEXMAP0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetAlphaUpdate(GX_TRUE);
    GXSetAlphaCompare(GX_GREATER, 1, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetDstAlpha(GX_TRUE, 64);
    sendVtx();
    GXSetDstAlpha(GX_FALSE, 0);
}

void CollisionShadow::sendVtx() const {
    if (!_54) {
        return;
    }
    u16 vertexCount = _54 * _5A;
    if (_5A == 4) {
        GXBegin(GX_QUADS, GX_VTXFMT0, vertexCount);
    } else {
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, vertexCount);
    }
    u32 index = 0;
    for (u32 i = 0; i < _54; i++) {
        for (u32 j = 0; j < _5A; j++) {
            const TVec3f& point = getDrawPos(index);
            const TVec2f& tex = _64[_68[index]];
            index++;
            GXPosition3f32(point.x, point.y, point.z);
            GXTexCoord2f32(tex.x, tex.y);
        }
    }
}

void CollisionShadow::sendZsortedVtx(bool offset) const {
    u16 vertexCount = _54 * _5A;
    if (_5A == 4) {
        GXBegin(GX_QUADS, GX_VTXFMT0, vertexCount);
    } else {
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, vertexCount);
    }
    TVec3f shift(-_24 * 5.0f);
    for (u32 i = 0; i < _54; i++) {
        for (u32 j = 0; j < _5A; j++) {
            const TVec3f& point = getDrawPos(i, j);
            const TVec2f& tex = getDrawTx(i, j);
            if (offset) {
                GXPosition3f32(point.x + shift.x, point.y + shift.y, point.z + shift.z);
            } else {
                GXPosition3f32(point.x, point.y, point.z);
            }
            GXTexCoord2f32(tex.x, tex.y);
        }
    }
}

void CollisionShadow::initCaptureTex() {
    _2FC = 160;
    _2FE = 160;
    {
        MR::CurrentHeapRestorer restorer(MR::getSceneHeapGDDR3());
        _300 = new JUTTexture(_2FC, _2FE, GX_TF_I8);
        MR::zeroMemory(_300->mImage, _2FC * _2FE);
        MR::setMarioShadowTex(_300);
    }
    _300->mWrapS = GX_CLAMP;
    _300->mWrapT = GX_CLAMP;
    _300->mMinType = GX_LINEAR;
    _300->mMagType = GX_LINEAR;
    _300->init();
}

void CollisionShadow::setViewMtx(const TVec3f& direction) {
    TVec3f normal(direction);
    MR::normalize(&normal);
    TVec3f eye(_2F0 - normal * 10000.0f);
    TPos3f matrix;
    matrix.identity();
    TVec3f side;
    TVec3f up;
    TVec3f forward;
    TVec3f delta;
    delta.sub(_2F0, eye);
    forward.set(delta.x, delta.y, delta.z);
    forward.length();
    PSVECNormalize(&forward, &forward);
    forward.negate();
    side.cross(_30, forward);
    up.cross(forward, side);
    side.length();
    PSVECNormalize(&side, &side);
    up.length();
    PSVECNormalize(&up, &up);
    matrix.mMtx[0][0] = side.x;
    matrix.mMtx[0][1] = side.y;
    matrix.mMtx[0][2] = side.z;

    matrix.mMtx[1][0] = up.x;
    matrix.mMtx[1][1] = up.y;
    matrix.mMtx[1][2] = up.z;

    matrix.mMtx[2][0] = forward.x;
    matrix.mMtx[2][1] = forward.y;
    matrix.mMtx[2][2] = forward.z;

    matrix.mMtx[0][3] = eye.x * -side.x - eye.y * side.y - eye.z * side.z;
    matrix.mMtx[1][3] = eye.x * -up.x - eye.y * up.y - eye.z * up.z;
    matrix.mMtx[2][3] = eye.x * -forward.x - eye.y * forward.y - eye.z * forward.z;
    PSMTXCopy(matrix.toMtxPtr(), j3dSys.mViewMtx);
    TDDraw::setViewMtx(matrix.toMtxPtr());
    MR::setMarioShadowVec(direction);
}

void CollisionShadow::setUpdateFlag() {
    if (!MR::isNearZero(_24)) {
        _304 = 1;
    }
}

void CollisionShadow::calcView(J3DModelX* model, u32 view, J3DModelX* reference) {
    if (!_304) {
        return;
    }
    _304 = 0;
    _306 = 1;
    if (_C == 3) {
        Mtx matrix;
        PSMTXCopy(reference->getBaseTRMtx(), matrix);
        if (reference) {
            model->viewCalcRefPos(view, reference, _48, _24);
        }
    } else {
        setViewMtx(_24);
        if (reference) {
            model->viewCalcRef(view, reference);
        } else {
            model->viewCalc3(view, nullptr);
        }
    }
    if (_C == 2) {
        doSortPolygons();
    }
}

void CollisionShadow::drawAndCaptureTex(J3DModelX* model, const TVec3f& position) {
    if (!_306) {
        return;
    }
    if (_C == 3) {
        _340 = model;
        return;
    }
    if (!(_308 & 2)) {
        setViewMtx(_24);
        TDDraw::setup(0, 0, 1);
        Mtx44 projection;
        C_MTXOrtho(projection, -150.0f, 150.0f, 150.0f, -150.0f, 10.0f, 100000.0f);
        GXSetProjection(projection, GX_ORTHOGRAPHIC);
        if (_307) {
            GXSetViewport(608 - (_2FC + 32), -32.0f, 64.0f + _2FC, 64.0f + _2FE, 0.0f, 1.0f);
            GXSetScissor(608 - _2FC + 1, 1, _2FC - 2, _2FE - 2);
            model->setDrawView(2);
            model->mFlags.clear();
            model->mFlags._11 = true;
            model->mFlags._1E = true;
            model->directDraw(nullptr);
        }
        GXSetViewport(608 - _2FC, 0.0f, _2FC, _2FE, 0.0f, 1.0f);
        GXSetScissor(608 - _2FC + 1, 1, _2FC - 2, _2FE - 2);
        model->setDrawView(2);
        model->mFlags.clear();
        model->mFlags._1E = true;
        model->directDraw(nullptr);
        model->mFlags._1E = false;
    }
    if (!(_308 & 4)) {
        TDDraw::setup(0, 1, 2);
        GXSetColorUpdate(GX_FALSE);
        GXSetAlphaUpdate(GX_TRUE);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTALPHA, GX_BL_ZERO, GX_LO_NOOP);
        GXSetDstAlpha(GX_FALSE, 0);
        TVec3f center(608 - _2FC / 2, _2FE / 2, 0);
        TDDraw::fix2Dpos(&center);
        TDDraw::drawFillCircle(center, _2FC / 2, 0xFFFFFFC0, 0, 16);
        const ResTIMG* image = _300->getTexInfo();
        void* destination = const_cast< u8* >(reinterpret_cast< const u8* >(image) + image->mImageDataOffset);
        GXRenderModeObj* mode = JUTVideo::getManager()->getRenderMode();
        GXSetCopyFilter(GX_FALSE, mode->sample_pattern, GX_FALSE, mode->vfilter);
        JUTTexture::captureDolTexture(destination, _2FC, _2FE, 608 - _2FC, 0, false, GX_CTF_A8);
        GXSetCopyFilter(GX_FALSE, mode->sample_pattern, GX_TRUE, mode->vfilter);
        GXInvalidateTexAll();
    }
    GXSetAlphaUpdate(GX_FALSE);
    GXSetColorUpdate(GX_TRUE);
    J3DShape::resetVcdVatCache();
    MR::setDefaultViewportAndScissor();
    TDDraw::cameraInit3D();
    GXSetDstAlpha(GX_FALSE, 0);
}

void CollisionShadow::clearAlphaBuffer() {
    TVec2f position;
    TVec2f size(static_cast< f32 >(_2FC), static_cast< f32 >(_2FE));
    position.x = 608.0f - _2FC;
    position.y = 0.0f;
    MR::clearAlphaBuffer(0, position, size);
}

void CollisionShadow::drawVolumeBox(const TVec3f& position, const TVec3f& depth) const {
    TVec3f up(_3C);
    TVec3f side;
    side.cross(_24, up);
    MR::normalizeOrZero(&side);
    TDDraw::setup(0, 1, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetAlphaUpdate(GX_TRUE);
    GXSetColorUpdate(GX_FALSE);
    GXSetDstAlpha(GX_FALSE, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXColorS10 color = {255, 255, 255, 255};
    GXSetTevColorS10(GX_TEVREG0, color);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
    GXSetCullMode(GX_CULL_BACK);
    TDDraw::drawFillBox3D(position, depth * 2.0f, side * _1C, up * _18, 0xFFFFFF01);
    GXSetBlendMode(GX_BM_SUBTRACT, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
    GXSetCullMode(GX_CULL_FRONT);
    TDDraw::drawFillBox3D(position, depth * 2.0f, side * _1C, up * _18, 0xFFFFFF01);
}

void CollisionShadow::draw() const {
    MR::loadViewMtx();
    MR::loadProjectionMtx();
    switch (_C) {
    case 0:
        draw1();
        break;
    case 1:
        draw2();
        break;
    case 2:
        draw3();
        break;
    case 3:
        drawVolume();
        break;
    }
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
}

void CollisionShadow::draw2() const {
    if (_308 & 8) {
        return;
    }
    TVec3f position(_2F0 - _24 * _30C);
    drawVolumeBox(position, _24 * _20);
    TDDraw::setup(1, 1, 0);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetAlphaUpdate(GX_TRUE);
    GXSetColorUpdate(GX_TRUE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTALPHA, GX_BL_INVDSTALPHA, GX_LO_NOOP);
    GXSetAlphaCompare(GX_GREATER, 1, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetDstAlpha(GX_TRUE, 0);
    _300->load(GX_TEXMAP0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    sendVtx();
    GXSetDstAlpha(GX_FALSE, 0);
}

void CollisionShadow::draw3() const {
    if (_308 & 8) {
        return;
    }
    TVec3f position(_2F0 - _24 * _30C);
    drawVolumeBox(position, _24 * _20);
    TDDraw::setup(1, 1, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetAlphaUpdate(GX_TRUE);
    GXSetColorUpdate(GX_FALSE);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ZERO, GX_BL_ZERO, GX_LO_NOOP);
    GXSetAlphaCompare(GX_EQUAL, 0, GX_AOP_AND, GX_EQUAL, 0);
    GXSetDstAlpha(GX_TRUE, 0);
    _300->load(GX_TEXMAP0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    sendZsortedVtx(true);
    if (_6E) {
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, _6E * 3);
        for (u32 i = 0; i < _6E; i++) {
            for (u32 j = 0; j < 3; j++) {
                const TVec3f& point = *_32C[_70[i]].getPos(j);
                GXPosition3f32(point.x, point.y, point.z);
                GXTexCoord2f32(-1.0f, -1.0f);
            }
        }
    }
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetAlphaUpdate(GX_TRUE);
    GXSetColorUpdate(GX_TRUE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTALPHA, GX_BL_INVDSTALPHA, GX_LO_NOOP);
    GXSetAlphaCompare(GX_NEQUAL, 0, GX_AOP_AND, GX_NEQUAL, 0);
    GXSetMisc(GX_MT_XF_FLUSH, 8);
    for (u32 i = 0; i < _54; i++) {
        GXSetColorUpdate(GX_FALSE);
        GXSetDstAlpha(GX_FALSE, 0);
        if (_5A == 4) {
            GXBegin(GX_QUADS, GX_VTXFMT0, _5A);
        } else {
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, _5A);
        }
        for (u32 j = 0; j < _5A; j++) {
            const TVec3f& point = getDrawPos(i, j);
            const TVec2f& tex = getDrawTx(i, j);
            GXPosition3f32(point.x, point.y, point.z);
            GXTexCoord2f32(tex.x, tex.y);
        }
        GXSetColorUpdate(GX_TRUE);
        GXSetDstAlpha(GX_TRUE, 0);
        if (_5A == 4) {
            GXBegin(GX_QUADS, GX_VTXFMT0, _5A);
        } else {
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, _5A);
        }
        for (u32 j = 0; j < _5A; j++) {
            const TVec3f& point = getDrawPos(i, j);
            const TVec2f& tex = getDrawTx(i, j);
            GXPosition3f32(point.x, point.y, point.z);
            GXTexCoord2f32(tex.x, tex.y);
        }
    }
    GXSetMisc(GX_MT_XF_FLUSH, 0);
    GXSetDstAlpha(GX_FALSE, 0);
}

void CollisionShadow::createDL() {
    u8 buffer[512] ATTRIBUTE_ALIGN(32);
    GDLObj displayList;
    MR::ProhibitSchedulerAndInterrupts guard(false);
    GDInitGDLObj(&displayList, buffer, sizeof(buffer));
    GDSetCurrent(&displayList);
    GDSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GDSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GDSetTevDirect(GX_TEVSTAGE0);
    GDSetGenMode(0, 1, 1);
    GDSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_CLAMP, GX_AF_NONE);
    GDSetTevAlphaCalcAndSwap(GX_TEVSTAGE0, GX_CA_A0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV,
                             GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXColor color = {0, 0, 0, 4};
    GDSetTevColor(GX_TEVREG0, color);
    GDPadCurr32();
    u32 size = GDGetGDLObjOffset(&displayList);
    _348 = size;
    size = (size + 31) & ~31;
    _344 = new (32) u8[size];
    MR::copyMemory(_344, buffer, size);
    DCStoreRange(_344, size);
}

void CollisionShadow::drawVolume() const {
    if (_308 & 8) {
        return;
    }
    _340->setDrawView(2);
    TVec3f position(_2F0 - _24 * _30C);
    drawVolumeBox(position, _24 * _20);
    _340->mFlags.clear();
    _340->mFlags._15 = true;
    _340->directDraw(nullptr);
    GXSetDstAlpha(GX_FALSE, 0);
}

void CollisionShadow::doSortPolygons() {
    f32 distances[128];
    f32 inverseVertexCount = 0.33333334f;
    for (u32 i = 0; i < _54; i++) {
        TVec3f center(getDrawPos(0));
        center += getDrawPos(1);
        center += getDrawPos(2);
        center.scale(inverseVertexCount);
        TVec3f relative(center);
        relative.sub(_2F0);
        distances[i] = relative.dot(_24);
    }
    MR::sortSmall(_54, distances, _F0);
}

const TVec3f& CollisionShadow::getDrawPos(u32 index) const {
    return _60[_68[index]];
}

const TVec3f& CollisionShadow::getDrawPos(u32 polygon, u32 vertex) const {
    return _60[_68[vertex + _5A * _F0[polygon]]];
}

const TVec2f& CollisionShadow::getDrawTx(u32 polygon, u32 vertex) const {
    return _64[_68[vertex + _5A * _F0[polygon]]];
}
