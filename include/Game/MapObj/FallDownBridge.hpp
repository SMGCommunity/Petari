#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class FallDownBridge : public LiveActor {
public:
    FallDownBridge(const char*);
    virtual ~FallDownBridge();

private:
    u8 mPad[(0xAC) - sizeof(LiveActor)];
};
