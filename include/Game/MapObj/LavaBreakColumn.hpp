#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/MapObj/AnmModelObj.hpp"


class LavaBreakColumn : public AnmModelSwitchMove {
public:
    LavaBreakColumn(const char*);
    virtual ~LavaBreakColumn();

private:
    u8 mPad[(0xE0) - sizeof(AnmModelSwitchMove)];
};
