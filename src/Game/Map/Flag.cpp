#include "Game/Map/Flag.hpp"
#include "Game/Ride/SwingRopePoint.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"
#include "JSystem/JUtility/JUTTexture.hpp"
#include "revolution/gx/GXBump.h"
#include "revolution/gx/GXCull.h"
#include "revolution/gx/GXEnum.h"
#include "revolution/gx/GXGeometry.h"
#include "revolution/gx/GXLighting.h"
#include "revolution/gx/GXPixel.h"
#include "revolution/gx/GXTev.h"
#include "revolution/gx/GXTransform.h"
#include "revolution/gx/GXVert.h"
#include "revolution/types.h"

namespace {
    static const f32 sDefaultPointIntervalU = 40.0f;
    static const f32 sDefaultPointIntervalV = 40.0f;
    static const f32 sDefaultGravity = 0.1f;
    static const f32 sDefaultFrictionRateMin = 0.6f;
    static const f32 sDefaultFrictionRateMax = 1.0f;
    static const f32 sWindTimeSpeed = 5.0f;
    static const f32 sWindPosRateU = 10.0f;
    static const f32 sWindPosRateV = 10.0f;
    static const f32 sDefaultWindAccelRate = 10.0f;
    static const f32 sDefaultWindAccelMin = 0.1f;
    static const f32 sDefaultRandomWindAccelMin = 1.0f;
    static const f32 sDefaultRandomWindAccelMax = 4.0f;
    static const s32 sRandomWaitTimeMax = 2;
    static const f32 sLightBorderMin = -1.0f;
    static const f32 sLightBorderMax = 0.5f;
    static const f32 sLightColorMinNear = 200.0f;
    static const f32 sLightColorMinFar = 120.0f;
    static const f32 sLightDistanceNear = 500.0f;
    static const f32 sLightDistanceFar = 1000.0f;
    static const f32 sDefaultAlphaDistanceNear = 1000.0f;
    static const f32 sDefaultAlphaDistanceFar = 2000.0f;
    static const f32 sStickWidth = 5.0f;
    static const f32 sClippingRadiusOffset = 300.0f;
    static const s32 sSeStepMin = 36;
    static const s32 sSeStepMax = 66;
    static const s32 sRandomWaitTimeMin = 0;
}  // namespace

namespace {
    GXColor sStickColors[] = {{0x00, 0xC8, 0x00, 0x00}, {0x00, 0xDC, 0x00, 0x00}, {0x00, 0xB4, 0x00, 0x00}, {0x00, 0xC8, 0x00, 0x00}};
}  // namespace

Flag::~Flag() {
}

Flag::Flag(const char* pName)
    : LiveActor(pName), mObjName(nullptr), mDisableLighting(false), mNumPointsV(10), mNumPointsU(10), mPointIntervalU(sDefaultPointIntervalU),
      mPointIntervalV(sDefaultPointIntervalV), mFixPoints(nullptr), mUp(0.0f, 1.0f, 0.0f), mClipCenter(0.0f, 0.0f, 0.0f), mBasePos(nullptr),
      mBaseMtx(nullptr), mStickLength(0.0f), mWavePhase(0.0f), mSide(0.0f, 0.0f, 1.0f), mFront(0.0f, 0.0f, 0.0f), mIsVerticalFlag(false),
      mWaitTime(0), mGravityScale(sDefaultGravity), mFrictionRateMin(sDefaultFrictionRateMin), mFrictionRateMax(sDefaultFrictionRateMax),
      mWindAccelRate(sDefaultWindAccelRate), mWindAccelMin(sDefaultWindAccelMin), mRandomWindAccelMin(sDefaultRandomWindAccelMin),
      mRandomWindAccelMax(sDefaultRandomWindAccelMax), mLightColorMin(sLightColorMinFar), mColors(nullptr), mTexST(nullptr), mTexture(nullptr),
      mUseAlpha(false), mAlpha(0xFF), mAlphaDistanceNear(sDefaultAlphaDistanceNear), mAlphaDistanceFar(sDefaultAlphaDistanceFar), mSeStep(0),
      mDisableSound(false) {
}

