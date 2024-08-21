#include "JSystem/JUtility/JUTException.hpp"
#include "JSystem/JUtility/JUTDirectPrint.hpp"
#include <revolution/os.h>
#include <cstdio>


struct CallbackObject {
    CallbackFunc callback;                  // 0x0
    u16 error;                              // 0x4
    u16 pad;                                // 0x6
    OSContext* context;                     // 0x8
    int param_3;                            // 0xC
    int param_4;                            // 0x10
};

static CallbackObject exCallbackObject;
static OSContext context;

s32 JUTException::run() {
    PPCMtmsr(PPCMfmsr() & 0xFFFFF6FF);
    OSInitMessageQueue(&sMessageQueue, &sMessageBuffer, 1);
    OSMessage msg;
    while (true) {
        OSReceiveMessage(&sMessageQueue, &msg, 1);
        VISetPreRetraceCallback(0);
        VISetPostRetraceCallback(0);
        CallbackObject* obj = (CallbackObject*)msg;

        CallbackFunc hndlr = obj->callback;
        u16 error = obj->error;
        OSContext* ctxt = obj->context;
        u32 param_3 = obj->param_3;
        u32 param_4 = obj->param_4;


        if (error < 0x11) {
            mStackPointer = ctxt->gpr[1];
        }

        void* frameBuf = VIGetCurrentFrameBuffer();
        mFrameMemory = (JUTExternalFB*)frameBuf;

        if (frameBuf == nullptr) {
            sErrorManager->createFB();
        }

        sErrorManager->mDirectPrint->changeFrameBuffer(mFrameMemory, sErrorManager->mDirectPrint->mFrameBufferWidth, sErrorManager->mDirectPrint->mFrameBufferHeight);
    
        if (hndlr != nullptr) {
            hndlr(error, ctxt, param_3, param_4);
        }

        OSDisableInterrupts();
        void* frameBuffer = VIGetCurrentFrameBuffer();
        mFrameMemory = (JUTExternalFB*)frameBuffer;
        sErrorManager->mDirectPrint->changeFrameBuffer(frameBuffer, sErrorManager->mDirectPrint->mFrameBufferWidth, sErrorManager->mDirectPrint->mFrameBufferHeight);
        sErrorManager->printContext(error, ctxt, param_3, param_4);
    }
}

void JUTException::errorHandler(u16 err, OSContext *pContext, u32 a3, u32 a4) {
    JUTException::msr = PPCMfmsr();
    JUTException::fpscr = pContext->fpscr;
    OSFillFPUContext(pContext);
    OSSetErrorHandler(err, nullptr);

    if (err == 15) {
        OSProtectRange(0, 0, 0, 3);
        OSProtectRange(1, 0, 0, 3);
        OSProtectRange(2, 0, 0, 3);
        OSProtectRange(3, 0, 0, 3);
    }

    exCallbackObject.callback = sPreUserCallback;
    exCallbackObject.error = err;
    exCallbackObject.context = pContext;
    exCallbackObject.param_3 = a3;
    exCallbackObject.param_4 = a4;
    OSSendMessage(&sMessageQueue, &exCallbackObject, 1);
    OSEnableScheduler();
    OSYieldThread();
}

void JUTException::panic_f_va(const char* file, int line, const char* format, va_list args) {
    char buffer[256];
    vsnprintf(buffer, 0xFF, format, args);

    if (sErrorManager == nullptr) {
        OSPanic((char*)file, line, buffer);
    }

    OSContext* current_context = OSGetCurrentContext();
    memcpy(&context, current_context, sizeof(OSContext));
    sErrorManager->mStackPointer = (u32)OSGetStackPointer();

    exCallbackObject.callback = sPreUserCallback;
    exCallbackObject.error = 0xFF;
    exCallbackObject.context = &context;
    exCallbackObject.param_3 = 0;
    exCallbackObject.param_4 = 0;

    if (sConsole == nullptr || (sConsole && (sConsole->getOutput() & 2) == 0)) {
        OSReport("%s in \"%s\" on line %d\n", buffer, file, line);
    }

    if (sConsole != nullptr) {
        sConsole->print_f("%s in \"%s\" on line %d\n", buffer, file, line);
    }

    OSSendMessage(&sMessageQueue, &exCallbackObject, 1);
    OSThread* current_thread = OSGetCurrentThread();
    OSSuspendThread(current_thread);
}

void JUTException::panic_f(const char *file, int line, const char *format, ...) {
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
    if (_84 == 0xFFFFFFFF) {
        return true;
    }

    if (mGamePadPort >= JUTGamePad::Port_1) {
        return 1;
    }

    return _84 != 0;
}