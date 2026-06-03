#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class SphereSelectorHandle : public LiveActor {
public:
    SphereSelectorHandle(const char*);
    virtual ~SphereSelectorHandle();

private:
    u8 mPad[(0x12C) - sizeof(LiveActor)];
};
