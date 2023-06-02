#include "nw4r/db/assert.h"
#include "nw4r/db/console.h"
#include <revolution.h>

namespace nw4r {
    namespace db {
        static ConsoleHandle sAssertionConsole = nullptr;

        void Assertion_Printf_(const char *fmt, ...) {
            va_list vlist;
            va_start(vlist, fmt);

            if (sAssertionConsole) {
                return;
            }
            else {
                OSVReport(fmt, vlist);
            }

            va_end(vlist);
        }

        static void ShowStack_(u32 sp) __attribute__((noinline)) {
            Assertion_Printf_("-------------------------------- TRACE\n");
            u32 i;
            u32* p;

            Assertion_Printf_("Address:   BackChain   LR save\n");
            p = (u32*)sp;

            for (i = 0; i < 16; i++) {
                if (p == nullptr || (u32)p == 0xFFFFFFFF || ((u32)p & 0x80000000)) {
                    break;
                }

                Assertion_Printf_("%08X:  %08X    %08X ", p, p[0], p[1]);
                Assertion_Printf_("\n");

                p = (u32*)*p;
            }
        }

        __declspec(weak) void VPanic(const char *file, int line, const char *fmt, va_list vlist, bool halt) {
            register u32 stackPointer;

            asm {
                mr stackPointer, r1
            }

            stackPointer = *((u32*)stackPointer);

            OSDisableInterrupts();
            OSDisableScheduler();

            VISetPreRetraceCallback(nullptr);
            VISetPostRetraceCallback(nullptr);

            ShowStack_(stackPointer);

            if (sAssertionConsole) {
                Console_Printf(sAssertionConsole, "%s:%d Panic:", file, line);
                Console_Printf(sAssertionConsole, "\n");

                Console_ShowLatestLine(sAssertionConsole);
                Console_SetVisible(sAssertionConsole, true);
            }
            else {
                OSReport ("%s:%d Panic:", file, line);
                OSVReport(fmt, vlist);
                OSReport ("\n");
            }

            if (halt) {
                PPCHalt();
            }
        }

        __declspec(weak) void Panic(const char *file, int line, const char *fmt, ...) {
            va_list vlist;
            va_start(vlist, fmt);
            nw4r::db::VPanic(file, line, fmt, vlist, true);
            va_end(vlist);
            PPCHalt();
        }
    };
};