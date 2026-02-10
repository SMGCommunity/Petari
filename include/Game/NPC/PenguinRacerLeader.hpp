#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/NPC/PenguinRacer.hpp"


class PenguinRacerLeader : public PenguinRacer {
public:
    PenguinRacerLeader(const char*);
    virtual ~PenguinRacerLeader();

private:
    u8 mPad[(0x198) - sizeof(PenguinRacer)];
};
