#include "Game/MapObj/MapPartsRailPointPassChecker.hpp"
#include "Game/Util.hpp"

MapPartsRailPointPassChecker::MapPartsRailPointPassChecker(LiveActor *pActor) : MapPartsFunction(pActor, "レール点通過チェッカー") {
    mRailPointNo = -1;
}

void MapPartsRailPointPassChecker::movement() {
    mRailPointNo = MR::getCurrentRailPointNo(mHost);
}


void MapPartsRailPointPassChecker::start() {
    mRailPointNo = MR::getCurrentRailPointNo(mHost);
}
 
bool MapPartsRailPointPassChecker::isPassed() const {
    return MR::getCurrentRailPointNo(mHost) != mRailPointNo;
}

bool MapPartsRailPointPassChecker::isReachedEnd() const {
    return MR::isRailReachedGoal(mHost);
}

bool MapPartsRailPointPassChecker::isPassedStartPoint() const {
    if (MR::getCurrentRailPointNo(mHost) <= 0) {
        return false;
    } 

    return MR::getCurrentRailPointNo(mHost) != mRailPointNo;
}

bool MapPartsRailPointPassChecker::isPassedEndPoint() const {
    LiveActor* host = mHost;
    if (MR::getCurrentRailPointNo(host) != MR::getRailPointNum(host) - 1) {
        return false;
    }

    return MR::getCurrentRailPointNo(host) != mRailPointNo;
}

MapPartsRailPointPassChecker::~MapPartsRailPointPassChecker() {

}

bool MapPartsRailPointPassChecker::isWorking() const {
    return true;
}
