#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class BombHei : public LiveActor {
public:
    BombHei(const char*);
    virtual ~BombHei();

private:
    u8 mPad[(0xE0) - sizeof(LiveActor)];
};
