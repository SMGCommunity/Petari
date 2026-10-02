#include "Game/Ride/PlantStalk.hpp"
#include "Game/MapObj/PlantPoint.hpp"
#include "Game/MapObj/PlantRailInfo.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/Color.hpp"
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

void PlantStalk_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

s32 PlantStalk_FORCE_MATCH_CLAMP(s32 a, s32 b, s32 c) {
    return MR::clamp(a, b, c) + MR::clamp(b, c, a) + MR::clamp(c, a, b);
}

namespace {
    static const f32 sGrowSpeedMin = 15.0f;
    static const f32 sGrowSpeedMax = 100.0f;
    static const f32 sGrowSpeedFriction = 0.95f;
    static const f32 sGrowAccelMin = 15.0f;
    static const f32 sGrowAccelMax = 30.0f;
    static const s32 sGrowAccelTimeMin = 5;
    static const s32 sGrowAccelTimeMax = 30;
    static const s32 sOffsetPointNum = 3;
    static const f32 sInterval = 30.0f;
    static const f32 sWidthRateMin = 0.45f;
    static const f32 sDrawWidthX = 10.0f;
    static const f32 sDrawWidthZ = 10.0f;
    static const f32 sTexRate = 1.0f;
    static const f32 sDrawWidthLongOffsetX = ::sDrawWidthX;
    static const f32 sDrawWidthShortOffsetX = ::sDrawWidthX;
    static const f32 sDrawWidthShortOffsetZ = ::sDrawWidthZ;

    static Color8 sColorPlusZ(0xFF, 0xFF, 0xFF, 0xFF);
    static Color8 sColorPlusX(0x64, 0x64, 0x64, 0xFF);
    static Color8 sColorMinusX(0x96, 0x96, 0x96, 0xFF);
};  // namespace

PlantStalk::PlantStalk()
    : LiveActor("茎"), mNumPlantPoints(), mPlantPoints(), mRailInfo(), mStalkLength(), mGrowthPercent(),
      mGrowSpeed(MR::getRandom(::sGrowSpeedMin, ::sGrowSpeedMax)), mGrowAccelTime(MR::getRandom(::sGrowAccelTimeMin, ::sGrowAccelTimeMax)),
      mGrownPlantPoints() {
}

void PlantStalk::init(const JMapInfoIter& rIter) {
    MR::connectToScene(this, MR::MovementType_None, MR::CalcAnimType_None, MR::DrawBufferType_None, MR::DrawType_PlantStalk);

    initRailRider(rIter);
    mRailInfo = new PlantRailInfo(rIter, 30.0f);

    mPosition.set(MR::getRailPos(this));
    mNumPlantPoints = mRailInfo->mNumPlantPoints;

    mPlantPoints = new PlantPoint*[mNumPlantPoints];

    MR::moveCoordToStartPos(this);

    f32 ratio = 1.0f / mNumPlantPoints;

    for (s32 idx = 0; idx < mNumPlantPoints; idx++) {
        s32 index = (mNumPlantPoints - 1) - idx;
        mPlantPoints[idx] = new PlantPoint(mRailInfo->mPlantPoints[index]->mPosition, mRailInfo->mPlantPoints[index]->mUp,
                                           MR::getEaseOutValue(idx * ratio, ::sWidthRateMin, 1.0f, 1.0f));
    }

    PlantPoint* point = mPlantPoints[0];
    point->setAxisAndPos(point->mSide, point->mUp, point->mFront, mPosition);

    MR::invalidateClipping(this);
    makeActorAppeared();
}

void PlantStalk::draw() const {
    if (MR::isValidDraw(this)) {
        drawGrowUp();
    }
}

bool PlantStalk::updateGrowUp() {
    mGrowAccelTime--;
    if (mGrowAccelTime <= 0) {
        mGrowSpeed += MR::getRandom(::sGrowAccelMin, ::sGrowAccelMax);
        mGrowAccelTime = MR::getRandom(::sGrowAccelTimeMin, ::sGrowAccelTimeMax);
    }

    mStalkLength += mGrowSpeed;

    f32 length = mStalkLength;
    if (length <= 0.0f) {
        mStalkLength = 0.0f;
        mGrowSpeed = 0.0f;
    }

    mGrowSpeed *= ::sGrowSpeedFriction;
    mGrowSpeed = MR::clamp(mGrowSpeed, ::sGrowSpeedMin, ::sGrowSpeedMax);

    mGrowthPercent = mStalkLength / MR::getRailTotalLength(this);
    mGrowthPercent = MR::clamp(mGrowthPercent, 0.0f, 1.0f);

    if (mStalkLength >= MR::getRailTotalLength(this)) {
        mStalkLength = MR::getRailTotalLength(this);
        mGrowthPercent = 1.0f;
        mGrownPlantPoints = mNumPlantPoints;
        return true;
    }

    mGrownPlantPoints = mStalkLength / ::sInterval;
    (void)static_cast< f32 >(mGrownPlantPoints);
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
        point->setAxisAndPos(other->mSide, other->mUp, other->mFront, other->mPosition);
    }

    return false;
}

void PlantStalk::calcPosAndAxisY(TVec3f* pPos, TVec3f* pAxisY, f32 lengthAlongRail) const {
    mRailInfo->calcPosAndAxisY(pPos, pAxisY, lengthAlongRail);
}

