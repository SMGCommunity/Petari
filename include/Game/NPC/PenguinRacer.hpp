#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/NPC/NPCActor.hpp"


class PenguinRacer : public NPCActor {
public:
    PenguinRacer(const char*);
    virtual ~PenguinRacer();

private:
    u8 mPad[(0x190) - sizeof(NPCActor)];
};
