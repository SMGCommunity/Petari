#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class SunLightArea : public AreaObj {
public:
    SunLightArea(int, const char*);
    virtual ~SunLightArea();

    virtual const char* getManagerName() const;
};
