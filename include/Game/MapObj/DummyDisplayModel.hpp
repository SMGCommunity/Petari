#pragma once

#include "Game/LiveActor/PartsModel.hpp"

class LodCtrl;
class NameObjArchiveListCollector;

#define ITEM_TYPE_COIN 0
#define ITEM_TYPE_KINOPIO 1
#define ITEM_TYPE_STARPIECE 4
#define ITEM_TYPE_POWER_STAR 7
#define ITEM_TYPE_GRAND_STAR 13

struct DummyDisplayModelInfo {
    const char* mName;  // 0x00
    TVec3f _4;
    u32 _10;
    const char* mAnim;     // 0x14
    bool mHasColorChange;  // 0x18
};

class DummyDisplayModel : public PartsModel {
public:
    DummyDisplayModel(LiveActor*, const DummyDisplayModelInfo*, int, s32, s32);

    virtual void init(const JMapInfoIter&);
    virtual void makeActorAppeared();
    virtual void makeActorDead();
    virtual void control();
    virtual void calcAndSetBaseMtx();

    const DummyDisplayModelInfo* mModelInfo;  // 0x9C
    s32 mItemType;                            // 0xA0
    s32 _A4;
    LodCtrl* _A8;
    bool _AC;
};

namespace MR {
    DummyDisplayModel* createDummyDisplayModel(LiveActor*, const JMapInfoIter&, s32, const TVec3f&, const TVec3f&);
    DummyDisplayModel* createDummyDisplayModel(LiveActor*, const JMapInfoIter&, const char*, s32);
    DummyDisplayModel* createDummyDisplayModel(LiveActor*, const JMapInfoIter&, MtxPtr, s32, const TVec3f&, const TVec3f&);
    DummyDisplayModel* createDummyDisplayModelCrystalItem(LiveActor*, const JMapInfoIter&, const TVec3f&, const TVec3f&);
    DummyDisplayModel* createDummyDisplayModelCrystalItem(LiveActor*, s32, const TVec3f&, const TVec3f&);
    s32 getDummyDisplayModelId(const JMapInfoIter&, s32);
    s32 getDummyDisplayModelId(const LiveActor*);
    void makeArchiveListDummyDisplayModel(NameObjArchiveListCollector*, const JMapInfoIter&);
};  // namespace MR
