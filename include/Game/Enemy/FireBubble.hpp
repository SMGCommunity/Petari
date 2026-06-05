#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class FireBubble : public LiveActor {
public:
    FireBubble(const char*);
    virtual ~FireBubble();

private:
    u8 mPad[(0xC8) - sizeof(LiveActor)];
};
