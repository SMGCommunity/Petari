#include "Game/NWC24/LuigiMailDirector.hpp"
#include "Game/Map/FileSelectFunc.hpp"
#include "Game/NWC24/NWC24Function.hpp"
#include "Game/NWC24/NWC24Messenger.hpp"
#include "Game/Screen/ReplaceTagProcessor.hpp"
#include "Game/System/GameDataFunction.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/StringUtil.hpp"

namespace {
    const char* cLostMessageID = "KinopioEmergencyLetter_000";
    const char* cFoundMessageID = "KinopioEmergencyLetter_001";
};  // namespace

LuigiMailDirector::LuigiMailDirector()
    : mLostMessageLength(0), mLostMessage(nullptr), mFoundMessageLength(0), mFoundMessage(nullptr), mLostMessageNum(0), mFoundMessageNum(0) {
}

void LuigiMailDirector::initAfterResourceLoaded() {
    u32 lostMessageLength = MR::getStringLengthWithMessageTag(MR::getGameMessageDirect(::cLostMessageID)) + FileSelectFunc::getMiiNameBufferSize();

    mLostMessageLength = lostMessageLength;
    mLostMessage = new wchar_t[lostMessageLength];

    u32 foundMessageLength = MR::getStringLengthWithMessageTag(MR::getGameMessageDirect(::cFoundMessageID)) + FileSelectFunc::getMiiNameBufferSize();

    mFoundMessageLength = foundMessageLength;
    mFoundMessage = new wchar_t[foundMessageLength];
}

void LuigiMailDirector::lost() {
#if (VERSION == RMGJ01 || VERSION == RMGE01 || VERSION == RMGP01)
    mLostMessageNum++;
#elif (VERSION == RMGK01)
    mLostMessageNum = 1;
    mFoundMessageNum = 0;
#endif
}

void LuigiMailDirector::found() {
#if (VERSION == RMGJ01 || VERSION == RMGE01 || VERSION == RMGP01)
    mFoundMessageNum++;
#elif (VERSION == RMGK01)
    mFoundMessageNum = 1;
    mLostMessageNum = 0;
#endif
}

void LuigiMailDirector::sendMail() {
#if (VERSION == RMGJ01 || VERSION == RMGE01 || VERSION == RMGP01)
    prepareMessage();

    while (mLostMessageNum != 0 || mFoundMessageNum != 0) {
        if (mLostMessageNum != 0 && mLostMessageNum >= mFoundMessageNum) {
            MR::sendMail("FindingLuigi", mLostMessage, "WiiMessageFromKinopio", nullptr, 0, true);
            mLostMessageNum--;
        }

        if (mFoundMessageNum != 0) {
            MR::sendMail("FindingLuigi", mFoundMessage, "WiiMessageFromKinopio", nullptr, 0, true);
            mFoundMessageNum--;
        }
    }
#elif (VERSION == RMGK01)
    sendMail(calcDelayHours());
#endif
}

#if (VERSION == RMGK01)
void LuigiMailDirector::sendMail(u8 delayHours) {
    prepareMessage();

    if (mLostMessageNum == 0 && mFoundMessageNum == 0) {
        return;
    }

    MR::SendMailObj sendMailObj = MR::SendMailObj("FindingLuigi");

    sendMailObj.setSenderID("WiiMessageFromKinopio");
    sendMailObj.setBGEnable();

    if (!MR::isMsgLedPattern()) {
        sendMailObj.setLedOff();
    }

    sendMailObj.setTag(GameDataFunction::getUserFileIndex());

    if (delayHours != 0) {
        sendMailObj.setDelay(delayHours);
    }

    if (mLostMessageNum != 0) {
        sendMailObj.setMessageDirect(mLostMessage);
    } else if (mFoundMessageNum != 0) {
        sendMailObj.setMessageDirect(mFoundMessage);
    }

    sendMailObj.send();

    mLostMessageNum = 0;
    mFoundMessageNum = 0;
}
#endif

void LuigiMailDirector::writeSendSize() {
    prepareMessage();

    const u16* pMailSender = reinterpret_cast< const u16* >(MR::getMailSender("WiiMessageFromKinopio"));
    u32 lostMessageNum = mLostMessageNum;
    u32 foundMessageNum = mFoundMessageNum;
    u32 mailSize = 0;

#if (VERSION == RMGJ01 || VERSION == RMGE01 || VERSION == RMGP01)
    while (lostMessageNum != 0 || foundMessageNum != 0) {
        if (lostMessageNum != 0 && lostMessageNum >= foundMessageNum) {
            mailSize += MR::calcWiiMailSize(pMailSender, reinterpret_cast< u16* >(mLostMessage), 0, 0);
            lostMessageNum--;
        }

        if (foundMessageNum != 0) {
            mailSize += MR::calcWiiMailSize(pMailSender, reinterpret_cast< u16* >(mFoundMessage), 0, 0);
            foundMessageNum--;
        }
    }
#elif (VERSION == RMGK01)
    if (lostMessageNum != 0 || foundMessageNum != 0) {
        if (lostMessageNum != 0) {
            mailSize = MR::calcWiiMailSize(pMailSender, reinterpret_cast< u16* >(mLostMessage), 0, 0);
        } else if (foundMessageNum != 0) {
            mailSize = MR::calcWiiMailSize(pMailSender, reinterpret_cast< u16* >(mFoundMessage), 0, 0);
        }
    }
#endif

    if (MR::checkWiiMailLimit(mailSize)) {
        MR::updateWiiMailSentSize(mailSize);
    } else {
        mLostMessageNum = 0;
        mFoundMessageNum = 0;
    }
}

void LuigiMailDirector::reset() {
    mLostMessageNum = 0;
    mFoundMessageNum = 0;
}

void LuigiMailDirector::prepareMessage() {
    ReplaceTagFunction::ReplaceArgs(mLostMessage, mLostMessageLength, MR::getGameMessageDirect(::cLostMessageID), GameDataFunction::getUserName());
    ReplaceTagFunction::ReplaceArgs(mFoundMessage, mFoundMessageLength, MR::getGameMessageDirect(::cFoundMessageID), GameDataFunction::getUserName());
}

#if (VERSION == RMGK01)
u8 LuigiMailDirector::calcDelayHours() const {
    OSCalendarTime td;

    OSTicksToCalendarTime(OSGetTime(), &td);

    if (td.hour >= 1 && td.hour < 13) {
        return static_cast< u8 >(13 - td.hour) + 4;
    } else if (td.hour < 1) {
        return static_cast< u8 >(1 - td.hour) + 4;
    } else {
        return static_cast< u8 >(25 - td.hour) + 4;
    }
}
#endif
