#ifndef OS_H
#define OS_H

#include <revolution/types.h>
#include <cstdarg.h>

void OSRegisterVersion(const char *);

inline void* OSPhysicalToCached(u32 addr) {
    return (void*)(addr | 0x8000 << 16);
}
void* OSPhysicalToUncached(u32);

void OSReport(const char *, ...);
void OSVReport(const char *, va_list);
void OSPanic(const char *, int, const char *, ...);

#include <revolution/base/PPCArch.h>
#include <revolution/os/OSCache.h>
#include <revolution/os/OSException.h>
#include <revolution/os/OSInterrupt.h>
#include <revolution/os/OSMessage.h>
#include <revolution/os/OSMutex.h>
#include <revolution/os/OSThread.h>

#endif // OS_H