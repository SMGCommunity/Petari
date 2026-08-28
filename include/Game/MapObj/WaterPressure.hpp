#pragma once

#include "Game/MapObj/PressureBase.hpp"

class WaterPressure : public PressureBase {
public:
    WaterPressure(const char*);
    /* 0x08 */ virtual ~WaterPressure();

private:
    u8 mPad[(0xC8) - sizeof(PressureBase)];
};
