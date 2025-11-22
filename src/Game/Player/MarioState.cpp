#include "Game/Player/MarioState.hpp"

MarioState::MarioState(MarioActor* actor, u32 statusId)
    : MarioModule(actor) {
    _8 = 0;
    mStatusId = statusId;
    _10 = 0;
}
