#pragma once

#include "Game/MapObj/ChipBase.hpp"

class BlueChip : public ChipBase {
public:
    BlueChip(const char*);

    /* 0x08 */ virtual ~BlueChip() {};
};
