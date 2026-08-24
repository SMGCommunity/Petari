#include "Game/MapObj/AstroDomeOrbit.hpp"
#include "Game/Map/SphereSelector.hpp"
#include "Game/MapObj/MiniatureGalaxyHolder.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/Color.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace {
    const static Vec cRotateOutermost = {20.0f, 45.0f, 0.0f};
    const static f32 cRadius[] = {4000.0f, 6200.0f, 8100.0f, 10300.0f, 12000.0f};
    const static f32 cRadiusLastDome[] = {4000.0f, 6700.0f, 9100.0f, 11800.0f};
    const static s32 cDevideNum = 64;
    const static f32 cWidth = 100.0f;
    const static f32 cHeight = 50.0f;
    const static f32 cGalaxyRotateSpeed = -0.05f;
    const static f32 cGalaxyRotateCoordOffset = 230.0f;
    const static f32 cBloomWidth = 131.0f;
    const static f32 cBloomHeight = 60.0f;

    const static Color8 cColor(0x13, 0xB1, 0xFF, 0xFF);
    const static Color8 cBloomColor(0x00, 0xB4, 0x64, 0xFF);
};  // namespace

AstroDomeOrbit::AstroDomeOrbit() : LiveActor("天文ドームの軌道"), mOrbitRadius(5000.0f), mAngle() {
}

void AstroDomeOrbit::init(const JMapInfoIter& rIter) {
    MR::connectToScene(this, -1, -1, -1, MR::DrawType_AstroDomeOrbit);

    MR::invalidateClipping(this);

    MR::createAdaptorAndConnectToDrawBloomModel("天文ドーム軌道ブルーム描画", MR::Functor_InlineC(this, &AstroDomeOrbit::drawBloom));

    makeActorDead();
}

void AstroDomeOrbit::draw() const {
    if (!MR::isValidDraw(this)) {
        return;
    }

    initDraw(::cColor);

    drawCelling(::cWidth, true, ::cHeight);
    drawCelling(::cWidth, false, ::cHeight);

    drawSide(::cWidth, true, ::cHeight);
    drawSide(::cWidth, false, ::cHeight);
}

void AstroDomeOrbit::drawBloom() const {
    if (!MR::isValidDraw(this)) {
        return;
    }

    initDraw(::cBloomColor);

    drawCelling(::cBloomWidth, true, ::cBloomHeight);
    drawCelling(::cBloomWidth, false, ::cBloomHeight);

    drawSide(::cBloomWidth, true, ::cBloomHeight);
    drawSide(::cBloomWidth, false, ::cBloomHeight);
}

void AstroDomeOrbit::setup(s32 radiusIdx) {
    s32 miniNum = MiniatureGalaxyFunction::getMiniatureGalaxyNum();
    const f32* radii = ::cRadiusLastDome;

    if (miniNum == 5) {
        radii = ::cRadius;
    }

    mOrbitRadius = radii[radiusIdx];
    mAngle = ::cGalaxyRotateCoordOffset * radiusIdx;

    if (radiusIdx >= 4) {
        mRotation.set(::cRotateOutermost);
    }
}

void AstroDomeOrbit::moveCoord() {
    mAngle = calcRepeatedRotateCoord(::cGalaxyRotateSpeed + mAngle);
}

void AstroDomeOrbit::calcGalaxyPos(TVec3f* pPos) const {
    f32 angle = calcRepeatedRotateCoord(mAngle) * TWO_PI / 360.0f;
    pPos->set< f32 >(MR::cos(angle), 0.0f, MR::sin(angle));
    pPos->mult(mOrbitRadius);
    pPos->add(*pPos, SphereSelectorFunction::getHandleTrans());

    TPos3f rotateMtx;
    SphereSelectorFunction::calcHandledRotateMtx(mRotation, &rotateMtx);
    rotateMtx.mult(*pPos, *pPos);
}

void AstroDomeOrbit::initDraw(const Color8& rColor) const {
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);

    TPos3f rotateMtx;
    SphereSelectorFunction::calcHandledRotateMtx(mRotation, &rotateMtx);
    rotateMtx.concat(MR::getCameraViewMtx(), rotateMtx);
    GXLoadPosMtxImm(rotateMtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
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

void AstroDomeOrbit::drawCelling(f32 width, bool isAddHeight, f32 height) const {
    TVec3f handleTrans;
    handleTrans.set(SphereSelectorFunction::getHandleTrans());

    if (isAddHeight) {
        handleTrans.y += (0.5f * height);
    } else {
        handleTrans.y -= (0.5f * height);
    }

    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 130);

    for (s32 i = 0; i < ::cDevideNum + 1; i++) {
        f32 angle = TWO_PI * i / ::cDevideNum;

        TVec3f rotateVec;
        rotateVec.set< f32 >(MR::cos(angle), 0.0f, MR::sin(angle));

        TVec3f negVec;
        negVec.scale(mOrbitRadius - (0.5f * width), rotateVec);
        negVec.add(handleTrans);

        TVec3f posVec;
        posVec.scale(mOrbitRadius + (0.5f * width), rotateVec);
        posVec.add(handleTrans);

        if (isAddHeight) {
            GXPosition3f32(negVec.x, negVec.y, negVec.z);
            GXPosition3f32(posVec.x, posVec.y, posVec.z);
        } else {
            GXPosition3f32(posVec.x, posVec.y, posVec.z);
            GXPosition3f32(negVec.x, negVec.y, negVec.z);
        }
    }

    GXEnd();
}

void AstroDomeOrbit::drawSide(f32 width, bool isAddWidth, f32 height) const {
    TVec3f handleTrans;
    handleTrans.set(SphereSelectorFunction::getHandleTrans());
    handleTrans.y += 0.5f * height;

    f32 scale;
    if (isAddWidth) {
        scale = mOrbitRadius + (0.5f * width);
    } else {
        scale = mOrbitRadius - (0.5f * width);
    }

    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, (::cDevideNum + 1) * 2);

    for (s32 i = 0; i < ::cDevideNum + 1; i++) {
        f32 angle = TWO_PI * i / ::cDevideNum;

        TVec3f rotateVec;
        rotateVec.set< f32 >(MR::cos(angle), 0.0f, MR::sin(angle));

        TVec3f v16;
        v16.scale(scale, rotateVec);
        v16.add(handleTrans);

        TVec3f v15;
        v15.set(v16);
        v15.y -= height;

        if (isAddWidth) {
            GXPosition3f32(v16.x, v16.y, v16.z);
            GXPosition3f32(v15.x, v15.y, v15.z);
        } else {
            GXPosition3f32(v15.x, v15.y, v15.z);
            GXPosition3f32(v16.x, v16.y, v16.z);
        }
    }

    GXEnd();
}

f32 AstroDomeOrbit::calcRepeatedRotateCoord(f32 coord) const {
    return MR::repeatDegree(coord);
}

AstroDomeOrbit::~AstroDomeOrbit() {
}
