#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class CutBushModelObj : public LiveActor {
public:
    CutBushModelObj(const char*, const char*, bool, MtxPtr);

    virtual ~CutBushModelObj();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void calcAndSetBaseMtx();

    MtxPtr mMtx;  // 0x8C
};