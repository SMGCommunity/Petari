#pragma once

#include "Game/MapObj/MapPartsFunction.h"
#include "Game/MapObj/MapPartsRailGuidePoint.h"
#include "Game/Util.h"

class MapPartsRailGuideDrawer : public MapPartsFunction {
public:
    MapPartsRailGuideDrawer(LiveActor *, const char *);

    virtual ~MapPartsRailGuideDrawer();
    virtual void init(const JMapInfoIter &);
    virtual bool isWorking() const;
    virtual void start();
    virtual void end();

    void initGuidePoints(const JMapInfoIter &);

    void show();
    void hide();

    MR::Vector<MR::FixedArray<MapPartsRailGuidePoint*, 0x100> > mGuidePoints;   // _18
    u32 _41C;
    s32 _420;
    const char* _424;
};