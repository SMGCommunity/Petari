#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class SmallStone : public LiveActor {
public:
    SmallStone(const char*);
    virtual ~SmallStone();

private:
    u8 mPad[(0xA0) - sizeof(LiveActor)];
};
