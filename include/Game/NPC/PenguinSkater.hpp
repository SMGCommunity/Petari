#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/NPC/NPCActor.hpp"


class PenguinSkater : public NPCActor {
public:
    PenguinSkater(const char*);
    virtual ~PenguinSkater();

private:
    u8 mPad[(0x1D0) - sizeof(NPCActor)];
};
