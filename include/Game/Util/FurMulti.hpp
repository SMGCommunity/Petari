#pragma once

#include <revolution/types.h>

class FurCtrl;
class FurParam;
class J3DModel;
class J3DModelData;
class LiveActor;
struct ResTIMG;

class FurMulti {
public:
    FurMulti(LiveActor*, u32);

    void setLayerDirect(u32, u32, u32, u32, FurParam*, ResTIMG*, ResTIMG*, ResTIMG*, ResTIMG*);
    void addToManager();
    void offDraw(u32);
    void onDraw(u32);

    /* 0x00 */ u8 mAddedToManager;
    /* 0x01 */ u8 _1;
    /* 0x02 */ u8 mCount;
    /* 0x04 */ u8* mShapeIndices;
    /* 0x08 */ u8* mDrawOrders;
    /* 0x0C */ u32 _C;
    /* 0x10 */ u32 _10;
    /* 0x14 */ u32 _14;
    /* 0x18 */ LiveActor* mActor;
    /* 0x1C */ J3DModel* mModel;
    /* 0x20 */ FurCtrl** mFurCtrls;
};

class FurBank {
public:
    FurBank() : mCount() {
        for (u32 i = 0; i < 32; i++) {
            mEntries[i] = nullptr;
            mLayerMasks[i] = 0;
        }
    }

    FurMulti* check(J3DModelData*, u32);
    void regist(FurMulti*, u32);

    /* 0x00 */ u32 mCount;
    /* 0x04 */ FurMulti* mEntries[32];
    /* 0x84 */ u32 mLayerMasks[32];
};

namespace MR {
    FurMulti* initMultiFur(LiveActor*, s32);
}
