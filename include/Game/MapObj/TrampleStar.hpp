#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class TrampleStar : public LiveActor {
public:
    TrampleStar(const char*);
    virtual ~TrampleStar();

private:
    u8 mPad[(0xEC) - sizeof(LiveActor)];
};
