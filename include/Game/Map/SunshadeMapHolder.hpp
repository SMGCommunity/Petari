#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class SunshadeMapHolder : public NameObj {
public:
    SunshadeMapHolder();

    /* 0x08 */ virtual ~SunshadeMapHolder();

    TVec3f _C;
};

namespace MR {
    bool isInShadeFromTheSun(const TVec3f&, f32);
    void createSunshadeMapHolder();
};  // namespace MR
