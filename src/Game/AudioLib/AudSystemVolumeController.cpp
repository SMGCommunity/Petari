#include "Game/AudioLib/AudSystemVolumeController.hpp"
#include "Game/AudioLib/AudParams.hpp"
#include "Game/AudioLib/AudSystem.hpp"

AudSystemVolumeController::AudSystemVolumeController(AudSystem* pSystem)
    : mSystem(pSystem), mVolumeSetDelay(-1), mCurrVolumeSet(0), mNumVolumeSets(0) {
    init();
}

void AudSystemVolumeController::init() {
    mNumVolumeSets = 0;
    mCurrVolumeSet = 0;
    for (u32 idx = 0; idx < 8; idx++) {
        mVolumeSets[idx] = 0;
    }
    pushVolumeSet();
}

void AudSystemVolumeController::update() {
    updateSeVolumeSetLevel();
}

void AudSystemVolumeController::setSeVolumeSetTrig(s32 volumeSet, u32 maxSteps) {
    setSeVolumeSetInner(volumeSet, maxSteps);
    pushVolumeSet();
    mCurrVolumeSet = volumeSet;
}

void AudSystemVolumeController::recoverSeVolumeSet(u32 maxSteps) {
    popVolumeSet();
    setSeVolumeSetInner(mCurrVolumeSet, maxSteps);
}

void AudSystemVolumeController::setSeVolumeSetLevel(s32 volumeSet) {
    if (mVolumeSetDelay < 0) {
        setSeVolumeSetTrig(volumeSet, 60);
    }
    mVolumeSetDelay = 2;
}

void AudSystemVolumeController::updateSeVolumeSetLevel() {
    if (mVolumeSetDelay > 0) {
        mVolumeSetDelay--;
    }

    if (mVolumeSetDelay == 0) {
        mVolumeSetDelay = -1;
        recoverSeVolumeSet(60);
    }
}

void AudSystemVolumeController::setSeVolumeSetInner(s32 volumeSet, u32 maxSteps) {
    for (int idx = 0; idx < 16; idx++) {
        mSystem->mSeMgr.mCategoryMgrs[idx].mParams.moveVolume(AudParams::scCtgVolume[volumeSet][idx], maxSteps);
    }
}

void AudSystemVolumeController::pushVolumeSet() {
    mVolumeSets[mNumVolumeSets] = mCurrVolumeSet;
    mNumVolumeSets++;
}

void AudSystemVolumeController::popVolumeSet() {
    if (mNumVolumeSets > 0) {
        mNumVolumeSets--;
    }
    mCurrVolumeSet = mVolumeSets[mNumVolumeSets];
    mVolumeSets[mNumVolumeSets] = 0;
}
