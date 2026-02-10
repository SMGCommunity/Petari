#pragma once

#include "JSystem/JGadget/linklist.hpp"
#include "JSystem/JKernel/JKRDisposer.hpp"
#include "JSystem/JUtility/TColor.hpp"
#include <revolution.h>

void JUTWarningConsole_f(const char*, ...);

class JUTFont;

class JUTConsole : public JKRDisposer {
public:
    static void print_f(const char*, ...);

    enum EConsoleType {
        CONSOLE_TYPE_0 = 0,
        CONSOLE_TYPE_1 = 1,
        CONSOLE_TYPE_2 = 2,
    };

    void doDraw(EConsoleType) const;
    u32 getOutput() const { return mOutput; }

    void print(char const*);

    JGadget::TLinkListNode mListNode;  // 0x18
    u32 _20;
    int mMaxLines;  // 0x24
    u8* mBuf;       // 0x28
    bool _2C;
    int _30;
    int _34;
    int _38;
    int _3C;
    int mPositionX;  // 0x40
    int mPositionY;  // 0x44
    u32 mHeight;     // 0x48
    JUTFont* mFont;  // 0x4C
    f32 mFontSizeX;  // 0x50
    f32 mFontSizeY;  // 0x54
    int mOutput;     // 0x58
    JUtility::TColor _5C;
    JUtility::TColor _60;
    int _64;
    bool mVisible;  // 0x68
    bool _69;
    bool _6A;
    bool _6B;
};

class JUTConsoleManager {
public:
    JUTConsoleManager();

    void appendConsole(JUTConsole*);
    void removeConsole(JUTConsole*);
    void draw() const;
    void drawDirect(bool) const;
    void setDirectConsole(JUTConsole*);

    static JUTConsoleManager* createManager(JKRHeap*);

#ifdef __MWERKS__
    typedef JGadget::TLinkList< JUTConsole, -offsetof(JUTConsole, mListNode) > ConsoleList;
#else
    typedef JGadget::TLinkList< JUTConsole, 0 > ConsoleList;
#endif

    ConsoleList mList;           // 0x0
    JUTConsole* mActiveConsole;  // 0xC
    JUTConsole* mDirectConsole;  // 0x10

    static JUTConsoleManager* const getManager() { return sManager; }

    static JUTConsoleManager* sManager;
};
