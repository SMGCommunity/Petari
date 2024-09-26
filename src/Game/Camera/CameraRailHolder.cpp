#include "Game/Camera/CameraRailHolder.hpp"
#include "Game/Util/SceneUtil.hpp"

CameraRailHolder::ZoneRail::ZoneRail() {
    _0 = 0;
    _4 = 0;
}

CameraRailHolder::RailSet &CameraRailHolder::RailSet::operator=(const RailSet &rOther) {
    _0 = rOther._0;
    _4 = rOther._4;
    return *this;
}

/*CameraRailHolder::CameraRailHolder(const char *pName) : NameObj(pName) {
    mRails = nullptr;
    s32 nrZones = MR::getZoneNum();
    mRails = new ZoneRail[nrZones];

    for (u32 i = 0; i < MR::getZoneNum(); i++) {
        for (u32 j = 0; j < MR::getPlacedRailNum(i); i++) {
            
        }
    }
}*/

CameraRailHolder::~CameraRailHolder() {
    
}