#pragma once

#include "JSystem/JAudio2/JASHeapCtrl.hpp"
#include "JSystem/JAudio2/JASLfo.hpp"
#include "JSystem/JAudio2/JASOscillator.hpp"
#include "JSystem/JAudio2/JASSoundParams.hpp"
#include "JSystem/JAudio2/JASWaveInfo.hpp"
#include <revolution.h>

struct JASDSPChannel;

namespace JASDsp {
    struct TChannel;
};

class JASChannelParams {
public:
    JASChannelParams() {
        mVolume = 1.0f;
        mPitch = 1.0f;
        _8 = 0.0f;
        mPan = 0.5f;
        mFxMix = 0.0f;
        mDolby = 0.0f;
    }
    void init() {
        mVolume = 1.0f;
        mPitch = 1.0f;
        _8 = 0.0f;
        mPan = 0.5f;
        mFxMix = 0.0f;
        mDolby = 0.0f;
    }

    /* 0x00 */ f32 mVolume;
    /* 0x04 */ f32 mPitch;
    /* 0x08 */ f32 _8;
    /* 0x0C */ f32 mPan;
    /* 0x10 */ f32 mFxMix;
    /* 0x14 */ f32 mDolby;
};

class JASChannel : public JASPoolAllocObject_MultiThreaded< JASChannel > {
public:
    typedef void (*Callback)(u32, JASChannel*, JASDsp::TChannel*, void*);
    static const int BUSOUT_CPUCH = 6;
    static const int OSC_NUM = 2;

    enum CallbackType {
        /* 0 */ CB_PLAY,
        /* 1 */ CB_START,
        /* 2 */ CB_STOP,
        /* 3 */ CB_TIMER,
    };

    enum Status {
        /* 0 */ STATUS_STOP,
        /* 1 */ STATUS_PLAY,
        /* 2 */ STATUS_RELEASE,
    };

    enum ChannelType {
        /* 0 */ CH_WAVE = 0,
        /* 2 */ CH_OSC = 2,
    };

    struct PanVector {
        /* 0x0 */ f32 mSound;
        /* 0x4 */ f32 mEffect;
        /* 0x8 */ f32 mChannel;
    };

    union MixConfig {
        u16 whole;
        struct {
            u8 upper;
            u8 lower0 : 4;
            u8 lower1 : 4;
        } parts;
    };

    JASChannel(Callback, void*);
    ~JASChannel();
    int play();
    int playForce();
    void release(u16);
    void setOscInit(u32, JASOscillator::Data const*);
    void setMixConfig(u32, u16);
    static f32 calcEffect(JASChannel::PanVector const*);
    static f32 calcPan(JASChannel::PanVector const*);
    void effectOsc(u32, JASOscillator::EffectParams*);
    void setKeySweepTarget(s32, u32);
    void updateEffectorParam(JASDsp::TChannel*, u16*, JASOscillator::EffectParams const&);
    static s32 dspUpdateCallback(u32, JASDsp::TChannel*, void*);
    s32 initialUpdateDSPChannel(JASDsp::TChannel*);
    s32 updateDSPChannel(JASDsp::TChannel*);
    void updateAutoMixer(JASDsp::TChannel*, f32, f32, f32, f32);
    void updateMixer(f32, f32, f32, f32, u16*);
    void free();
    static void initBankDisposeMsgQueue();
    static void receiveBankDisposeMsg();
    bool checkBankDispose() const;

    void setPauseFlag(bool flag) {
        mPauseFlag = flag;
    }
    void setUpdateTimer(u32 timer) {
        mUpdateTimer = timer;
    }
    void setBankDisposeID(const void* id) {
        mBankDisposeID = id;
    }
    void setDirectRelease(u16 release) {
        mOscillators[0].setDirectRelease(release);
    }
    void setVibrate(f32 depth, f32 pitch) {
        mVibrate.setDepth(depth);
        mVibrate.setPitch(pitch);
    }
    void setVibrateDelay(u16 delay) {
        mVibrate.setDelay(delay);
    }
    void setTremolo(f32 depth, f32 pitch) {
        mTremolo.setDepth(depth);
        mTremolo.setPitch(pitch);
    }
    void setTremoloDelay(u16 delay) {
        mTremolo.setDelay(delay);
    }
    void setPriority(u16 prio) {
        mPriority = prio;
    }
    void setParams(const JASChannelParams& params) {
        mParams = params;
    }
    void setInitVolume(f32 volume) {
        mSoundParams.mVolume = volume;
    }
    void setInitFxmix(f32 fxMix) {
        mSoundParams.mFxMix = fxMix;
    }
    void setInitPitch(f32 pitch) {
        mSoundParams.mPitch = pitch;
    }
    void setInitPan(f32 pan) {
        mSoundParams.mPan = pan;
    }
    void setInitDolby(f32 dolby) {
        mSoundParams.mDolby = dolby;
    }
    void setKey(s32 key) {
        mKey = key;
    }
    void setVelocity(u32 velocity) {
        mVelocity = velocity;
    }
    void setSkipSamples(u32 skipSamples) {
        mSkipSamples = skipSamples;
    }
    bool isDolbyMode() const {
        return mMixConfig[0].whole == 0xffff;
    }

    /* 0x00 */ int mStatus;
    /* 0x04 */ u8 mPauseFlag;
    /* 0x08 */ JASDSPChannel* mDspCh;
    /* 0x0C */ Callback mCallback;
    /* 0x10 */ void* mCallbackData;
    /* 0x14 */ u32 mUpdateTimer;
    /* 0x18 */ const void* mBankDisposeID;
    /* 0x1C */ JASOscillator mOscillators[2];
    /* 0x5C */ JASLfo mVibrate;
    /* 0x74 */ JASLfo mTremolo;
    /* 0x8C */ MixConfig mMixConfig[BUSOUT_CPUCH];
    /* 0x98 */ u16 mPriority;
    /* 0x9C */ JASChannelParams mParams;
    /* 0xB4 */ JASSoundParams mSoundParams;
    /* 0xC8 */ s16 mKey;
    /* 0xCA */ u16 mVelocity;
    /* 0xCC */ f32 mKeySweep;
    /* 0xD0 */ f32 mKeySweepTarget;
    /* 0xD4 */ u32 mKeySweepCount;
    /* 0xD8 */ u32 mSkipSamples;
    /* 0xDC */ u32 mChannelType;
    /* 0xE0 */ JASWaveInfo mWaveInfo;
    union {
        /* 0x104 */ int mWavePtr;
        /* 0x104 */ int mProgNo;
    };

    static OSMessageQueue sBankDisposeMsgQ;
    static OSMessage sBankDisposeMsg[16];
    static OSMessage sBankDisposeList[16];
    static int sBankDisposeListSize;
};