void PlantStalk::calcNearestPos(TVec3f* pRailPos, const TVec3f& rSrcPos) const {
    mRailInfo->calcNearestPointPos(pRailPos, rSrcPos, ::sOffsetPointNum);
}

f32 PlantStalk::calcPlayerCoord() const {
    TVec3f playerPos = *MR::getPlayerCenterPos();
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
        return MR::clamp((((f32)(mGrownPlantPoints - closestPoint) * ::sInterval) / mStalkLength) * MR::getRailTotalLength(this), 0.0f, maxLength);
    } else {
        return MR::getRailTotalLength(this);
    }
}

void PlantStalk::drawGrowUp() const {
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, mGrownPlantPoints * 2);
    for (s32 idx = 0; idx < mGrownPlantPoints; idx++) {
        PlantPoint* point = mPlantPoints[idx];
        TVec3f side(mPlantPoints[idx]->mSide);
        TVec3f front(mPlantPoints[idx]->mFront);
        side.scale(mPlantPoints[idx]->mThickness);
        front.scale(mPlantPoints[idx]->mThickness);

        GXPosition3f32(point->mPosition.x + ::sDrawWidthLongOffsetX * front.x, point->mPosition.y + ::sDrawWidthLongOffsetX * front.y,
                       point->mPosition.z + ::sDrawWidthLongOffsetX * front.z);
        GXColor1u32(::sColorPlusZ);
        GXTexCoord2f32(0.0f, idx * ::sTexRate);

        GXPosition3f32(point->mPosition.x + ::sDrawWidthShortOffsetX * side.x - ::sDrawWidthShortOffsetZ * front.x,
                       point->mPosition.y + ::sDrawWidthShortOffsetX * side.y - ::sDrawWidthShortOffsetZ * front.y,
                       point->mPosition.z + ::sDrawWidthShortOffsetX * side.z - ::sDrawWidthShortOffsetZ * front.z);
        GXColor1u32(::sColorPlusX);
        GXTexCoord2f32(1.0f, idx * ::sTexRate);
    }

    GXEnd();

    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, mGrownPlantPoints * 2);
    for (s32 idx = 0; idx < mGrownPlantPoints; idx++) {
        PlantPoint* point = mPlantPoints[idx];
        TVec3f side(mPlantPoints[idx]->mSide);
        TVec3f front(mPlantPoints[idx]->mFront);
        side.scale(mPlantPoints[idx]->mThickness);
        front.scale(mPlantPoints[idx]->mThickness);

        GXPosition3f32(point->mPosition.x - ::sDrawWidthShortOffsetX * side.x - ::sDrawWidthShortOffsetZ * front.x,
                       point->mPosition.y - ::sDrawWidthShortOffsetX * side.y - ::sDrawWidthShortOffsetZ * front.y,
                       point->mPosition.z - ::sDrawWidthShortOffsetX * side.z - ::sDrawWidthShortOffsetZ * front.z);
        GXColor1u32(::sColorMinusX);
        GXTexCoord2f32(0.0f, idx * ::sTexRate);

        GXPosition3f32(point->mPosition.x + ::sDrawWidthLongOffsetX * front.x, point->mPosition.y + ::sDrawWidthLongOffsetX * front.y,
                       point->mPosition.z + ::sDrawWidthLongOffsetX * front.z);
        GXColor1u32(::sColorPlusZ);
        GXTexCoord2f32(1.0f, idx * ::sTexRate);
    }

    GXEnd();

    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, mGrownPlantPoints * 2);
    for (s32 idx = 0; idx < mGrownPlantPoints; idx++) {
        PlantPoint* point = mPlantPoints[idx];
        TVec3f side(mPlantPoints[idx]->mSide);
        TVec3f front(mPlantPoints[idx]->mFront);
        side.scale(mPlantPoints[idx]->mThickness);
        front.scale(mPlantPoints[idx]->mThickness);

        GXPosition3f32(point->mPosition.x + ::sDrawWidthShortOffsetX * side.x - ::sDrawWidthShortOffsetZ * front.x,
                       point->mPosition.y + ::sDrawWidthShortOffsetX * side.y - ::sDrawWidthShortOffsetZ * front.y,
                       point->mPosition.z + ::sDrawWidthShortOffsetX * side.z - ::sDrawWidthShortOffsetZ * front.z);
        GXColor1u32(::sColorPlusX);
        GXTexCoord2f32(0.0f, idx * ::sTexRate);

        GXPosition3f32(point->mPosition.x - ::sDrawWidthShortOffsetX * side.x - ::sDrawWidthShortOffsetZ * front.x,
                       point->mPosition.y - ::sDrawWidthShortOffsetX * side.y - ::sDrawWidthShortOffsetZ * front.y,
                       point->mPosition.z - ::sDrawWidthShortOffsetX * side.z - ::sDrawWidthShortOffsetZ * front.z);
        GXColor1u32(::sColorMinusX);
        GXTexCoord2f32(1.0f, idx * ::sTexRate);
    }

    GXEnd();
}

PlantStalkDrawInit::PlantStalkDrawInit(const char* pName) : NameObj(pName) {
    mTexture = nullptr;
    mTexture = new JUTTexture(MR::loadTexFromArc("Plant.arc", "PlantStalk.bti"), 0);

    MR::registerPreDrawFunction(MR::Functor(this, &PlantStalkDrawInit::initDraw), MR::DrawType_PlantStalk);
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