void Flag::init(const JMapInfoIter& rIter) {
    // FIXME : JUTTexture ctor needs to not inline

    MR::connectToScene(this, MR::MovementType_MapObj, -1, -1, MR::DrawType_Flag);

    if (MR::isValidInfo(rIter)) {
        MR::initDefaultPos(this, rIter);
        MR::getJMapInfoArg1NoInit(rIter, &mStickLength);
        MR::getObjectName(&mObjName, rIter);
        if (MR::isEqualString(mObjName, "FlagKoopaCastle")) {
            mPointIntervalU = 1000.0f / mNumPointsU;
            mPointIntervalV = 500.0f / (mNumPointsV - 1);
        } else if (MR::isEqualString(mObjName, "FlagKoopaA")) {
            mPointIntervalU = 450.0f / mNumPointsU;
            mPointIntervalV = 275.0f / (mNumPointsV - 1);
        } else if (MR::isEqualString(mObjName, "FlagKoopaB")) {
            mPointIntervalU = 450.0f / mNumPointsU;
            mPointIntervalV = 112.5f / (mNumPointsV - 1);
        } else if (MR::isEqualString(mObjName, "FlagPeachCastleA")) {
            mNumPointsV = 5;
            mNumPointsU = 6;
            mPointIntervalU = 170.0f / mNumPointsU;
            mPointIntervalV = 145.0f / (mNumPointsV - 1);
            mAlphaDistanceNear = 200.0f;
            mAlphaDistanceFar = 500.0f;
            mIsVerticalFlag = true;
            mGravityScale = 0.5f;
            mFrictionRateMin = 0.85f;
            mFrictionRateMax = 1.0f;
            mWindAccelRate = 0.0f;
            mWindAccelMin = 0.01f;
            mRandomWindAccelMin = 0.5f;
            mRandomWindAccelMax = 1.5f;
            mDisableSound = true;
        } else if (MR::isEqualString(mObjName, "FlagPeachCastleB")) {
            mNumPointsV = 5;
            mNumPointsU = 5;
            mPointIntervalU = 500.0f / mNumPointsU;
            mPointIntervalV = 400.0f / (mNumPointsV - 1);
        } else if (MR::isEqualString(mObjName, "FlagPeachCastleC")) {
            mNumPointsV = 5;
            mNumPointsU = 5;
            mPointIntervalU = 500.0f / mNumPointsU;
            mPointIntervalV = 400.0f / (mNumPointsV - 1);
        } else if (MR::isEqualString(mObjName, "FlagRaceA")) {
            mDisableSound = true;
        }
        TVec3f front;
        MR::calcActorAxis(&front, &mUp, &mSide, this);
    }

    if (MR::isEqualString(mObjName, "FlagSurfing")) {
        mDisableLighting = true;
        mUseAlpha = true;
    }

    mFixPoints = new FixPoints[mNumPointsV];

    for (s32 idxV = 0; idxV < mNumPointsV; idxV++) {
        TVec3f pos = mUp;
        pos.scale(mStickLength + mPointIntervalV * ((mNumPointsV - 1) - idxV));
        pos.add(mPosition);
        mFixPoints[idxV].mPos.set(pos);

        mFixPoints[idxV].mPoints = new SwingRopePoint*[mNumPointsU];
        for (s32 idxU = 0; idxU < mNumPointsU; idxU++) {
            TVec3f posU = pos;
            if (mIsVerticalFlag) {
                posU.add(mGravity.scaleInline(mPointIntervalU * (idxU + 1)));
            }
            mFixPoints[idxV].mPoints[idxU] = new SwingRopePoint(posU);
        }
    }

    mColors = new Color8[mNumPointsV * (mNumPointsU + 1)];
    for (s32 idx = 0; idx < mNumPointsV * (mNumPointsU + 1); idx++) {
        mColors[idx].set(0xFF, 0xFF, 0xFF, 0xFF);
    }

    mTexST = new f32[mNumPointsV * (mNumPointsU + 1) * 2];
    s32 idx = 0;
    for (s32 idxV = 0; idxV < mNumPointsV; idxV++) {
        f32 texV = static_cast< f32 >(idxV) / static_cast< f32 >(mNumPointsV - 1);
        mTexST[idx + 0] = 0.0f;
        mTexST[idx + 1] = texV;
        idx += 2;

        for (s32 idxU = 0; idxU < mNumPointsU; idxU++) {
            mTexST[idx + 0] = static_cast< f32 >(idxU + 1) / static_cast< f32 >(mNumPointsU);
            mTexST[idx + 1] = texV;
            idx += 2;
        }
    }

    // FIXME: JUTTexture inlines
    if (mObjName != nullptr) {
        mTexture = new JUTTexture(MR::loadTexFromArc(mObjName), 0);
    } else {
        mTexture = new JUTTexture(MR::loadTexFromArc("Flag.arc", "Flag.bti"), 0);
    }

    initSound(4, false);

    if (MR::isEqualString(mObjName, "FlagKoopaCastle")) {
        mClipCenter = mUp.scaleInline(500.0f).addOperatorInLine(mPosition);
        MR::setClippingTypeSphere(this, 500.0f + sClippingRadiusOffset, &mClipCenter);
    } else if (mStickLength > 0.0f) {
        if (mBasePos == nullptr && mBaseMtx == nullptr) {
            // TODO: I dont think they use both "* 0.5f" and "/ 2"
            mClipCenter = mUp.scaleInline(mStickLength * 0.5f).addOperatorInLine(mPosition);
            MR::setClippingTypeSphere(this, mStickLength / 2 + sClippingRadiusOffset, &mClipCenter);
        } else {
            MR::setClippingTypeSphere(this, mStickLength + sClippingRadiusOffset);
        }
    }

    if (MR::tryRegisterDemoCast(this, rIter)) {
        makeActorDead();
    } else {
        MR::registerDemoSimpleCastAll(this);
        makeActorAppeared();
    }
}

