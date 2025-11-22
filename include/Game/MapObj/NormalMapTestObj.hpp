#pragma once

#include "Game/MapObj/NormalMapBase.hpp"

class NormalMapTestObj : public NormalMapBase {
public:
    NormalMapTestObj(const char*);

    virtual ~NormalMapTestObj();
    virtual void init(const JMapInfoIter&);
};
