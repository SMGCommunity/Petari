#pragma once

#include "JSystem/JUtility/JUTConsole.hpp"
#include "JSystem/JKernel/JKRThread.hpp"
#include "JSystem/JUtility/JUTGamePad.hpp"

typedef void (*CallbackFunc)(OSError, OSContext *, u32, u32);

typedef struct _GXRenderModeObj GXRenderModeObj;
typedef struct OSContext OSContext;
class JUTDirectPrint;

class JUTExternalFB {
public:
    JUTExternalFB(_GXRenderModeObj *, GXGamma, void *, u32);

    void changeFrameBuffer(void *, u16, u16);

public:
    _GXRenderModeObj* mRenderMode;      // 0x0
    u32 mSize;                          // 0x4
    u32 _8;
    u16 _C;
    u16 mGamma;                         // 0xE
    bool _10;
    u8 _11[3];
};

class JUTException : public JKRThread {
public:

    virtual ~JUTException();
    virtual s32 run();

    void createFB();
    void printContext(u16, OSContext *, u32, u32);
    bool isEnablePad() const;

    static void setPreUserCallback(CallbackFunc);
    static void errorHandler(u16, OSContext *, u32, u32);
    static void panic_f_va(char const*, int, char const*, va_list);
    static void panic_f(const char *, int, const char *, ...);

    static u32 msr;
    static u32 fpscr;
    static OSMessageQueue sMessageQueue;
    static JUTException* sErrorManager;
    static CallbackFunc sPreUserCallback;
    static CallbackFunc sPostUserCallback;
    static OSMessage sMessageBuffer;
    static u32 sConsoleBufferSize;
    static JUTConsole* sConsole;

    JUTExternalFB* mFrameMemory;            // 0x7C
    JUTDirectPrint* mDirectPrint;           // 0x80
    u32 _84;
    //JUTGamePad* mGamePad;                   // 0x84
    JUTGamePad::EPadPort mGamePadPort;      // 0x88
    s32 mPrintWaitTime0;                    // 0x8C
    s32 mPrintWaitTime1;                    // 0x90
    u32 mTraceSuppress;                     // 0x94
    u32 _98;
    u32 mPrintFlags;                        // 0x9C
    u32 mStackPointer;                      // 0xA0
};