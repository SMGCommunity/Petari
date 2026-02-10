#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class AstroDomeDemoStarter : public LiveActor {
public:
    AstroDomeDemoStarter(const char*);
    virtual ~AstroDomeDemoStarter();

private:
    u8 mPad[(0xF8) - sizeof(LiveActor)];
};
