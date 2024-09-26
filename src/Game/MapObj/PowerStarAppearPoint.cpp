#include "Game/MapObj/PowerStarAppearPoint.hpp"
#include "Game/Util.hpp"

PowerStarAppearPoint::PowerStarAppearPoint(const char *pName) : LiveActor(pName) {
    mCameraInfo = 0;
}

PowerStarAppearPoint::~PowerStarAppearPoint() {

}

void PowerStarAppearPoint::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    MR::joinToGroupArray(this, rIter, "パワースター出現ポイントグループ", 0x10);
    MR::initActorCamera(this, rIter, &mCameraInfo);
    makeActorAppeared();
}