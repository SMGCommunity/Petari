#include "Game/Ride/PlantStalk.hpp"
#include "Game/MapObj/PlantPoint.hpp"
#include "Game/MapObj/PlantRailInfo.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include <JSystem/JUtility/JUTTexture.hpp>
#include <revolution/gx/GXCull.h>
#include <revolution/gx/GXEnum.h>
#include <revolution/gx/GXGeometry.h>
#include <revolution/gx/GXLighting.h>
#include <revolution/gx/GXPixel.h>
#include <revolution/gx/GXTev.h>
#include <revolution/gx/GXTransform.h>
#include <revolution/gx/GXVert.h>

namespace {
    static Color8 sColorPlusZ(0xFF, 0xFF, 0xFF, 0xFF);
    static Color8 sColorPlusX(0x64, 0x64, 0x64, 0xFF);
    static Color8 sColorMinusX(0x96, 0x96, 0x96, 0xFF);
}  // namespace

PlantStalk::PlantStalk()
    : LiveActor("茎"), mNumPlantPoints(0), mPlantPoints(nullptr), mRailInfo(nullptr), mStalkLength(0.0f), mGrowthPercent(0.0f),
      mGrowthRate(MR::getRandom(15.0f, 100.0f)), mGrowthTimer(MR::getRandom(static_cast< s32 >(5), static_cast< s32 >(30))), mGrownPlantPoints(0) {}

void PlantStalk::init(const JMapInfoIter& rIter) {
    MR::connectToScene(this, -1, -1, -1, 4);

    initRailRider(rIter);
    mRailInfo = new PlantRailInfo(rIter, 30.0f);

    // this is... not how you write this but
    // otherwise the compiler tries to optimize with
    // paired single optimization
    const TVec3f* railPos = &MR::getRailPos(this);
    mPosition.x = railPos->x;
    mPosition.y = railPos->y;
    mPosition.z = railPos->z;
    mNumPlantPoints = mRailInfo->mNumPlantPoints;

    mPlantPoints = new PlantPoint*[mNumPlantPoints];

    MR::moveCoordToStartPos(this);

    f32 ratio = 1.0f / mNumPlantPoints;

    for (s32 idx = 0; idx < mNumPlantPoints; idx++) {
        s32 index = (mNumPlantPoints - 1) - idx;
        mPlantPoints[idx] = new PlantPoint(mRailInfo->mPlantPoints[index]->mPosition, mRailInfo->mPlantPoints[index]->mUp,
                                           MR::getEaseOutValue(idx * ratio, 0.45f, 1.0f, 1.0f));
    }

    PlantPoint* point = mPlantPoints[0];
    point->setAxisAndPos(point->mFront, point->mUp, point->mSide, mPosition);

    MR::invalidateClipping(this);
    makeActorAppeared();
}

void PlantStalk::draw() const {
    if (MR::isValidDraw(this)) {
        drawGrowUp();
    }
}

bool PlantStalk::updateGrowUp() {
    // issues with float to int conversion
    // https://decomp.me/scratch/pe9bX

    if (--mGrowthTimer <= 0) {
        mGrowthRate += MR::getRandom(15.0f, 30.0f);
        mGrowthTimer = MR::getRandom(static_cast< s32 >(5), static_cast< s32 >(30));
    }

    mStalkLength += mGrowthRate;

    // lol
    f32 length = mStalkLength;
    if (length <= 0.0f) {
        mStalkLength = 0.0f;
        mGrowthRate = 0.0f;
    }

    mGrowthRate *= 0.95f;
    mGrowthRate = MR::clamp(mGrowthRate, 15.0f, 100.0f);

    mGrowthPercent = mStalkLength / MR::getRailTotalLength(this);
    mGrowthPercent = MR::clamp(mGrowthPercent, 0.0f, 1.0f);

    if (mStalkLength >= MR::getRailTotalLength(this)) {
        mStalkLength = MR::getRailTotalLength(this);
        mGrowthPercent = 1.0f;
        mGrownPlantPoints = mNumPlantPoints;
        return true;
    }

    // FIXME: float to int conversion not wanting to play nice here :(
    mGrownPlantPoints = mStalkLength / 30.0f;
    mGrownPlantPoints = MR::clamp(mGrownPlantPoints, 2, mNumPlantPoints);

    TVec3f stalkPos;
    TVec3f axisY;
    f32 posAlongStalk = mStalkLength;
    mRailInfo->calcPosAndAxisY(&stalkPos, &axisY, posAlongStalk);

    TVec3f side(1.0f, 0.0f, 0.0f);
    TVec3f up;
    MR::makeAxisUpSide(&up, &side, axisY, side);
    mPlantPoints[0]->setAxisAndPos(side, axisY, up, stalkPos);

    for (s32 idx = 1; idx < mGrownPlantPoints; idx++) {
        PlantPoint* point = mPlantPoints[idx];
        PlantPoint* other = mRailInfo->mPlantPoints[(mGrownPlantPoints - 1) - idx];
        point->setAxisAndPos(other->mFront, other->mUp, other->mSide, other->mPosition);
    }

    return false;
}

void PlantStalk::calcPosAndAxisY(TVec3f* pPos, TVec3f* pAxisY, f32 lengthAlongRail) const {
    mRailInfo->calcPosAndAxisY(pPos, pAxisY, lengthAlongRail);
}

void PlantStalk::calcNearestPos(TVec3f* pRailPos, const TVec3f& rSrcPos) const {
    mRailInfo->calcNearestPointPos(pRailPos, rSrcPos, 3);
}

