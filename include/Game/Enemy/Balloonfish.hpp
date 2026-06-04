#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class Balloonfish : public LiveActor {
public:
    Balloonfish(const char*);
    virtual ~Balloonfish();

private:
    u8 mPad[(0xAC) - sizeof(LiveActor)];
};
