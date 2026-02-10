#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class ItemBubble : public LiveActor {
public:
    ItemBubble(const char*);
    virtual ~ItemBubble();

private:
    u8 mPad[(0x11C) - sizeof(LiveActor)];
};
