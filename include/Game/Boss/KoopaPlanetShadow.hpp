#pragma once

#include "Game/Boss/Koopa.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "revolution/mtx.h"
class KoopaPlanetShadow : public LiveActor {
public:
    KoopaPlanetShadow(const Koopa*);

    virtual void init(const JMapInfoIter&);
    virtual void calcAndSetBaseMtx();


    /* 0x8c */  const Koopa* mParent;
    /* 0x90 */  MtxPtr mParentJointMtx;
};
