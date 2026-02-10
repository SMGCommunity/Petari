#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class Kanina : public LiveActor {
public:
    Kanina(const char*);
    virtual ~Kanina();

private:
    u8 mPad[(0xE0) - sizeof(LiveActor)];
};
