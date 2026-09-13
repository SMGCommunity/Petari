#include "Game/Util/FurShader.hpp"
#include "Game/Util/MathUtil.hpp"
#include "JSystem/J3DGraphBase/J3DMaterial.hpp"
#include "JSystem/J3DGraphBase/J3DShape.hpp"
#include "JSystem/J3DGraphBase/J3DShapeDraw.hpp"
#include "JSystem/JUtility/JUTNameTab.hpp"
#include "JSystem/JUtility/JUTTexture.hpp"
#include "revolution/gx/GXEnum.h"
#include <JSystem/J3DGraphAnimator/J3DModelData.hpp>
#include <cstring>

CShader::CShader(const J3DModelData* pModelData, const ResTIMG* pTimg) : mIndexArray(), mLengthMap(pTimg) {
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

void CShader::calc(J3DModel* pModel) {
    pModel->mVertexBuffer.swapTransformedVtxPos();
    J3DVertexData* pData = pModel->mVertexBuffer.getVertexData();
    u32 count;
    void* pPositions = pModel->mVertexBuffer.mCurrentVtxPos;
    count = pData->mVtxNum;
    void* pNormals = pModel->mVertexBuffer.mCurrentVtxNrm;
    void* pTexCoords = pData->mVtxTexCoordArray[0];
    void* pOutput = pModel->mVertexBuffer.mTransformedVtxPosArray[0];
    f32 maxV;
    f32 maxU;
    f32 minV;
    f32 minU;
    minU = 0.0f;
    minV = 0.0f;
    maxU = 0.0f;
    maxV = 0.0f;

    for (u32 i = 0; i < count; i++) {
        u16 index = mIndexArray[i]._2;

        if (index != 0xFFFF) {
            TVec2s* pFixed = static_cast< TVec2s* >(pTexCoords) + index;
            TVec2f* pFloat = static_cast< TVec2f* >(pTexCoords) + index;
            f32 u;
            f32 v;

            switch (_23) {
            case GX_S16: {
                f32 scale = 1 << _20;
                u = f32(pFixed->x) / scale;
                v = f32(pFixed->y) / scale;
            } break;
            case GX_F32:
                u = pFloat->x;
                v = pFloat->y;
                break;
            }

            if (u > maxU) {
                maxU = u;
            }

            if (u < minU) {
                minU = u;
            }

            if (v > maxV) {
                maxV = v;
            }

            if (v < minV) {
                minV = v;
            }
        }
    }

    f32 rangeU = maxU - minU;
    f32 rangeV = maxV - minV;
    s32 spanU = 0.45f + rangeU;
    s32 spanV = 0.45f + rangeV;

    for (u32 i = 0; i < count; i++) {
        u16 normalIndex = mIndexArray[i]._0;
        u16 texCoordIndex = mIndexArray[i]._2;

        if (normalIndex != 0xFFFF && texCoordIndex != 0xFFFF) {
            u32 offset = i * 3;
            s32 normalOffset = normalIndex * 3;
            TVec3s* pFixedOutput = reinterpret_cast< TVec3s* >(static_cast< s16* >(pOutput) + offset);
            TVec3s* pFixedPosition = reinterpret_cast< TVec3s* >(static_cast< s16* >(pPositions) + offset);
            TVec3s* pFixedNormal = reinterpret_cast< TVec3s* >(static_cast< s16* >(pNormals) + normalOffset);
            TVec2s* pFixed = static_cast< TVec2s* >(pTexCoords) + texCoordIndex;
            TVec3f* pFloatOutput = reinterpret_cast< TVec3f* >(static_cast< f32* >(pOutput) + offset);
            TVec3f* pFloatPosition = reinterpret_cast< TVec3f* >(static_cast< f32* >(pPositions) + offset);
            TVec3f* pFloatNormal = reinterpret_cast< TVec3f* >(static_cast< f32* >(pNormals) + normalOffset);
            TVec2f* pFloat = static_cast< TVec2f* >(pTexCoords) + texCoordIndex;
            f32 length;

            switch (_23) {
            case GX_S16: {
                f32 scale = 1 << _20;
                f32 u = f32(pFixed->x) / scale;
                f32 v = f32(pFixed->y) / scale;
                u = u / f32(spanU);
                v = v / f32(spanV);

                if (u < 0.0f) {
                    u += 1.0f;
                }

                if (v < 0.0f) {
                    v += 1.0f;
                }

                length = mLengthMap.refer(u, v);
                break;
            }

            case GX_F32:
                length = mLengthMap.refer(pFloat->x, pFloat->y);
                break;
            }

            TVec3f position;
            TVec3f normal;
            TVec3f result;

            switch (_25) {
            case GX_S16:
                MR::fixed16ToFloat(&normal, *pFixedNormal, _22);
                break;
            case GX_F32:
                normal = *pFloatNormal;
                break;
            }

            MR::normalize(&normal);
            MR::isNan(normal);

            switch (_24) {
            case GX_S16:
                MR::fixed16ToFloat(&position, *pFixedPosition, _21);
                break;
            case GX_F32:
                position = *pFloatPosition;
                break;
            }

            MR::isNan(position);

            if (length == 0.0f) {
                result = position + normal * -1.0f;
            } else {
                result = position + normal * (_1C * length);
            }

            MR::isNan(result);

            switch (_24) {
            case GX_S16:
                MR::floatToFixed16(pFixedOutput, result, _21);
                break;
            case GX_F32:
                *pFloatOutput = result;
                break;
            }
        }
    }

    switch (_24) {
    case GX_S16:
        DCStoreRange(pOutput, count * sizeof(TVec3s));
        break;
    case GX_F32:
        DCStoreRange(pOutput, count * sizeof(TVec3f));
        break;
    }

    pModel->mVertexBuffer.setCurrentVtxPos(pOutput);
}

void CShader::setup(J3DModelData* pData) {
}

void CShader::makeIndexData(J3DShape* pShape) const {
    GXVtxDescList* pDesc = pShape->getVtxDesc();
    s32 stride = 0;
    s32 posOffset = -1;
    s32 nrmOffset = -1;
    s32 texOffset = -1;
    s32 sizes[4] = {0, 1, 1, 2};

    while (pDesc->attr != GX_VA_NULL) {
        switch (pDesc->attr) {
        case GX_VA_POS:
            posOffset = stride;
            break;
        case GX_VA_NRM:
            nrmOffset = stride;
            break;
        case GX_VA_TEX0:
            texOffset = stride;
            break;
        }

        stride += sizes[pDesc->type];
        pDesc++;
    }

    if (posOffset == -1) {
        return;
    }

    if (nrmOffset == -1) {
        return;
    }

    if (texOffset == -1) {
        return;
    }

    for (u16 group = 0; group < pShape->getMtxGroupNum(); group++) {
        s32 count;
        u8* pStart;
        u8* pRead;
        pStart = pShape->getShapeDraw(group)->getDisplayList();
        pRead = pStart;

        while (u32(pRead - pStart) < pShape->getShapeDraw(group)->getDisplayListSize()) {
            if (*pRead == 0) {
                break;
            }

            count = *reinterpret_cast< u16* >(pRead + 1);

            for (s32 i = 0; i < count; i++) {
                u8* pVertex = pRead + stride * i + 3;
                u16 pos = *reinterpret_cast< u16* >(pVertex + posOffset);
                u16 nrm = *reinterpret_cast< u16* >(pVertex + nrmOffset);
                u16 tex = *reinterpret_cast< u16* >(pVertex + texOffset);
                mIndexArray[pos]._0 = nrm;
                mIndexArray[pos]._2 = tex;
            }

            pRead = pRead + stride * count + 3;
        }
    }
}

void CShader::checkBorderVtx(J3DModelData* pData, u32 shapeIndex) {
    for (u32 shape = 0; shape < pData->getShapeNum(); shape++) {
        if (shape == shapeIndex) {
            continue;
        }

        J3DShape* pShape = pData->getShapeNodePointer(shape);

        if (strstr(pData->getMaterialName()->getName(pShape->mMaterial->mIndex), "Fur")) {
            continue;
        }

        GXVtxDescList* pDesc = pShape->getVtxDesc();
        s32 stride = 0;
        s32 posOffset = -1;
        s32 sizes[4] = {0, 1, 1, 2};

        while (pDesc->attr != GX_VA_NULL) {
            switch (pDesc->attr) {
            case GX_VA_POS:
                posOffset = stride;
                break;
            case GX_VA_NRM:
                break;
            case GX_VA_TEX0:
                break;
            }

            stride += sizes[pDesc->type];
            pDesc++;
        }

        if (posOffset == -1) {
            continue;
        }

        for (u16 group = 0; group < pShape->getMtxGroupNum(); group++) {
            u8* pStart = pShape->getShapeDraw(group)->getDisplayList();
            u8* pRead = pStart;

            while (u32(pRead - pStart) < pShape->getShapeDraw(group)->getDisplayListSize()) {
                if (*pRead == 0) {
                    break;
                }

                s32 count = *reinterpret_cast< u16* >(pRead + 1);

                for (s32 i = 0; i < count; i++) {
                    u16 pos = *reinterpret_cast< u16* >(pRead + stride * i + posOffset + 3);
                    mIndexArray[pos]._0 = 0xFFFF;
                    mIndexArray[pos]._2 = 0xFFFF;
                }

                pRead = pRead + stride * count + 3;
            }
        }
    }
}

CShader::CLengthMap::CLengthMap(const ResTIMG* pTimg) {
    _0 = pTimg;
    _4 = reinterpret_cast< const u8* >(pTimg) + sizeof(ResTIMG);
    setLengthMap(pTimg);
}

void CShader::CLengthMap::setLengthMap(const ResTIMG* pTimg) {
    if (!pTimg) {
        _8 = 1;
        return;
    }

    if (pTimg->mFormat != GX_TF_I8) {
        _8 = 1;
        return;
    }

    _0 = pTimg;
    _4 = reinterpret_cast< const u8* >(pTimg) + sizeof(ResTIMG);
    _8 = 0;
}

f32 CShader::CLengthMap::refer(f32 u, f32 v) const {
    if (_8) {
        return 1.0f;
    }

    if (_0->mFormat != GX_TF_I8) {
        return 1.0f;
    }

    u16 x = getTexelOrder(_0->mWidth, u, static_cast< GXTexWrapMode >(_0->mWrapS));
    u16 y = getTexelOrder(_0->mHeight, v, static_cast< GXTexWrapMode >(_0->mWrapT));
    u32 tile = x / 8 + (_0->mWidth / 8) * (y / 4);
    const u8* pTile = _4 + tile * 32;
    return f32(pTile[(x % 8) + (y % 4) * 8]) / 255.0f;
}

u16 CShader::CLengthMap::getTexelOrder(u16 a1, f32 a2, _GXTexWrapMode mode) const {
    if (a2 > 1.0f) {
        switch (mode) {
        case GX_CLAMP:
            a2 = 1.0f;
            break;
        case GX_REPEAT:
            while (a2 > 1.0f) {
                a2 -= 1.0f;
            }

            while (a2 < 0.0f) {
                a2 += 1.0f;
            }

            break;
        case GX_MIRROR:
            while (a2 > 1.0f) {
                a2 -= 1.0f;
            }

            while (a2 < 0.0f) {
                a2 += 1.0f;
            }
        }
    }

    return (a1 - 1) * a2;
}

CShader::CIndex::CIndex() {
    _0 = -1;
    _2 = -1;
}
