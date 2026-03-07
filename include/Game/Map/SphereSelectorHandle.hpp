#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class SphereSelectorHandle : public LiveActor {
public:
    SphereSelectorHandle(const char*);
    virtual ~SphereSelectorHandle();

private:
    u8 mPad[(0x12C) - sizeof(LiveActor)];
};
