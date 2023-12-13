#include "Game/MapObj/MapPartsRailGuideHolder.hpp"

MapPartsRailGuideHolder::~MapPartsRailGuideHolder() {
    
}

MapPartsRailGuideHolder::MapPartsRailGuideHolder() : NameObj("レールガイド保持") {
    mNumRailGuides = 0;
}

void MapPartsRailGuideHolder::init(const JMapInfoIter &) {

}

/*
MapPartsRailGuideDrawer* MapPartsRailGuideHolder::find(s32 id) {
    MapPartsRailGuideDrawer* drawer = mDrawers[0];
    MapPartsRailGuideDrawer* last = mDrawers[mNumRailGuides];

    while (drawer != last) {
        if (drawer->_420 == id) {
            return drawer; 
        }
        drawer++;
    }

    return 0;
}
*/