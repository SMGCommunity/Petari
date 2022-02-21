#include "Game/AreaObj/RestartCube.h"
#include "Game/System/GameDataTemporaryInGalaxy.h"
#include "Game/GameAudio/AudStageBgmTable.h"

RestartCube::RestartCube(int type, const char *pName) : AreaObj(type, pName) {
    mIdInfo = NULL;
    _40 = -1;
    _44 = -1;
    _48 = false;
}

RestartCube::~RestartCube() {

}

void RestartCube::init(const JMapInfoIter &rIter) {
    AreaObj::init(rIter);
    mIdInfo = new JMapIdInfo(mObjArg0, rIter);
    _40 = mObjArg1;
    _44 = mObjArg2;
    _48 = false;
}

void RestartCube::updatePlayerRestartIdInfo() {
    if (mObjArg3 == -1 || MR::isOnGroundPlayer()) {
        MR::setPlayerRestartIdInfo(*mIdInfo);
        
        if (!_48 && !MR::isCubeBgmChangeInvalid() && _40 >= 0) {
            changeBgm();
        }
    }
}

/*void RestartCube::changeBgm() {
    if (MR::isPowerStarGetDemoActive()) {
        _48 = true;
    }
    else if (MR::isPlayingStageBgmID(0x2000014)) {
        _48 = true;
    }
    else if (MR::isPlayingStageBgmID(0x200003E)) {
        _48 = true;
    }
    else if (MR::isPlayerDead()) {
        _48 = true;
    }
    else if (!MR::isPlayingStageBgmID(0x2000039)) {
        s32 id = AudStageBgmTable::getBgmId(MR::getCurrentStageName(), _40);

        if (id != -1) {

        }

        if (_44 > -1) {

        }
    }
}*/
