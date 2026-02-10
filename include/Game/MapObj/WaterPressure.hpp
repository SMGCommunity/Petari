#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/MapObj/PressureBase.hpp"


class WaterPressure : public PressureBase {
public:
    WaterPressure(const char*);
    virtual ~WaterPressure();

private:
    u8 mPad[(0xC8) - sizeof(PressureBase)];
};
