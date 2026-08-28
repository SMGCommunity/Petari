#pragma once

#include "Game/MapObj/MapPartsFunction.hpp"
#include "Game/MapObj/MapPartsRailGuidePoint.hpp"
#include "Game/Util/Array.hpp"

class MapPartsRailGuideDrawer : public MapPartsFunction {
public:
    MapPartsRailGuideDrawer(LiveActor*, const char*);

    /* 0x08 */ virtual ~MapPartsRailGuideDrawer();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x24 */ virtual bool isWorking() const;
    /* 0x28 */ virtual void start();
    /* 0x2C */ virtual void end();

    void initGuidePoints(const JMapInfoIter&);

    void show();
    void hide();

    MR::Vector< MR::FixedArray< MapPartsRailGuidePoint*, 256 > > mGuidePoints;  // 0x18
    u32 _41C;
    s32 _420;
    const char* _424;
};