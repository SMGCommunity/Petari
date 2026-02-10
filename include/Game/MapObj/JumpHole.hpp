#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class JumpHole : public LiveActor {
public:
    JumpHole(const char*);
    virtual ~JumpHole();

private:
    u8 mPad[(0xE8) - sizeof(LiveActor)];
};
