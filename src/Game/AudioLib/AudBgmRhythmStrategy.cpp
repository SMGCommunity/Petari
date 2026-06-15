#include "Game/AudioLib/AudBgmRhythmStrategy.hpp"
#include "Game/AudioLib/AudBgm.hpp"
#include "Game/AudioLib/AudSoundInfo.hpp"
#include "Game/AudioLib/AudSystem.hpp"
#include "Game/AudioLib/AudWrap.hpp"
#include "Game/RhythmLib/AudChordInfo.hpp"
#include "Game/RhythmLib/AudRhythmMeSystem.hpp"

bool AudBgmRhythmStrategy::set(AudBgm* pBgm, s32 bgmIndex) {
    AudSoundInfo* info = AudWrap::getSoundInfo();
    if (info == nullptr) {
        return false;
    }

    JAISoundHandle* handle = pBgm->getRhythmHandle();
    if (handle == nullptr) {
        return false;
    }

    u32 soundID = handle->getSound()->getID();
    if (handle->isSoundAttached()) {
        if (info->isRhythmBgm(soundID)) {
            AudWrap::getRhythmMeSystem()->setSeq(*handle, bgmIndex);
            mBgmIdx = bgmIndex;
            mBgm = pBgm;
            return true;
        }
    }

    return false;
}

void AudBgmRhythmStrategy::reject() {
    if (mBgmIdx < 0) {
        return;
    }

    AudWrap::getRhythmMeSystem()->rejectSeq(mBgmIdx);
    if (isDominant()) {
        AudWrap::getSystem()->getChordInfo()->invalidiate();  // [sic]
    }
    mBgm = nullptr;
    mBgmIdx = -1;
}

bool AudBgmRhythmStrategy::setDominant() {
    if (mBgmIdx < 0) {
        return false;
    }

    AudSoundInfo* info = AudWrap::getSoundInfo();
    if (info == nullptr) {
        return false;
    }

    JAISoundHandle* handle = mBgm->getRhythmHandle();
    if (handle == nullptr) {
        return false;
    }

    u32 soundID = handle->getSound()->getID();

    if (!info->isRhythmBgm(soundID)) {
        return false;
    }

    AudWrap::getRhythmMeSystem()->setUsingRhythmParser(mBgmIdx);

    u16 id = info->getChordResIdBgm(soundID);
    if (id != 0xFFFF) {
        AudWrap::getSystem()->getChordInfo()->loadChordInfo(id, false);
    } else {
        AudWrap::getSystem()->getChordInfo()->invalidiate();  // [sic]
    }

    return true;
}

bool AudBgmRhythmStrategy::isDominant() const {
    if (mBgmIdx < 0) {
        return false;
    }

    return mBgmIdx == AudWrap::getRhythmMeSystem()->getBgmIdx();
}

// TODO: static init of pool memory for AudMe and AudMeTrack
