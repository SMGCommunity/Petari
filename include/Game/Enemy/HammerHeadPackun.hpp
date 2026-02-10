#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class HammerHeadPackun : public LiveActor {
public:
    HammerHeadPackun(const char*);
    virtual ~HammerHeadPackun();

private:
    u8 mPad[(0xC0) - sizeof(LiveActor)];
};
