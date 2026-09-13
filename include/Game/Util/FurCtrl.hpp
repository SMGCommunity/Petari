#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Util/FurParam.hpp"

class CShader;
class FurBank;
class FurDrawer;
class J3DModel;
class LiveActor;
struct ResTIMG;

class FurCtrl {
public:
    FurCtrl(LiveActor*, FurParam*, bool, u8);

    void calcLayerForm();
    void drawFur();
    void createFurMap();
    void setupFur(J3DModel*, ResTIMG*, ResTIMG*, ResTIMG*, u16, u8);
    void setupFurClone(J3DModel*, FurCtrl*);

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
    FurDrawManager(u8);

    virtual ~FurDrawManager();
    virtual void draw() const;

    void add(FurCtrl*, u8);

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
