#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/NPC/NPCActor.hpp"


class PenguinCoach : public NPCActor {
public:
    PenguinCoach(const char*);
    virtual ~PenguinCoach();

private:
    u8 mPad[(0x17C) - sizeof(NPCActor)];
};
