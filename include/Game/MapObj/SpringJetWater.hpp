#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class SpringJetWater : public LiveActor {
public:
    SpringJetWater(const char*);
    virtual ~SpringJetWater();

private:
    u8 mPad[(0x9C) - sizeof(LiveActor)];
};
