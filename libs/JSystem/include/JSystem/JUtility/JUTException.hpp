#pragma once

#include "JSystem/JKernel/JKRThread.hpp"
#include "JSystem/JUtility/JUTConsole.hpp"
#include "JSystem/JUtility/JUTGamePad.hpp"

typedef void (*CallbackFunc)(OSError, OSContext*, u32, u32);

class JUTDirectPrint;

class JUTExternalFB {
public:
    JUTExternalFB(GXRenderModeObj*, GXGamma, void*, u32);

    void changeFrameBuffer(void*, u16, u16);

public:
    /* 0x00 */ GXRenderModeObj* mRenderMode;
    /* 0x04 */ u32 mSize;
    /* 0x08 */ u32 _8;
    /* 0x0C */ u16 _C;
    /* 0x0E */ u16 mGamma;
    /* 0x10 */ bool _10;
};

class JUTException : public JKRThread {
public:
    enum EInfoPage {};

    JUTException(JUTDirectPrint*);

    virtual s32 run();

    static void create(JUTDirectPrint*);
    static void errorHandler(OSError, OSContext*, u32, u32);
    static void panic_f_va(char const*, int, char const*, va_list);
    static void panic_f(const char*, int, const char*, ...);
    void showFloatSub(int, f32);
    void showFloat(OSContext*);
    void searchPartialModule(u32, u32*, u32*, u32*, u32*);
    void showStack(OSContext*);
    void showMainInfo(OSError, OSContext*, u32, u32);
    void showGPR(OSContext*);
    void showMapInfo_subroutine(u32, bool);
    void showGPRMap(OSContext*);
    void showSRR0Map(OSContext*);
    void printDebugInfo(EInfoPage, OSError, OSContext*, u32, u32);
    bool isEnablePad() const;
    void readPad(u32*, u32*);
    void printContext(OSError, OSContext*, u32, u32);
    static void waitTime(s32);
    void createFB();
    static void setPreUserCallback(CallbackFunc);
    static bool queryMapAddress(char*, u32, s32, u32*, u32*, char*, u32, bool, bool);
    static bool queryMapAddress_single(char*, u32, s32, u32*, u32*, char*, u32, bool, bool);
    static void createConsole(void*, u32);
    static JUTConsole* getConsole() NO_INLINE { return sConsole; }

    static OSMessageQueue sMessageQueue;
    static JUTException* sErrorManager;
    static CallbackFunc sPreUserCallback;
    static CallbackFunc sPostUserCallback;
    static OSMessage sMessageBuffer[1];
    static void* sConsoleBuffer;
    static u32 sConsoleBufferSize;
    static JUTConsole* sConsole;
    static u32 msr;
    static u32 fpscr;

    /* 0x7C */ JUTExternalFB* mFrameMemory;
    /* 0x80 */ JUTDirectPrint* mDirectPrint;
    /* 0x84 */ u32 _84;  // JUTGamePad* mGamePad;
    /* 0x88 */ JUTGamePad::EPadPort mGamePadPort;
    /* 0x8C */ s32 mPrintWaitTime0;
    /* 0x90 */ s32 mPrintWaitTime1;
    /* 0x94 */ u32 mTraceSuppress;
    /* 0x98 */ u32 _98;
    /* 0x9C */ u32 mPrintFlags;
    /* 0xA0 */ u32 mStackPointer;
};
