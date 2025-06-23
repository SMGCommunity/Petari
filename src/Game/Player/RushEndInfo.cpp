#include "Game/Player/RushEndInfo.hpp"

RushEndInfo::RushEndInfo(LiveActor *pActor, u32 a2, const TVec3f &a3, bool a4, u32 a5) {
    _0 = 0x12345678;
    _4 = a2;
    _8.setPS2(a3);
    _14 = a4;
    _18 = a5;
    _1C = pActor;
    _20 = 0;
}
