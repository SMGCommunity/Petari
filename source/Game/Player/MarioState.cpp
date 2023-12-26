#include "Game/Player/MarioState.h"

MarioState::MarioState(MarioActor *actor, u32 number) {
    mActor = actor;
    _8 = 0;
    _C = number;
    _10 = 0;
}