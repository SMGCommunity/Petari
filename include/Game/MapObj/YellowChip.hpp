#pragma once

#include "Game/MapObj/ChipBase.hpp"

class YellowChip : public ChipBase {
public:
    YellowChip(const char*);

    /* 0x08 */ virtual ~YellowChip();
};