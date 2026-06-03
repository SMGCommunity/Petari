#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class JumpHole : public LiveActor {
public:
    JumpHole(const char*);
    virtual ~JumpHole();

private:
    u8 mPad[(0xE8) - sizeof(LiveActor)];
};
