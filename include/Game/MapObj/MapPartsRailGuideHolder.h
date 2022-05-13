#pragma once

#include "Game/NameObj/NameObj.h"
#include "Game/MapObj/MapPartsRailGuideDrawer.h"

class LiveActor;

class MapPartsRailGuideHolder : public NameObj {
public:
    MapPartsRailGuideHolder();

    virtual ~MapPartsRailGuideHolder();
    virtual void init(const JMapInfoIter &);

    MapPartsRailGuideDrawer* createRailGuide(LiveActor *, const char *, const JMapInfoIter &);
    MapPartsRailGuideDrawer* find(s32);

    MapPartsRailGuideDrawer* mDrawers[0x40];    // _C
    s32 mNumRailGuides;                         // _10C
};