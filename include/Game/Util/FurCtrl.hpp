#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Util/FurParam.hpp"
#include <JSystem/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <cstring>

class CShader;
class FurBank;
class FurDrawer;
class LiveActor;
struct ResTIMG;

class J3DModel2 : public J3DModel {
public:
    J3DModel2(J3DModel* pModel);

    virtual ~J3DModel2();

    void setVtxShader(J3DVtxShader* pShader, s32 componentSize) {
        mUnkCalc1 = pShader;

        if (pShader != nullptr) {
            mUnkCalc1->setup(getModelData());

            if (mVertexBuffer.mTransformedVtxPosArray[0] == nullptr || mVertexBuffer.mTransformedVtxPosArray[1] == nullptr) {
                u32 size = (componentSize * (getVertexBuffer()->getVertexData()->getVtxNum() * 3) + 31) & ~31;
                mVertexBuffer.mTransformedVtxPosArray[0] = new (32) u8[size];

                if (mVertexBuffer.mTransformedVtxPosArray[0] != nullptr) {
                    mVertexBuffer.mTransformedVtxPosArray[1] = mVertexBuffer.mTransformedVtxPosArray[0];
                }
            }

            for (s32 j = 0; j < 2; j++) {
                s32 componentBytes = componentSize * getVertexBuffer()->getVertexData()->getVtxNum();
                memcpy(mVertexBuffer.mTransformedVtxPosArray[j], getVertexBuffer()->getVertexData()->getVtxPosArray(), componentBytes * 3);
                DCStoreRange(mVertexBuffer.mTransformedVtxPosArray[j], componentSize * (getVertexBuffer()->getVertexData()->getVtxNum() * 3));
            }
        }
    }
};

class FurCtrl {
public:
    FurCtrl(LiveActor* pActor, FurParam* pParam, bool addToManager, u8 drawOrder);

    void calcLayerForm();
    void drawFur();
    void createFurMap();
    void setupFur(J3DModel* pModel, ResTIMG* pLength, ResTIMG* pIndirect, ResTIMG* pBody, u16 shape, u8 numLayers);
    void setupFurClone(J3DModel* pModel, FurCtrl* pOther);

    /* 0x00 */ LiveActor* mActor;
    /* 0x04 */ DynamicFurParam mDynamicParam;
    /* 0x0C */ u8 mIsClone;
    /* 0x0D */ u8 _D;
    /* 0x0E */ u8 _E;
    /* 0x0F */ u8 _F;
    /* 0x10 */ J3DModel* mModel;
    /* 0x14 */ u16 mShapeIndex;
    /* 0x18 */ FurParam* mParam;
    /* 0x1C */ u8 mVisible;
    /* 0x20 */ f32 mFurUVScale;
    /* 0x24 */ u16 mNumLayers;
    /* 0x28 */ FurDrawer* mDrawer;
    /* 0x2C */ CShader* mShader;
    /* 0x30 */ J3DModel** mLayerModels;
    /* 0x34 */ ResTIMG* mLengthMap;
    /* 0x38 */ ResTIMG* mBodyMap;
    /* 0x3C */ ResTIMG* mIndirectMap;
};

class FurDrawManager : public NameObj {
public:
    FurDrawManager(u8 capacity);

    virtual ~FurDrawManager();
    virtual void draw() const;

    void add(FurCtrl* pCtrl, u8 index);

    FurBank* getFurBank() const {
        return mBank;
    }

    /* 0x0C */ u8 mCapacity;
    /* 0x0D */ u8 mCounts[2];
    /* 0x10 */ FurCtrl** mFurCtrls[2];
    /* 0x18 */ FurBank* mBank;
};

namespace MR {
    FurDrawManager* getFurDrawManager();
}
