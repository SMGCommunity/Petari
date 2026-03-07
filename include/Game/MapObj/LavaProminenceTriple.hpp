#pragma once

#include "Game/NameObj/NameObj.hpp"


class LavaProminenceTriple : public NameObj {
public:
    LavaProminenceTriple(const char*);
    virtual ~LavaProminenceTriple();

private:
    u8 mPad[(0x108) - sizeof(NameObj)];
};
