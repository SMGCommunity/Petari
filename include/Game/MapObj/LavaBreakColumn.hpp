#pragma once

#include "Game/MapObj/AnmModelObj.hpp"

class LavaBreakColumn : public AnmModelSwitchMove {
public:
    LavaBreakColumn(const char*);
    /* 0x08 */ virtual ~LavaBreakColumn();

private:
    u8 mPad[(0xE0) - sizeof(AnmModelSwitchMove)];
};
