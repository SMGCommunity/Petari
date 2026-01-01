#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ItemAppearStone : public LiveActor {
public:
    ItemAppearStone(const char*);

    virtual ~ItemAppearStone();
    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual void calcAndSetBaseMtx();

    void exeWait();
};
