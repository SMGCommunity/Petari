#pragma once

#include <revolution/nwc24.h>
#include <revolution/os.h>

class JKRHeap;
class NWC24MsgObj;

class NWC24SendThread : OSThread {
public:
    NWC24SendThread(s32, JKRHeap*);

    bool requestSend(const u16*, const u16*, const u8*, u32, const u8*, u32, u16, bool, u8);
    bool isDone(NWC24Err*, u32*);

private:
    struct MsgSendStatus {
        /* 0x00 */ const u16* mText;
        /* 0x04 */ const u16* mAltName;
        /* 0x08 */ const u8*  mLetter;
        /* 0x0C */ u32        mLetterSize;
        /* 0x10 */ const u8*  mPicture;
        /* 0x14 */ u32        mPictureSize;
        /* 0x18 */ bool       _18;
        /* 0x19 */ bool       mIsMsgLedPattern;
        /* 0x1C */ NWC24Err   mSentErr;
        /* 0x20 */ u32        mSentSize;
        /* 0x24 */ u16        mTag;
        /* 0x28 */ u8         mDelayHours;
    };

    static void*    threadProc(void*);
    void            initMsgSendStatus();
    static NWC24Err sendMessage(NWC24SendThread::MsgSendStatus*, u32*);
    static bool     checkTotalSize(NWC24SendThread::MsgSendStatus*);
    NWC24Err        setToMyself(NWC24MsgObj*);

private:
    static OSMessage      mMessage;
    static s32            mMessageMax;
    static OSMessageQueue mMessageQueue;

    u32 _unused;

    // TODO: Alignment?
    /* 0x31C */ MsgSendStatus mMsgSendStatus;
};

OSMessage      NWC24SendThread::mMessage;
s32            NWC24SendThread::mMessageMax = 1;
OSMessageQueue NWC24SendThread::mMessageQueue;
