#include "revolution/os/OSThread.h"
#include <cstdio>
#include <private/OSLoMem.h>
#include <private/flipper.h>
#include <revolution/os.h>
#include <revolution/types.h>

#ifdef __MWERKS__
OSThread* __OSCurrentThread : (OS_BASE_CACHED | OS_CURRENTTHREAD_ADDR);
OSThreadQueue __OSActiveThreadQueue : (OS_BASE_CACHED | OS_ACTIVETHREADQUEUE_ADDR);
volatile OSContext* __OSFPUContext : (OS_BASE_CACHED | OS_FPUCONTEXT_ADDR);
#else
OSThread* __OSCurrentThread;
OSThreadQueue __OSActiveThreadQueue;
volatile OSContext* __OSFPUContext;
#endif

OSErrorHandler __OSErrorTable[17];
u32 __OSFpscrEnableBits = 0xF8;

__declspec(weak) void OSReport(const char* msg, ...) {
    va_list mark;
    va_start(mark, msg);
    vprintf(msg, mark);
    va_end(mark);
}

__declspec(weak) void OSVReport(const char* msg, va_list list) {
    vprintf(msg, list);
}

__declspec(weak) void OSPanic(const char* pFile, int line_num, const char* msg, ...) {
    va_list mark;
    u32 i;
    u32* stackPtr;

    OSDisableInterrupts();
    va_start(mark, msg);
    vprintf(msg, mark);
    va_end(mark);
    // report line number
    OSReport(" in \"%s\" on line %d.\n", pFile, line_num);
    // report the header of the table for the stack trace
    OSReport("\nAddress:      Back Chain    LR Save\n");

    // dump stack trace
    for (i = 0, stackPtr = (u32*)OSGetStackPointer(); stackPtr && (u32)stackPtr != 0xFFFFFFFF && i++ < 16; stackPtr = (u32*)*stackPtr) {
        OSReport("0x%08x:   0x%08x    0x%08x\n", stackPtr, stackPtr[0], stackPtr[1]);
    }

    PPCHalt();
}

OSErrorHandler OSSetErrorHandler(OSError error, OSErrorHandler handler) {
    OSErrorHandler oldHandler;
    BOOL enabled;

    enabled = OSDisableInterrupts();
    oldHandler = __OSErrorTable[error];
    __OSErrorTable[error] = handler;

    if (error == 16) {
        u32 msr;
        u32 fpscr;
        OSThread* thread;
        msr = PPCMfmsr();
        PPCMtmsr(msr | 0x2000);
        fpscr = PPCMffpscr();

        if (handler) {
            for (thread = __OSActiveThreadQueue.head; thread; thread = thread->linkActive.next) {
                thread->context.srr1 |= 0x800 | 0x100;

                if ((thread->context.state & 1) == 0) {
                    int i;
                    thread->context.state |= 1;

                    for (i = 0; i < 32; ++i) {
                        *(u64*)&thread->context.fpr[i] = (u64)0xffffffffffffffffLL;
                        *(u64*)&thread->context.psf[i] = (u64)0xffffffffffffffffLL;
                    }

                    thread->context.fpscr = 4;
                }

                thread->context.fpscr |= __OSFpscrEnableBits & 0xF8;
                thread->context.fpscr &= 0x6005F8FF;
            }

            fpscr |= __OSFpscrEnableBits & 0xF8;
            msr |= 0x800 | 0x100;
        } else {
            for (thread = __OSActiveThreadQueue.head; thread; thread = thread->linkActive.next) {
                thread->context.srr1 &= ~(0x800 | 0x100);
                thread->context.fpscr &= ~0xF8;
                thread->context.fpscr &= ~0x6005F8FF;
            }

            fpscr &= ~0xF8;
            msr &= ~(0x800 | 0x100);
        }

        fpscr &= ~(0x6005F8FF);
        PPCMtfpscr(fpscr);
        PPCMtmsr(msr);
    }

    OSRestoreInterrupts(enabled);
    return oldHandler;
}

void __OSUnhandledException(__OSException exception, OSContext* context, u32 dsisr, u32 dar) {
    OSTime now;
    now = OSGetTime();

    if (!(context->srr1 & 2)) {
        OSReport("Non-recoverable Exception %d", exception);
    } else {
        if (exception == 6 && (context->srr1 & (0x80000000 >> 11)) && __OSErrorTable[16] != 0) {
            u32 fpscr;
            u32 msr;
            exception = 16;
            msr = PPCMfmsr();
            PPCMtmsr(msr | 0x2000);

            if (__OSFPUContext) {
                OSSaveFPUContext((OSContext*)__OSFPUContext);
            }

            fpscr = PPCMffpscr();
            fpscr &= 0x6005F8FF;
            PPCMtfpscr(fpscr);

            PPCMtmsr(msr);

            if (__OSFPUContext == context) {
                OSDisableScheduler();
                __OSErrorTable[exception](exception, context, dsisr, dar);
                context->srr1 &= ~0x2000;
                __OSFPUContext = 0;
                context->fpscr &= 0x6005F8FF;
                OSEnableScheduler();
                __OSReschedule();
            } else {
                context->srr1 &= ~0x00002000;
                __OSFPUContext = NULL;
            }

            OSLoadContext(context);
        }

        if (__OSErrorTable[exception]) {
            OSDisableScheduler();
            __OSErrorTable[exception](exception, context, dsisr, dar);
            OSEnableScheduler();
            __OSReschedule();
            OSLoadContext(context);
        }

        if (exception == 8) {
            OSLoadContext(context);
        }

        OSReport("Unhandled Exception %d", exception);
    }

    OSReport("\n");
    OSDumpContext(context);
    OSReport("\nDSISR = 0x%08x                   DAR  = 0x%08x\n", dsisr, dar);
    OSReport("TB = 0x%016llx\n", now);

    switch (exception) {
    case __OS_EXCEPTION_DSI:
        OSReport("\nInstruction at 0x%x (read from SRR0) attempted to access "
                 "invalid address 0x%x (read from DAR)\n",
                 context->srr0, dar);
        break;
    case __OS_EXCEPTION_ISI:
        OSReport("\nAttempted to fetch instruction from invalid address 0x%x "
                 "(read from SRR0)\n",
                 context->srr0);
        break;
    case __OS_EXCEPTION_ALIGNMENT:
        OSReport("\nInstruction at 0x%x (read from SRR0) attempted to access "
                 "unaligned address 0x%x (read from DAR)\n",
                 context->srr0, dar);
        break;
    case __OS_EXCEPTION_PROGRAM:
        OSReport("\nProgram exception : Possible illegal instruction/operation "
                 "at or around 0x%x (read from SRR0)\n",
                 context->srr0, dar);
        break;
    case OS_ERROR_PROTECTION:
        OSReport("\n");
        OSReport("AI DMA Address =   0x%04x%04x\n", __DSPRegs[0x00000018], __DSPRegs[0x00000018 + 1]);
        OSReport("ARAM DMA Address = 0x%04x%04x\n", __DSPRegs[0x00000010], __DSPRegs[0x00000010 + 1]);
        OSReport("DI DMA Address =   0x%08x\n", __DIRegs[5]);
        break;
    }

    OSReport("\nLast interrupt (%d): SRR0 = 0x%08x  TB = 0x%016llx\n", __OSLastInterrupt, __OSLastInterruptSrr0, __OSLastInterruptTime);

    PPCHalt();
}
