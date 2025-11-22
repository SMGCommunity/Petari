#include "Game/NWC24/ReceiverTagMail.hpp"
#include "Game/Map/FileSelectFunc.hpp"
#include "Game/NWC24/NWC24Messenger.hpp"
#include "Game/Screen/ReplaceTagProcessor.hpp"
#include "Game/System/GameDataFunction.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/StringUtil.hpp"

ReceiverTagMail::ReceiverTagMail(const char* pTaskName, const char* pMessageID, const char* pSenderID)
    : mMessage(nullptr), mMessageLength(0), mIsError(false), _18(1) {
    mTaskName = pTaskName;
    mMessageID = pMessageID;
    mSenderID = pSenderID;
    mMessageLength = MR::getStringLengthWithMessageTag(MR::getGameMessageDirect(mMessageID)) + FileSelectFunc::getMiiNameBufferSize();
    mMessage = new wchar_t[mMessageLength];

    MR::zeroMemory(mMessage, mMessageLength * sizeof(wchar_t));
}

/*
// The non-Korean implementation of the function.
void ReceiverTagMail::send(const u8* pImage, u32 imageSize) {
    ReplaceTagFunction::ReplaceArgs(
        mMessage,
        mMessageLength,
        MR::getGameMessageDirect(mMessageID),
        GameDataFunction::getUserName());

    MR::sendMail(mTaskName, mMessage, mSenderID, pImage, imageSize, false);

    _18 = 0;
}
*/

void ReceiverTagMail::send(const u8* pImage, u32 imageSize, bool isLedOn) {
    // FIXME: Missing crclr instruction.
    ReplaceTagFunction::ReplaceArgs(mMessage, mMessageLength, MR::getGameMessageDirect(mMessageID), GameDataFunction::getUserName());

    MR::SendMailObj sendMailObj = MR::SendMailObj(mTaskName);

    sendMailObj.setMessageDirect(mMessage);
    sendMailObj.setSenderID(mSenderID);
    sendMailObj.setImage(pImage, imageSize);
    sendMailObj.setBGDisable();

    if (!isLedOn) {
        sendMailObj.setLedOff();
    }

    sendMailObj.send();

    _18 = 0;
}

void ReceiverTagMail::update() {
    if (_18 != 0) {
        return;
    }

    if (!MR::isMailSent(mTaskName)) {
        return;
    }

    mIsError = MR::isMailErrorHappened(mTaskName);

    MR::termMail(mTaskName);

    _18 = 1;
}

bool ReceiverTagMail::isDone() const {
    return _18 - 1 == 0;
}

bool ReceiverTagMail::isError() const {
    return mIsError;
}
