#pragma once

#include "Game/Enemy/Onimasu.hpp"

class OnimasuJump : public Onimasu {
public:
    OnimasuJump(const char*);
    virtual ~OnimasuJump();

private:
    u8 mPad[(0x110) - sizeof(Onimasu)];
};
