#include "JSystem/JAudio2/JAUSoundMgr.hpp"
#include "JSystem/JAudio2/JAIAudience.hpp"
#include "JSystem/JAudio2/JAISound.hpp"
#include "JSystem/JAudio2/JAISoundInfo.hpp"
#include "JSystem/JAudio2/JASDriverIF.hpp"

JAUSoundMgr::JAUSoundMgr() : JAISoundStarter(true), mSeMgr(true), mSeqMgr(true), mStreamMgr(true) {
}

bool JAUSoundMgr::startSound(JAISoundID soundID, JAISoundHandle* pHandle, const TVec3f* pPos) {
    JAISoundInfo* info = JASGlobalInstance< JAISoundInfo >::sInstance;

    int sectionID;
    if (info != nullptr) {
        sectionID = info->getSoundType(soundID);
    } else {
        sectionID = soundID.getSectionID();
    }

    switch (sectionID) {
    case JAISoundID::SOUND_SE:
        return mSeMgr.startSound(soundID, pHandle, pPos);
    case JAISoundID::SOUND_SEQ:
        return mSeqMgr.startSound(soundID, pHandle, pPos);
    case JAISoundID::SOUND_STREAM:
        return mStreamMgr.startSound(soundID, pHandle, pPos);
    }

    return false;
}

void JAUSoundMgr::setAudience(JAIAudience* pAudience) {
    mSeMgr.setAudience(pAudience);
    mSeqMgr.setAudience(pAudience);
    mStreamMgr.setAudience(pAudience);
}

void JAUSoundMgr::calc() {
    mSeMgr.calc();
    mSeqMgr.calc();
    mStreamMgr.calc();
}

void JAUSoundMgr::mixOut() {
    mSeMgr.mixOut();
    mSeqMgr.mixOut();
    mStreamMgr.mixOut();
}

void JAUSoundMgr::stopSoundID(JAISoundID soundID) {
    mSeMgr.stopSoundID(soundID);
    mSeqMgr.stopSoundID(soundID);
    mStreamMgr.stopSoundID(soundID);
}

void JAUSoundMgr::stopSync() {
    mSeMgr.stop();
    mSeqMgr.stop();
    mStreamMgr.stop();

    while (getNumActiveSounds() > 0) {
        calc();
        if (getNumActiveSounds() == 0) {
            break;
        }
        mixOut();
        JASDriver::waitSubFrame();
    }
}

void JAUSoundMgr::stop(u32 fadeTime) {
    mSeMgr.stop(fadeTime);
    mSeqMgr.stop(fadeTime);
    mStreamMgr.stop(fadeTime);
}

void JAUSoundMgr::initParams() {
    mSeMgr.initParams();
    mSeqMgr.initParams();
    mStreamMgr.initParams();
}

int JAUSoundMgr::getNumActiveSounds() const {
    return mSeMgr.getNumActiveSe() + mSeqMgr.getNumActiveSeqs() + mStreamMgr.getNumActiveStreams();
}
