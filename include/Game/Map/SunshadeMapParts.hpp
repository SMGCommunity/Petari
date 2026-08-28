#pragma once

#include "Game/MapObj/GeneralMapParts.hpp"

class SunshadeMapParts : public GeneralMapParts {
public:
    SunshadeMapParts(const char*);

    /* 0x08 */ virtual ~SunshadeMapParts();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x1C */ virtual void calcAnim();

    CollisionParts* mCollision;  // 0xE8
};

namespace MR {
    NameObj* createSunshadeMapParts(const char*);
};  // namespace MR
