#include "Game/AreaObj/LightArea.hpp"
#include "Game/Util/SceneUtil.hpp"

LightArea::LightArea(int formType, const char* pName) : AreaObj(formType, pName), mPlacedZoneID(-1) {
}

void LightArea::init(const JMapInfoIter& rIter) {
    AreaObj::init(rIter);
    mPlacedZoneID = MR::getPlacedZoneId(rIter);
}

LightArea::~LightArea() {
}
