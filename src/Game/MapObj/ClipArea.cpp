#include "Game/MapObj/ClipArea.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/ClipAreaHolder.hpp"
#include "Game/MapObj/ClipAreaShape.hpp"
#include "Game/Util/Color.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/gx/GXCull.h"
#include "revolution/gx/GXEnum.h"
#include "revolution/gx/GXPixel.h"
#include "revolution/gx/GXStruct.h"
#include "revolution/gx/GXTev.h"
#include "revolution/types.h"

namespace {
    Color8 sShapeColor(0, 0, 0, 4);
    Color8 sDebugShapeColor(255, 0, 0, 128);
} // namespace

ClipArea::ClipArea(const char* pName)
    : LiveActor(pName) {
    mShape = 0;
    mBaseMatrix.identity();
    MR::createClipAreaHolder();
    MR::addClipArea(this);
}

void ClipArea::init(const JMapInfoIter& rIter) {
    initBaseMatrix(rIter);
    MR::connectToScene(this, 0. - 1, -1, -1, 42);
}

void ClipArea::initBaseMatrix(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::makeMtxTR(mBaseMatrix, this);
}

void ClipArea::draw() const {
    if (MR::isActiveClipArea()) {
        GXSetTevColor(GX_TEVREG0, sShapeColor);
        GXSetColorUpdate(false);
        GXSetDstAlpha(false, 0);
        GXSetCullMode(GX_CULL_FRONT);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_COPY);
        mShape->drawVolumeShape(mBaseMatrix, mScale);
        GXSetCullMode(GX_CULL_BACK);
        GXSetBlendMode(GX_BM_SUBTRACT, GX_BL_ZERO, GX_BL_ZERO, GX_LO_COPY);
        mShape->drawVolumeShape(mBaseMatrix, mScale);
    }
}

void ClipArea::setShape(ClipAreaShape* mShape) {
    this->mShape = mShape;
}

bool ClipArea::isInArea(const TVec3f& vec, f32 f1) const {
    return mShape->isInArea(vec, f1, mBaseMatrix, mScale);
}

namespace MR {
    void setClipSphereArea(ClipArea* mArea) {
        mArea->mShape = new ClipAreaShapeSphere();
    }

    void setClipCenterBoxArea(ClipArea* mArea) {
        mArea->mShape = new ClipAreaShapeBox(0);
    }

    void setClipBottomBoxArea(ClipArea* mArea) {
        mArea->mShape = new ClipAreaShapeBox(1);
    }

    void setClipTopConeArea(ClipArea* mArea) {
        mArea->mShape = new ClipAreaShapeCone(0);
    }

    void setClipBottomConeArea(ClipArea* mArea) {
        mArea->mShape = new ClipAreaShapeCone(1);
    }
} // namespace MR

MtxPtr ClipArea::getBaseMtx() const {
    return (MtxPtr)&mBaseMatrix;
}

ClipArea::~ClipArea() {}
