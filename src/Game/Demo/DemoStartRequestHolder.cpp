#include "Game/Demo/DemoStartRequestHolder.hpp"

#ifdef NON_MATCHING
// reg scheduling issues, 0x40 / 0x9C are likely a part of FixedRingRuffer ctor
DemoStartRequestHolder::DemoStartRequestHolder() : _40(0), _84(&_44, &_44), _9C(0), _94(&_44, &_44) {
    _9C = 0;
    _A0 = new NameObj("代理人");

    s32 idx = 0;

    while (idx < 0x10) {
        u32 cnt = _40;
        mStartInfos[idx] = new DemoStartInfo();
        idx++;
        _40 = cnt + 1;
    }
}
#endif