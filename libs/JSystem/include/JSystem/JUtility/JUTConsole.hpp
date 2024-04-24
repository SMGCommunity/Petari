#pragma once

#include <revolution.h>
#include "JSystem/JKernel/JKRDisposer.hpp"
#include "JSystem/JGadget/linklist.hpp"
#include "JSystem/JUtility/TColor.hpp"

void JUTWarningConsole_f(const char *, ...);

class JUTFont;

class JUTConsole : public JKRDisposer {
public:
    static void print_f(const char *, ...);

    enum EConsoleType {
        CONSOLE_TYPE_0 = 0,
        CONSOLE_TYPE_1 = 1,
        CONSOLE_TYPE_2 = 2,
    };

    void doDraw(EConsoleType) const;
    u32 getOutput() const { return mOutput; }

    JGadget::TLinkListNode mListNode;           // _18
    u32 _20;
    int mMaxLines;                              // _24
    u8* mBuf;                                   // _28
    bool _2C;
    int _30;
    int _34;
    int _38;
    int _3C;
    int mPositionX;                             // _40
    int mPositionY;                             // _44
    u32 mHeight;                                // _48
    JUTFont* mFont;                             // _4C
    f32 mFontSizeX;                             // _50
    f32 mFontSizeY;                             // _54
    int mOutput;                                // _58
    JUtility::TColor _5C;
    JUtility::TColor _60;
    int _64;
    bool mVisible;                              // _68
    bool _69;
    bool _6A;
    bool _6B;
};

class JUTConsoleManager {
public:
    JUTConsoleManager();

    void appendConsole(JUTConsole *);
    void removeConsole(JUTConsole *);
    void draw() const;
    void drawDirect(bool) const;
    void setDirectConsole(JUTConsole *);

    static JUTConsoleManager* createManager(JKRHeap *);

    typedef JGadget::TLinkList<JUTConsole, -offsetof(JUTConsole, mListNode)> ConsoleList;

    ConsoleList mList;                  // _0
    JUTConsole* mActiveConsole;         // _C
    JUTConsole* mDirectConsole;         // _10
};