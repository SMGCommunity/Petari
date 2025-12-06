#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class SpinGuidanceArea : public AreaObj {
public:
    SpinGuidanceArea(int, const char*);
    virtual ~SpinGuidanceArea();

    virtual void init(const JMapInfoIter&);
};
