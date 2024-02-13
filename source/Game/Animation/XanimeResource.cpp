#include "Game/Animation/XanimeResource.hpp"

void XanimeGroupInfo::init()
{
    _4 = 0.f;
    mParent.animationName = "not-initialized";
    _8 = 0;
    _C = 0.f;
    _10 = 0.f;
    _14 = 0.f;
    _18 = 0;
    _1C = 0;
    _1D = 0;
    for (u32 i = 0; i < 4; i++) {
        _30[i] = 0.f;
        _20[i] = 0;
    }
    _40 = 0;
    _44 = 0;
}
