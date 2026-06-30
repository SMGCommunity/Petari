#include "Game/Screen/LayoutCoreUtil.hpp"
#include "Game/Screen/CustomTagProcessor.hpp"
#include "Game/Screen/ReplaceTagProcessor.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include <cstring>
#include <revolution/mtx.h>

namespace LayoutCoreUtil {

    const char* getPaneUserData(const nw4r::lyt::Pane* pPane, char* pUserData) {
        MR::copyMemory(pUserData, pPane->mUserData, 8);
        pUserData[8] = '\0';
        return pUserData;
    }

    u16 setTextBoxMessage(nw4r::lyt::TextBox* pTextBox, const wchar_t* pMsg) {
        wchar_t buffer[512];
        ReplaceTagProcessor::Replace(buffer, pMsg);
        return pTextBox->SetString(buffer, 0, MR::getStringLengthWithMessageTag(buffer));
    }

    void initTextBoxPane(nw4r::lyt::TextBox* pTextBox, const char* pMessageId, u32 allocSize) {
        CustomTagProcessor* processor = new CustomTagProcessor(pTextBox);

        if (strncmp(pTextBox->mName, "Sha", 3) == 0) {
            processor->mAlphaCtrl._0 = true;
        } else {
            processor->mAlphaCtrl._0 = false;
        }

        if (strncmp(pTextBox->mName, "Txt", 3) == 0) {
            processor->mAlphaCtrl._1 = true;
        } else {
            processor->mAlphaCtrl._1 = false;
        }

        if (strncmp(pTextBox->mName, "Inf", 3) == 0) {
            processor->mAlphaCtrl._2 = true;
        } else {
            processor->mAlphaCtrl._2 = false;
        }

        pTextBox->mpTagProcessor = processor;

        const wchar_t* message = nullptr;
        if (pMessageId != nullptr) {
            message = MR::getLayoutMessageDirect(pMessageId);
        }

        if (message != nullptr) {
            reallocateTextBoxMessage(pTextBox, MR::getStringLengthWithMessageTag(message));
            setTextBoxMessage(pTextBox, message);
        } else {
            reallocateTextBoxMessage(pTextBox, allocSize - 1);
            s32 maxNumChars = pTextBox->mSize.width / pTextBox->mFontSize.width;
            wchar_t defaultStr[] = L"XXXXXXXX";

            s32 end = maxNumChars <= 9 ? maxNumChars : 9;
            defaultStr[end] = L'\0';
            setTextBoxMessage(pTextBox, defaultStr);
        }
    }

    void reallocateTextBoxMessage(nw4r::lyt::TextBox* pTextBox, u32 allocSize) {
        pTextBox->AllocStringBuffer(allocSize + 1);
    }
};  // namespace LayoutCoreUtil

namespace MR {
    void convertScreenPosToLayoutPos(TVec2f* pLayoutPos, const TVec2f& rScreenPos) {
        pLayoutPos->x = rScreenPos.x * 608.0f / MR::getScreenWidth() - 304.0f;
        pLayoutPos->y = -(rScreenPos.y * MR::getScreenHeight() / MR::getScreenHeight() - (MR::getScreenHeight() * 0.5f));
    }

    void convertLayoutPosToScreenPos(TVec2f* pScreenPos, const TVec2f& rLayoutPos) {
        pScreenPos->x = rLayoutPos.x * MR::getScreenWidth() / 608.0f + MR::getScreenWidth() * 0.5f;
        pScreenPos->y = MR::getScreenHeight() * 0.5f + -rLayoutPos.y * MR::getScreenHeight() / MR::getScreenHeight();
    }

    void convertPaneLocalPosToScreenPos(TVec2f* pScreenPos, const nw4r::lyt::Pane* pPane, const TVec2f& rLocalPos) {
        TVec3f pos(rLocalPos);
        TVec3f layoutPos;
        PSMTXMultVec(pPane->mGlbMtx, pos, layoutPos);
        convertLayoutPosToScreenPos(pScreenPos, layoutPos);
    }
};  // namespace MR
