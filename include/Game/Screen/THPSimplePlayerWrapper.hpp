#pragma once

#include "Game/System/NerveExecutor.hpp"
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
    s32 drawCurrentFrame(GXRenderModeObj*, u32, u32, u32, u32);
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
    void exeWait();
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

    static THPSimplePlayerStaticAudio mStaticAudioPlayer;

    /* 0x008 */u8 _8;
    /* 0x009 */u8 _9;
    /* 0x00C */u32 _C;
    /* 0x010 */u32 _10;
    /* 0x014 */ DVDFileInfo mFileInfo;
    /* 0x050 */ THPHeader mHeader;
    /* 0x080 */ THPFrameCompInfo mFrameComp;
    /* 0x094 */ THPVideoInfo mVideoInfo;
    /* 0x0A0 */ THPAudioInfo mAudioInfo;
    /* 0x0B0 */ void* mTHPWork;
    /* 0x0B4 */ u8 mOpen;
    /* 0x0B5 */ u8 mPreFetchState;
    /* 0x0B6 */ u8 mAudioState;
    /* 0x0B7 */ u8 mLoop;
    /* 0x0B8 */ u8 mAudioExist;
    /* 0x0BC */ u32 mCurOffset;
    /* 0x0C0 */ u8 mDvdError;
    /* 0x0C4 */ u32 mReadProgress;
    /* 0x0C8 */ s32 mNextDecodeIndex;
    /* 0x0CC */ volatile s32 mReadIndex;
    /* 0x0D0 */ s32 mReadSize;
    /* 0x0D4 */ s32 mTotalReadFrame;
    /* 0x0D8 */ f32 mCurrentVolume;
    /* 0x0DC */ f32 mTargetVolume;
    /* 0x0E0 */ f32 mDeltaVolume;
    /* 0x0E4 */ s32 mRampCount;
    /* 0x0E8 */ THPReadBuffer mReadBuffer[20];
    /* 0x1D8 */ THPTextureSet mTextureSet[2];
    /* 0x1F8 */ THPAudioBuffer mAudioBuffer[20];
    /* 0x2E8 */ s32 mAudioDecodeIndex;
    /* 0x2EC */ s32 mAudioOutputIndex;
    /* 0x2F0 */ u8 _2F0;
    /* 0x2F4 */ f32 _2F4;
    /* 0x2F8 */ f32 _2F8;
    /* 0x2FC */ s32 _2FC;
    /* 0x300 */ s32* mSoundBuffer[2];
    /* 0x308 */ u32 mSoundBufferIndex;
    /* 0x30C */ u8 _30C;
    /* 0x30D */ u8 _30D;
    /* 0x30E */ u8 _30E;
    /* 0x30F */ u8 _30F;
    /* 0x310 */ u8 _310;
    /* 0x314 */ s32 _314;
};
