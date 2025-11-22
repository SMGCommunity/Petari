#pragma once

#include "Game/MapObj/NormalMapBase.hpp"

class SimpleNormalMapObj : public NormalMapBase {
public:
    SimpleNormalMapObj(const char*);
    virtual ~SimpleNormalMapObj();

    virtual void init(const JMapInfoIter&);
};
