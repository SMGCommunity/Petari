#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class IceMerameraKing;

class Meramera : public LiveActor {
public:
    Meramera(const char*);

    u8 _8C[0x138];
};
