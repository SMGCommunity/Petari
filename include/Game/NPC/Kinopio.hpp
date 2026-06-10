#pragma once

#include "Game/NPC/NPCActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class Kinopio : public NPCActor {
public:
    Kinopio(const char*);
    virtual ~Kinopio();

private:
    u8 mPad[(0x188) - sizeof(NPCActor)];
};
