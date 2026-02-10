#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class FireBubble : public LiveActor {
public:
    FireBubble(const char*);
    virtual ~FireBubble();

private:
    u8 mPad[(0xC8) - sizeof(LiveActor)];
};
