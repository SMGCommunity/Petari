#include "Game/NPC/TimeAttackEventKeeper.hpp"
#include "Game/LiveActor/LiveActor.hpp"

TimeAttackEventKeeper::TimeAttackEventKeeper() : _0() {
}

void TimeAttackEventKeeper::init(bool) {
}

void TimeAttackEventKeeper::startEventIfExecute() {
    if (_0 != nullptr) {
        _0->appear();
    }
}

void TimeAttackEventKeeper::endEvent() {
    if (_0 != nullptr) {
        _0->kill();
    }
}
