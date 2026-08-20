#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioMove.hpp"

MarioMove::MarioMove(MarioActor* pActor) : MarioModule(pActor) {
}

void MarioMove::initAfter() {
    mHeadVec = getPlayer()->mHeadVec;
    mFrontVec = getPlayer()->mFrontVec;
    mSideVec = getPlayer()->mSideVec;

    mHeadVec2 = mHeadVec;
    mFrontVec2 = mFrontVec;
    mSideVec2 = mSideVec;

    _50 = 0.0f;
    _54 = 0.0f;
}
