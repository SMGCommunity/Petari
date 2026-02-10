#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class Shellfish : public LiveActor {
public:
    Shellfish(const char*);
    virtual ~Shellfish();

private:
    u8 mPad[(0xA8) - sizeof(LiveActor)];
};
