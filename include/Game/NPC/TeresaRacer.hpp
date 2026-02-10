#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/NPC/NPCActor.hpp"


class TeresaRacer : public NPCActor {
public:
    TeresaRacer(const char*);
    virtual ~TeresaRacer();

private:
    u8 mPad[(0x194) - sizeof(NPCActor)];
};
