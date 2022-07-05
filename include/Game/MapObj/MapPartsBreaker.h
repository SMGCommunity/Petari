#pragma once

#include "Game/MapObj/MapPartsFunction.h"

class MapPartsBreaker : MapPartsFunction {
public:

    void exeReactToTornado();
    void exeBreak();

    u32 _18;
    TVec3f _1C;
    TVec3f _28;
    f32 _34;
    u8 _38;
};

namespace NrvMapPartsBreaker {
    NERVE(HostTypeWait);
    NERVE(HostTypeReactToTornado);
    NERVE(HostTypeBreak);
};