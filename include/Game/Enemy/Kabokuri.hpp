#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class Kabokuri : public LiveActor {
public:
    Kabokuri(const char*);
    virtual ~Kabokuri();

private:
    u8 mPad[(0xC0) - sizeof(LiveActor)];
};
