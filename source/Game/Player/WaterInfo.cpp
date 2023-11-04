#include "Game/Player/WaterInfo.h"

WaterInfo::WaterInfo() {
    clear();
}

void WaterInfo::clear() {
    _0 = 0f;
    _4 = 0f;
    _8.zero();
    _14 = 0f;
    _18 = 0f;
    _1C = 0f;
    _20 = 0f;
    _24.zero();
    _30 = -1f;
    _34.zero();
    _40 = 0;
    _44 = 0;
    _48 = 0;
    _4C = 0;
}
