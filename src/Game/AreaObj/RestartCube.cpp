#include "Game/AreaObj/RestartCube.hpp"
#include "Game/AudioLib/AudBgmMgr.hpp"
#include "Game/AudioLib/AudSoundId.hpp"
#include "Game/AudioLib/AudWrap.hpp"
#include "Game/GameAudio/AudStageBgmTable.hpp"
#include "Game/System/GameDataTemporaryInGalaxy.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/JMapIdInfo.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/SystemUtil.hpp"

RestartCube::RestartCube(int formType, const char* pName) : AreaObj(formType, pName), mIdInfo(), _40(-1), _44(-1), _48() {
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

RestartCube::~RestartCube() {
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

    if (MR::isPlayingStageBgmID(STM_BOSS_MECHA_KOOPA)) {
        return;
    }

    u32 bgmId = AudStageBgmTable::getBgmId(MR::getCurrentStageName(), _40);

    if (bgmId != -1) {
        u32 currBGM = AudWrap::getBgmMgr()->mCurrentBGM[AudBgmMgr::BgmType_Stage];

        if (currBGM == bgmId && MR::isPlayingStageBgm()) {
            return;
        }

        AudWrap::startStageBgm(bgmId, false);

        if (MR::isEqualStageName("ReverseKingdomGalaxy") && bgmId == MBGM_BOSS_05_A) {
            MR::setCubeBgmChangeInvalid();
        }

        if (MR::isEqualStageName("CannonFleetGalaxy") && bgmId == MBGM_BOSS_01_A) {
            MR::setCubeBgmChangeInvalid();
        }

        if (MR::isEqualStageName("BattleShipGalaxy") && bgmId == MBGM_BOSS_01_A) {
            MR::setCubeBgmChangeInvalid();
        }
    }

    if (_44 >= 0) {
        s32 state = AudStageBgmTable::getBgmState(MR::getCurrentStageName(), _44);

        if (state >= 0) {
            AudBgm* stageBgm = AudWrap::getStageBgm();

            if (stageBgm != nullptr) {
                stageBgm->changeTrackMuteState(state, 0);
            }
        }
    }
}
