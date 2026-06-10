#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class Unizo : public LiveActor {
public:
    Unizo(const char*);
    virtual ~Unizo();

private:
    u8 mPad[(0x134) - sizeof(LiveActor)];
};
