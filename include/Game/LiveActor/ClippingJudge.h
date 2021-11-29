#pragma once

#include "Game/NameObj/NameObj.h"
#include "JSystem/JGeometry/THex.h"

class ClippingJudge : public NameObj {
public:
    ClippingJudge(const char *);

    JGeometry::THexahedron3<f32> _C[0x9];
};