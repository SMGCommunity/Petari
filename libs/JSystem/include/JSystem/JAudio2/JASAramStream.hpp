#pragma once

#include "JSystem/JAudio2/JASTaskThread.hpp"

class JASChannel;

namespace JASDsp {
    struct TChannel;
}

class JASAramStream {
public:
    static const int CHANNEL_MAX = 6;

    typedef void (*StreamCallback)(u32, JASAramStream*, void*);

    enum CallbackType {
        /* 0 */ CB_START,
        /* 1 */ CB_STOP,
    };

    // Used internally for passing data to task functions
    struct TaskData {
        /* 0x0 */ JASAramStream* stream;
        /* 0x4 */ u32 _4;
        /* 0x8 */ int _8;
    };

    struct Header {
        /* 0x00 */ u32 tag;
        /* 0x04 */ u8 _4[5];
        /* 0x09 */ u8 format;
        /* 0x0A */ u8 bits;
        /* 0x0C */ u16 channels;
        /* 0x0E */ u16 loop;
        /* 0x10 */ int _10;
        /* 0x14 */ u8 _14[4];
        /* 0x18 */ int loop_start;
        /* 0x1C */ int loop_end;
        /* 0x20 */ u32 block_size;
        /* 0x24 */ u8 _24[4];
        /* 0x28 */ u8 _28;
        /* 0x29 */ u8 _29[0x17];
    };

    struct BlockHeader {
        /* 0x00 */ u32 tag;
        /* 0x04 */ u32 _4;
        /* 0x08 */ struct {
            s16 _0;
            s16 _2;
        } _8[6];
    };

    static void initSystem(u32, u32);
    JASAramStream();
    void init(u32, u32, StreamCallback, void*);
    bool prepare(s32, int);
    bool start();
    bool stop(u16);
    bool pause(bool);
    bool cancel();
    u32 getBlockSamples() const;
    static void headerLoadTask(void*);
    static void firstLoadTask(void*);
    static void loadToAramTask(void*);
    static void finishTask(void*);
    static void prepareFinishTask(void*);
    bool headerLoad(u32, int);
    bool load();
    static s32 channelProcCallback(void*);
    static s32 dvdErrorCheck(void*);
    static void channelCallback(u32, JASChannel*, JASDsp::TChannel*, void*);
    void updateChannel(u32, JASChannel*, JASDsp::TChannel*);
    s32 channelProc();
    void channelStart();
    void channelStop(u16);

    void setPitch(f32 pitch) {
        mPitch = pitch;
    }
    void setVolume(f32 volume) {
        for (int i = 0; i < CHANNEL_MAX; i++) {
            mChannelVolume[i] = volume;
        }
    }

    void setPan(f32 pan) {
        for (int i = 0; i < CHANNEL_MAX; i++) {
            mChannelPan[i] = pan;
        }
    }

    void setFxmix(f32 fxMix) {
        for (int i = 0; i < CHANNEL_MAX; i++) {
            mChannelFxMix[i] = fxMix;
        }
    }

    void setDolby(f32 dolby) {
        for (int i = 0; i < CHANNEL_MAX; i++) {
            mChannelDolby[i] = dolby;
        }
    }

    void setChannelVolume(int channel, f32 volume) {
        mChannelVolume[channel] = volume;
    }

    void setChannelPan(int channel, f32 pan) {
        mChannelPan[channel] = pan;
    }

    void setChannelFxmix(int channel, f32 fxMix) {
        mChannelFxMix[channel] = fxMix;
    }

    void setChannelDolby(int channel, f32 dolby) {
        mChannelDolby[channel] = dolby;
    }

    static u32 getBlockSize() {
        return sBlockSize;
    }

    /* 0x000 */ OSMessageQueue _000;
    /* 0x020 */ OSMessageQueue _020;
    /* 0x040 */ void* _040[16];
    /* 0x080 */ void* _080[4];
    /* 0x090 */ JASChannel* mChannels[CHANNEL_MAX];
    /* 0x0A8 */ JASChannel* mUpdateChannel;
    /* 0x0AC */ bool _0AC;
    /* 0x0AD */ bool _0AD;
    /* 0x0AE */ u8 _0AE;
    /* 0x0B0 */ int _0B0;
    /* 0x0B4 */ int _0B4;
    /* 0x0B8 */ u32 _0B8;
    /* 0x0BC */ int _0BC;
    /* 0x0C0 */ bool _0C0;
    /* 0x0C4 */ volatile u32 _0C4;
    /* 0x0C8 */ volatile f32 _0C8;
    /* 0x0CC */ DVDFileInfo mDvdFileInfo;
    /* 0x108 */ u32 _108;
    /* 0x10C */ int _10C;
    /* 0x110 */ u32 mBlock;
    /* 0x114 */ u8 _114;
    /* 0x118 */ u32 _118;
    /* 0x11C */ int _11C;
    /* 0x120 */ int _120;
    /* 0x124 */ int _124;
    /* 0x128 */ u16 _128;
    /* 0x12C */ int _12C;
    /* 0x130 */ s16 _130[CHANNEL_MAX];
    /* 0x13C */ s16 _13C[CHANNEL_MAX];
    /* 0x148 */ int _148;
    /* 0x14C */ u32 _14C;
    /* 0x150 */ StreamCallback mCallback;
    /* 0x154 */ void* mCallbackData;
    /* 0x158 */ u16 _158;
    /* 0x15A */ u16 mChannelNum;
    /* 0x15C */ u32 mBufCount;
    /* 0x160 */ u32 _160;
    /* 0x164 */ u32 _164;
    /* 0x168 */ bool mLoop;
    /* 0x16C */ u32 mLoopStart;
    /* 0x170 */ u32 mLoopEnd;
    /* 0x174 */ f32 mVolume;
    /* 0x178 */ f32 mPitch;
    /* 0x17C */ f32 mChannelVolume[CHANNEL_MAX];
    /* 0x194 */ f32 mChannelPan[CHANNEL_MAX];
    /* 0x1AC */ f32 mChannelFxMix[CHANNEL_MAX];
    /* 0x1C4 */ f32 mChannelDolby[CHANNEL_MAX];
    /* 0x1DC */ u16 _1DC[CHANNEL_MAX];

    static JASTaskThread* sLoadThread;
    static u8* sReadBuffer;
    static u32 sBlockSize;
    static u32 sChannelMax;
};
