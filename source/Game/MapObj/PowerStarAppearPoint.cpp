#include "Game/MapObj/PowerStarAppearPoint.h"
#include "Game/Util.h"

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