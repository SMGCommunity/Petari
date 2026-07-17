#include "Game/Player/MarineSnow.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DirectDraw.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>

MarineSnow::MarineSnow() {
    _0 = 16;
    _4 = new (32) TVec3f[_0];

    _8 = 1000.0f;
    for (s32 idx = 0; idx < _0; idx++) {
        TVec3f randVec;
        randVec.set(MR::getRandom() * _8, MR::getRandom() * _8, MR::getRandom() * _8);
        _4[idx] = randVec;
    }

    _C = 0;

    mTexture = new JUTTexture(MR::loadTexFromArc("MarineSnow"), 0);
}

void MarineSnow::view() {
    if (_C < 60) {
        _C++;
    }
}

void MarineSnow::clear() {
    if (_C != 0) {
        _C--;
    }
}

void MarineSnow::draw(const TVec3f& rVec1, const TVec3f& rVec2, f32 myFloat) const {
    // FIXME: biiiig mess, but general structure should be correct
    // https://decomp.me/scratch/QeXwZ
    if (_C == 0) {
        return;
    }

    TDDraw::setup(0, 1, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetPointSize(27, GX_TO_ZERO);

    f32 C_60 = static_cast< f32 >(_C) / 60.0f;

    TVec3f camZDir = MR::getCamZdir();
    TVec3f camYDir = MR::getCamYdir();

    TVec3f crossnegZY;
    crossnegZY.cross(-camZDir, camYDir);
    MR::normalizeOrZero(&crossnegZY);

    TVec3f crossneg_negZY_negZ;
    crossneg_negZY_negZ.cross(-crossnegZY, -camZDir);
    MR::normalizeOrZero(&crossneg_negZY_negZ);

    TVec3f vec1C0 = (crossnegZY + crossneg_negZY_negZ) * 10.0f;
    TVec3f vec1CC = (crossnegZY - crossneg_negZY_negZ) * 10.0f;

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
                TVec3f vec1D8(i * _8, j * _8, k * _8);
                TVec3f vec1E4 = vec1D8 + rVec2 * _8 - rVec1;

                if (vec1E4.dot(rVec2) > _0) {
                    continue;
                }

                s32 totalX = -i;
                if (i >= 0) {
                    totalX = i;
                }

                s32 totalY = -j;
                if (j >= 0) {
                    totalX = j;
                }

                f32 total = totalX + totalY + (((k >> 31) ^ k) - (k >> 31));  // this is basically abs(i) + abs(j) + abs(k)

                s32 unk1 = 1;
                if (total > 3.0f) {
                    unk1 = 2;
                }

                if (total > 5.0f) {
                    unk1 = 4;
                }

                if (total > 8.0f) {
                    unk1 = 8;
                }

                f32 val = 1.0f - total / 12.0f;
                if (val < 0.0f) {
                    val = 0.0f;
                }

                u32 u_val = val * val * val * 242.0f * C_60;

                if (u_val == 0) {
                    continue;
                }

                GXColor color = {0, 0, 0, u_val};
                GXSetTevColor(GX_TEVREG0, color);

                TMtx34f mtx;
                PSMTXConcat(MR::getCameraViewMtx(), MR::tmpMtxTrans(vec1D8), mtx);
                GXLoadPosMtxImm(mtx, 0);

                u16 a;
                u16 b;

                if (val > 3.0f) {
                    a = 0;
                    b = _0;
                } else {
                    a = 1;
                    b = 2;
                }

                while (a < b) {
                    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                    {
                        TVec3f vec;
                        TDDraw::sendPoint(_4[a] - vec1CC);
                        GXPosition2f32(0.0f, 0.0f);

                        TDDraw::sendPoint(_4[a] + vec1C0);
                        GXPosition2f32(1.0f, 0.0f);

                        TDDraw::sendPoint(_4[a] + vec1CC);
                        GXPosition2f32(0.0f, 1.0f);

                        TDDraw::sendPoint(_4[a] - vec1C0);
                        GXPosition2f32(1.0f, 1.0f);
                    }
                    GXEnd();

                    a += unk1;
                }
            }
        }
    }
}
