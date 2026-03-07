#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class Metbo : public LiveActor {
public:
    Metbo(const char*);
    virtual ~Metbo();

private:
    u8 mPad[(0xC0) - sizeof(LiveActor)];
};
