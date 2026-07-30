#include "Game/AudioLib/AudSystem.hpp"
#include "Game/AudioLib/AudRemixMgr.hpp"
#include "Game/AudioLib/AudSoundObjHolder.hpp"
#include "Game/AudioLib/AudSystemVolumeController.hpp"
#include "Game/RhythmLib/AudRhythmMeSystem.hpp"
#include "Game/Speaker/SpkSystem.hpp"

void AudSystem::frameWork() {
    screenSaverProcess();
    updateHomeButtonMenu();
    updatePauseMenu();
    seFanfareProcess();

    if (mVolumeController != nullptr) {
        mVolumeController->update();
    }

    mBgmMgr.movement();
    mRhythmMeSystem->update();
    mRemixMgr->update();
    JAUSoundMgr::calc();
    mixOut();
    mSoundObjHolder->update();

    if (mSpkSystem != nullptr) {
        mSpkSystem->framework();
    }

    if (_830 >= 0) {
        s32 val = _830 - 1;
        _830 = val;

        if (val <= 0) {
            _830 = -1;
        }
    }

    updateLimitedSound();
}

void AudSystem::calc() {
    if (_815 && mReseter.checkDone()) {
        if (!_816) {
            stopSync();
        }

    } else {
        JAUSoundMgr::calc();
    }
}
