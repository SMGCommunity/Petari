#ifndef OS_H
#define OS_H

#include <revolution/types.h>
#include <revolution/base/PPCArch.h>
#include <revolution/os/OSException.h>
#include <revolution/os/OSInterrupt.h>
#include <cstdarg.h>

void OSRegisterVersion(const char *);

inline void* OSPhysicalToCached(u32 addr) {
    return (void*)(addr | 0x8000 << 16);
}
void* OSPhysicalToUncached(u32);

void OSReport(const char *, ...);
void OSVReport(const char *, va_list);
void OSPanic(const char *, int, const char *, ...);

#endif // OS_H