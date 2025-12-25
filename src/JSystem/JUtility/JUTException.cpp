#include "JSystem/JUtility/JUTException.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"
#include "JSystem/JUtility/JUTConsole.hpp"
#include "JSystem/JUtility/JUTDirectPrint.hpp"
#include <cstdio>
#include <revolution/os.h>

OSMessageQueue JUTException::sMessageQueue = {0};
const char* JUTException::sCpuExpName[] = {
    "SYSTEM RESET",
    "MACHINE CHECK",
    "DSI",
    "ISI",
    "EXTERNAL INTERRUPT",
    "ALIGNMENT",
    "PROGRAM",
    "FLOATING POINT",
    "DECREMENTER",
    "SYSTEM CALL",
    "TRACE",
    "PERFORMACE MONITOR",
    "BREAK POINT",
    "SYSTEM INTERRUPT",
    "THERMAL INTERRUPT",
    "PROTECTION",
    "FLOATING POINT",
};
JUTException* JUTException::sErrorManager;
JUTExceptionUserCallback JUTException::sPreUserCallback;
JUTExceptionUserCallback JUTException::sPostUserCallback;
OSMessage JUTException::sMessageBuffer[1];
void* JUTException::sConsoleBuffer;
u32 JUTException::sConsoleBufferSize;
JUTConsole* JUTException::sConsole;
u32 JUTException::msr;
u32 JUTException::fpscr;

struct CallbackObject {
    /* 0x00 */ JUTExceptionUserCallback callback;
    /* 0x04 */ OSError error;
    /* 0x08 */ OSContext* context;
    /* 0x0C */ u32 dsisr;
    /* 0x10 */ u32 dar;
};

static CallbackObject exCallbackObject;

JUTException::JUTException(JUTDirectPrint* pDirectPrint) : JKRThread(0x4000, 16, 0) {
    mDirectPrint = pDirectPrint;

    OSSetErrorHandler(OS_ERROR_DSI, reinterpret_cast< OSErrorHandler >(errorHandler));
    OSSetErrorHandler(OS_ERROR_ISI, reinterpret_cast< OSErrorHandler >(errorHandler));
    OSSetErrorHandler(OS_ERROR_PROGRAM, reinterpret_cast< OSErrorHandler >(errorHandler));
    OSSetErrorHandler(OS_ERROR_ALIGNMENT, reinterpret_cast< OSErrorHandler >(errorHandler));
    OSSetErrorHandler(OS_ERROR_PROTECTION, reinterpret_cast< OSErrorHandler >(errorHandler));
    __OSFpscrEnableBits = 0b00000000;
    OSSetErrorHandler(OS_ERROR_FPE, nullptr);

    sPreUserCallback = nullptr;
    sPostUserCallback = nullptr;
    mGamePad = nullptr;
    mGamePadPort = JUTGamePad::Port_Unknown;
    mPrintWaitTime0 = 10;
    mPrintWaitTime1 = 10;
    mTraceSuppress = -1;
    _98 = 0;
    mPrintFlags = 0b11111;
}

JUTException* JUTException::create(JUTDirectPrint* pDirectPrint) {
    if (sErrorManager == nullptr) {
        sErrorManager = new (JKRHeap::sSystemHeap, 0) JUTException(pDirectPrint);
        OSResumeThread(sErrorManager->mThread);
    }

    return sErrorManager;
}

s32 JUTException::run() {
    PPCMtmsr(PPCMfmsr() & ~0x0900);
    OSInitMessageQueue(&sMessageQueue, sMessageBuffer, sizeof(sMessageBuffer) / sizeof(*sMessageBuffer));
    OSMessage msg;

    while (true) {
        OSReceiveMessage(&sMessageQueue, &msg, OS_MESSAGE_BLOCK);
        VISetPreRetraceCallback(nullptr);
        VISetPostRetraceCallback(nullptr);

        CallbackObject* obj = static_cast< CallbackObject* >(msg);
        JUTExceptionUserCallback callback = obj->callback;
        OSError error = obj->error;
        OSContext* context = obj->context;
        u32 dsisr = obj->dsisr;
        u32 dar = obj->dar;

        if (error < OS_ERROR_MAX) {
            mStackPointer = context->gpr[1];
        }

        mFrameMemory = static_cast< JUTExternalFB* >(VIGetCurrentFrameBuffer());

        if (mFrameMemory == nullptr) {
            sErrorManager->createFB();
        }

        sErrorManager->mDirectPrint->changeFrameBuffer(mFrameMemory, sErrorManager->mDirectPrint->mFrameBufferWidth,
                                                       sErrorManager->mDirectPrint->mFrameBufferHeight);

        if (callback != nullptr) {
            callback(error, context, dsisr, dar);
        }

        OSDisableInterrupts();

        mFrameMemory = static_cast< JUTExternalFB* >(VIGetCurrentFrameBuffer());
        sErrorManager->mDirectPrint->changeFrameBuffer(mFrameMemory, sErrorManager->mDirectPrint->mFrameBufferWidth,
                                                       sErrorManager->mDirectPrint->mFrameBufferHeight);
        sErrorManager->printContext(error, context, dsisr, dar);
    }
}

