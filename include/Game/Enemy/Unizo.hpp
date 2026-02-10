#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class Unizo : public LiveActor {
public:
    Unizo(const char*);
    virtual ~Unizo();

private:
    u8 mPad[(0x134) - sizeof(LiveActor)];
};
