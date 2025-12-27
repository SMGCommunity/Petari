#include "Game/NWC24/NWC24Function.hpp"
// #include "Game/NWC24/UTF16Util.hpp"
#include "Game/System/GameDataFunction.hpp"
#include "Game/Util/MessageUtil.hpp"
#include <revolution/os.h>

namespace {
    /// @brief The maximum number of bytes that can be sent to the Wii Message Board on any given date.
    static u32 sSentSizeMax = 400000;

    /// @brief Determines if the most recent message was sent to the Wii Message Board today.
    /// @param[out] pTicks An optional pointer to initialize with the current number of ticks.
    /// @return `true` if the most recent message was sent today, `false` otherwise.
    bool isLastUpdateToday(OSTime* pTicks) {
        OSTime ticks;
        OSCalendarTime tdSent;
        OSCalendarTime td;

        ticks = GameDataFunction::getSysConfigFileTimeSent();
        OSTicksToCalendarTime(ticks, &tdSent);

        ticks = OSGetTime();
        OSTicksToCalendarTime(ticks, &td);

        if (pTicks != nullptr) {
            *pTicks = ticks;
        }

        return td.year == tdSent.year && td.mon == tdSent.mon && td.mday == tdSent.mday;
    }
};  // namespace

namespace MR {
    // calcWiiMailSize

    bool checkWiiMailLimit(u32 mailSize) {
        if (isLastUpdateToday(nullptr)) {
            if (GameDataFunction::getSysConfigFileSentBytes() + mailSize > sSentSizeMax) {
                return false;
            }
        }

        return true;
    }

    void updateWiiMailSentSize(u32 mailSize) {
        u32 sentBytes = GameDataFunction::getSysConfigFileSentBytes();
        OSTime ticks;

        if (!isLastUpdateToday(&ticks)) {
            sentBytes = 0;
        }

        sentBytes += mailSize;

        GameDataFunction::setSysConfigFileTimeSent(ticks);
        GameDataFunction::setSysConfigFileSentBytes(sentBytes);
    }

    const wchar_t* getMailSender(const char* pMessageId) {
        return MR::getGameMessageDirect(pMessageId);
    }
};  // namespace MR
