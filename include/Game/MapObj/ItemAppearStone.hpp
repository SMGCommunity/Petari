#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ItemAppearStone : public LiveActor {
public:
    ItemAppearStone(const char*);

    /* 0x08 */ virtual ~ItemAppearStone();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x2C */ virtual void kill();
    /* 0x4C */ virtual void calcAndSetBaseMtx();

    void exeWait();
};
