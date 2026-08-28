#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class FallingSmallRock : public LiveActor {
public:
    FallingSmallRock(const char*);

    /* 0x08 */ virtual ~FallingSmallRock();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x40 */ virtual void startClipped();
    /* 0x44 */ virtual void endClipped();

    void exeMove();
    inline void exeWait();
};
