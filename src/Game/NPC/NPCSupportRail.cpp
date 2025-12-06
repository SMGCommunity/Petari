#include "Game/NPC/NPCSupportRail.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "revolution/types.h"
#include <cstddef>

NPCSupportRail::NPCSupportRail(const char* pName) : LiveActor(pName) {}

NPCSupportRail::~NPCSupportRail() {}

void NPCSupportRail::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initRailRider(rIter);
    MR::joinToGroupArray(this, rIter, nullptr, 32);
    makeActorDead();
}
