#include "Game/AreaObj/ChangeBgmCube.hpp"
#include "Game/AudioLib/AudWrap.hpp"
#include "Game/GameAudio/AudStageBgmTable.hpp"
#include "Game/Util/ObjUtil.hpp"

ChangeBgmCube::ChangeBgmCube(int formType, const char* pName) : AreaObj(formType, pName) {
    _3C = 0;
}

ChangeBgmCube::~ChangeBgmCube() {
}

void ChangeBgmCube::init(const JMapInfoIter& rIter) {
    AreaObj::init(rIter);
    MR::connectToSceneAreaObj(this);
}

// this is one massive function, todo
/*
void ChangeBgmCube::movement() {
    bool valid = false;

    if (mIsValid && _15 && _16) {
        valid = true;
    }

    if (!valid) {
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

    if (!isInVolume(*MR::getPlayerPos())) {
        _3C = false;
        return;
    }

    if (!_3C) {
        return;
    }

    switch(mObjArg0) {
        case 1:
            if (mObjArg3 != 1) {
                if (MR::isGalaxyRedCometAppearInCurrentStage()) {
                    _3C = true;
                    return;
                }

                if (MR::isGalaxyBlackCometAppearInCurrentStage()) {
                    _3C = true;
                    return;
                }
            }

            if (mObjArg1 < 0) {
                mObjArg1 = 0x5A;
            }

            if (mObjArg2 >= 0) {
                u32 bgmID = AudStageBgmTable::getBgmId(MR::getCurrentStageName(), mObjArg2);
                if (bgmID != 0xFFFF) {
                    if (AudWrap::getBgmMgr()->_10 == bgmID) {
                        if (MR::isPlayingStageBgm()) {
                            _3C = 1;
                            return;
                        }
                    }

                    AudWrap::setNextIdStageBgm(bgmID);
                }

                MR::stopStageBGM(mObjArg1);
                _3C = 1;
                return;
            }
            break;
    }
}
*/
