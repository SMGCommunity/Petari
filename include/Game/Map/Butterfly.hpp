#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class Butterfly : public LiveActor {
public:
    Butterfly(const char*);
    virtual ~Butterfly();

private:
    u8 mPad[(0xB4) - sizeof(LiveActor)];
};
