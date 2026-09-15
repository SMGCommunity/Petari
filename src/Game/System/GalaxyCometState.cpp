#include "Game/System/GalaxyCometState.hpp"
#include "Game/LiveActor/Nerve.hpp"

#define STATE_HIDE 0
#define STATE_LAND 1

namespace {
    NEW_NERVE(GalaxyCometStateHide, GalaxyCometState, Hide);
    NEW_NERVE(GalaxyCometStateLand, GalaxyCometState, Land);
};  // namespace

GalaxyCometState::GalaxyCometState() : NerveExecutor("GalaxyCometState"), mIsUpdateNerve(true) {
    initNerve(GET_NERVE_GLOBAL(GalaxyCometStateLand));
}

void GalaxyCometState::update() {
    if (!mIsUpdateNerve) {
        return;
    }

    updateNerve();
}

bool GalaxyCometState::isLand() const {
    return isNerve(GET_NERVE_GLOBAL(GalaxyCometStateLand));
}

bool GalaxyCometState::isHide() const {
    return isNerve(GET_NERVE_GLOBAL(GalaxyCometStateHide));
}

void GalaxyCometState::forceToNext() {
    Nerve* pNerve =
        isLand() ? static_cast< Nerve* >(GET_NERVE_GLOBAL(GalaxyCometStateHide)) : static_cast< Nerve* >(GET_NERVE_GLOBAL(GalaxyCometStateLand));

    setNerve(pNerve);
}

s32 GalaxyCometState::getStateIndex() const {
    s32 stateIndex = isHide() ? STATE_HIDE : isLand();

    return stateIndex;
}

s32 GalaxyCometState::getPastSecond() const {
    return 0;
}

void GalaxyCometState::setStateAndPastSecond(int state, u16 pastSecond) {
    switch (state) {
    case STATE_HIDE:
        setNerve(GET_NERVE_GLOBAL(GalaxyCometStateHide));
        break;
    case STATE_LAND:
        setNerve(GET_NERVE_GLOBAL(GalaxyCometStateLand));
        break;
    default:
        break;
    }
}

void GalaxyCometState::exeHide() {
}

void GalaxyCometState::exeLand() {
}
