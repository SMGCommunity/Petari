#include <revolution/types.h>
#include <revolution/os.h>
#include <cstdio>

OSErrorHandler __OSErrorTable[17];
u32 __OSFpscrEnableBits = 0xF8;

__declspec(weak) void OSReport(const char* msg, ...) {
    va_list mark;
    va_start(mark, msg);
    vprintf(msg, mark);
    va_end(mark);
}

__declspec(weak) void OSVReport(const char* msg, va_list list)
{
    vprintf(msg, list);
}

__declspec(weak) void OSPanic(const char *pFile, int line_num, const char *msg, ...) {
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
                        *(u64*) &thread->context.fpr[i] = (u64) 0xffffffffffffffffLL;
                        *(u64*) &thread->context.psf[i] = (u64) 0xffffffffffffffffLL;
                    }

                    thread->context.fpscr = 4;
                }

                thread->context.fpscr |= __OSFpscrEnableBits & 0xF8;
                thread->context.fpscr &= 0x6005F8FF;
            }

            fpscr |= __OSFpscrEnableBits & 0xF8;
            msr |= 0x800 | 0x100;
        }
        else {
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

void __OSUnhandledException(__OSException exception, OSContext *context, u32 dsisr, u32 dar) {
    OSTime now;
    now = OSGetTime();

    if (!(context->srr1 & 2)) {
        OSReport("Non-recoverable Exception %d", exception);
    }
    else {
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
            
            if (__OSFPUContext == context) {
                OSDisableScheduler();
                __OSErrorTable[exception](exception, context, dsisr, dar);
                context->srr1 &= ~0x2000;
                __OSFPUContext = 0;
                context->fpscr &= 0x6005F8FF;
                OSEnableScheduler();
                
            }
        }
    }
}