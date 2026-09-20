#include "Game/MapObj/MapPartsRailGuideDrawer.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapPartsRailGuidePoint.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapPartsUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include <algorithm>

void MapPartsRailGuideDrawer_FORCE_MATCH_SDATA2() {
    (void)0.0f;
    (void)2.0f;
    (void)200.0f;
}

namespace NrvMapPartsRailGuideDrawer {
    NEW_NERVE(HostTypeHideAll, MapPartsRailGuideDrawer, HideAll);
    NEW_NERVE(HostTypeDrawAll, MapPartsRailGuideDrawer, DrawAll);
    NEW_NERVE(HostTypeDrawForward, MapPartsRailGuideDrawer, DrawForward);
}  // namespace NrvMapPartsRailGuideDrawer

MapPartsRailGuideDrawer::MapPartsRailGuideDrawer(LiveActor* pHost, const char* pModelName)
    : MapPartsFunction(pHost, "ガイド描画"), mGuidePoints(), mGuideType(), mRailId(-1), mModelName(pModelName) {
}

void MapPartsRailGuideDrawer::init(const JMapInfoIter& rIter) {
    MR::getMapPartsArgRailGuideType(&mGuideType, mHost);
    rIter.getValue("CommonPath_ID", &mRailId);
    if (mGuideType == -1) {
        mGuideType = 0;
    }

    if (mGuideType == 0) {
        initNerve(GET_NERVE(MapPartsRailGuideDrawer, HostTypeHideAll));
    } else {
        initGuidePoints(rIter);
        if (mGuideType == 1 || mGuideType == 3) {
            initNerve(GET_NERVE(MapPartsRailGuideDrawer, HostTypeDrawAll));
        } else if (mGuideType == 2) {
            initNerve(GET_NERVE(MapPartsRailGuideDrawer, HostTypeDrawForward));
        }
    }
}

void MapPartsRailGuideDrawer::start() {
    show();
}

void MapPartsRailGuideDrawer::end() {
    hide();
}

bool MapPartsRailGuideDrawer::isWorking() const {
    for (MapPartsRailGuidePoint* const* pPoint = mGuidePoints.begin(); pPoint != mGuidePoints.end(); pPoint++) {
        if (!MR::isDead(*pPoint)) {
            return true;
        }
    }

    return false;
}

void MapPartsRailGuideDrawer::show() {
    std::for_each(mGuidePoints.begin(), mGuidePoints.end(), std::mem_fun(&LiveActor::appear));
}

void MapPartsRailGuideDrawer::hide() {
    std::for_each(mGuidePoints.begin(), mGuidePoints.end(), std::mem_fun(&LiveActor::kill));
}

void MapPartsRailGuideDrawer::initGuidePoints(const JMapInfoIter& rIter) {
    s32 shadowType = 0;
    MR::getMapPartsArgShadowType(&shadowType, rIter);
    bool hasShadow = MR::hasMapPartsShadow(shadowType);
    f32 railLength = MR::getRailTotalLength(mHost);
    f32 curLen = 0.0f;

    while (curLen < railLength) {
        MapPartsRailGuidePoint* pPoint = new MapPartsRailGuidePoint(mHost, mModelName, curLen, hasShadow);
        pPoint->initWithoutIter();
        mGuidePoints.push_back(pPoint);
        curLen += 200.0f;
    }

    if (mGuideType == 3) {
        int curPointNum = 0;

        while (curPointNum < MR::getRailPointNum(mHost)) {
            MapPartsRailGuidePoint* pPoint = new MapPartsRailGuidePoint(mHost, mModelName, curPointNum, hasShadow);
            pPoint->initWithoutIter();
            pPoint->mScale.set(2.0f);
            mGuidePoints.push_back(pPoint);
            curPointNum++;
        }
    }
}

void MapPartsRailGuideDrawer::exeHideAll() {
}

void MapPartsRailGuideDrawer::exeDrawAll() {
}

void MapPartsRailGuideDrawer::exeDrawForward() {
    const f32 coord = MR::getRailCoord(mHost);
    for (MapPartsRailGuidePoint** pPoint = mGuidePoints.begin(); pPoint != mGuidePoints.end(); pPoint++) {
        if (coord < (*pPoint)->_8C) {
            break;
        }

        if (!MR::isDead(*pPoint)) {
            (*pPoint)->kill();
        }
    }
}
