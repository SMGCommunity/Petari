#pragma once

#include "Game/Enemy/Onimasu.hpp"

class OnimasuPivot : public Onimasu {
public:
    OnimasuPivot(const char*);
    virtual ~OnimasuPivot();

private:
    u8 mPad[(0x130) - sizeof(Onimasu)];
};
