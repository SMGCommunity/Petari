#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class Balloonfish : public LiveActor {
public:
    Balloonfish(const char*);
    virtual ~Balloonfish();

private:
    u8 mPad[(0xAC) - sizeof(LiveActor)];
};
