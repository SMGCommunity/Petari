#include "Game/AreaObj/DeathArea.hpp"
#include "Game/Util.hpp"

DeathArea::DeathArea(int a1, const char *pName) : AreaObj(a1, pName) {

}

void DeathArea::init(const JMapInfoIter &rIter) {
    AreaObj::init(rIter);
    MR::connectToSceneAreaObj(this);
}

void DeathArea::movement() {
    if (isInVolume(*MR::getPlayerPos())) {
        bool canKill = getDeathType() == 0;
        if (canKill) {
            MR::forceKillPlayerByAbyss();
        }
    }
}

bool DeathArea::isInVolume(const TVec3f &rVec) const {
    bool ret;

    if (isValidSwitchA() && !isOnSwitchA()) {
        ret = false;
    }
    else {
        ret = AreaObj::isInVolume(rVec);
    }

    return ret;
}

s32 DeathArea::getDeathType() const {
    return mObjArg0 != -1 ? mObjArg0 : 0;
}

DeathArea::~DeathArea() {
    
}

const char* DeathArea::getManagerName() const {
    return "DeathArea";
}