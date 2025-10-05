#pragma once

#include <revolution/types.h>

namespace MR {
    /// @brief Computes the size of a message, in bytes.
    /// @param[in] pTitle A pointer to the null-terminated wide title of the message.
    /// @param[in] pBody A pointer to the null-terminated wide body of the message.
    /// @param imageSize The size of the embedded image, in bytes.
    /// @param param4 TODO.
    /// @return The size of the message, in bytes.
    u32 calcWiiMailSize(const wchar_t* pTitle, const wchar_t* pBody, u32 imageSize, u32 param4);

    /// @brief Determines if the Wii Message Board can receive more messages from this application.
    /// @param mailSize The size of the message, in bytes.
    /// @return `true` if there is sufficient free space, `false` otherwise.
    bool checkWiiMailLimit(u32 mailSize);

    /// @brief Updates the number of bytes sent to the Wii Message Board from the current date.
    /// @param mailSize The size of the message, in bytes.
    void updateWiiMailSentSize(u32 mailSize);

    /// @brief Returns the name of the message's sender.
    /// @param[in] pMessageId A pointer to the null-terminated name of the target message identifier.
    /// @return A pointer to the null-terminated wide name.
    const wchar_t* getMailSender(const char* pMessageId);
};
