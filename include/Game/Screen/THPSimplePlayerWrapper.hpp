#pragma once

#include "Game/System/NerveExecutor.hpp"
#include "revolution/types.h"
#include <revolution/gx/GXStruct.h>
#include <revolution/thp.h>

class THPSimplePlayerWrapper;

class THPSimplePlayerStaticAudio {
public:
    THPSimplePlayerStaticAudio();

    static s16* audioCallback(s32);
    static THPSimplePlayerWrapper* mPlayer;
};

class THPSimplePlayerWrapper : public NerveExecutor {
public:
    THPSimplePlayerWrapper(const char*);

    bool init(s32);
    void quit();
    bool open(const char*);
    bool close();
    s32 getUseTextureCount() const;
    u32 calcNeedMemory();
    bool setBuffer(u8*);
    bool preLoad(s32);
    bool loadStop();
    s32 decode(s32);
    s32 drawCurrentFrame(_GXRenderModeObj*, u32, u32, u32, u32);
    bool getVideoInfo(THPVideoInfo*) const;
    f32 getFrameRate() const;
    s32 getTotalFrame() const;
    bool videoDecode(u8*);
    void readFrameAsync();
    void checkPrefetch();
    void dvdCallBack(s32);
    void readAsyncCallBack(s32);
    s32 getNextBuffer(u32) const NO_INLINE;
    bool tryDvdOpen(const char*);
    void setupParams();
    void exeReadHeader();
    void exeReadFrameComp();
    void exeReadVideoComp();
    void exeReadAudioComp();
    void endReadHeader();
    void endReadFrameComp();
    void endReadVideoComp();
    void endReadAudioComp();
    void exeReadPreLoad();
    void endReadPreLoadOne();
    bool checkComponentsInFrame(s32);
    bool tryFinishDvdOpen();
    void initAudio();
    bool isAudioProcessValid();
    s16* audioCallback(s32);
    void mixAudio(s16*, u32);
    void resetAudioParams();
    bool setVolume(s32, s32);
    bool isPreLoading() const;
    void setUnpauseFrameFlag();

    void exeWait() {}

    static THPSimplePlayerStaticAudio mStaticAudioPlayer;

    u8 _8;
    u8 _9;
    u32 _C;
    u32 _10;
    DVDFileInfo mFileInfo;              // 0x14
    THPHeader mHeader;                  // 0x50
    THPFrameCompInfo mFrameComp;        // 0x80
    THPVideoInfo mVideoInfo;            // 0x94
    THPAudioInfo mAudioInfo;            // 0xA0
    void* mTHPWork;                     // 0xB0
    u8 mOpen;                           // 0xB4
    u8 mPreFetchState;                  // 0xB5
    u8 mAudioState;                     // 0xB6
    u8 mLoop;                           // 0xB7
    u8 mAudioExist;                     // 0xB8
    u32 mCurOffset;                     // 0xBC
    u8 mDvdError;                       // 0xC0
    u32 mReadProgress;                  // 0xC4
    s32 mNextDecodeIndex;               // 0xC8
    volatile s32 mReadIndex;            // 0xCC
    s32 mReadSize;                      // 0xD0
    s32 mTotalReadFrame;                // 0xD4
    f32 mCurrentVolume;                 // 0xD8
    f32 mTargetVolume;                  // 0xDC
    f32 mDeltaVolume;                   // 0xE0
    s32 mRampCount;                     // 0xE4
    THPReadBuffer mReadBuffer[0x14];    // 0xE8
    THPTextureSet mTextureSet[2];       // 0x1D8
    THPAudioBuffer mAudioBuffer[0x14];  // 0x1F8
    s32 mAudioDecodeIndex;              // 0x2E8
    s32 mAudioOutputIndex;              // 0x2EC
    u8 _2F0;
    f32 _2F4;
    f32 _2F8;
    s32 _2FC;
    s32* mSoundBuffer[2];   // 0x300
    u32 mSoundBufferIndex;  // 0x308
    u8 _30C;
    u8 _30D;
    u8 _30E;
    u8 _30F;
    u8 _310;
    s32 _314;
};
