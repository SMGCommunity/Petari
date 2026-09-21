#include "Game/Map/WhirlPoolAccelerator.hpp"
#include "Game/Map/WaterAreaHolder.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util.hpp"
#include <JSystem/JUtility/JUTTexture.hpp>
#include <revolution/gx/GXVert.h>

void WhirlPoolAccelerator_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)3.0f;
    (void)-1.0f;
    (void)2.0f;
}

GXColor sWhirlPoolAcceleratorTevReg1 = {0x00, 0x34, 0x52, 0xFF};
GXColor sWhirlPoolAcceleratorTevReg2 = {0x7C, 0xA9, 0xBD, 0xFF};

namespace {
    extern const f32 sAlphaPointNum;
}

WhirlPoolPoint::WhirlPoolPoint(const TVec3f& rPosition, const TVec3f& rDirection, const TVec3f& rNormal, f32 radius, f32 texCoord, u8 alpha)
    : mPosition(rPosition), mSide(1.0f, 0.0f, 0.0f), mNormal(rNormal), mDirection(0.0f, 0.0f, 1.0f), mRadius(radius), mTexCoord(texCoord),
      mAlpha(alpha), _39() {
    mDirection.set(rDirection);
    mDirection.mult(-1.0f);
    mSide.cross(mNormal, mDirection);
    MR::normalize(&mSide);
    mNormal.cross(mDirection, mSide);
    MR::normalize(&mNormal);
}

WhirlPoolAccelerator::WhirlPoolAccelerator(const char* pName)
    : LiveActor(pName), mRadius(100.0f), mHeight(100.0f), mAxis(0.0f, 1.0f, 0.0f), mPointCount(), mPoints(), mRotation(),
      mTexOffsetU0(MR::getRandom()), mTexOffsetV0(MR::getRandom()), mTexture(), mClippingCenter(0.0f, 0.0f, 0.0f) {
}

void WhirlPoolAccelerator::init(const JMapInfoIter& rIter) {
    MR::connectToScene(this, MR::MovementType_MapObj, -1, -1, MR::DrawType_WhirlPoolAccelerator);
    MR::initDefaultPos(this, rIter);
    mRadius = 100.0f * mScale.x;
    mHeight = 100.0f * mScale.y;
    MR::calcActorAxisY(&mAxis, this);
    initPoints();
    mTexture = new JUTTexture(MR::loadTexFromArc("Whirlpool.arc", "Whirlpool.bti"), 0);
    initSound(4, false);
    mClippingCenter.set(mPosition);
    mClippingCenter.y += mHeight / 2.0f;
    MR::setClippingTypeSphere(this, mHeight / 2.0f, &mClippingCenter);
    WaterAreaFunction::entryWhirlPoolAccelerator(this);
    makeActorAppeared();
}

bool WhirlPoolAccelerator::calcInfo(const TVec3f& rPosition, TVec3f* pAcceleration) const {
    pAcceleration->zero();
    TVec3f relative(rPosition);
    relative.sub(mPosition);
    f32 height = mAxis.dot(relative);
    if (height < 0.0f || height > mHeight) {
        return false;
    }

    MR::vecKillElement(relative, mAxis, &relative);
    if (relative.length() > mRadius) {
        return false;
    }

    pAcceleration->cross(relative, mAxis);
    MR::normalize(pAcceleration);
    pAcceleration->scale(3.0f);
    return true;
}

void WhirlPoolAccelerator::movement() {
    mRotation = MR::repeatDegree(-10.0f + mRotation);
    mTexOffsetU0 = MR::repeat(0.01f + mTexOffsetU0, 0.0f, 1.0f);
    mTexOffsetV0 = MR::repeat(-0.025f + mTexOffsetV0, 0.0f, 1.0f);
    MR::startLevelSound(this, "SE_AT_LV_WHIRL_POOL");
}

