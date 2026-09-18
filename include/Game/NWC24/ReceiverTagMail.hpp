#pragma once

#include <revolution/types.h>

class ReceiverTagMail {
public:
    /// @brief Creates a new `ReceiverTagMail`.
    /// @param[in] pTaskName A pointer to the null-terminated name of the messenger's job.
    /// @param[in] pMessageId A pointer to the null-terminated name of the body message.
    /// @param[in] pSenderID A pointer to the null-terminated name of the title message.
    ReceiverTagMail(const char* pTaskName, const char* pMessageID, const char* pSenderID);

    void send(const u8* pImage, u32 imageSize, bool isLedOn);
    void update();
    bool isDone() const;
    bool isError() const;

public:
    /* 0x00 */ const char* mTaskName;
    /* 0x04 */ const char* mMessageID;
    /* 0x08 */ const char* mSenderID;
    /* 0x0C */ wchar_t* mMessage;
    /* 0x10 */ u32 mMessageLength;
    /* 0x14 */ bool mIsError;
    /* 0x18 */ u32 _18;
};
