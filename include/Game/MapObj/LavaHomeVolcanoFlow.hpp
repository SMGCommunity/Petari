#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class LavaHomeVolcanoFlow : public LiveActor {
public:
    LavaHomeVolcanoFlow(const char*);

    /* 0x08 */ virtual ~LavaHomeVolcanoFlow();
    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x40 */ virtual void startClipped();
    /* 0x44 */ virtual void endClipped();

    void exeMove();
    void exeWait();
};
