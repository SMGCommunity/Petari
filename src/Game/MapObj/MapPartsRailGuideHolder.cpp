#include "Game/MapObj/MapPartsRailGuideHolder.hpp"
#include "Game/MapObj/MapPartsRailGuideDrawer.hpp"
#include "Game/Util/JMapInfo.hpp"

MapPartsRailGuideHolder::MapPartsRailGuideHolder() : NameObj("レールガイド保持"), mNumRailGuides() {
}

MapPartsRailGuideDrawer* MapPartsRailGuideHolder::createRailGuide(LiveActor* pHost, const char* pModelName, const JMapInfoIter& rIter) {
    s32 railId = -1;
    rIter.getValue("CommonPath_ID", &railId);
    MapPartsRailGuideDrawer* pDrawer = find(railId);
    if (pDrawer == nullptr) {
        pDrawer = new MapPartsRailGuideDrawer(pHost, pModelName);
        pDrawer->init(rIter);

        u32 index = mNumRailGuides;
        mNumRailGuides++;
        mDrawers[index] = pDrawer;
    }

    return pDrawer;
}

void MapPartsRailGuideHolder::init(const JMapInfoIter& rIter) {
}

MapPartsRailGuideDrawer* MapPartsRailGuideHolder::find(s32 railId) {
    MapPartsRailGuideDrawer** pEnd = mDrawers + mNumRailGuides;
    for (MapPartsRailGuideDrawer** pDrawer = mDrawers; pDrawer != pEnd; pDrawer++) {
        if (railId == (*pDrawer)->mRailId) {
            return *pDrawer;
        }
    }

    return nullptr;
}

MapPartsRailGuideHolder::~MapPartsRailGuideHolder() {
}
