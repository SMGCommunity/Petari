#include "Game/LiveActor/Nerve.hpp"
#include "Game/System/GalaxyCometState.hpp"

#define STATE_HIDE 0
#define STATE_LAND 1

namespace {
    NEW_NERVE(GalaxyCometStateHide, GalaxyCometState, Hide);
    NEW_NERVE(GalaxyCometStateLand, GalaxyCometState, Land);
}; // namespace

GalaxyCometState::GalaxyCometState()
    : NerveExecutor("GalaxyCometState"),
      mIsUpdateNerve(true) {
    initNerve(&GalaxyCometStateLand::sInstance);
}

GalaxyCometState::~GalaxyCometState() {}

void GalaxyCometState::update() {
    if (!mIsUpdateNerve) {
        return;
    }

    updateNerve();
}

bool GalaxyCometState::isLand() const {
    return isNerve(&GalaxyCometStateLand::sInstance);
}

bool GalaxyCometState::isHide() const {
    return isNerve(&GalaxyCometStateHide::sInstance);
}

void GalaxyCometState::forceToNext() {
    Nerve* pNerve = isLand()
                        ? static_cast<Nerve*>(&GalaxyCometStateHide::sInstance)
                        : static_cast<Nerve*>(&GalaxyCometStateLand::sInstance);

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
        setNerve(&GalaxyCometStateHide::sInstance);
        break;
    case STATE_LAND:
        setNerve(&GalaxyCometStateLand::sInstance);
        break;
    default:
        break;
    }
}

void GalaxyCometState::exeHide() {
}

void GalaxyCometState::exeLand() {
}
