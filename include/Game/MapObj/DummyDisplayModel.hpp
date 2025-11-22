#pragma once

#include "Game/LiveActor/PartsModel.hpp"

class DummyDisplayModelInfo;
class NameObjArchiveListCollector;

class DummyDisplayModel : public PartsModel {
public:
    DummyDisplayModel(LiveActor*, const DummyDisplayModelInfo*, int, s32, s32);

    virtual void init(const JMapInfoIter&);
    virtual void makeActorAppeared();
    virtual void makeActorDead();
    virtual void control();
    virtual void calcAndSetBaseMtx();
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
