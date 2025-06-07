#include "Game/AreaObj/DeathArea.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"

DeathArea::DeathArea(int type, const char *pName) :
    AreaObj(type, pName)
{
    
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
    if (isValidSwitchA() && !isOnSwitchA()) {
        return false;
    }

    return AreaObj::isInVolume(rVec);
}

s32 DeathArea::getDeathType() const {
    return mObjArg0 != -1 ? mObjArg0 : 0;
}

DeathArea::~DeathArea() {
    
}

const char* DeathArea::getManagerName() const {
    return "DeathArea";
}
