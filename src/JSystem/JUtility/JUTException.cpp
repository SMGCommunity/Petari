#include "JSystem/JUtility/JUTException.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"
#include "JSystem/JUtility/JUTConsole.hpp"
#include "JSystem/JUtility/JUTDirectPrint.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <new>

#pragma inline_max_size(250)
#include <revolution/os.h>
#include <revolution/os/OSModule.h>

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

JSUList< JUTException::JUTExMapFile > JUTException::sMapFileList(false);
OSTime c3bcnt[4] = {0, 0, 0, 0};

JUTException::JUTException(JUTDirectPrint* pDirectPrint) : JKRThread(0x4000, 16, 0) {
    mDirectPrint = pDirectPrint;

    OSSetErrorHandler(OS_ERROR_DSI, reinterpret_cast< OSErrorHandler >(errorHandler));
    OSSetErrorHandler(OS_ERROR_ISI, reinterpret_cast< OSErrorHandler >(errorHandler));
    OSSetErrorHandler(OS_ERROR_PROGRAM, reinterpret_cast< OSErrorHandler >(errorHandler));
    OSSetErrorHandler(OS_ERROR_ALIGNMENT, reinterpret_cast< OSErrorHandler >(errorHandler));
    OSSetErrorHandler(OS_ERROR_PROTECTION, reinterpret_cast< OSErrorHandler >(errorHandler));
    __OSFpscrEnableBits = 0;
    OSSetErrorHandler(OS_ERROR_FPE, nullptr);

    sPreUserCallback = nullptr;
    sPostUserCallback = nullptr;
    mGamePad = nullptr;
    mGamePadPort = JUTGamePad::Port_Unknown;
    mPrintWaitTime0 = 10;
    mPrintWaitTime1 = 10;
    mTraceSuppress = -1;
    field_0x98 = 0;
    mPrintFlags = 0x1F;
}

JUTException* JUTException::create(JUTDirectPrint* pDirectPrint) {
    if (sErrorManager == nullptr) {
        sErrorManager = new (JKRHeap::sSystemHeap, 0) JUTException(pDirectPrint);
        OSResumeThread(sErrorManager->mThread);
    }

    return sErrorManager;
}

