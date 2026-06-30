#include "Game/AudioLib/AudBgmMgr.hpp"
#include "Game/AudioLib/AudBgm.hpp"
#include "Game/AudioLib/AudBgmKeeper.hpp"
#include <JSystem/JAudio2/JAISound.hpp>

AudBgmMgr::AudBgmMgr() : _8FC(false) {
    for (int idx = 0; idx < 2; idx++) {
        mBgm[idx] = nullptr;
        mNextBGM[idx] = -1;
        mCurrentBGM[idx] = -1;
        mLastBGM[idx] = -1;
    }
}

void AudBgmMgr::movement() {
    volDownStageBgmWhenSubBgmPlaying();
    for (int idx = 0; idx < 2; idx++) {
        if (_8FC) {
            mVolumeController[idx]._8C = true;
        } else {
            mVolumeController[idx]._8C = false;
        }
        mVolumeController[idx].update();
        if (mBgm[idx] != nullptr) {
            mBgm[idx]->movement();
        }
        startNextBgmWhenStopping(idx);
        releaseStoppingBgm(idx);
    }
}

JAISoundHandle* AudBgmMgr::start(s32 bgmIndex, u32 soundID, bool b1) {
    if (mBgm[bgmIndex] != nullptr) {
        mBgm[bgmIndex]->rejectFromSyncStream();
        mBgm[bgmIndex]->stop(false);
        mBgm[bgmIndex]->getRhythmStrategy()->reject();
        mKeeper.release(mBgm[bgmIndex]);
    }

    if ((soundID & 0x10000) == 0) {
        mBgm[bgmIndex] = mKeeper.get(AudBgmKeeper::BgmType_Single);
    } else {
        mBgm[bgmIndex] = mKeeper.get(AudBgmKeeper::BgmType_Multi);
    }

    if (mBgm[bgmIndex] == nullptr) {
        return nullptr;
    }

    mBgm[bgmIndex]->setVolumeController(&mVolumeController[bgmIndex]);

    JAISoundHandle* handle;
    if ((soundID & 0x10000) == 0) {
        handle = mBgm[bgmIndex]->start(soundID, b1);
    } else {
        handle = mBgm[bgmIndex]->start(soundID & 0xFEFEFFFF, b1);
    }

    if (handle != nullptr) {
        mLastBGM[bgmIndex] = mCurrentBGM[bgmIndex];
        sendToRhythmSystem(bgmIndex);
        mCurrentBGM[bgmIndex] = soundID;
    }

    return handle;
}

void AudBgmMgr::setNextBGM(s32 bgmIndex, u32 soundID) {
    mNextBGM[bgmIndex] = soundID;
}

void AudBgmMgr::clearNextBGM(s32 bgmIndex) {
    mNextBGM[bgmIndex] = -1;
}

JAISoundHandle* AudBgmMgr::startLastBGM(s32 bgmIndex) {
    if (mLastBGM[bgmIndex] == -1) {
        return nullptr;
    }
    return start(bgmIndex, mLastBGM[bgmIndex], false);
}

void AudBgmMgr::clearLastBGM(s32 bgmIndex) {
    mLastBGM[bgmIndex] = -1;
}

void AudBgmMgr::pause() {
    for (int idx = 0; idx < 2; idx++) {
        if (mBgm[idx] != nullptr) {
            mBgm[idx]->pause(true);
        }
    }
}

void AudBgmMgr::unpause() {
    for (int idx = 0; idx < 2; idx++) {
        if (mBgm[idx] != nullptr) {
            mBgm[idx]->pause(false);
        }
    }
}

void AudBgmMgr::volDownLevel(bool immediate) {
    for (int idx = 0; idx < 2; idx++) {
        mVolumeController[idx].volDown(immediate);
    }
}

void AudBgmMgr::volDownStageBgmWhenSubBgmPlaying() {
    AudBgm* bgm = mBgm[BgmType_Sub];
    if (bgm == nullptr) {
        return;
    }

    JAISoundHandle* handle = bgm->getHandle();
    if (handle == nullptr) {
        return;
    }

    if (handle->isSoundAttached() && mBgm[BgmType_Stage] != nullptr) {
        mVolumeController[BgmType_Stage].interruptedByOther();
    }

    JAISoundHandle* rhythmHandle = bgm->getRhythmHandle();
    if (rhythmHandle == nullptr) {
        return;
    }

    if (rhythmHandle->isSoundAttached()) {
        setBgmToRhythmDominant(BgmType_Sub);
    }
}

void AudBgmMgr::startNextBgmWhenStopping(s32 bgmIndex) {
    if (mNextBGM[bgmIndex] == -1) {
        return;
    }

    if (mBgm[bgmIndex] == nullptr) {
        return;
    }

    if (mBgm[bgmIndex]->isStopping()) {
        start(bgmIndex, mNextBGM[bgmIndex], false);
        clearNextBGM(bgmIndex);
    }
}

void AudBgmMgr::releaseStoppingBgm(s32 bgmIndex) {
    if (mBgm[bgmIndex] == nullptr) {
        return;
    }

    JAISoundHandle* handle = mBgm[bgmIndex]->getHandle();
    if (handle == nullptr) {
        return;
    }

    JAISoundHandle* rhythmHandle = mBgm[bgmIndex]->getRhythmHandle();
    if (rhythmHandle != nullptr) {
        if (!rhythmHandle->isSoundAttached()) {
            stopRhythmProcess(bgmIndex);
        }
    } else {
        stopRhythmProcess(bgmIndex);
    }

    if (!handle->isSoundAttached()) {
        mKeeper.release(mBgm[bgmIndex]);
        mBgm[bgmIndex] = nullptr;
    }
}

bool AudBgmMgr::sendToRhythmSystem(s32 bgmIndex) {
    AudBgm* bgm = mBgm[bgmIndex];
    if (bgm == nullptr) {
        return false;
    }

    bool success = bgm->getRhythmStrategy()->set(bgm, bgmIndex);
    if (success) {
        if (bgmIndex == BgmType_Stage && mBgm[BgmType_Sub] == nullptr) {
            bgm->getRhythmStrategy()->setDominant();
        }
    }

    bgm->sendToSyncStream();

    return success;
}

void AudBgmMgr::setBgmToRhythmDominant(s32 bgmIndex) {
    if (mBgm[bgmIndex] == nullptr) {
        return;
    }

    if (!mBgm[bgmIndex]->getRhythmStrategy()->isDominant()) {
        mBgm[bgmIndex]->getRhythmStrategy()->setDominant();
    }
}

void AudBgmMgr::stopRhythmProcess(s32 bgmIndex) {
    if (mBgm[bgmIndex] == nullptr) {
        return;
    }

    bool dominant = mBgm[bgmIndex]->getRhythmStrategy()->isDominant();
    mBgm[bgmIndex]->rejectFromSyncStream();
    mBgm[bgmIndex]->getRhythmStrategy()->reject();

    if (bgmIndex == BgmType_Sub && dominant && mBgm[BgmType_Stage] != nullptr) {
        mBgm[BgmType_Stage]->getRhythmStrategy()->setDominant();
    }
}
