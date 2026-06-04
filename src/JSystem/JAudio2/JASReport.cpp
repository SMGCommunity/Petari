#include "JSystem/JAudio2/JASReport.hpp"
#include "JSystem/JAudio2/JASCalc.hpp"
#include "JSystem/JAudio2/JASMutex.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"
#include "revolution/os/OSThread.h"
#include <cstdio>
#include <revolution/os/OSMutex.h>

static OSMutex sMutex;
static char* sBuffer;
static int sLineMax;
static int sLineCount;
static int sTop;

// FIXME: JASMutexLock ctor/dtor aren't inlined
void JASReport(const char* format, ...) {
    va_list vl;
    if (sBuffer == nullptr) {
        return;
    }

    va_start(vl, format);
    JASMutexLock mutexLock(&sMutex);
    vsnprintf(sBuffer + (sTop * 64), 64, format, vl);
    va_end(vl);

    sTop++;
    if (sTop >= sLineMax) {
        sTop = 0;
    }
    if (sLineCount < sLineMax) {
        sLineCount++;
    }
}
