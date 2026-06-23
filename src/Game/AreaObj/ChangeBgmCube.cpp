#include "Game/AreaObj/ChangeBgmCube.hpp"
#include "Game/AudioLib/AudBgmMgr.hpp"
#include "Game/AudioLib/AudSoundId.hpp"
#include "Game/AudioLib/AudWrap.hpp"
#include "Game/GameAudio/AudStageBgmTable.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <JSystem/JAudio2/JAISound.hpp>

ChangeBgmCube::ChangeBgmCube(int formType, const char* pName) : AreaObj(formType, pName), _3C() {
}

void ChangeBgmCube::init(const JMapInfoIter& rIter) {
    AreaObj::init(rIter);
    MR::connectToSceneAreaObj(this);
}

ChangeBgmCube::~ChangeBgmCube() {
}

void ChangeBgmCube::movement() {
    if (!isValid()) {
        return;
    }

    if (MR::isCubeBgmChangeInvalid()) {
        return;
    }

    if (MR::isPowerStarGetDemoActive()) {
        mIsValid = false;
        return;
    }

    if (MR::isPlayingStageBgmID(STM_STAR_EXIST)) {
        mIsValid = false;
        return;
    }

    if (MR::isPlayingStageBgmID(STM_STAR_EXIST_2)) {
        mIsValid = false;
        return;
    }

    if (MR::isPlayerDead()) {
        mIsValid = false;
        return;
    }

    if (MR::isStageStateScenarioOpeningCamera()) {
        return;
    }

    if (MR::isPlayingStageBgmID(STM_GALAXY_02_CHASE)) {
        return;
    }

    if (MR::isPlayingStageBgmID(STM_BOSS_MECHA_KOOPA)) {
        return;
    }

    if (MR::isPlayingStageBgmID(STM_BOSS_01_A)) {
        return;
    }

    if (isInVolume(*MR::getPlayerPos())) {
        if (_3C) {
            return;
        }

        s32 objArg0 = mObjArg0;
        s32 objArg1 = mObjArg1;
        s32 objArg2 = mObjArg2;

        switch (objArg0) {
        case 0:
            if (objArg1 < 0) {
                MR::startCurrentStageBGM();
                break;
            }

            JAISoundID bgmId = JAISoundID(AudStageBgmTable::getBgmId(MR::getCurrentStageName(), objArg1));

            if (!bgmId.isAnonymous()) {
                if (AudWrap::getBgmMgr()->mCurrentBGM[0] == bgmId) {
                    if (MR::isPlayingStageBgm()) {
                        break;
                    }
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

            if (objArg2 >= 0) {
                s32 bgmState = AudStageBgmTable::getBgmState(MR::getCurrentStageName(), objArg2);

                if (bgmState >= 0) {
                    AudBgm* stageBgm = AudWrap::getStageBgm();

                    if (stageBgm != nullptr) {
                        stageBgm->changeTrackMuteState(bgmState, 0);
                    }
                }
            }
            break;
        case 1:
            if (mObjArg3 != 1) {
                if (MR::isGalaxyRedCometAppearInCurrentStage()) {
                    break;
                }

                if (MR::isGalaxyBlackCometAppearInCurrentStage()) {
                    break;
                }
            }

            s32 arg1 = objArg1;

            if (arg1 < 0) {
                arg1 = 90;
            }

            if (objArg2 >= 0) {
                JAISoundID bgmId = JAISoundID(AudStageBgmTable::getBgmId(MR::getCurrentStageName(), objArg2));

                if (!bgmId.isAnonymous()) {
                    if (AudWrap::getBgmMgr()->mCurrentBGM[0] == bgmId) {
                        if (MR::isPlayingStageBgm()) {
                            break;
                        }
                    }

                    AudWrap::setNextIdStageBgm(bgmId);
                }
            }

            MR::stopStageBGM(arg1);
            break;
        case 2:
            if (objArg1 < 0) {
                AudBgm* stageBgm = AudWrap::getStageBgm();

                if (stageBgm != nullptr) {
                    s32 arg2 = 30;

                    if (objArg2 >= 0) {
                        arg2 = objArg2;
                    }

                    stageBgm->changeTrackMuteState(0, arg2);
                }
            } else {
                s32 bgmState = AudStageBgmTable::getBgmState(MR::getCurrentStageName(), objArg1);

                if (bgmState >= 0) {
                    AudBgm* stageBgm = AudWrap::getStageBgm();

                    if (stageBgm != nullptr) {
                        s32 arg2 = 30;

                        if (objArg2 >= 0) {
                            arg2 = objArg2;
                        }

                        stageBgm->changeTrackMuteState(bgmState, arg2);
                    }
                }
            }
            break;
        }

        _3C = true;
    } else {
        _3C = false;
    }
}
