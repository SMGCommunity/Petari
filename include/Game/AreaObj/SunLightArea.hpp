#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class SunLightArea : public AreaObj {
public:
    SunLightArea(int, const char*);

    /* 0x28 */ virtual const char* getManagerName() const {
        return "SunLightArea";
    }
};
