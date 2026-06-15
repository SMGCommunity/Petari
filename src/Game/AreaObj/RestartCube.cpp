#include "Game/AreaObj/RestartCube.hpp"
#include "Game/AudioLib/AudBgmMgr.hpp"
#include "Game/AudioLib/AudSoundId.hpp"
#include "Game/AudioLib/AudWrap.hpp"
#include "Game/GameAudio/AudStageBgmTable.hpp"
#include "Game/System/GameDataTemporaryInGalaxy.hpp"
#include "Game/Util.hpp"

RestartCube::RestartCube(int formType, const char* pName) : AreaObj(formType, pName) {
    mIdInfo = nullptr;
    _40 = -1;
    _44 = -1;
    _48 = false;
}

RestartCube::~RestartCube() {
}

void RestartCube::init(const JMapInfoIter& rIter) {
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

    if (MR::isPlayingStageBgmID(STM_STAR_EXIST)) {
        _48 = true;
        return;
    }

    if (MR::isPlayingStageBgmID(STM_STAR_EXIST_2)) {
        _48 = true;
        return;
    }

    if (MR::isPlayerDead()) {
        _48 = true;
        return;
    }

    if (!MR::isPlayingStageBgmID(STM_BOSS_MECHA_KOOPA)) {
        u32 bgmID = AudStageBgmTable::getBgmId(MR::getCurrentStageName(), _40);

        if (bgmID != -1) {
            u32 currBGM = AudWrap::getBgmMgr()->mCurrentBGM[AudBgmMgr::BgmType_Stage];

            if (currBGM == bgmID && MR::isPlayingStageBgm()) {
                return;
            }

            AudWrap::startStageBgm(bgmID, false);

            if (MR::isEqualStageName("ReverseKingdomGalaxy") && bgmID == MBGM_BOSS_05_A) {
                MR::setCubeBgmChangeInvalid();
            }

            if (MR::isEqualStageName("CannonFleetGalaxy") && bgmID == MBGM_BOSS_01_A) {
                MR::setCubeBgmChangeInvalid();
            }

            if (MR::isEqualStageName("BattleShipGalaxy") && bgmID == MBGM_BOSS_01_A) {
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
