#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/System/NerveExecutor.hpp"
#include <revolution/nwc24.h>

class NWC24Messenger;
class NWC24System;
class SysInfoWindow;

namespace NWC24MessengerSub {
    class SendTask {
    public:
        /// @brief Creates a new `SendTask`.
        SendTask();

        /* 0x00 */ bool           _0;
        /* 0x01 */ bool           mIsBG;
        /* 0x02 */ bool           _2;
        /* 0x03 */ bool           mIsMsgLedPattern;
        /* 0x04 */ s32            mRetryNo;
        /* 0x08 */ NWC24Err       mErr;
        /* 0x0C */ s32            mErrCode;
        /* 0x10 */ u32            mSentSize;
        /* 0x14 */ u16            mTag;
        /* 0x14 */ u8             mDelayHours;
        /* 0x18 */ const char*    mTaskName;
        /* 0x1C */ const wchar_t* mMessage;
        /* 0x20 */ const wchar_t* mAltName;
        /* 0x24 */ const u8*      mPicture;
        /* 0x28 */ u32            mPictureSize;
    };

    class SendState : public NerveExecutor {
    public:
        /// @brief Creates a new `SendState`.
        /// @param[in] pHost A pointer to the owning WiiConnect24 messenger.
        SendState(NWC24Messenger* pHost);

        void initAfterResourceLoaded();
        void updateNerve();
        void calcAnim();
        void draw() const;
        void reset();
        void exeWait();
        void exeRetry();
        void exeOpenBG();
        void exeRunBG();
        void exeCloseBG();
        void exeOpenFG();
        void exeRunFG();
        void exeCloseFG();
        void exeUpdateLimitFG();
        void exeRetryErrorFG();
        void exeOpenErrorFG();
        void exeSendFatalErrorFG();
        void exeCloseErrorFG();
        void exeSendFullErrorFG();
        void exeWaitCloseSysInfoMini();
        bool selectTask();
        bool openSystem();
        bool isRestorableError() const;
        bool send();
        bool checkFinish();
        bool closeSystem();
        bool hasFatalErrorOccured() const;
        void doneTask();
        void appearSysInfo(const char*, s32);
        bool isEndSysInfoMini() const;
        void setNerveAfterSysInfoWindowMiniDisappear(const Nerve*);

    private:
        /* 0x08 */ NWC24Messenger* mHost;
        /* 0x0C */ SendTask*       mTask;
        /* 0x10 */ SysInfoWindow*  mWindow;
        /* 0x14 */ SysInfoWindow*  mMiniWindow;
        /* 0x18 */ const Nerve*    _18;
    };
}; // namespace NWC24MessengerSub

class NWC24Messenger : public NameObj {
public:
    /// @brief Creates a new `NWC24Messenger`.
    /// @param pName A pointer to the null-terminated name of the object.
    NWC24Messenger(const char* pName);

    virtual void movement();
    virtual void draw() const;
    virtual void calcAnim();

    void                         initAfterResourceLoaded();
    void                         send(const char*, const wchar_t*, const wchar_t*, const u8*, u32, bool, bool, u16, u8);
    void                         term(const char*);
    bool                         isSent(const char*) const;
    bool                         isError(const char*) const;
    void                         prepareReset();
    bool                         isRunning() const;
    void                         reset();
    NWC24MessengerSub::SendTask* findTask(const char*) const;
    NWC24MessengerSub::SendTask* selectTask() const;
    void                         clearBackgroundTask();

    /* 0x0C */ NWC24MessengerSub::SendTask*  mForegroundTask;
    /* 0x10 */ NWC24MessengerSub::SendTask*  mBackgroundTaskArray;
    /* 0x14 */ NWC24MessengerSub::SendState* mSendState;
    /* 0x18 */ NWC24System*                  mSystem;
    /* 0x1C */ bool                          _1C;
};

namespace MR {
    class SendMailObj {
    public:
        /// @brief Creates a new `SendMailObj`.
        /// @param[in] pTaskName A pointer to the null-terminated name of the messenger's job.
        SendMailObj(const char* pTaskName);

        void setMessageDirect(const wchar_t*);
        void setSenderID(const char*);
        void setImage(const u8*, u32);
        void setBGEnable();
        void setBGDisable();
        void setLedOff();
        void setTag(u16);
        void setDelay(u8);
        void send();

    private:
        /* 0x00 */ const char*    mTaskName;
        /* 0x04 */ const wchar_t* mMessage;
        /* 0x08 */ const wchar_t* mSenderID;
        /* 0x0C */ const u8*      mImage;
        /* 0x10 */ u32            mImageSize;
        /* 0x14 */ bool           mIsBG;
        /* 0x15 */ bool           mIsLed;
        /* 0x16 */ u16            mTag;
        /* 0x18 */ u8             mDelay;
    };

    void termMail(const char*);
    bool isMailSent(const char*);
    bool isMailErrorHappened(const char*);
}; // namespace MR
