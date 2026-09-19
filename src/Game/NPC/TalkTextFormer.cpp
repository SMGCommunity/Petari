#include "Game/NPC/TalkTextFormer.hpp"
#include "Game/NPC/TalkMessageCtrl.hpp"
#include "Game/Screen/CustomTagProcessor.hpp"
#include "Game/System/Language.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include <nw4r/lyt/textBox.h>

void TalkTextFormer_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

namespace {
    nw4r::lyt::TextBox* getTextBoxPane(LayoutActor* pActor, const char* pPaneName) NO_INLINE {
        nw4r::lyt::Pane* pPane = MR::getPane(pActor, pPaneName);
        const nw4r::ut::detail::RuntimeTypeInfo* pTypeInfo = &nw4r::lyt::TextBox::typeInfo;
        nw4r::lyt::TextBox* pTextBox;
        if (pPane != nullptr && pPane->GetRuntimeTypeInfo()->IsDerivedFrom(pTypeInfo)) {
            pTextBox = static_cast< nw4r::lyt::TextBox* >(pPane);
        } else {
            pTextBox = nullptr;
        }

        return pTextBox;
    }
}  // namespace

namespace MR {
    void initTagProcessorRecursive(LayoutActor* pActor, const char* pPaneName, s32 mode) {
        nw4r::lyt::TextBox* pTextBox = getTextBoxPane(pActor, pPaneName);
        if (pTextBox != nullptr) {
            CustomTagProcessor* pProcessor = static_cast< CustomTagProcessor* >(pTextBox->mpTagProcessor);
            f32 rate = 1.25f;
            if (getLanguage() == 0x10) {
                rate = 1.0f;
            } else if (getLanguage() == 0x37) {
                rate = 0.75f;
            }

            switch (mode) {
            case 2:
                pProcessor->initAlpha(0.0f, 0.0f, 0, 0);
                break;
            case 0:
                pProcessor->initAlpha(0.8f * rate, 0.9f, 0, 0);
                break;
            case 1:
                pProcessor->initAlpha(0.35f * rate, 0.9f, 0, 0);
                break;
            }
        }

        nw4r::lyt::Pane* pPane = getPane(pActor, pPaneName);
        for (nw4r::lyt::PaneList::Iterator it = pPane->GetChildList().GetBeginIter(); it != pPane->GetChildList().GetEndIter(); ++it) {
            initTagProcessorRecursive(pActor, it->mName, mode);
        }
    }

    void nextStepTagProcessorRecursive(LayoutActor* pActor, const char* pPaneName) {
        nw4r::lyt::TextBox* pTextBox = getTextBoxPane(pActor, pPaneName);
        if (pTextBox != nullptr) {
            CustomTagProcessor* pProcessor = static_cast< CustomTagProcessor* >(pTextBox->mpTagProcessor);
            pProcessor->mColorIndex = 0;
            pProcessor->mSoundIndex = 0;
            pProcessor->mAlphaCtrl.update();
        }

        nw4r::lyt::Pane* pPane = getPane(pActor, pPaneName);
        for (nw4r::lyt::PaneList::Iterator it = pPane->GetChildList().GetBeginIter(); it != pPane->GetChildList().GetEndIter(); ++it) {
            nextStepTagProcessorRecursive(pActor, it->mName);
        }
    }

    bool isEndStepTagProcessorRecursive(const LayoutActor* pActor, const char* pPaneName, bool isEnd) {
        nw4r::lyt::Pane* pTargetPane = getPane(pActor, pPaneName);
        const nw4r::ut::detail::RuntimeTypeInfo* pTypeInfo = &nw4r::lyt::TextBox::typeInfo;
        nw4r::lyt::TextBox* pTextBox;
        if (pTargetPane != nullptr && pTargetPane->GetRuntimeTypeInfo()->IsDerivedFrom(pTypeInfo)) {
            pTextBox = static_cast< nw4r::lyt::TextBox* >(pTargetPane);
        } else {
            pTextBox = nullptr;
        }
        if (pTextBox != nullptr) {
            CustomTagProcessor* pProcessor = static_cast< CustomTagProcessor* >(pTextBox->mpTagProcessor);
            isEnd &= pProcessor->mAlphaCtrl.isEnd();
        }

        nw4r::lyt::Pane* pPane = getPane(pActor, pPaneName);
        for (nw4r::lyt::PaneList::Iterator it = pPane->GetChildList().GetBeginIter(); it != pPane->GetChildList().GetEndIter(); ++it) {
            isEnd &= isEndStepTagProcessorRecursive(pActor, it->mName, true);
        }

        return isEnd;
    }
}  // namespace MR

TalkTextFormer::TalkTextFormer(LayoutActor* pActor, const char* pPaneName) : mHostActor(pActor), mMsg(), _8(), mPaneName(pPaneName) {
}

void TalkTextFormer::formMessage(const wchar_t* pMessage, s32 mode) {
    mMsg = pMessage;
    _8 = mode;

    MR::setTextBoxMessageRecursive(mHostActor, mPaneName, mMsg);
    MR::initTagProcessorRecursive(mHostActor, mPaneName, _8);
}

void TalkTextFormer::setArg(const CustomTagArg& rTag, s32 argIndex) {
    if (rTag.mArgType == CustomTagArg::Type_Int) {
        MR::setTextBoxArgNumberRecursive(mHostActor, mPaneName, rTag.mIntArg, argIndex);
    } else if (rTag.mArgType == CustomTagArg::Type_Char) {
        MR::setTextBoxArgStringRecursive(mHostActor, mPaneName, rTag.mCharArg, argIndex);
    }

    MR::initTagProcessorRecursive(mHostActor, mPaneName, _8);
}

bool TalkTextFormer::nextPage() {
    const wchar_t* pMessage = MR::getNextMessagePage(mMsg);
    if (pMessage != nullptr) {
        mMsg = pMessage;
        formMessage(pMessage, _8);
        return true;
    }

    return false;
}

bool TalkTextFormer::hasNextPage() const {
    if (mMsg != nullptr) {
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
