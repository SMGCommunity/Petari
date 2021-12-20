#include "Game/AreaObj/LightArea.h"
#include "Game/Util.h"

LightArea::LightArea(int type, const char *pName) : AreaObj(type, pName) {
    mPlacedZoneID = -1;
}

void LightArea::init(const JMapInfoIter &rIter) {
    AreaObj::init(rIter);
    mPlacedZoneID = MR::getPlacedZoneId(rIter);
}

LightArea::~LightArea() {

}

const char* LightArea::getManagerName() const {
    return "LightArea";
}
