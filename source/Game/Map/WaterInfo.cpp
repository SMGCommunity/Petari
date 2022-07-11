#include "Game/Map/WaterInfo.h"

WaterInfo::WaterInfo() {
    clear();
}

bool WaterInfo::isInWater() const {
    if (_40 || _44 || _48 || _4C) {
        return true;
    }
    
    return false;
}

void WaterInfo::clear() {
    _0 = 0.0f;
    _4 = 0.0f;
    _8.zero();
    _14 = 0.0f;
    _18 = 1.0f;
    _1C = 0.0f;
    _20 = 0.0f;
    _24.zero();
    _30 = -1.0f;
    _34.zero();
    _40 = 0;
    _44 = 0;
    _48 = 0;
    _4C = 0;
}