#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NPC/TimeAttackEventKeeper.hpp"

TimeAttackEventKeeper::TimeAttackEventKeeper() :
    _0(NULL)
{
    
}

void TimeAttackEventKeeper::init(bool) {
    
}

void TimeAttackEventKeeper::startEventIfExecute() {
    if (_0 == NULL) {
        return;
    }

    _0->appear();
}

void TimeAttackEventKeeper::endEvent() {
    if (_0 == NULL) {
        return;
    }

    _0->kill();
}
