#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class JumpStand : public LiveActor {
public:
    JumpStand(const char*);
    virtual ~JumpStand();

private:
    u8 mPad[(0xC4) - sizeof(LiveActor)];
};
