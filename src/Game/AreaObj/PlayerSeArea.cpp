#include "Game/AreaObj/PlayerSeArea.hpp"
#include "Game/Util.hpp"

PlayerSeArea::PlayerSeArea(int type, const char *pName) : AreaObj(type, pName) {

}

PlayerSeArea::~PlayerSeArea() {

}

void PlayerSeArea::init(const JMapInfoIter &rIter) {
    AreaObj::init(rIter);
    MR::connectToSceneAreaObj(this);
    _3C = mObjArg0;
}

void PlayerSeArea::movement() {
    if (!isInVolume(*MR::getPlayerPos()) || _3C != 0) {
        return;
    }

    if (!MR::isPlayerJumpRising()) {
        MR::startLevelSoundPlayer("SE_PM_LV_LONG_FALL", -1);
    }
}

const char *PlayerSeArea::getManagerName() const {
    return "PlayerSeArea";
}
