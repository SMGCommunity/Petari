#pragma once

#include "Game/MapObj/GeneralMapParts.hpp"

class SunshadeMapParts : public GeneralMapParts {
public:
    SunshadeMapParts(const char*);

    virtual ~SunshadeMapParts();
    virtual void init(const JMapInfoIter&);
    virtual void calcAnim();

    /* 0xE8 */ CollisionParts* mCollision;
};

namespace MR {
    NameObj* createSunshadeMapParts(const char*);
};  // namespace MR
