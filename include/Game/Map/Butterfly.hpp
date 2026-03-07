#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class Butterfly : public LiveActor {
public:
    Butterfly(const char*);
    virtual ~Butterfly();

private:
    u8 mPad[(0xB4) - sizeof(LiveActor)];
};
