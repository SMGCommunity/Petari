#include "JSystem/JAudio2/JASBank.hpp"
#include "JSystem/JAudio2/JASAiCtrl.hpp"
#include "JSystem/JAudio2/JASBasicInst.hpp"
#include "JSystem/JAudio2/JASChannel.hpp"

JASChannel* JASBank::noteOn(JASBank const* pBank, int progNo, u8 pitch, u8 velocity, u16 priority, JASChannel::Callback pCallback,
                            void* pChannelMgr) {
    if (progNo >= 0xf0) {
        return noteOnOsc(progNo - 0xf0, pitch, velocity, priority, pCallback, pChannelMgr);
    }

    if (pBank == nullptr) {
        return nullptr;
    }

    JASInstParam params;
    if (!pBank->getInstParam(progNo, pitch, velocity, &params)) {
        return nullptr;
    }

    JASWaveBank* waveBank = pBank->getWaveBank();
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

    JASChannel* channel = new JASChannel(pCallback, pChannelMgr);
    if (!channel) {
        return nullptr;
    }

    channel->setPriority(priority);
    channel->mWaveInfo = *waveInfo;
    channel->mWavePtr = wavePtr;
    channel->mChannelType = params.mChannelType;
    channel->setBankDisposeID(pBank);
    channel->setInitPitch(params.mPitch * (waveInfo->mSampleRate / JASDriver::getDacRate()));
    if (params._24 == 0) {
        channel->setKey(pitch - waveInfo->mBaseKey);
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
        return nullptr;
    }
    return channel;
}

static JASOscillator::Point const OSC_RELEASE_TABLE[] = {
    {0x0001, 0x000A, 0x0000},
    {0x000F, 0x0000, 0x0000},
};

JASChannel* JASBank::noteOnOsc(int progNo, u8 pitch, u8 velocity, u16 priority, void (*param_4)(u32, JASChannel*, JASDsp::TChannel*, void*),
                               void* param_5) {
    static const JASOscillator::Data OSC_ENV = {0, 1.0f, nullptr, OSC_RELEASE_TABLE, 1.0f, 0.0f};

    JASChannel* channel = new JASChannel(param_4, param_5);
    if (channel == nullptr) {
        return nullptr;
    }
    channel->setPriority(priority);
    channel->mProgNo = progNo;
    channel->mChannelType = JASChannel::CH_OSC;
    channel->setInitPitch(16736.016f / JASDriver::getDacRate());
    channel->setKey(pitch - channel->mWaveInfo.mBaseKey);
    channel->setVelocity(velocity);
    channel->setOscInit(0, &OSC_ENV);
    if (!channel->play()) {
        return nullptr;
    }
    return channel;
}
