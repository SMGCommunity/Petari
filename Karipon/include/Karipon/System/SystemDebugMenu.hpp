#pragma once

#include <JSystem/JGeometry/TVec.hpp>
#include <nw4r/ut/WideTextWriter.h>

class SystemDebugMenu {
public:
    SystemDebugMenu();

    ~SystemDebugMenu();

    void update();
    void initWriter();
    void initDraw(u32 color);
    bool printText(bool selectable, const wchar_t* pText);
    bool printTextF(bool selectable, const wchar_t* pText, ...);
    void printFillBox(const TVec2f& tl, const TVec2f& br, u32 color);

    nw4r::ut::WideTextWriter mWriter;
    u32 mPageIndex;
    u32 mSelectIndex;
    u32 mSelectCurrentIndex;
    bool mIsVisible;
};