void Flag::appear() {
    LiveActor::appear();

    for (s32 idxU = 0; idxU < mNumPointsU; idxU++) {
        for (s32 idxV = 0; idxV < mNumPointsV; idxV++) {
            SwingRopePoint* point = mFixPoints[idxV].mPoints[idxU];
            point->setPosAndAxis(mFixPoints[idxV].mPos, point->mSide, point->mUp, point->mFront);
        }
    }

    MR::startSound(this, "SE_OJ_FLAG_APPEAR", -1, -1);
}

void Flag::setInfoPos(const char* pObjName, const TVec3f* pPos, const TVec3f& rSide, f32 stickLength, f32 width, f32 height, s32 numPointsU,
                      s32 numPointsV, f32 /* unused */) {
    mObjName = pObjName;
    mBasePos = pPos;
    mSide.set(rSide);
    mPosition.set(*mBasePos);
    mStickLength = stickLength;

    if (numPointsU > 0) {
        mNumPointsU = numPointsU;
    }
    if (numPointsV > 0) {
        mNumPointsV = numPointsV;
    }

    if (width > 0.0f) {
        mPointIntervalU = width / mNumPointsU;
    }
    if (height > 0.0f) {
        mPointIntervalV = height / (mNumPointsV - 1);
    }
}

void Flag::movement() {
    if (!mDisableSound) {
        if (mSeStep <= 0) {
            MR::startSound(this, "SE_OJ_FLAG", -1, -1);
            mSeStep = MR::getRandom(sSeStepMin, sSeStepMax);
        } else {
            mSeStep--;
        }
    }
    mWavePhase += sWindTimeSpeed;
    updateFlag();
}

