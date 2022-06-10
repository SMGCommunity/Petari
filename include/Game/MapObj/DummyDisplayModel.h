#pragma once

#include "Game/LiveActor/PartsModel.h"
#include "JSystem/JGeometry.h"

class DummyDisplayModel : public PartsModel {
public:

};

namespace MR {
    DummyDisplayModel* createDummyDisplayModelCrystalItem(LiveActor *, s32, const TVec3f &, const TVec3f &);
    DummyDisplayModel* createDummyDisplayModelCrystalItem(LiveActor *, const JMapInfoIter &, const TVec3f &, const TVec3f &);

    s32 getDummyDisplayModelId(const JMapInfoIter &, s32);

    s32 getDummyDisplayModelId(const LiveActor *);
};
