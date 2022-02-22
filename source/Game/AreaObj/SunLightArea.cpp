#include "Game/AreaObj/SunLightArea.h"

SunLightArea::SunLightArea(int type, const char *pName) : AreaObj(type, pName) {

}

SunLightArea::~SunLightArea() {

}

const char *SunLightArea::getManagerName() const {
	return "SunLightArea";
}
