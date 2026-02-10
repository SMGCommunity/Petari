#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/NPC/NPCActor.hpp"


class Kinopio : public NPCActor {
public:
    Kinopio(const char*);
    virtual ~Kinopio();

private:
    u8 mPad[(0x188) - sizeof(NPCActor)];
};
