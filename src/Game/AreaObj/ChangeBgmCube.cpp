#include "Game/AreaObj/ChangeBgmCube.hpp"
#include "Game/AudioLib/AudWrap.hpp"
#include "Game/GameAudio/AudStageBgmTable.hpp"
#include "Game/Util/ObjUtil.hpp"

ChangeBgmCube::ChangeBgmCube(int a1, const char *pName) : AreaObj(a1, pName) {
    _3C = 0;
}

ChangeBgmCube::~ChangeBgmCube() {

}

void ChangeBgmCube::init(const JMapInfoIter &rIter) {
    AreaObj::init(rIter);
    MR::connectToSceneAreaObj(this);
}

// this is one massive function, todo
/*
void ChangeBgmCube::movement() {
    bool valid = false;

    if (mValid && _15 && _16) {
        valid = true;
    }

    if (!valid) {
        return;
    }

    if (MR::isCubeBgmChangeInvalid()) {
        return;
    }

    if (MR::isPowerStarGetDemoActive()) {
        mValid = false;
        return;
    }

    if (MR::isPlayingStageBgmID(0x2000014)) {
        mValid = false;
        return;
    }

    if (MR::isPlayingStageBgmID(0x200003E)) {
        mValid = false;
        return;
    }

    if (MR::isPlayerDead()) {
        mValid = false;
    }

    if (MR::isStageStateScenarioOpeningCamera()) {
        return;
    }

    if (MR::isPlayingStageBgmID(0x200000A)) {
        return;
    }

    if (MR::isPlayingStageBgmID(0x2000039)) {
        return;
    }

    if (MR::isPlayingStageBgmID(0x2000003)) {
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
