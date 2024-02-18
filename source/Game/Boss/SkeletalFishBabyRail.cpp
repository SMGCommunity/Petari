#include "Game/Boss/SkeletalFishBabyRail.hpp"
#include "Game/Boss/SkeletalFishBabyRailHolder.hpp"

SkeletalFishBabyRail::SkeletalFishBabyRail(const char *pName) : LiveActor(pName) {
    _8C = -1;
};

void SkeletalFishBabyRail::init(const JMapInfoIter &rIter) {
    MR::createSkeletalFishBabyRailHolder();
    initRailRider(rIter);
    MR::getJMapInfoArg0NoInit(rIter, &_8C);
    MR::getSkeletalFishBabyRailHolder()->add(this);
    makeActorDead();
}

SkeletalFishBabyRail::~SkeletalFishBabyRail() {

}
