#include "Game/Map/FileSelectFunc.hpp"
#include "Game/NWC24/LuigiMailDirector.hpp"
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
}; // namespace

LuigiMailDirector::LuigiMailDirector()
    : mLostMessageLength(0),
      mLostMessage(nullptr),
      mFoundMessageLength(0),
      mFoundMessage(nullptr),
      mLostMessageNum(0),
      mFoundMessageNum(0) {}

void LuigiMailDirector::initAfterResourceLoaded() {
    u32 lostMessageLength = MR::getStringLengthWithMessageTag(MR::getGameMessageDirect(cLostMessageID)) + FileSelectFunc::getMiiNameBufferSize();

    mLostMessageLength = lostMessageLength;
    mLostMessage = new wchar_t[lostMessageLength];

    u32 foundMessageLength = MR::getStringLengthWithMessageTag(MR::getGameMessageDirect(cFoundMessageID)) + FileSelectFunc::getMiiNameBufferSize();

    mFoundMessageLength = foundMessageLength;
    mFoundMessage = new wchar_t[foundMessageLength];
}

/*
// The non-Korean implementation of the function.
void LuigiMailDirector::lost() {
    mLostMessageNum++;
}
*/

void LuigiMailDirector::lost() {
    mLostMessageNum = 1;
    mFoundMessageNum = 0;
}

/*
// The non-Korean implementation of the function.
void LuigiMailDirector::found() {
    mFoundMessageNum = 1;
    mLostMessageNum = 0;
}
*/

void LuigiMailDirector::found() {
    mFoundMessageNum = 1;
    mLostMessageNum = 0;
}

/*
// The non-Korean implementation of the function.
void LuigiMailDirector::sendMail() {
    prepareMessage();

    while (mLostMessageNum != 0 || mFoundMessageNum != 0) {
        if (mLostMessageNum != 0 && mFoundMessageNum <= mLostMessageNum) {
            MR::sendMail("FindingLuigi", mLostMessage, "WiiMessageFromKinopio", nullptr, 0, true);
            mLostMessageNum--;
        }

        if (mFoundMessageNum != 0) {
            MR::sendMail("FindingLuigi", mFoundMessage, "WiiMessageFromKinopio", nullptr, 0, true);
            mFoundMessageNum--;
        }
    }
}
*/

void LuigiMailDirector::sendMail() {
    sendMail(calcDelayHours());
}

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

/*
// The non-Korean implementation of the function.
void LuigiMailDirector::writeSendSize() {
    prepareMessage();

    const wchar_t* pMailSender = MR::getMailSender("WiiMessageFromKinopio");
    u32 mailSize = 0;
    s32 lostMessageNum = mLostMessageNum;
    s32 foundMessageNum = mFoundMessageNum;

    while (lostMessageNum != 0 || foundMessageNum != 0) {
        if (lostMessageNum != 0 && foundMessageNum <= lostMessageNum) {
            mailSize += MR::calcWiiMailSize(pMailSender, mLostMessage, 0, 0);
            lostMessageNum--;
        }

        if (foundMessageNum != 0) {
            mailSize += MR::calcWiiMailSize(pMailSender, mFoundMessage, 0, 0);
            foundMessageNum--;
        }
    }

    if (MR::checkWiiMailLimit(mailSize)) {
        MR::updateWiiMailSentSize(mailSize);
    }
    else {
        mLostMessageNum = 0;
        mFoundMessageNum = 0;
    }
}
*/

void LuigiMailDirector::writeSendSize() {
    prepareMessage();

    const u16* pMailSender = reinterpret_cast<const u16*>(MR::getMailSender("WiiMessageFromKinopio"));
    u32        mailSize = 0;
    s32        lostMessageNum = mLostMessageNum;
    s32        foundMessageNum = mFoundMessageNum;

    if (lostMessageNum != 0 || foundMessageNum != 0) {
        if (lostMessageNum != 0) {
            mailSize = MR::calcWiiMailSize(pMailSender, reinterpret_cast<u16*>(mLostMessage), 0, 0);
        } else if (foundMessageNum != 0) {
            mailSize = MR::calcWiiMailSize(pMailSender, reinterpret_cast<u16*>(mFoundMessage), 0, 0);
        }
    }

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
    // FIXME: Missing crclr instruction.
    ReplaceTagFunction::ReplaceArgs(
        mLostMessage,
        mLostMessageLength,
        MR::getGameMessageDirect(cLostMessageID),
        GameDataFunction::getUserName());
    // FIXME: Missing crclr instruction.
    ReplaceTagFunction::ReplaceArgs(
        mFoundMessage,
        mFoundMessageLength,
        MR::getGameMessageDirect(cFoundMessageID),
        GameDataFunction::getUserName());
}

u8 LuigiMailDirector::calcDelayHours() const {
    OSCalendarTime td;

    OSTicksToCalendarTime(OSGetTime(), &td);

    // FIXME: Missing addi and clrlwi instructions within return statements.
    if (td.hour >= 1 && td.hour < 13) {
        return 13 - td.hour;
    } else if (td.hour < 1) {
        return 1 - td.hour;
    } else {
        return 25 - td.hour;
    }
}
