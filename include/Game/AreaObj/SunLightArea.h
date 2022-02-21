#pragma once

#include "Game/AreaObj/AreaObj.h"

class SunLightArea : public AreaObj {
public:
    SunLightArea(int, const char *);
    virtual ~SunLightArea();

    virtual const char *getManagerName() const;
};