void Flag::updateFlag() {
    // FIXME: a few float, reg, and instruction order swaps
    // https://decomp.me/scratch/Jx2O3

    mFront.cross(mSide, mUp);
    if (MR::isNearZero(mFront)) {
        mFront.set(mSide);
    } else {
        MR::normalize(&mFront);
    }

    f32 camDist = mPosition.distance(MR::getCamPos());
    if (!mDisableLighting) {
        if (camDist < sLightDistanceNear) {
            mLightColorMin = sLightColorMinNear;
        } else if (camDist >= sLightDistanceFar) {
            mLightColorMin = sLightColorMinFar;

        } else {
            f32 d = (camDist - sLightDistanceNear) / (sLightDistanceFar - sLightDistanceNear);
            mLightColorMin = d * sLightColorMinFar + (1.0f - d) * sLightColorMinNear;
        }
    }

    if (mUseAlpha) {
        if (camDist < mAlphaDistanceNear) {
            mAlpha = 0;
        } else if (camDist > mAlphaDistanceFar) {
            mAlpha = 255;
        } else {
            mAlpha = ((camDist - mAlphaDistanceNear) / (mAlphaDistanceFar - mAlphaDistanceNear)) * 255;
        }
    }

    if (mBasePos != nullptr) {
        mPosition.set(*mBasePos);
        for (s32 idxV = 0; idxV < mNumPointsV; idxV++) {
            TVec3f pos = mUp;
            pos.scale(mStickLength + mPointIntervalV * ((mNumPointsV - 1) - idxV));
            pos.add(mPosition);  // FIXME: reg load order swapped
            mFixPoints[idxV].mPos.set(pos);
        }
    }

    if (mBaseMtx != nullptr) {
        mBaseMtx->getTransInline2(mPosition);
        mBaseMtx->getYDirInline2(mUp);

        for (s32 idxV = 0; idxV < mNumPointsV; idxV++) {
            TVec3f pos = mUp;
            pos.scale(mStickLength + mPointIntervalV * ((mNumPointsV - 1) - idxV));
            pos.add(mPosition);  // FIXME: reg load order swapped
            mFixPoints[idxV].mPos.set(pos);
        }
    }

    // Add gravity and side wind
    TVec3f grav = mGravity;
    grav.scale(mGravityScale);  // FIXME: reg load order swapped
    for (s32 idxV = 0; idxV < mNumPointsV; idxV++) {
        for (s32 idxU = 0; idxU < mNumPointsU; idxU++) {
            SwingRopePoint* point = mFixPoints[idxV].mPoints[idxU];  // FIXME: indexing reg swap
            point->addAccel(grav);

            TVec3f side = mSide;
            f32 wave = JMASinDegree(mWavePhase + sWindPosRateU * idxU + sWindPosRateV * idxV);
            if (wave < 0.0f) {
                wave *= -1.0f;
            }
            side.scale(mWindAccelMin + wave * mWindAccelRate);
            point->addAccel(side);
        }
    }

    if (mIsVerticalFlag) {
        if (mWaitTime <= 0) {
            // Add random wind accel at single point
            s32 pointV = MR::getRandom(0, mNumPointsV);
            s32 pointU = MR::getRandom(0, mNumPointsU);
            TVec3f wind(MR::getRandom(-1.0f, 1.0f), 0.0f, MR::getRandom(-1.0f, 1.0f));
            MR::vecKillElement(wind, mUp, &wind);
            wind.scale(MR::getRandom(mRandomWindAccelMin, mRandomWindAccelMax));
            mFixPoints[pointV].mPoints[pointU]->addAccel(wind);
            mWaitTime = MR::getRandom(sRandomWaitTimeMin, sRandomWaitTimeMax);
        } else {
            mWaitTime--;
        }

    } else {
        // Add random wind accel across whole flag
        for (s32 idxV = 0; idxV < mNumPointsV; idxV++) {
            TVec3f wind(MR::getRandom(-1.0f, 1.0f), 0.0f, MR::getRandom(-1.0f, 1.0f));
            wind.scale(MR::getRandom(mRandomWindAccelMin, mRandomWindAccelMax));
            mFixPoints[idxV].mPoints[0]->addAccel(wind);
        }
    }

    // restrict in horizontal direction
    for (s32 idxU = 0; idxU < mNumPointsU; idxU++) {
        TVec3f* pos = &mFixPoints[0].mPoints[idxU]->mPosition;
        TVec3f* vel = &mFixPoints[0].mPoints[idxU]->mVelocity;
        for (s32 idxV = 1; idxV < mNumPointsV; idxV++) {
            SwingRopePoint* point = mFixPoints[idxV].mPoints[idxU];  // FIXME: indexing reg swap
            point->restrict(*pos, mPointIntervalV, vel);
            pos = &point->mPosition;
            vel = &point->mVelocity;
        }
    }

    // restrict in vertical direction
    for (s32 idxV = 0; idxV < mNumPointsV; idxV++) {
        TVec3f* pos = &mFixPoints[idxV].mPos;
        for (s32 idxU = 0; idxU < mNumPointsU; idxU++) {
            SwingRopePoint* point = mFixPoints[idxV].mPoints[idxU];  // FIXME: indexing reg swap
            point->restrict(*pos, mPointIntervalU, nullptr);
            pos = &point->mPosition;
        }
    }

    for (s32 idxU = 0; idxU < getNumPointsU(); idxU++) {
        f32 friction =
            mFrictionRateMin + (1.0f - (static_cast< f32 >(idxU) / static_cast< f32 >(mNumPointsU - 1))) * (mFrictionRateMax - mFrictionRateMin);

        for (s32 idxV = 0; idxV < getNumPointsV(); idxV++) {
            SwingRopePoint* point = mFixPoints[idxV].mPoints[idxU];  // FIXME: indexing reg swap
            point->updatePos(friction);

            if (!mDisableLighting) {
                f32 lightColor = 255.0f * ((point->mUp.dot(mFront) - sLightBorderMin) / (sLightBorderMax - sLightBorderMin));
                u8 color = MR::clamp(lightColor, mLightColorMin, 255.0f);
                mColors[idxV * (mNumPointsU + 1) + idxU + 1].set(color, color, color, 0xFF);
            }
        }
    }
}

