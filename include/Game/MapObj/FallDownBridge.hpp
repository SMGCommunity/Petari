#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class FallDownBridge : public LiveActor {
public:
    FallDownBridge(const char*);
    virtual ~FallDownBridge();

private:
    u8 mPad[(0xAC) - sizeof(LiveActor)];
};
