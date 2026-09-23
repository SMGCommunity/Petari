#include "Game/Player/MarineSnow.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DirectDraw.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>

void MarineSnow_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)3.0f;
}

MarineSnow::MarineSnow() : mParticleCount(16), mParticlePositions(new (32) TVec3f[mParticleCount]), mCellSize(1000.0f) {
    for (s32 idx = 0; idx < mParticleCount; idx++) {
        TVec3f randVec;
        randVec.set(MR::getRandom() * mCellSize, MR::getRandom() * mCellSize, MR::getRandom() * mCellSize);
        mParticlePositions[idx] = randVec;
    }

    mFadeStep = 0;

    mTexture = new JUTTexture(MR::loadTexFromArc("MarineSnow"), 0);
}

void MarineSnow::view() {
    if (mFadeStep < 60) {
        mFadeStep++;
    }
}

void MarineSnow::clear() {
    if (mFadeStep != 0) {
        mFadeStep--;
    }
}

void MarineSnow::draw(const TVec3f& rPosition, const TVec3f& rUp, f32 surfaceDistance) const {
    if (mFadeStep == 0) {
        return;
    }

    f32 fadeRate = static_cast< f32 >(mFadeStep) / 60.0f;

    s32 cellX = static_cast< s32 >((rPosition.x + mCellSize / 2.0f) / mCellSize);
    s32 originX = cellX * mCellSize;
    s32 cellY = static_cast< s32 >((rPosition.y + mCellSize / 2.0f) / mCellSize);
    s32 originY = cellY * mCellSize;
    s32 cellZ = static_cast< s32 >((rPosition.z + mCellSize / 2.0f) / mCellSize);
    s32 originZ = cellZ * mCellSize;

    TDDraw::setup(0, 1, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetPointSize(27, GX_TO_ZERO);

    static u8 phase = 0;
    phase = (phase + 1) & 3;
    u32 pointCount;
    u32 start = (mParticleCount / 2) * phase;
    u32 end = start + mParticleCount / 2;
    pointCount = mParticleCount;

    TVec3f cameraForward = -MR::getCamZdir();
    TVec3f cameraUp = MR::getCamYdir();

    TVec3f billboardRight;
    billboardRight.cross(cameraForward, cameraUp);
    MR::normalizeOrZero(&billboardRight);

    TVec3f billboardUp;
    billboardUp.cross(billboardRight, cameraForward);
    MR::normalizeOrZero(&billboardUp);

    TVec3f diagonalUp = (billboardRight + billboardUp) * 10.0f;
    TVec3f diagonalDown = (billboardRight - billboardUp) * 10.0f;

    TDDraw::setup(1, 1, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);

    mTexture->load(GX_TEXMAP0);

    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ONE, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);

    for (s32 i = -4; i < 4; i++) {
        for (s32 j = -4; j < 4; j++) {
            for (s32 k = -4; k < 4; k++) {
                TVec3f cellOrigin;
                cellOrigin.x = originX + i * mCellSize;
                cellOrigin.y = originY + j * mCellSize;
                cellOrigin.z = originZ + k * mCellSize;
                TVec3f surfaceOffset = cellOrigin + rUp * mCellSize - rPosition;

                if (surfaceOffset.dot(rUp) > surfaceDistance) {
                    continue;
                }

                f32 cellDistance = (i >= 0 ? i : -i) + (j >= 0 ? j : -j) + (k >= 0 ? k : -k);

                s32 particleStride = 1;
                if (cellDistance > 3.0f) {
                    particleStride = 2;
                }

                if (cellDistance > 5.0f) {
                    particleStride = 4;
                }

                if (cellDistance > 8.0f) {
                    particleStride = 8;
                }

                f32 opacity = 1.0f - cellDistance / 12.0f;
                if (opacity < 0.0f) {
                    opacity = 0.0f;
                }

                opacity *= opacity * opacity;
                u32 alpha = 242.0f * opacity * fadeRate;

                if (alpha == 0) {
                    continue;
                }

                GXColor color = {0, 0, 0, alpha};
                GXSetTevColor(GX_TEVREG0, color);

                TMtx34f mtx;
                MtxPtr pTranslationMtx = MR::tmpMtxTrans(cellOrigin);
                MR::multMtx(mtx, pTranslationMtx, MR::getCameraViewMtx());
                GXLoadPosMtxImm(mtx, 0);

                u16 lastParticle;
                u16 firstParticle;

                if (opacity > 3.0f) {
                    firstParticle = start;
                    lastParticle = end;
                } else {
                    firstParticle = 0;
                    lastParticle = pointCount;
                }

                for (u16 point = firstParticle; point < lastParticle; point += particleStride) {
                    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                    {
                        TDDraw::sendPoint(mParticlePositions[point] - diagonalDown);
                        GXTexCoord2f32(0.0f, 0.0f);

                        TDDraw::sendPoint(mParticlePositions[point] + diagonalUp);
                        GXTexCoord2f32(1.0f, 0.0f);

                        TDDraw::sendPoint(mParticlePositions[point] + diagonalDown);
                        GXTexCoord2f32(1.0f, 1.0f);

                        TDDraw::sendPoint(mParticlePositions[point] - diagonalUp);
                        GXTexCoord2f32(0.0f, 1.0f);
                    }

                    GXEnd();
                }
            }
        }
    }
}

MarineSnow* MarineSnow_FORCE_MATCH() {
    return new MarineSnow();
}
