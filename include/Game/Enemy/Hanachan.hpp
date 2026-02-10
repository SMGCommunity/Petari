#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class Hanachan : public LiveActor {
public:
    Hanachan(const char*);
    virtual ~Hanachan();

private:
    u8 mPad[(0xC0) - sizeof(LiveActor)];
};
