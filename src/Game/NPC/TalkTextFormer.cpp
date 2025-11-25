#include "Game/NPC/TalkTextFormer.hpp"
#include "Game/NPC/TalkMessageCtrl.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MessageUtil.hpp"

TalkTextFormer::TalkTextFormer(LayoutActor* actor, const char* paneName) : mHostActor(actor), mMsg(nullptr), _8(0), mPaneName(paneName) {}

bool TalkTextFormer::nextPage() {
    const wchar_t* message = MR::getNextMessagePage(mMsg);
    if (message) {
        mMsg = message;
        formMessage(message, _8);
        return true;
    }

    return false;
}

bool TalkTextFormer::hasNextPage() const {
    if (mMsg) {
        return MR::getNextMessagePage(mMsg) != nullptr;
    }

    return false;
}

void TalkTextFormer::updateTalking() {
    MR::nextStepTagProcessorRecursive(mHostActor, mPaneName);
}

bool TalkTextFormer::isTextAppearedAll() const {
    if (_8 == 2) {
        return true;
    }

    return MR::isEndStepTagProcessorRecursive(mHostActor, mPaneName, true);
}

void TalkTextFormer::setArg(const CustomTagArg& tag, s32 arg2) {
    if (tag._4 == 0) {
        MR::setTextBoxArgNumberRecursive(mHostActor, mPaneName, tag.mIntArg, arg2);
    } else if (tag._4 == 1) {
        MR::setTextBoxArgStringRecursive(mHostActor, mPaneName, tag.mCharArg, arg2);
    }
    MR::initTagProcessorRecursive(mHostActor, mPaneName, _8);
}

void TalkTextFormer::formMessage(const wchar_t* message, long arg2) {
    mMsg = message;
    _8 = arg2;

    MR::setTextBoxMessageRecursive(mHostActor, mPaneName, mMsg);
    MR::initTagProcessorRecursive(mHostActor, mPaneName, _8);
}