void* JUTException::run() {
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

#define __signbit(x) ((*reinterpret_cast< unsigned char* >(&(x))) & 0x80)

void JUTException::showFloatSub(int index, f32 value) {
    if (isnan(value)) {
        sConsole->print_f("F%02d: Nan      ", index);
    } else if (isinf(value)) {
        if (__signbit(value)) {
            sConsole->print_f("F%02d:+Inf     ", index);
        } else {
            sConsole->print_f("F%02d:-Inf     ", index);
        }
    } else if (value == 0.0f) {
        sConsole->print_f("F%02d: 0.0      ", index);
    } else {
        sConsole->print_f("F%02d:%+.3E", index, value);
    }
}

void JUTException::showFloat(OSContext* context) {
    if (!sConsole) {
        return;
    }

    sConsole->print("-------------------------------- FPR\n");
    for (int i = 0; i < 10; i++) {
        showFloatSub(i, context->fpr[i]);
        sConsole->print(" ");
        showFloatSub(i + 11, context->fpr[i + 11]);
        sConsole->print(" ");
        showFloatSub(i + 22, context->fpr[i + 22]);
        sConsole->print("\n");
    }
    showFloatSub(10, context->fpr[10]);
    sConsole->print(" ");
    showFloatSub(21, context->fpr[21]);
    sConsole->print("\n");
}

bool JUTException::searchPartialModule(u32 address, u32* module_id, u32* section_id, u32* section_offset, u32* name_offset) {
    if (!address) {
        return false;
    }

    OSModuleInfo* module = *(OSModuleInfo**)0x800030C8;
    while (module) {
        OSSectionInfo* section = reinterpret_cast< OSSectionInfo* >(module->sectionInfoOffset);
        for (u32 i = 0; i < module->numSections; section = section + 1, i++) {
            if (section->size) {
                u32 addr = ALIGN_PREV(section->offset, 2);
                if ((addr <= address) && (address < addr + section->size)) {
                    if (module_id)
                        *module_id = module->id;
                    if (section_id)
                        *section_id = i;
                    if (section_offset)
                        *section_offset = address - addr;
                    if (name_offset)
                        *name_offset = module->nameOffset;
                    return true;
                }
            }
        }

        module = reinterpret_cast< OSModuleInfo* >(module->link.next);
    }

    return false;
}

void JUTException::showStack(OSContext* context) {
    if (!sConsole) {
        return;
    }

    u32 i;
    sConsole->print("-------------------------------- TRACE\n");
    u32* stackPointer = reinterpret_cast< u32* >(mStackPointer);
    sConsole->print_f("Address:   BackChain   LR save\n");

    for (i = 0; (stackPointer != nullptr) && (stackPointer != reinterpret_cast< u32* >(0xFFFFFFFF)) && (i++ < 0x10);) {
        if (i > mTraceSuppress) {
            sConsole->print("Suppress trace.\n");
            return;
        }

        sConsole->print_f("%08X:  %08X    %08X\n", stackPointer, stackPointer[0], stackPointer[1]);
        showMapInfo_subroutine(stackPointer[1], false);
        JUTConsoleManager* manager = JUTConsoleManager::sManager;
        manager->drawDirect(true);
        waitTime(mPrintWaitTime1);
        stackPointer = reinterpret_cast< u32* >(stackPointer[0]);
    }
}

void JUTException::showMainInfo(u16 error, OSContext* context, u32 dsisr, u32 dar) {
    if (!sConsole) {
        return;
    }

    if (error < (OS_ERROR_MACHINE_CHECK | 16)) {
        sConsole->print_f("CONTEXT:%08XH  (%s EXCEPTION)\n", context, sCpuExpName[error]);
    } else {
        sConsole->print_f("CONTEXT:%08XH\n", context);
    }

    if (error == 16) {
        u32 flags = fpscr & (((fpscr & 0xf8) << 0x16) | 0x1f80700);
        if ((flags & 0x20000000) != 0) {
            sConsole->print_f(" FPE: Invalid operation\n");
            if ((fpscr & 0x1000000) != 0) {
                sConsole->print_f(" SNaN\n");
            }
            if ((fpscr & 0x800000) != 0) {
                sConsole->print_f(" Infinity - Infinity\n");
            }
            if ((fpscr & 0x400000) != 0) {
                sConsole->print_f(" Infinity / Infinity\n");
            }
            if ((fpscr & 0x200000) != 0) {
                sConsole->print_f(" 0 / 0\n");
            }
            if ((fpscr & 0x100000) != 0) {
                sConsole->print_f(" Infinity * 0\n");
            }
            if ((fpscr & 0x80000) != 0) {
                sConsole->print_f(" Invalid compare\n");
            }
            if ((fpscr & 0x400) != 0) {
                sConsole->print_f(" Software request\n");
            }
            if ((fpscr & 0x200) != 0) {
                sConsole->print_f(" Invalid square root\n");
            }
            if ((fpscr & 0x100) != 0) {
                sConsole->print_f(" Invalid integer convert\n");
            }
        }
        if ((flags & 0x10000000) != 0) {
            sConsole->print_f(" FPE: Overflow\n");
        }
        if ((flags & 0x8000000) != 0) {
            sConsole->print_f(" FPE: Underflow\n");
        }
        if ((flags & 0x4000000) != 0) {
            sConsole->print_f(" FPE: Zero division\n");
        }
        if ((flags & 0x2000000) != 0) {
            sConsole->print_f(" FPE: Inexact result\n");
        }
    }
    sConsole->print_f("SRR0:   %08XH   SRR1:%08XH\n", context->srr0, context->srr1);
    sConsole->print_f("DSISR:  %08XH   DAR: %08XH\n", dsisr, dar);
}

void JUTException::showGPR(OSContext* context) {
    if (!sConsole) {
        return;
    }

    sConsole->print("-------------------------------- GPR\n");
    for (int i = 0; i < 10; i++) {
        sConsole->print_f("R%02d:%08XH  R%02d:%08XH  R%02d:%08XH\n", i, context->gpr[i], i + 11, context->gpr[i + 11], i + 22, context->gpr[i + 22]);
    }
    sConsole->print_f("R%02d:%08XH  R%02d:%08XH\n", 10, context->gpr[10], 21, context->gpr[21]);
}

static void search_name_part(u8* src, u8* dst, int dst_length) {
    u8* name = src;
    for (u8* p = src; *p; p++) {
        if (*p == '\\') {
            name = p;
        }
    }

    if (*name == '\\') {
        name++;
    }

    for (int i = 0; *name != 0 && i < dst_length; *dst++ = *name++, i++) {
        if (*name == '.') {
            break;
        }
    }

    *dst = '\0';
}

bool JUTException::showMapInfo_subroutine(u32 address, bool begin_with_newline) {
    if ((address < 0x80000000) || (0x82ffffff < address)) {
        return false;
    }

    u32 name_offset;
    u32 module_id;
    u32 section_id;
    u32 section_offset;
    u8 name_part[33];

    const char* new_line = "\n";
    if (begin_with_newline == false) {
        new_line = "";
    }

    bool result = searchPartialModule(address, &module_id, &section_id, &section_offset, &name_offset);
    if (result == true) {
        search_name_part(reinterpret_cast< u8* >(name_offset), name_part, 32);
        sConsole->print_f("%s %s:%x section:%d\n", new_line, name_part, section_offset, section_id);
        begin_with_newline = false;
    }

    JSUListIterator< JUTException::JUTExMapFile > last = sMapFileList.getEnd();
    JSUListIterator< JUTException::JUTExMapFile > first = sMapFileList.getFirst();
    if (first != last) {
        u32 out_addr;
        u32 out_size;
        char out_line[256];

        if (result == true) {
            result = queryMapAddress(reinterpret_cast< char* >(name_part), section_offset, section_id, &out_addr, &out_size, out_line,
                                     ARRAY_SIZE(out_line), true, begin_with_newline);
        } else {
            result = queryMapAddress(nullptr, address, -1, &out_addr, &out_size, out_line, ARRAY_SIZE(out_line), true, begin_with_newline);
        }

        if (result == true) {
            return true;
        }
    }

    return false;
}

void JUTException::showGPRMap(OSContext* context) {
    if (!sConsole) {
        return;
    }

    bool found_address_register = false;
    sConsole->print("-------------------------------- GPRMAP\n");

    for (int i = 0; i < 31; i++) {
        u32 address = context->gpr[i];

        if (address >= 0x80000000 && address <= 0x83000000 - 1) {
            found_address_register = true;

            sConsole->print_f("R%02d: %08XH", i, address);
            if (!showMapInfo_subroutine(address, true)) {
                sConsole->print("  no information\n");
            }
            JUTConsoleManager::sManager->drawDirect(true);
            waitTime(mPrintWaitTime1);
        }
    }

    if (!found_address_register) {
        sConsole->print("  no register which seem to address.\n");
    }
}

void JUTException::showSRR0Map(OSContext* context) {
    if (!sConsole) {
        return;
    }

    sConsole->print("-------------------------------- SRR0MAP\n");
    u32 address = context->srr0;
    if (address >= 0x80000000 && address <= 0x83000000 - 1) {
        sConsole->print_f("SRR0: %08XH", address);
        if (showMapInfo_subroutine(address, true) == false) {
            sConsole->print("  no information\n");
        }
        JUTConsoleManager::getManager()->drawDirect(true);
    }
}

void JUTException::printDebugInfo(JUTException::EInfoPage page, OSError error, OSContext* context, u32 param_3, u32 param_4) {
    switch (page) {
    case EINFO_PAGE_GPR:
        return showGPR(context);
    case EINFO_PAGE_FLOAT:
        showFloat(context);
        if (sConsole) {
            sConsole->print_f(" MSR:%08XH\t FPSCR:%08XH\n", msr, fpscr);
        }
        break;
    case EINFO_PAGE_STACK:
        return showStack(context);
    case EINFO_PAGE_GPR_MAP:
        return showGPRMap(context);
    case EINFO_PAGE_SSR0_MAP:
        return showSRR0Map(context);
    }
}

bool JUTException::isEnablePad() const {
    if (mGamePad == reinterpret_cast< JUTGamePad* >(0xFFFFFFFF)) {
        return true;
    }

    if (mGamePadPort >= JUTGamePad::Port_1) {
        return true;
    }

    return mGamePad != nullptr;
}

void JUTException::printContext(u16 error, OSContext* context, u32 dsisr, u32 dar) {
    bool is_pad_enabled = isEnablePad() ? false : true;
    if (!sErrorManager->mDirectPrint->field_0x00) {
        return;
    }

    VISetPreRetraceCallback(nullptr);
    VISetPostRetraceCallback(nullptr);
    VISetBlack(FALSE);
    VIFlush();

    if (!sConsole) {
        return;
    }

    if (error < 17) {
        sConsole->print_f("******** EXCEPTION OCCURRED! ********\nFrameMemory:%XH\n", getFrameMemory());
    } else {
        sConsole->print_f("******** USER HALT ********\nFrameMemory:%XH\n", getFrameMemory());
    }

    int post_callback_executed = false;
    while (true) {
        showMainInfo(error, context, dsisr, dar);

        JUTConsoleManager::getManager()->drawDirect(true);
        waitTime(mPrintWaitTime0);

        if ((mPrintFlags & JUT_PRINT_GPR) != 0) {
            printDebugInfo(EINFO_PAGE_GPR, error, context, dsisr, dar);
            JUTConsoleManager::getManager()->drawDirect(true);
            waitTime(mPrintWaitTime0);
        }
        if ((mPrintFlags & JUT_PRINT_SRR0_MAP) != 0) {
            printDebugInfo(EINFO_PAGE_SSR0_MAP, error, context, dsisr, dar);
            JUTConsoleManager::getManager()->drawDirect(true);
            waitTime(mPrintWaitTime0);
        }
        if ((mPrintFlags & JUT_PRINT_GPR_MAP) != 0) {
            printDebugInfo(EINFO_PAGE_GPR_MAP, error, context, dsisr, dar);
            JUTConsoleManager::getManager()->drawDirect(true);
            waitTime(mPrintWaitTime0);
        }
        if ((mPrintFlags & JUT_PRINT_FLOAT) != 0) {
            printDebugInfo(EINFO_PAGE_FLOAT, error, context, dsisr, dar);
            JUTConsoleManager::getManager()->drawDirect(true);
            waitTime(mPrintWaitTime0);
        }
        if ((mPrintFlags & JUT_PRINT_STACK) != 0) {
            printDebugInfo(EINFO_PAGE_STACK, error, context, dsisr, dar);
            JUTConsoleManager::getManager()->drawDirect(true);
            waitTime(mPrintWaitTime1);
        }

        sConsole->print("--------------------------------\n");
        JUTConsoleManager::getManager()->drawDirect(true);

        if (post_callback_executed == 0 && sPostUserCallback) {
            BOOL enable = OSEnableInterrupts();
            post_callback_executed = true;
            (*sPostUserCallback)(error, context, dsisr, dar);
            OSRestoreInterrupts(enable);
        }

        if (this->field_0x98 == 0 || !is_pad_enabled) {
            break;
        }

        u32 output = sConsole->getOutput();
        sConsole->mOutput = output & 1;
    }

    if (!is_pad_enabled) {
        OSEnableInterrupts();

        u32 button;
        u32 trigger;

        int down = 0;
        int up = 0;
        while (true) {
            readPad(&trigger, &button);

            bool draw = false;
            if (trigger == 0x100) {
                sConsole->scroll(sConsole->mMaxLines);
                draw = true;
            }

            if (trigger == 0x200) {
                sConsole->scroll(-sConsole->mMaxLines);
                draw = true;
            }

            if (button == 8) {
                sConsole->scroll((down < 3) ? -1 : ((down < 5) ? -2 : ((down < 7) ? -4 : -8)));
                draw = true;
                down++;
                up = 0;
            } else if (button == 4) {
                sConsole->scroll((up < 3) ? 1 : ((up < 5) ? 2 : ((up < 7) ? 4 : 8)));
                draw = true;
                up++;
                down = 0;
            } else {
                down = 0;
                up = 0;
            }

            if (draw == true) {
                u32 start = VIGetRetraceCount();
                while (start == VIGetRetraceCount())
                    ;
                JUTConsoleManager::getManager()->drawDirect(true);
            }

            waitTime(30);
        }
    }

    while (true) {
        sConsole->scroll(-sConsole->mMaxLines);
        JUTConsoleManager::getManager()->drawDirect(true);
        waitTime(2000);

        u32 i;
        int line_offset;
        int used_line;
        u32 height;
    next:
        for (i = sConsole->mHeight; i > 0; i--) {
            sConsole->scroll(1);
            JUTConsoleManager::getManager()->drawDirect(true);

            height = sConsole->mHeight;
            line_offset = sConsole->getLineOffset();
            used_line = sConsole->getUsedLine();
            if ((used_line - height) + 1U <= line_offset)
                break;
            waitTime(20);
        }

        waitTime(3000);
        height = sConsole->mHeight;
        line_offset = sConsole->getLineOffset();
        used_line = sConsole->getUsedLine();
        if ((used_line - height) + 1U <= line_offset) {
            continue;
        }
        goto next;
    }
}

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

void JUTException::createFB() {
    _GXRenderModeObj* renderMode = &GXNtsc480Int;
    void* end = OSGetArenaHi();
    u32 size = (static_cast< u16 >(ALIGN_NEXT(renderMode->fbWidth, 16)) * renderMode->xfbHeight) * 2;

    void* begin = reinterpret_cast< void* >(ALIGN_PREV(reinterpret_cast< u32 >(end) - size, 32));
    void* object = reinterpret_cast< void* >(ALIGN_PREV(reinterpret_cast< u32 >(begin) - sizeof(JUTExternalFB), 32));
    new (object) JUTExternalFB(renderMode, GX_GM_1_7, begin, size);

    mDirectPrint->changeFrameBuffer(begin, renderMode->fbWidth, renderMode->efbHeight);
    VIConfigure(renderMode);
    VISetNextFrameBuffer(begin);
    VISetBlack(FALSE);
    VIFlush();
    VIFlush();

    for (int i = 0; i < 3; i++) {
        u32 start = VIGetRetraceCount();
        while (start == VIGetRetraceCount()) {
        }
    }

    mFrameMemory = static_cast< JUTExternalFB* >(object);
}

JUTExceptionUserCallback JUTException::setPreUserCallback(JUTExceptionUserCallback callback) {
    JUTExceptionUserCallback previous = sPreUserCallback;
    sPreUserCallback = callback;

    return previous;
}

bool JUTException::queryMapAddress(char* mapPath, u32 address, s32 section_id, u32* out_addr, u32* out_size, char* out_line, u32 line_length,
                                   bool print, bool begin_with_newline) {
    if (mapPath) {
        char buffer[80];
        strcpy(buffer, mapPath);
        strcat(buffer, ".map");
        if (queryMapAddress_single(buffer, address, section_id, out_addr, out_size, out_line, line_length, print, begin_with_newline) == true) {
            return true;
        }
    } else if (sMapFileList.getFirst()) {
        if (queryMapAddress_single(sMapFileList.getFirst()->getObject()->mPath, address, -1, out_addr, out_size, out_line, line_length, print,
                                   begin_with_newline) == true) {
            return true;
        }
    }

    return false;
}
char mapAddressFormat[] = "  [%08X]: .%s [%08X: %XH]\n  %s\n";

void JUTException::createConsole(void* console_buffer, u32 console_buffer_size) {
    if (console_buffer == nullptr || console_buffer_size == 0) {
        return;
    }

    u32 lines = JUTConsole::getLineFromObjectSize(console_buffer_size, 0x32);
    if (lines != 0) {
        sConsoleBuffer = console_buffer;
        sConsoleBufferSize = console_buffer_size;
        sConsole = JUTConsole::create(0x32, sConsoleBuffer, sConsoleBufferSize);

        JUTConsoleManager::getManager()->setDirectConsole(sConsole);

        JUTConsole* console = sConsole;
        console->mFontSizeX = 10.0f;
        console->mFontSizeY = 6.0f;
        console = sConsole;
        console->mPositionX = 15;
        console->mPositionY = 26;
        console = sConsole;
        console->mHeight = 23;
        if (console->mHeight > console->mMaxLines) {
            console->mHeight = console->mMaxLines;
        }
        sConsole->mVisible = true;
        sConsole->mOutput = JUTConsole::OUTPUT_OSREPORT | JUTConsole::OUTPUT_CONSOLE;
    }
}

JUTExternalFB::JUTExternalFB(_GXRenderModeObj* renderMode, GXGamma gamma, void* buffer, u32 size) {
    mRenderMode = renderMode;
    mSize = size;
    field_0x0C = 1;
    mGamma = gamma;
    field_0x10 = false;
}
