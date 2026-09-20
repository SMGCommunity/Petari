#pragma once

#include "Game/MapObj/MapPartsFunction.hpp"
#include "Game/Util/Array.hpp"

class MapPartsRailGuidePoint;

class MapPartsRailGuideDrawer : public MapPartsFunction {
public:
    MapPartsRailGuideDrawer(LiveActor* pHost, const char* pModelName);

    virtual void init(const JMapInfoIter& rIter);
    virtual bool isWorking() const;
    virtual void start();
    virtual void end();

    void initGuidePoints(const JMapInfoIter& rIter);

    void show();
    void hide();
    void exeHideAll();
    void exeDrawAll();
    void exeDrawForward();

    /* 0x018 */ MR::Vector< MR::FixedArray< MapPartsRailGuidePoint*, 256 > > mGuidePoints;
    /* 0x41C */ s32 mGuideType;
    /* 0x420 */ s32 mRailId;
    /* 0x424 */ const char* mModelName;
};
