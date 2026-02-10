#pragma once

#include "JSystem/JUtility/TColor.hpp"
#include <cstring>

struct ResFONT {};

class JUTFont {
public:
    struct TWidth {
        u8 _0;
        u8 _1;
    };

    virtual ~JUTFont();
    virtual void setGX() = 0;
    virtual void setGX(JUtility::TColor, JUtility::TColor);
    virtual f32 drawChar_scale(f32, f32, f32, f32, int, bool) = 0;
    virtual int getLeading() const = 0;
    virtual s32 getAscent() const = 0;
    virtual s32 getDescent() const = 0;
    virtual s32 getHeight() const = 0;
    virtual s32 getWidth() const = 0;
    virtual void getWidthEntry(int i_no, TWidth* width) const = 0;
    virtual s32 getCellWidth() const { return getWidth(); }
    virtual s32 getCellHeight() const { return getHeight(); }
    virtual int getFontType() const = 0;
    virtual ResFONT* getResFont() const = 0;
    virtual bool isLeadByte(int a1) const = 0;

    void setCharColor(JUtility::TColor);
    f32 drawString_size_scale(f32, f32, f32, f32, const char*, u32, bool);

    void drawString(int posX, int posY, const char* str, bool visible) { drawString_size(posX, posY, str, strlen(str), visible); }

    void drawString_size(int posX, int posY, const char* str, u32 len, bool visible) {
        drawString_size_scale(posX, posY, getWidth(), getHeight(), str, len, visible);
    }

    bool mValid;               // 0x04
    bool mFixed;               // 0x05
    int mFixedWidth;           // 0x08
    JUtility::TColor mColor1;  // 0x0C
    JUtility::TColor mColor2;  // 0x10
    JUtility::TColor mColor3;  // 0x14
    JUtility::TColor mColor4;  // 0x18
};
