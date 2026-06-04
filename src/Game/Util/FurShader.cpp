#include "Game/Util/FurShader.hpp"
#include "revolution/gx/GXEnum.h"
#include <JSystem/J3DGraphAnimator/J3DModelData.hpp>

CShader::CShader(const J3DModelData* pModelData, const ResTIMG* pTimg) : _4(0), mIndexArray(nullptr), mLengthMap(pTimg) {
    _1C = 0.0f;
    _8 = 0.0f;
    mIndexArray = new CIndex[pModelData->mVertexData.mVtxNum];
    _22 = 8;
    _21 = 8;
    _20 = 8;
    _23 = 0;
    _24 = 0;
    _25 = 0;

    GXVtxAttrFmtList* list = pModelData->mVertexData.mVtxAttrFmtList;

    while (list->attr != GX_VA_NULL) {
        if (list->attr == GX_VA_POS) {
            _21 = list->frac;
            _24 = list->type;
        } else if (list->attr == GX_VA_NRM) {
            _22 = list->frac;
            _25 = list->type;
        } else if (list->attr == GX_VA_TEX0) {
            _20 = list->frac;
            _23 = list->type;
        }

        list++;
    }
}

CShader::~CShader() {
    delete mIndexArray;
}

// CShader::calc

void CShader::setup(J3DModelData* pData) {
}

// CShader::makeIndexData
// CShader::checkBorderVtx

CShader::CLengthMap::CLengthMap(const ResTIMG* pTimg) {
    _0 = pTimg;
    _4 = 0;
    setLengthMap(pTimg);
}

// https://decomp.me/scratch/Ft3ZG
s32 CShader::CLengthMap::getTexelOrder(u16 a1, f32 a2, _GXTexWrapMode mode) const {
    if (a2 > 1.0f) {
        switch (mode) {
        case GX_MIRROR:
            a2 = 1.0f;
            break;
        case GX_REPEAT: {
            while (a2 > 1.0f) {
                a2 = (a2 - 1.0f);
            }

            while (a2 < 0.0f) {
                a2 = (a2 + 1.0f);
            }

            break;
        }

        case GX_CLAMP: {
            while (a2 > 1.0f) {
                a2 = (a2 - 1.0f);
            }

            while (a2 < 0.0f) {
                a2 = (a2 + 1.0f);
            }

            break;
        }
        }
    } else {
        a2 = a2;
    }

    return (a1 - 1) * a2;
}

CShader::CIndex::CIndex() {
    _0 = -1;
    _2 = -1;
}
