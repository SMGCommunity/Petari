#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class CutBushModelObj : public LiveActor {
public:
    CutBushModelObj(const char*, const char*, bool, MtxPtr);

    /* 0x08 */ virtual ~CutBushModelObj();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x4C */ virtual void calcAndSetBaseMtx();

    /* 0x8C */ MtxPtr mMtx;
};
