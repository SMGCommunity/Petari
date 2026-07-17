#pragma once

#include "JSystem/JGadget/linklist.hpp"
#include "JSystem/JKernel/JKRDisposer.hpp"
#include "JSystem/JUtility/TColor.hpp"
#include <revolution.h>

class JUTConsole;

extern "C" void JUTConsole_print_f_va_(JUTConsole*, const char*, va_list);
extern "C" void JUTWarningConsole_f(const char*, ...);
extern "C" void JUTReportConsole_f(const char*, ...);
extern "C" JUTConsole* JUTGetWarningConsole();
extern "C" void JUTSetWarningConsole(JUTConsole*);
extern "C" void JUTSetReportConsole(JUTConsole*);
extern "C" JUTConsole* JUTGetReportConsole();

class JUTFont;

class JUTConsole : public JKRDisposer {
public:
    static void print_f(const char*, ...);

    enum EConsoleType {
        CONSOLE_TYPE_0 = 0,
        CONSOLE_TYPE_1 = 1,
        CONSOLE_TYPE_2 = 2,
    };

    enum OutputFlag {
        /* 0x0 */ OUTPUT_NONE,
        /* 0x1 */ OUTPUT_OSREPORT,
        /* 0x2 */ OUTPUT_CONSOLE,
        /* 0x3 */ OUTPUT_OSR_AND_CONSOLE,
    };

    JUTConsole(unsigned int, unsigned int, bool);

    virtual ~JUTConsole();

    static JUTConsole* create(unsigned int, void*, u32);

    static size_t getLineFromObjectSize(u32, unsigned int);

    void doDraw(EConsoleType) const;
    u32 getOutput() const {
        return mOutput;
    }

    u8 getLineAttr(int param_0) const {
        return mBuf[(field_0x20 + 2) * param_0];
    }
    void setLineAttr(int param_0, u8 param_1) {
        mBuf[(field_0x20 + 2) * param_0] = param_1;
    }
    u8* getLinePtr(int param_0) const {
        return &mBuf[(field_0x20 + 2) * param_0] + 1;
    }

    int prevIndex(int index) const {
        return --index < 0 ? index = mMaxLines - 1 : index;
    }

    int nextIndex(int index) const {
        return (++index >= mMaxLines) ? index = 0 : index;
    }

    int diffIndex(int param_0, int param_1) const {
        int result;
        int diff = param_1 - param_0;
        if (diff >= 0) {
            result = diff;
        } else {
            result = diff + mMaxLines;
        }
        return result;
    }

    void print_f_va(const char* fmt, va_list args) {
        JUTConsole_print_f_va_(this, fmt, args);
    }

    void print(char const*);
    void scroll(int);
    void clear();
    int getUsedLine() const;
    int getLineOffset() const;

    /* 0x18 */ JGadget::TLinkListNode mListNode;
    /* 0x20 */ unsigned int field_0x20;
    /* 0x24 */ int mMaxLines;
    /* 0x28 */ u8* mBuf;
    /* 0x2C */ bool field_0x2c;
    /* 0x30 */ int field_0x30;
    /* 0x34 */ int field_0x34;
    /* 0x38 */ int field_0x38;
    /* 0x3C */ int field_0x3c;
    /* 0x40 */ int mPositionX;
    /* 0x44 */ int mPositionY;
    /* 0x48 */ u32 mHeight;
    /* 0x4C */ JUTFont* mFont;
    /* 0x50 */ f32 mFontSizeX;
    /* 0x54 */ f32 mFontSizeY;
    /* 0x58 */ int mOutput;
    /* 0x5C */ JUtility::TColor field_0x5c;
    /* 0x60 */ JUtility::TColor field_0x60;
    /* 0x64 */ int field_0x64;
    /* 0x68 */ bool mVisible;
    /* 0x69 */ bool field_0x69;
    /* 0x6A */ bool field_0x6a;
    /* 0x6B */ bool field_0x6b;
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

    static JUTConsoleManager* const getManager() {
        return sManager;
    }

    static JUTConsoleManager* sManager;
};
