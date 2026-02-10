#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class Kiraira : public LiveActor {
public:
    Kiraira(const char*);
    virtual ~Kiraira();

private:
    u8 mPad[(0xB8) - sizeof(LiveActor)];
};