f32 PlantStalk::calcPlayerCoord() const {
    TVec3f playerPos(*MR::getPlayerCenterPos());
    f32 closestMag = 100000000.0;
    s32 closestPoint = 0;

    for (s32 idx = 0; idx < mGrownPlantPoints; idx++) {
        f32 mag = mPlantPoints[idx]->mPosition.squared(playerPos);

        if (mag < closestMag) {
            closestPoint = idx;
            closestMag = mag;
        }
    }

    if (mStalkLength > 15.0f) {
        f32 maxLength = MR::getRailTotalLength(this);
        return MR::clamp((((f32)(mGrownPlantPoints - closestPoint) * 30.0f) / mStalkLength) * MR::getRailTotalLength(this), 0.0f, maxLength);
    } else {
        return MR::getRailTotalLength(this);
    }
}

void PlantStalk::drawGrowUp() const {
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, mGrownPlantPoints * 2);
    for (s32 idx = 0; idx < mGrownPlantPoints; idx++) {
        PlantPoint* point = mPlantPoints[idx];
        TVec3f front(mPlantPoints[idx]->mFront);
        TVec3f side(mPlantPoints[idx]->mSide);
        front.scale(mPlantPoints[idx]->mThickness);
        side.scale(mPlantPoints[idx]->mThickness);

        GXPosition3f32(point->mPosition.x + 10.0f * side.x, point->mPosition.y + 10.0f * side.y, point->mPosition.z + 10.0f * side.z);
        GXColor1u32(::sColorPlusZ);
        GXTexCoord2f32(0.0f, idx);

        GXPosition3f32(point->mPosition.x + 10.0f * front.x - 10.0f * side.x, point->mPosition.y + 10.0f * front.y - 10.0f * side.y,
                       point->mPosition.z + 10.0f * front.z - 10.0f * side.z);
        GXColor1u32(::sColorPlusX);
        GXTexCoord2f32(1.0f, idx);
    }

    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, mGrownPlantPoints * 2);
    for (s32 idx = 0; idx < mGrownPlantPoints; idx++) {
        PlantPoint* point = mPlantPoints[idx];
        TVec3f front(mPlantPoints[idx]->mFront);
        TVec3f side(mPlantPoints[idx]->mSide);
        front.scale(mPlantPoints[idx]->mThickness);
        side.scale(mPlantPoints[idx]->mThickness);

        GXPosition3f32(point->mPosition.x - 10.0f * front.x - 10.0f * side.x, point->mPosition.y - 10.0f * front.y - 10.0f * side.y,
                       point->mPosition.z - 10.0f * front.z - 10.0f * side.z);
        GXColor1u32(::sColorMinusX);
        GXTexCoord2f32(0.0f, idx);

        GXPosition3f32(point->mPosition.x + 10.0f * side.x, point->mPosition.y + 10.0f * side.y, point->mPosition.z + 10.0f * side.z);
        GXColor1u32(::sColorPlusZ);
        GXTexCoord2f32(1.0f, idx);
    }

    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, mGrownPlantPoints * 2);
    for (s32 idx = 0; idx < mGrownPlantPoints; idx++) {
        PlantPoint* point = mPlantPoints[idx];
        TVec3f front(mPlantPoints[idx]->mFront);
        TVec3f side(mPlantPoints[idx]->mSide);
        front.scale(mPlantPoints[idx]->mThickness);
        side.scale(mPlantPoints[idx]->mThickness);

        GXPosition3f32(point->mPosition.x + 10.0f * front.x - 10.0f * side.x, point->mPosition.y + 10.0f * front.y - 10.0f * side.y,
                       point->mPosition.z + 10.0f * front.z - 10.0f * side.z);
        GXColor1u32(::sColorPlusX);
        GXTexCoord2f32(0.0f, idx);

        GXPosition3f32(point->mPosition.x - 10.0f * front.x - 10.0f * side.x, point->mPosition.y - 10.0f * front.y - 10.0f * side.y,
                       point->mPosition.z - 10.0f * front.z - 10.0f * side.z);
        GXColor1u32(::sColorMinusX);
        GXTexCoord2f32(1.0f, idx);
    }
}

PlantStalkDrawInit::PlantStalkDrawInit(const char* pName) : NameObj(pName) {
    mTexture = nullptr;
    mTexture = new JUTTexture(MR::loadTexFromArc("Plant.arc", "PlantStalk.bti"));

    MR::FunctorV0M< const PlantStalkDrawInit*, void (PlantStalkDrawInit::*)() const > preDrawFunctor(this, &PlantStalkDrawInit::initDraw);
    MR::registerPreDrawFunction(preDrawFunctor, 4);
    // The above should probably be this instead, but MR::Functor_Inline does not like consts at the moment
    // MR::registerPreDrawFunction(MR::Functor_Inline(const_cast< const PlantStalkDrawInit* >(this), &PlantStalkDrawInit::initDraw), 4);
}

void PlantStalkDrawInit::initDraw() const {
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_POS_XYZ, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_F32, 0);

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

    GXLoadPosMtxImm(MR::getCameraViewMtx(), 0);
    GXSetCurrentMtx(0);

    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetChanMatColor(GX_COLOR0A0, (GXColor){0xFF, 0x00, 0x00, 0xFF});

    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 60, GX_FALSE, 125);
    mTexture->load(GX_TEXMAP0);

    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);

    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);

    GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_ENABLE);
    GXSetZCompLoc(GX_DISABLE);
    GXSetCullMode(GX_CULL_BACK);
}