void Flag::draw() const {
    if (!MR::isValidDraw(this)) {
        return;
    }

    loadMaterial();

    u8 gxPosA = 0;
    u8 gxPosB = mNumPointsU + 1;
    for (s32 idxV = 0; idxV < mNumPointsV - 1; idxV++) {
        const TVec3f& posA = mFixPoints[idxV + 0].mPos;
        const TVec3f& posB = mFixPoints[idxV + 1].mPos;
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, (mNumPointsU + 1) * 2);
        GXPosition3f32(posB.x, posB.y, posB.z);
        GXPosition1x8(gxPosB);
        GXPosition1x8(gxPosB);
        GXPosition3f32(posA.x, posA.y, posA.z);
        GXPosition1x8(gxPosA);
        GXPosition1x8(gxPosA);
        gxPosB++;
        gxPosA++;

        for (s32 idxU = 0; idxU < mNumPointsU; idxU++) {
            const TVec3f& posA = mFixPoints[idxV].mPoints[idxU]->mPosition;
            const TVec3f& posB = mFixPoints[idxV + 1].mPoints[idxU]->mPosition;
            GXPosition3f32(posB.x, posB.y, posB.z);
            GXPosition1x8(gxPosB);
            GXPosition1x8(gxPosB);
            GXPosition3f32(posA.x, posA.y, posA.z);
            GXPosition1x8(gxPosA);
            GXPosition1x8(gxPosA);

            gxPosB++;
            gxPosA++;
        }
    }

    if (mStickLength > 0.0f) {
        TVec3f pos(mPosition.x, mPosition.y, mPosition.z);

        const Vec2 stickCoords[] = {{1.0f, 0.0f}, {0.0f, 1.0f}, {-1.0f, 0.0f}, {1.0f, 0.0f}};

        GXSetArray(GX_VA_CLR0, &sStickColors, ARRAY_SIZEU(sStickColors));
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 8);

        f32 length = mStickLength + mPointIntervalV * (mNumPointsV - 1);
        for (s32 idx = 0; idx < 4; idx++) {
            TVec3f v1(stickCoords[idx].x, 0.0f, stickCoords[idx].y);
            TVec3f v2(stickCoords[idx].x, 0.0f, stickCoords[idx].y);
            v1.scale(sStickWidth);
            v2.scale(sStickWidth);
            TVec3f posA = mUp.scaleInline(length);
            posA.add(v1);
            posA.add(v2);
            posA.add(mPosition);
            TVec3f posB = mPosition;
            posB.add(v1);
            posB.add(v2);

            GXPosition3f32(posA.x, posA.y, posA.z);
            GXPosition1x8(idx);
            GXPosition1x8(0);
            GXPosition3f32(posB.x, posB.y, posB.z);
            GXPosition1x8(idx);
            GXPosition1x8(0);
        }
    }
}

void Flag::loadMaterial() const {
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_INDEX8);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);

    GXSetArray(GX_VA_CLR0, mColors, 4);
    GXSetArray(GX_VA_TEX0, mTexST, 8);

    GXLoadPosMtxImm(MR::getCameraViewMtx(), 0);
    GXSetCurrentMtx(0);

    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);

    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 60, GX_FALSE, 125);
    mTexture->load(GX_TEXMAP0);

    GXSetNumIndStages(0);

    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevDirect(GX_TEVSTAGE1);
    GXSetTevDirect(GX_TEVSTAGE2);

    GXSetTevColor(GX_TEVREG0, Color8(0xFF, 0xFF, 0xFF, mAlpha));
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);

    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
    if (mAlpha == 0xFF) {
        GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    } else {
        GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    }
    GXSetZCompLoc(GX_FALSE);
    GXSetCullMode(GX_CULL_NONE);
    GXSetClipMode(GX_CLIP_ENABLE);

    GXSetFog(GX_FOG_NONE, 0.0f, 1.0f, MR::getNearZ(), MR::getFarZ(), Color8(0xFF, 0xFF, 0xFF, 0xFF));
}
