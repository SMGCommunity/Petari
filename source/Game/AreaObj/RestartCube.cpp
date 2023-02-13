#include "Game/AreaObj/RestartCube.h"
#include "Game/AudioLib/AudWrap.h"
#include "Game/System/GameDataTemporaryInGalaxy.h"
#include "Game/GameAudio/AudStageBgmTable.h"

RestartCube::RestartCube(int type, const char *pName) : AreaObj(type, pName) {
    mIdInfo = nullptr;
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

void RestartCube::changeBgm() {
    if (MR::isPowerStarGetDemoActive()) {
        _48 = true;
        return;
    }

    if (MR::isPlayingStageBgmID(0x2000014)) {
        _48 = true;
        return;
    }

    if (MR::isPlayingStageBgmID(0x200003E)) {
        _48 = 1;
        return;
    }

    if (MR::isPlayerDead()) {
        _48 = 1;
        return;
    }

    if (!MR::isPlayingStageBgmID(0x2000039)) {
        u32 bgmID = AudStageBgmTable::getBgmId(MR::getCurrentStageName(), _40);

        if (bgmID + 0x10000 != 0xFFFF) {
            u32 val = AudWrap::getBgmMgr()->_10;
            if (val == bgmID && MR::isPlayingStageBgm()) {
                return;
            }

            AudWrap::startStageBgm(bgmID, false);

            if (MR::isEqualStageName("ReverseKingdomGalaxy") && bgmID == 0x1010012) {
                MR::setCubeBgmChangeInvalid();
            }

            if (MR::isEqualStageName("CannonFleetGalaxy") && bgmID == 0x1010002) {
                MR::setCubeBgmChangeInvalid();
            }

            if (MR::isEqualStageName("BattleShipGalaxy") && bgmID == 0x1010002) {
                MR::setCubeBgmChangeInvalid();
            }
        }

        if (_44 >= 0) {
            s32 state = AudStageBgmTable::getBgmState(MR::getCurrentStageName(), _44);

            if (state >= 0) {
                AudBgm* bgm = AudWrap::getStageBgm();

                if (bgm) {
                    bgm->changeTrackMuteState(state, 0);
                }
            }
        }
    }
}