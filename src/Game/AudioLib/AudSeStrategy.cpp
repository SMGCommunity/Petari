#include "Game/AudioLib/AudSeStrategy.hpp"
#include "Game/AudioLib/AudEffector.hpp"
#include "Game/AudioLib/AudSoundId.hpp"
#include "Game/AudioLib/AudSoundInfo.hpp"
#include "Game/AudioLib/AudSystem.hpp"
#include "Game/AudioLib/AudWrap.hpp"
#include <JSystem/JAudio2/JAISe.hpp>
#include <JSystem/JAudio2/JASCriticalSection.hpp>


void AudSeStrategy_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

extern const s16 sIIRCutoff[128][4];

void AudSeStrategy::calc(JAISe* pSe) {
    JAISoundID soundID = pSe->mSoundID;

    AudSoundInfo* info = AudWrap::getSoundInfo();
    if (info == nullptr) {
        return;
    }

    if (info->isUseFxMix(soundID)) {
        pSe->getAuxiliary().moveFxMix(AudWrap::getSystem()->mAudEffector->mFxSend, 0);
    }

    if (!info->isUseLpf(soundID)) {
        return;
    }

    AudEffector* effector = AudWrap::getSystem()->mAudEffector;
    s32 cutoff = effector->mCutoff;
    if (cutoff < 0 || cutoff > 0x7F) {
        cutoff = 0x7F;
    }

    if (soundID.getGroupID() == 1 && cutoff < 50) {  // SE_PV
        cutoff = 50;
    }

    switch (soundID) {
    case SE_AT_LV_WATERFALL_S:
    case SE_AT_LV_WATERFALL_L:
    case SE_AT_LV_WATERFALL_UPPER_S:
    case SE_AT_LV_WATERFALL_UPPER_L:
    case SE_OJ_SEAGULL_CHIRP: {
        if (effector->mSpecialFx == 2) {
            cutoff = 10;
            pSe->getAuxiliary().moveVolume(0.5f, 0);
        }
        break;
    }
    case SE_AT_LV_WIND_RABBIT_MAZE:
    case SE_OJ_LV_FOUNTAIN: {
        if (effector->mSpecialFx == 1) {
            cutoff = 20;
            pSe->getAuxiliary().moveVolume(0.5f, 5);
        } else {
            cutoff = 0x7F;
            pSe->getAuxiliary().moveVolume(1.0f, 5);
        }
        break;
    }
    }

    JASTrack* track = pSe->getTrack();
    if (track != nullptr) {
        JASCriticalSection crit;
        for (u32 i = 0; i < JASTrack::MAX_CHILDREN; i++) {
            if (track->getChild(i) != nullptr) {
                track->getChild(i)->setIIR(sIIRCutoff[cutoff]);
            }
        }
    }
}

void AudSeStrategy::mix(JAISe* pSe, JASSoundParams* pParams) {
}

JAISoundStrategy< JAISe >* AudSeStrategyMgr::newStrategy(JAISoundID soundID) {
    AudSeStrategy* strategy = new AudSeStrategy();
    if (strategy == nullptr) {
        return nullptr;
    }
    return strategy;
}

bool AudSeStrategyMgr::deleteStrategy(JAISoundStrategy< JAISe >* pStrategy) {
    if (pStrategy == nullptr) {
        return false;
    }

    delete pStrategy;
    return true;
}
