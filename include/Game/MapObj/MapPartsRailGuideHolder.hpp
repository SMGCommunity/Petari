#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/MapObj/MapPartsRailGuideDrawer.hpp"

class LiveActor;

class MapPartsRailGuideHolder : public NameObj {
public:
    MapPartsRailGuideHolder();

    virtual ~MapPartsRailGuideHolder();
    virtual void init(const JMapInfoIter &);

    MapPartsRailGuideDrawer* createRailGuide(LiveActor *, const char *, const JMapInfoIter &);
    MapPartsRailGuideDrawer* find(s32);

    MapPartsRailGuideDrawer* mDrawers[0x40];    // 0xC
    s32 mNumRailGuides;                         // 0x10C
};