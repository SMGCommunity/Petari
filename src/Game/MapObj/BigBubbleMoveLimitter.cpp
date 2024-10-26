#include "Game/MapObj/BigBubbleMoveLimitter.hpp"

BigBubbleMoveLimitter::BigBubbleMoveLimitter(const char *pName, s32 a2) : NameObj(pName),
    _C(-1), _10(0, 0, 0), _1C(0, 1, 0), _28(1, 0, 0), _34(0, 1, 0), _40(0, 0, 1) {
        _5C = -1;
        _4C = 500.0f;
        _50 = 500.0f;
        _54 = 500.0f;
        _58 = 500.0f;
        _60 = a2;
    }