void JUTException::errorHandler(OSError error, OSContext* pContext, u32 dsisr, u32 dar) {
    msr = PPCMfmsr();
    fpscr = pContext->fpscr;

    OSFillFPUContext(pContext);
    OSSetErrorHandler(error, nullptr);

    if (error == OS_ERROR_PROTECTION) {
        OSProtectRange(/* OS_PROTECT_CHAN0 */ 0, nullptr, 0, /* OS_PROTECT0_BIT | OS_PROTECT1_BIT*/ 3);
        OSProtectRange(/* OS_PROTECT_CHAN1 */ 1, nullptr, 0, /* OS_PROTECT0_BIT | OS_PROTECT1_BIT*/ 3);
        OSProtectRange(/* OS_PROTECT_CHAN2 */ 2, nullptr, 0, /* OS_PROTECT0_BIT | OS_PROTECT1_BIT*/ 3);
        OSProtectRange(/* OS_PROTECT_CHAN3 */ 3, nullptr, 0, /* OS_PROTECT0_BIT | OS_PROTECT1_BIT*/ 3);
    }

    exCallbackObject.callback = sPreUserCallback;
    exCallbackObject.error = error;
    exCallbackObject.context = pContext;
    exCallbackObject.dsisr = dsisr;
    exCallbackObject.dar = dar;

    OSSendMessage(&sMessageQueue, &exCallbackObject, OS_MESSAGE_BLOCK);
    OSEnableScheduler();
    OSYieldThread();
}

void JUTException::panic_f_va(const char* file, int line, const char* format, va_list args) {
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer) - 1, format, args);

    if (sErrorManager == nullptr) {
        OSPanic(file, line, buffer);
    }

    static OSContext context;
    memcpy(&context, OSGetCurrentContext(), sizeof(OSContext));

    sErrorManager->mStackPointer = OSGetStackPointer();

    exCallbackObject.callback = sPreUserCallback;
    exCallbackObject.error = 0xFF;
    exCallbackObject.context = &context;
    exCallbackObject.dsisr = 0;
    exCallbackObject.dar = 0;

    if (sConsole == nullptr || (sConsole != nullptr && (sConsole->getOutput() & 2) == 0)) {
        OSReport("%s in \"%s\" on line %d\n", buffer, file, line);
    }

    if (sConsole != nullptr) {
        sConsole->print_f("%s in \"%s\" on line %d\n", buffer, file, line);
    }

    OSSendMessage(&sMessageQueue, &exCallbackObject, OS_MESSAGE_BLOCK);
    OSSuspendThread(OSGetCurrentThread());
}

void JUTException::panic_f(const char* file, int line, const char* format, ...) {
    va_list args;
    va_start(args, format);
    panic_f_va(file, line, format, args);
    va_end();
}

// JUTException::showFloatSub
// JUTException::showFloat
// JUTException::searchPartialModule
// JUTException::showStack
// JUTException::showMainInfo
// JUTException::showGPR
// JUTException::showMapInfo_subroutine
// JUTException::showGPRMap
// JUTException::showSRR0Map
// JUTException::printDebugInfo

bool JUTException::isEnablePad() const {
    if (mGamePad == reinterpret_cast< JUTGamePad* >(0xFFFFFFFF)) {
        return true;
    }

    if (mGamePadPort >= JUTGamePad::Port_1) {
        return true;
    }

    return mGamePad != nullptr;
}

// JUTException::printContext

void JUTException::waitTime(s32 duration) {
    if (duration == 0) {
        return;
    }

    OSTime startTime = OSGetTime();
    OSTime elapsed;

    do {
        elapsed = OSTicksToMilliseconds(OSGetTime() - startTime);
    } while (elapsed < duration);
}

// JUTException::createFB

JUTExceptionUserCallback JUTException::setPreUserCallback(JUTExceptionUserCallback callback) {
    JUTExceptionUserCallback previous = sPreUserCallback;
    sPreUserCallback = callback;

    return previous;
}

// JUTException::queryMapAddress
// JUTException::queryMapAddress_single
// JUTException::createConsole
