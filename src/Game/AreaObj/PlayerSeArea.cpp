#include "Game/AreaObj/PlayerSeArea.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"

PlayerSeArea::PlayerSeArea(int formType, const char* pName) : AreaObj(formType, pName) {
}

void PlayerSeArea::init(const JMapInfoIter& rIter) {
    AreaObj::init(rIter);
    MR::connectToSceneAreaObj(this);

    _3C = mObjArg0;
}

PlayerSeArea::~PlayerSeArea() {
}

void PlayerSeArea::movement() {
    if (!isInVolume(*MR::getPlayerPos()) || _3C != 0) {
        return;
    }

    if (!MR::isPlayerJumpRising()) {
        MR::startLevelSoundPlayer("SE_PM_LV_LONG_FALL", -1);
    }
}
