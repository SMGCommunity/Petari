#include "Game/AreaObj/MessageArea.hpp"
#include "Game/Util.hpp"

MessageArea::MessageArea(int formType, const char* pName) : AreaObj(formType, pName) {
    mZoneID = -1;
}

MessageArea::~MessageArea() {}

void MessageArea::init(const JMapInfoIter& rIter) {
    AreaObj::init(rIter);
    mZoneID = MR::getPlacedZoneId(rIter);
}
