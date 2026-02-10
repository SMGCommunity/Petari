#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class Mogucchi : public LiveActor {
public:
    Mogucchi(const char*);
    virtual ~Mogucchi();

private:
    u8 mPad[(0xF4) - sizeof(LiveActor)];
};
