#include "Game/GameAudio/AudBgmConductor.hpp"
#include "Game/AudioLib/AudBgm.hpp"
#include "Game/AudioLib/AudSoundId.hpp"
#include "Game/AudioLib/AudWrap.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <JSystem/JAudio2/JAISound.hpp>

void AudBgmConductor_FORCE_MATCH_SDATA2() {
    f32 f1 = 1.0f;
}

AudBgmConductor::AudBgmConductor()
    : NameObj("AudBgmConductor"), mStateBits(0), mState(0), mStartDelay(-1), mFadeInDelay(-1), mStopImmediately(false) {
}

void AudBgmConductor::init(const JMapInfoIter& rIter) {
    MR::connectToScene(this, MR::MovementType_AudBgmConductor, -1, -1, -1);
    if (MR::isEqualStageName("IceVolcanoGalaxy") || MR::isEqualStageName("OceanRingGalaxy")) {
        MR::registerDemoSimpleCastAll(this);
    }
}

void AudBgmConductor::playPinchEventBgmLevel(bool secondPinch) {
    if (MR::isPlayingSubBgmID(BGM_MUTEKI_A) || MR::isPlayingSubBgmID(BGM_MUTEKI_B) || MR::isPlayingSubBgmID(BGM_FIRE_A) ||
        MR::isPlayingSubBgmID(BGM_FIRE_B) || MR::isPlayingSubBgmID(BGM_ICE_A) || MR::isPlayingSubBgmID(BGM_ICE_B)) {
        mStartDelay = -1;
        mFadeInDelay = -1;
        return;
    }

    if (!MR::isPlayerDead() && !MR::isPowerStarGetDemoActive()) {
        if (mStartDelay < 0) {
            if (secondPinch) {
                MR::startSubBGM("BGM_PINCH_2", false);
            } else {
                MR::startSubBGM("BGM_PINCH_1", false);
            }

            if (secondPinch) {
                MR::moveVolumeSubBGM(0.0f, 0);
                mFadeInDelay = 2;
            }

            mStopImmediately = false;
        }
        mStartDelay = 3;
    }
}

void AudBgmConductor::movement() {
    movementPinchEventBgm();

    AudBgm* bgm = AudWrap::getStageBgm();
    if (bgm == nullptr) {
        return;
    }

    JAISoundID id = bgm->getSoundID();

    u32 time = 90;
    s32 state;
    switch (id) {
    case STM_GALAXY_03:
    case STM_GALAXY_04: {
        if ((mStateBits & StateBit_1) != 0) {
            if ((mStateBits & StateBit_Delay) != 0) {
                state = 6;
                time = 20;
            } else {
                state = 5;
            }
        } else {
            if ((mStateBits & StateBit_Delay) != 0) {
                state = 4;
                time = 20;
            } else {
                state = 1;
            }
        }
        if (state != mState) {
            MR::setStageBGMState(state, time);
        }
        break;
    }
    default:
        state = 0;
        break;
    }
    mState = state;
    mStateBits = 0;
}

void AudBgmConductor::setStateBit(u32 bit) {
    mStateBits |= bit;
}

void AudBgmConductor::stopPinchEventBgmSoon() {
    mStartDelay = 0;
    mStopImmediately = true;
}

void AudBgmConductor::movementPinchEventBgm() {
    if (MR::isPlayingSubBgmID(BGM_MUTEKI_A) || MR::isPlayingSubBgmID(BGM_MUTEKI_B) || MR::isPlayingSubBgmID(BGM_FIRE_A) ||
        MR::isPlayingSubBgmID(BGM_FIRE_B) || MR::isPlayingSubBgmID(BGM_ICE_A) || MR::isPlayingSubBgmID(BGM_ICE_B)) {
        return;
    }

    if (MR::isPowerStarGetDemoActive() || MR::isPlayerDead()) {
        mStartDelay = -1;
        mFadeInDelay = -1;
        mStopImmediately = false;
        return;
    }

    if (mStartDelay >= 0) {
        if (mFadeInDelay == 0) {
            MR::moveVolumeSubBGM(1.0f, 90);
            mFadeInDelay = -1;
        }

        if (mFadeInDelay > 0) {
            mFadeInDelay--;
        }

        mStartDelay--;
        if (mStartDelay <= 0) {
            if (mStopImmediately) {
                MR::stopSubBGM(1);
            } else {
                MR::stopSubBGM(90);
            }

            mStartDelay = -1;
            mFadeInDelay = -1;
            mStopImmediately = false;
        }
    }
}
