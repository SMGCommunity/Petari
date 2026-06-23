#include "Game/GameAudio/AudTamakoroBgmCtrl.hpp"
#include "Game/AudioLib/AudSoundId.hpp"
#include "Game/AudioLib/AudSystem.hpp"
#include "Game/AudioLib/AudWrap.hpp"
#include <JSystem/JAudio2/JAISoundChild.hpp>

namespace {
    f32 sSpeedMin = 2.0f;
    f32 sSpeedMax = 22.0f;
    f32 sSpeedLowThreshold = 7.0f;
    f32 sSpeedMidThreshold = 12.0f;
    f32 sTempoRateMin = 0.55f;
    f32 sTempoRateMax = 1.22f;
    f32 sTempoRateVeryHigh = 1.478f;
    f32 sPitchRateNormal = 1.0f;
    f32 sPitchRateVeryHigh = 1.189f;
    s32 sFirstBarSteps = 107;
    s32 sStateNumLow = 1;
    s32 sStateNumMid = 2;
    s32 sStateNumHigh = 3;
    s32 sStateNumVeryHigh = 4;
    s32 sPercTrack = 14;
};  // namespace

AudTamakoroBgmCtrl::AudTamakoroBgmCtrl() : mSteps(0) {
}

void AudTamakoroBgmCtrl::init() {
    mSteps = 0;
}

void AudTamakoroBgmCtrl::control(f32 speed, bool disabled, s32 frames) {
    AudBgm* bgm = AudWrap::getStageBgm();
    if (bgm != nullptr) {
        JAISoundHandle* handle = bgm->getHandle();
        if (handle != nullptr && handle->isSoundAttached()) {
            JAISoundID id = handle->getSound()->getID();
            if (id == BGM_TAMAKORO || id == BGM_TAMAKORO_2) {
                if (mSteps > ::sFirstBarSteps) {
                    if (speed < ::sSpeedMin) {
                        speed = ::sSpeedMin;
                    }
                    if (speed > ::sSpeedMax) {
                        speed = ::sSpeedMax;
                    }
                    f32 t0;
                    f32 t1;
                    s32 stateNum;
                    f32 tempoRate;
                    f32 pitchRate;
                    if (disabled) {
                        stateNum = ::sStateNumVeryHigh;
                        tempoRate = ::sTempoRateVeryHigh;
                        pitchRate = ::sPitchRateVeryHigh;
                    } else {
                        if (speed < ::sSpeedLowThreshold) {
                            stateNum = ::sStateNumLow;
                        } else if (speed < ::sSpeedMidThreshold) {
                            stateNum = ::sStateNumMid;
                        } else {
                            stateNum = ::sStateNumHigh;
                        }

                        t0 = speed - ::sSpeedMin;
                        t1 = sSpeedMax - ::sSpeedMin;
                        tempoRate = ::sTempoRateMin + (t0 / t1) * (::sTempoRateMax - ::sTempoRateMin);
                        pitchRate = ::sPitchRateNormal;
                    }
                    bgm->changeTrackMuteState(stateNum, frames);
                    modifyTempo(handle, tempoRate);
                    modifyPitch(handle, pitchRate);
                }
                mSteps++;
            }
        }
    }
}

void AudTamakoroBgmCtrl::modifyTempo(JAISoundHandle* pHandle, f32 tempo) {
    if (pHandle != nullptr && pHandle->isSoundAttached()) {
        JAITempoMgr* mgr = pHandle->getSound()->getTempoMgr();
        if (mgr != nullptr) {
            mgr->mTransition.set(tempo, mgr->getTempo(), 10);
        }
    }
}

void AudTamakoroBgmCtrl::modifyPitch(JAISoundHandle* pHandle, f32 pitch) {
    if (pHandle != nullptr && pHandle->isSoundAttached()) {
        for (s32 i = 0; i < 16; i++) {
            if (i == ::sPercTrack) {
                continue;
            }
            JAISoundChild* child = pHandle->getSound()->getChild(i);
            if (child == nullptr) {
                continue;
            }
            child->mMove.movePitch(pitch, 30);
        }
    }
}
