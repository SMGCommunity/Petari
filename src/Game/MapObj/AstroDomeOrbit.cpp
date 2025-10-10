#include "Game/MapObj/AstroDomeOrbit.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/MiniatureGalaxyHolder.hpp"
#include "Game/Util/Color.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"
#include "Game/Map/SphereSelector.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "revolution/gx/GXEnum.h"
#include "revolution/gx/GXGeometry.h"
#include "revolution/gx/GXVert.h"


namespace {
    static f32 cRotateOutermost[3] = {
        20.0f, 45.0f, 0.0f
    };

    static f32 cRadius[5] = {
        4000.0f, 6200.0f, 8100.0f, 10300.0f, 12000.0f
    };

    static f32 cRadiusLastDome[4] = {
        4000.0f, 6700.0f, 9100.0f, 11800.0f
    };

    static Color8 cColor(0x13, 0xB1, 0xFF, 0xFF);
    static Color8 cBloomColor(0, 0xB4, 0x64, 0xFF);
};

AstroDomeOrbit::AstroDomeOrbit() : LiveActor("天文ドームの軌道") {
    _8C = 5000.0f;
    _90 = 0.0f;
}

void AstroDomeOrbit::init(const JMapInfoIter &rIter) {
    MR::connectToScene(this, -1, -1, -1, 0x1F);
    MR::invalidateClipping(this);
    MR::createAdaptorAndConnectToDrawBloomModel("天文ドーム軌道ブルーム描画", MR::Functor_Inline(this, &AstroDomeOrbit::drawBloom));
    makeActorDead();
}

void AstroDomeOrbit::draw() const {
    if (MR::isValidDraw(this)) {
        initDraw(cColor);
        drawCelling(100.0f, true, 50.0f);
        drawCelling(100.0f, false, 50.0f);
        drawSide(100.0f, true, 50.0f);
        drawSide(100.0f, false, 50.0f);
    }
}

void AstroDomeOrbit::drawBloom() const {
    if (MR::isValidDraw(this)) {
        initDraw(cBloomColor);
        drawCelling(131.0f, true, 60.0f);
        drawCelling(131.0f, false, 60.0f);
        drawSide(131.0f, true, 60.0f);
        drawSide(131.0f, false, 60.0f);
    }
}

void AstroDomeOrbit::setup(s32 radiusIdx) {
    s32 miniNum = MiniatureGalaxyFunction::getMiniatureGalaxyNum();
    f32* domes = cRadiusLastDome;

    if (radiusIdx == 5) {
        domes = cRadius;
    }

    f32 radiusFlt = radiusIdx;
    _8C = domes[radiusIdx];
    _90 = 230.0f * (radiusFlt - 4.503601774854144e15);

    if (radiusIdx >= 4) {
        f32 z = cRotateOutermost[0];
        f32 y = cRotateOutermost[1];
        f32 x = cRotateOutermost[2];
        
        mRotation.set(z, y, x);
    }
}

void AstroDomeOrbit::moveCoord() {
    _90 = calcRepeatedRotateCoord(-0.050000001f + _90);
}

void AstroDomeOrbit::calcGalaxyPos(TVec3f *pPos) const {
    f32 v4 = ((6.2831855f * calcRepeatedRotateCoord(_90) / 360.0f));
    f32 v5 = MR::sin(v4);
    f32 v6 = MR::cos(v4);
    pPos->set<f32>(v6, 0.0f, v5);
    pPos->x *= _8C;
    pPos->y *= _8C;
    pPos->z *= _8C;
    JMathInlineVEC::PSVECAdd(pPos, &SphereSelectorFunction::getHandleTrans(), pPos);
    TPos3f rotateMtx;
    SphereSelectorFunction::calcHandledRotateMtx(mRotation, &rotateMtx);
    rotateMtx.mult(*pPos, *pPos);
}

void AstroDomeOrbit::initDraw(const Color8 &rColor) const {
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    TPos3f v9;
    SphereSelectorFunction::calcHandledRotateMtx(mRotation, &v9);
    v9.concat(*(TMtx34f*)MR::getCameraViewMtx(), v9);
    GXLoadPosMtxImm(v9.mMtx, 0);
    GXSetCurrentMtx(0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetChanMatColor(GX_COLOR0A0, rColor);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1u);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetZMode(1u, GX_LEQUAL, 0);
    GXSetZCompLoc(0);
    GXSetCullMode(GX_CULL_BACK);
}

void AstroDomeOrbit::drawCelling(f32 a2, bool a3, f32 a4) const {
    TVec3f v16;
    v16.set<f32>(SphereSelectorFunction::getHandleTrans());

    if (a3) {
        v16.y += (0.5f * a4);
    }
    else {
        v16.y -= (0.5f * a4);
    }

    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 0x82);

    for (s32 i = 0; i < 65; i++) {
        f32 v10 = 6.2831855f * (i * 0.015625f);
        f32 v11 = MR::sin(v10);
        f32 v12 = MR::cos(v10);

        TVec3f v15;
        v15.set<f32>(v12, 0.0f, v11);
        TVec3f v14;
        v14.scale(_8C - (0.5f * a2), v15);
        JMathInlineVEC::PSVECAdd(&v14, &v16, &v14);
        TVec3f v13;
        v13.scale(_8C + (0.5f * a2), v15);
        JMathInlineVEC::PSVECAdd(&v13, &v16, &v13);

        if (a3) {
            GXPosition3f32(v14.x, v14.y, v14.z);
            GXPosition3f32(v13.x, v13.y, v13.z);
        }
        else {
            GXPosition3f32(v13.x, v13.y, v13.z);
            GXPosition3f32(v14.x, v14.y, v14.z);
        }
    }
}

void AstroDomeOrbit::drawSide(f32 a2, bool a3, f32 a4) const {
    TVec3f v18;
    v18.set<f32>(SphereSelectorFunction::getHandleTrans());
    v18.y += 0.5f * a4;

    f32 v9;
    if (a3) {
        v9 = _8C + (0.5f * a2);
    }
    else {
        v9 = _8C - (0.5f * a2);
    }

    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 0x82);

    for (s32 i = 0; i < 65; i++) {
        f32 v11 = 6.2831855f * (i * 0.015625f);
        f32 v12 = MR::sin(v11);
        f32 v13 = MR::cos(v11);
        TVec3f v17;
        v17.set<f32>(v13, 0.0f, v12);
        TVec3f v16;
        v16.scale(v9, v17);
        JMathInlineVEC::PSVECAdd(&v16, &v18, &v16);
        TVec3f v15;
        v15.set<f32>(v16);

        f32 v14 = (v15.y - a4);
        v15.y -= a4;

        if (a3) {
            GXPosition3f32(v16.x, v16.y, v16.z);
            GXPosition3f32(v15.x, v15.y, v15.z);
        }
        else {
            GXPosition3f32(v15.x, v14, v15.z);
            GXPosition3f32(v16.x, v16.y, v16.z);
        }
    }
}

f32 AstroDomeOrbit::calcRepeatedRotateCoord(f32 coord) const {
    return MR::wrapAngleTowards(0.0f, coord);
}

AstroDomeOrbit::~AstroDomeOrbit() {
    
}
