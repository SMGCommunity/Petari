#include "Game/LiveActor/ShadowSurfaceOval.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ShadowController.hpp"
#include "Game/LiveActor/ShadowSurfaceDrawer.hpp"
#include "Game/Util/Color.hpp"
#include "Game/Util/DirectDraw.hpp"
#include "Game/Util/DrawUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/gx/GXEnum.h"
#include "revolution/gx/GXLighting.h"


ShadowSurfaceOval::~ShadowSurfaceOval() {
}

ShadowSurfaceOval::ShadowSurfaceOval()
    : ShadowSurfaceDrawer("影描画[水面ボックス]"), mModelData(), mSize(100.0f, 100.0f, 200.0f), mColor(0, 0, 0, 64) {
    mModelData = MR::getJ3DModelData("ShadowVolumeSphere");
}

void ShadowSurfaceOval::setColor(Color8 color) {
    mColor.set(color);
}

void ShadowSurfaceOval::setAlpha(u8 alpha) {
    mColor.a = alpha;
}

void ShadowSurfaceOval::setSize(const TVec3f& pSize) {
    mSize = pSize;
}

void ShadowSurfaceOval::draw() const {
    MtxPtr baseMtx;
    ShadowController* controller = getController();
    baseMtx = controller->_18;

    if (!controller->isProjected() || !controller->isDraw()) {
        return;
    }

    f32 f1 = 100.0f;
    f32 f2 = 1.0f;
    TVec3f vec(mSize);
    vec.scale(f2 / f1);

    if (controller->isFollowHostScale()) {
        vec *= controller->getHost()->mScale;
    }

    TVec3f v23, v22;
    controller->getProjectionPos(&v23);
    controller->getProjectionNormal(&v22);
    GXSetChanMatColor(GX_COLOR0A0, mColor);
    TPos3f pos;
    pos.setInline(baseMtx);
    MR::preScaleMtx(pos, mSize);
    MR::flattenMtx(pos, v22);
    pos.setTrans(v23);
    TDDraw::setModelMtx(pos.toMtxPtr());
    MR::drawSimpleModel(mModelData);
    MR::preScaleMtx(pos, 0.9f);
    TDDraw::setModelMtx(pos.toMtxPtr());
    MR::drawSimpleModel(mModelData);
    MR::preScaleMtx(pos.toMtxPtr(), 0.9f);
    TDDraw::setModelMtx(pos.toMtxPtr());
    MR::drawSimpleModel(mModelData);
    GXSetChanMatColor(GX_COLOR0A0, Color8(128));
}