void WhirlPoolAccelerator::initPoints() {
    mPointCount = static_cast< s32 >(mHeight / 50.0f) + 1;
    mPoints = new WhirlPoolPoint*[mPointCount];
    TPos3f rotation;
    rotation.identity();
    MR::makeMtxRotate(rotation, 0.0f, -10.0f, 0.0f);
    TVec3f normal(0.0f, 0.0f, 1.0f);

    f32 angle = 360.0f;

    for (s32 i = 0; i < mPointCount; i++) {
        TVec3f position(mAxis);
        position.scale(mHeight - 50.0f * i);
        position.add(mPosition);

        f32 rate = MR::getEaseInValue(static_cast< f32 >(mPointCount - i) / mPointCount, 0.0f, 1.0f, 1.0f);
        f32 radius = mRadius * (rate + 0.6f * (1.0f - rate));
        u8 alpha = 0xFF;
        if (i < sAlphaPointNum) {
            alpha = 50.0f + 205.0f * (i / sAlphaPointNum);
        }

        if (i >= mPointCount - sAlphaPointNum) {
            alpha = 50.0f + 205.0f * ((mPointCount - 1 - i) / sAlphaPointNum);
        }

        mPoints[i] = new WhirlPoolPoint(position, mAxis, normal, radius, angle / 360.0f, alpha);
        rotation.mult(normal, normal);
        angle -= 10.0f;
    }
}

void WhirlPoolAccelerator::draw() const {
    loadMaterial();
    f32 u0 = mTexOffsetU0;
    f32 nextU0 = 1.0f / 6.0f + u0;

    drawPlane(0.5f, 0.70710677f, -0.5f, 0.70710677f, u0, nextU0);
    u0 = nextU0;
    nextU0 += 1.0f / 6.0f;

    drawPlane(-0.5f, 0.70710677f, -1.0f, 0.0f, u0, nextU0);
    u0 = nextU0;
    nextU0 += 1.0f / 6.0f;

    drawPlane(-1.0f, 0.0f, -0.5f, -0.70710677f, u0, nextU0);
    u0 = nextU0;
    nextU0 += 1.0f / 6.0f;

    drawPlane(-0.5f, -0.70710677f, 0.5f, -0.70710677f, u0, nextU0);
    u0 = nextU0;
    nextU0 += 1.0f / 6.0f;

    drawPlane(0.5f, -0.70710677f, 1.0f, 0.0f, u0, nextU0);
    u0 = nextU0;
    nextU0 += 1.0f / 6.0f;

    drawPlane(1.0f, 0.0f, 0.5f, 0.70710677f, u0, nextU0);
}

void WhirlPoolAccelerator::drawPlane(f32 x0, f32 y0, f32 x1, f32 y1, f32 u0, f32 u1) const {
    f32 v0 = mTexOffsetV0;

    TVec3f offset(0.0f, 0.0f, 30.0f);
    TPos3f rotation;
    rotation.identity();
    MR::makeMtxRotate(rotation, 0.0f, mRotation, 0.0f);
    rotation.mult(offset, offset);
    MR::makeMtxRotate(rotation, 0.0f, 25.0f, 0.0f);
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, mPointCount * 2);
    const WhirlPoolPoint* pPoint;

    for (s32 i = 0; i < mPointCount; i++) {
        pPoint = mPoints[i];
        TVec3f side(pPoint->mSide);
        TVec3f normal(pPoint->mNormal);
        side.scale(pPoint->mRadius);
        normal.scale(pPoint->mRadius);
        TVec3f position(pPoint->mPosition);
        position.add(offset);
        rotation.mult(offset, offset);

        GXPosition3f32(position.x + side.x * x0 + normal.x * y0, position.y + side.y * x0 + normal.y * y0, position.z + side.z * x0 + normal.z * y0);
        GXColor4u8(0xFF, 0xFF, 0xFF, mPoints[i]->mAlpha);
        GXTexCoord2f32(u0 + pPoint->mTexCoord, v0);

        GXPosition3f32(position.x + side.x * x1 + normal.x * y1, position.y + side.y * x1 + normal.y * y1, position.z + side.z * x1 + normal.z * y1);
        GXColor4u8(0xFF, 0xFF, 0xFF, mPoints[i]->mAlpha);
        GXTexCoord2f32(u1 + pPoint->mTexCoord, v0);
        v0 += 0.02f;
    }

    GXEnd();
}

void WhirlPoolAccelerator::loadMaterial() const {
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXLoadPosMtxImm(MR::getCameraViewMtx(), GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    mTexture->load(GX_TEXMAP0);
    GXSetNumIndStages(0);
    GXSetNumTevStages(1);
    GXSetTevColor(GX_TEVREG0, sWhirlPoolAcceleratorTevReg1);
    GXSetTevColor(GX_TEVREG1, sWhirlPoolAcceleratorTevReg2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_RASA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetZCompLoc(GX_TRUE);
    GXSetCullMode(GX_CULL_NONE);
    GXSetClipMode(GX_CLIP_ENABLE);
}

WhirlPoolAccelerator::~WhirlPoolAccelerator() {
}

namespace {
    const f32 sAlphaPointNum = 2.0f;
}
