#pragma once

#include <revolution/types.h>

class LuigiMailDirector {
public:
    /// @brief Creates a new `LuigiMailDirector`.
    LuigiMailDirector();

    void initAfterResourceLoaded();
    void lost();
    void found();
    void sendMail();
    void sendMail(u8 delayHours);
    void writeSendSize();
    void reset();
    void prepareMessage();
    u8 calcDelayHours() const;

public:
    /// @brief The number of characters in the message describing Luigi as having gone missing.
    /* 0x00 */ u32 mLostMessageLength;

    /// @brief The pointer to the null-terminated wide message describing Luigi as having gone missing.
    /* 0x04 */ wchar_t* mLostMessage;

    /// @brief The number of characters in the message describing Luigi as having been saved.
    /* 0x08 */ u32 mFoundMessageLength;

    /// @brief The pointer to the null-terminated wide message describing Luigi as having been saved.
    /* 0x0C */ wchar_t* mFoundMessage;

    /// @brief The number of messages to send describing Luigi as having gone missing.
    /* 0x10 */ s32 mLostMessageNum;

    /// @brief The number of messages to send describing Luigi as having been saved.
    /* 0x14 */ s32 mFoundMessageNum;
};
