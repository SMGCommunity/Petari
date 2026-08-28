#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class FallDownBridge : public LiveActor {
public:
    FallDownBridge(const char*);
    /* 0x08 */ virtual ~FallDownBridge();

private:
    u8 mPad[(0xAC) - sizeof(LiveActor)];
};
