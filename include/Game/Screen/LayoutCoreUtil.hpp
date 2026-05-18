#pragma once

#include <JSystem/JGeometry/TVec.hpp>
#include <nw4r/lyt/pane.h>
#include <nw4r/lyt/textBox.h>

namespace LayoutCoreUtil {
    void getPaneUserData(const nw4r::lyt::Pane*, char*);
    void setTextBoxMessage(nw4r::lyt::TextBox* pTextBox, const wchar_t* pMsg);
    void initTextBoxPane(nw4r::lyt::TextBox* pTextBox, const char* pStr, u32);
}  // namespace LayoutCoreUtil

namespace MR {
    void convertScreenPosToLayoutPos(TVec2f*, const TVec2f&);
    void convertLayoutPosToScreenPos(TVec2f*, const TVec2f&);
    void convertPaneLocalPosToScreenPos(TVec2f*, const nw4r::lyt::Pane*, const TVec2f&);
}  // namespace MR
