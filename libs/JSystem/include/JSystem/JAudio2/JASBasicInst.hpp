#pragma once

#include "JSystem/JAudio2/JASChannel.hpp"
#include "JSystem/JAudio2/JASGadget.hpp"
#include "JSystem/JAudio2/JASOscillator.hpp"
#include "JSystem/JAudio2/JASSoundParams.hpp"

struct JKRHeap;

struct JASInstEffect;

const int OSC_MAX = 2;

struct JASInstParam : public JASSoundParams {
    JASInstParam();  // gets overwritten in OverwriteJAudio

    void initChannelType() {
        mChannelType = JASChannel::CH_WAVE;
    }

    void init_18() {
        _18 = 0;
    }

    void initOscillators() {
        mOscillatorData = nullptr;
        mNumOscillators = 0;
    }

    void init_24() {
        _24 = false;
    }

    void initRelease() {
        mDirectRelease = 0;
    }

    void init() {
        initChannelType();
        init_18();
        initOscillators();
        init_24();
        initRelease();
    }

    /* 0x14 */ u32 mChannelType;
    /* 0x18 */ u32 _18;
    /* 0x1C */ JASOscillator::Data** mOscillatorData;
    /* 0x20 */ u32 mNumOscillators;
    /* 0x24 */ bool _24;
    /* 0x26 */ u16 mDirectRelease;
};

struct JASInst {
    virtual ~JASInst(){};
    virtual bool getParam(int, int, JASInstParam*) const = 0;
    virtual u32 getType() const = 0;
};

struct JASBasicInst : public JASInst {
    struct TVelomap {
        /* 0x00 */ s32 _0;
        /* 0x04 */ s32 _4;
        /* 0x08 */ f32 mVolume;
        /* 0x0C */ f32 mPitch;
    };

    struct TKeymap {
        ~TKeymap();
        TKeymap() {
            mHighKey = -1;
            mVelomapCount = 0;
            mVelomap = nullptr;
        }
        s32 getHighKey() const {
            return mHighKey;
        }
        void setHighKey(int key) {
            mHighKey = key;
        }

        void setVeloRegionCount(u32, JKRHeap*);
        TVelomap* getVeloRegion(int);
        const TVelomap* getVeloRegion(int) const;

        u32 getVeloRegionCount() const {
            return mVelomapCount;
        }

        /* 0x0 */ s32 mHighKey;
        /* 0x4 */ u32 mVelomapCount;
        /* 0x8 */ TVelomap* mVelomap;
    };

    JASBasicInst();
    void setKeyRegionCount(u32, JKRHeap*);
    void appendEffect(JASPtrListAdaptor< JASInstEffect >*);
    void setOsc(int, JASOscillator::Data const*);
    TKeymap* getKeyRegion(int);
    TKeymap* getKeyRegion(int) const;

    virtual ~JASBasicInst();
    virtual bool getParam(int, int, JASInstParam*) const;
    virtual u32 getType() const {
        return 'BSIC';
    };

    void setVolume(f32 volume) {
        mVolume = volume;
    }
    void setPitch(f32 pitch) {
        mPitch = pitch;
    }
    u32 getKeyRegionCount() const {
        return mKeymapCount;
    }

    /* 0x04 */ f32 mVolume;
    /* 0x08 */ f32 mPitch;
    /* 0x0C */ JASOscillator::Data const* mOscillatorData[OSC_MAX];
    /* 0x14 */ JASPtrList< JASInstEffect > mEffects;
    /* 0x18 */ u32 mKeymapCount;
    /* 0x1C */ TKeymap* mKeymap;
};
