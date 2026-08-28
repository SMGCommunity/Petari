#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class SpinGuidanceArea : public AreaObj {
public:
    SpinGuidanceArea(int, const char*);

    /* 0x08 */ virtual ~SpinGuidanceArea();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
};
