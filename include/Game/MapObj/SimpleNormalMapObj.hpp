#pragma once

#include "Game/MapObj/NormalMapBase.hpp"

class SimpleNormalMapObj : public NormalMapBase {
public:
    SimpleNormalMapObj(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
};
