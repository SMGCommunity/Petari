#include "Game/AreaObj/SunLightArea.hpp"

SunLightArea::SunLightArea(int formType, const char* pName) : AreaObj(formType, pName) {
}

SunLightArea::~SunLightArea() {
}

const char* SunLightArea::getManagerName() const {
    return "SunLightArea";
}
