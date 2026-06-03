#include "Game/AudioLib/AudUtil.hpp"
#include "Game/Util/FileUtil.hpp"
#include "JSystem/JAudio2/JASAiCtrl.hpp"
#include "JSystem/JAudio2/JASBasicInst.hpp"
#include "JSystem/JAudio2/JASMutex.hpp"
#include "JSystem/JAudio2/JASWaveArcLoader.hpp"
#include "JSystem/JSupport/JSupport.hpp"
#include "cstring"
#include <JSystem/JAudio2/JASTrack.hpp>

JASChannel* JASBank::noteOn(JASBank const* pBank, int progNo, u8 pitch, u8 velocity, u16 priority, JASChannel::Callback pCallback,
                            void* pChannelMgr) {
    if (progNo >= 0xf0) {
        return noteOnOsc(progNo - 0xf0, pitch, velocity, priority, pCallback, pChannelMgr);
    }

    if (pBank == nullptr) {
        return NULL;
    }

    JASInstParam params;
    if (!pBank->getInstParam(progNo, pitch, velocity, &params)) {
        return NULL;
    }

    JASWaveBank* waveBank = pBank->getWaveBank();
    if (!waveBank) {
        return NULL;
    }

    JASWaveHandle* waveHandle = waveBank->getWaveHandle(params._18);
    if (!waveHandle) {
        return NULL;
    }

    const JASWaveInfo* waveInfo = waveHandle->getWaveInfo();
    if (!waveInfo) {
        return NULL;
    }

    int wavePtr = waveHandle->getWavePtr();
    if (!wavePtr) {
        return NULL;
    }

    JASChannel* channel = new JASChannel(pCallback, pChannelMgr);
    if (!channel) {
        return NULL;
    }

    channel->setPriority(priority);
    channel->_DC.mWaveInfo = *waveInfo;
    channel->_104 = wavePtr;
    channel->_DC._0 = params._14;
    channel->setBankDisposeID(pBank);
    channel->setInitPitch(params.mPitch * (waveInfo->_4 / JASDriver::getDacRate()));
    if (params._24 == 0) {
        channel->setKey(pitch - waveInfo->_1);
    }
    channel->setInitVolume(params.mVolume);
    channel->setVelocity(velocity);
    channel->setInitPan(params.mPan);
    channel->setInitFxmix(params.mFxMix);
    channel->setInitDolby(params.mDolby);
    for (u32 i = 0; i < params.mNumOscillators; i++) {
        channel->setOscInit(i, params.mOscillatorData[i]);
    }
    channel->setDirectRelease(params.mDirectRelease);
    if (!channel->play()) {
        return NULL;
    }
    return channel;
}

JASChannel* JASTrack::channelStart(JASTrack::TChannelMgr* pChannelMgr, u32 pitch, u32 velocity, u32 updateTimer) {
    JASBank* bank = nullptr;
    if (mBankTable != nullptr) {
        bank = mBankTable->getBank(mBankNumber);
    }

    JASChannel* channel = JASBank::noteOn(bank, mProgNumber, pitch, velocity, mNoteOnPrio | mReleasePrio << 8, channelUpdateCallback, pChannelMgr);
    if (channel == nullptr) {
        if (AudUtil::isProperCurrentStageOrAudioDebugMode()) {
            JASInstParam params;
            if (!bank->getInstParam(mProgNumber, JSULoByte(pitch), JSULoByte(velocity), &params)) {
                return nullptr;
            }

            JASWaveBank* waveBank = bank->getWaveBank();
            if (!waveBank) {
                return nullptr;
            }

            JASWaveHandle* waveHandle = waveBank->getWaveHandle(params._18);
            if (!waveHandle) {
                return nullptr;
            }

            const JASWaveInfo* waveInfo = waveHandle->getWaveInfo();
            if (!waveInfo) {
                return nullptr;
            }

            int wavePtr = waveHandle->getWavePtr();
            if (!wavePtr) {
                return nullptr;
            }
        }
        return nullptr;
    }

    channel->setUpdateTimer(updateTimer);
    channel->setSkipSamples(mSkipSample);
    channel->setVibrateDelay(mVibDelay);
    channel->setTremoloDelay(mTremDelay);
    channel->setParams(pChannelMgr->mParams);

    for (u32 i = 0; i < 6; i++) {
        channel->setMixConfig(i, mMixConfig[i]);
    }

    overwriteOsc(channel);
    if (mDirectRelease != 0) {
        channel->setDirectRelease(mDirectRelease);
    }

    return channel;
}

void JASWaveArc::setFileName(const char* pName) {
    char* currentDir = JASWaveArcLoader::getCurrentDir();
    size_t length = strlen(currentDir);
    length = length + strlen(pName);
    char* path = new (JASKernel::getSystemHeap(), -4) char[length + 1];
    strcpy(path, currentDir);
    strcat(path, pName);
    path[length] = '\0';
    char name[256];
    MR::makeFileNameConsideringLanguage(name, sizeof(name), path);
    int entryNum = DVDConvertPathToEntrynum(name);
    delete[] path;
    if (entryNum < 0) {
        return;
    }
    setEntryNum(entryNum);
}

bool JASWaveArc::load(JASHeap* pHeap) {
    if (mEntryNum < 0) {
        return false;
    }

    JASMutexLock mutexLock(&mMutex);
    if (mStatus != 0) {
        return false;
    }

    if (pHeap == nullptr) {
        pHeap = JASWaveArcLoader::getRootHeap();
    }

    if (mHeap.alloc(pHeap, mFileLength) == false) {
        return false;
    }
    return sendLoadCmd();
}

JASInstParam::JASInstParam() : _14(0), _18(0), mOscillatorData(nullptr), mNumOscillators(0), _24(false), mDirectRelease(0) {
}
