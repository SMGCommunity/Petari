#include "Game/MapObj/MapPartsRailGuideDrawer.hpp"
#include "Game/Util.hpp"

void MapPartsRailGuideDrawer::start() {
    show();
}

void MapPartsRailGuideDrawer::end() {
    hide();
}

void MapPartsRailGuideDrawer::initGuidePoints(const JMapInfoIter &rIter) {
    s32 shadowType = 0;
    MR::getMapPartsArgShadowType(&shadowType, rIter);
    bool hasShadow = MR::hasMapPartsShadow(shadowType);
    f32 railLength = MR::getRailTotalLength(mHost);
    f32 curLen = 0.0f;

    while (curLen < railLength) {
        MapPartsRailGuidePoint* pnt = new MapPartsRailGuidePoint(mHost, _424, curLen, hasShadow);
        pnt->initWithoutIter();
        mGuidePoints.push_back(pnt);
        curLen += 200.0f;
    }

    if (!_41C) {
        int curPointNum = 0;

        while (curPointNum < MR::getRailPointNum(mHost)) {
            MapPartsRailGuidePoint* blah = new MapPartsRailGuidePoint(mHost, _424, curPointNum, hasShadow);
            blah->initWithoutIter();
            blah->mScale.set(2.0f);
            mGuidePoints.push_back(blah);
            curPointNum++;
        }
    }
}

MapPartsRailGuideDrawer::~MapPartsRailGuideDrawer() {

}
