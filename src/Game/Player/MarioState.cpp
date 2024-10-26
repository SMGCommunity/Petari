#include "Game/Player/MarioState.hpp"

MarioState::MarioState(MarioActor *actor, u32 number) : MarioModule(actor)
{
    _8 = 0;
    _C = number;
    _10 = 0;
}