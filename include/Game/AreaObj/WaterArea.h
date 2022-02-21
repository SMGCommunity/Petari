#pragma once

#include "Game/AreaObj/AreaObj.h"

class WaterArea : public AreaObj {
public:
    WaterArea(int, const char *);
    virtual ~WaterArea();

    virtual void init(const JMapInfoIter &);

    s32 _3C;
};
