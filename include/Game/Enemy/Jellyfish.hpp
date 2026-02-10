#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class Jellyfish : public LiveActor {
public:
    Jellyfish(const char*);
    virtual ~Jellyfish();

private:
    u8 mPad[(0xC0) - sizeof(LiveActor)];
};
