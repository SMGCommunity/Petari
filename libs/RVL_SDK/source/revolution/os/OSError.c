#include <revolution/types.h>
#include <revolution/os.h>
#include <cstdio>

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