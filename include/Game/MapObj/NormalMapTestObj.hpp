#pragma once

#include "Game/MapObj/NormalMapBase.hpp"

class NormalMapTestObj : public NormalMapBase {
public:
    NormalMapTestObj(const char*);

    /* 0x08 */ virtual ~NormalMapTestObj();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
};
