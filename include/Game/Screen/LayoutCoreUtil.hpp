#pragma once

#include <JSystem/JGeometry/TVec.hpp>
#include <nw4r/lyt/pane.h>
#include <nw4r/lyt/textBox.h>

namespace LayoutCoreUtil {
    const char* getPaneUserData(const nw4r::lyt::Pane*, char*);
    u16 setTextBoxMessage(nw4r::lyt::TextBox* pTextBox, const wchar_t* pMsg);
    void initTextBoxPane(nw4r::lyt::TextBox* pTextBox, const char* pStr, u32 allocSize);
    void reallocateTextBoxMessage(nw4r::lyt::TextBox* pTextBox, u32 allocSize);
};  // namespace LayoutCoreUtil

namespace MR {
    void convertScreenPosToLayoutPos(TVec2f*, const TVec2f&);
    void convertLayoutPosToScreenPos(TVec2f*, const TVec2f&);
    void convertPaneLocalPosToScreenPos(TVec2f*, const nw4r::lyt::Pane*, const TVec2f&);
};  // namespace MR
