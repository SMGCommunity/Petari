#include "Game/Player/MarioMove.hpp"
#include "Game/Player/Mario.hpp"

MarioMove::MarioMove(MarioActor *pActor) : MarioModule(pActor) {
}

void MarioMove::initAfter() {
    _8 = getPlayer()->mHeadVec;
    _14 = getPlayer()->mFrontVec;
    _20 = getPlayer()->mSideVec;
    _2C = _8;
    _38 = _14;
    _44 = _20;
    _50 = 0.0f;
    _54 = 0.0f;
}
