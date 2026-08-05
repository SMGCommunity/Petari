#include "Game/NPC/NPCSupportRail.hpp"
#include "Game/Util/LiveActorUtil.hpp"

NPCSupportRail::NPCSupportRail(const char* pName) : LiveActor(pName) {
}

void NPCSupportRail::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initRailRider(rIter);
    MR::joinToGroupArray(this, rIter, nullptr, 32);
    makeActorDead();
}
