#ifndef OS_H
#define OS_H

#include <revolution/types.h>
#include <cstdarg.h>

typedef s64         OSTime;
typedef u32         OSTick;

u32 __OSBusClock : (0x8000 << 16 | 0x00F8); 

#define OS_BUS_CLOCK        __OSBusClock
#ifndef OS_CORE_CLOCK
#define OS_CORE_CLOCK       729000000u
#endif
#ifndef OS_BUS_CLOCK
#define OS_BUS_CLOCK        243000000u
#endif
#ifndef OS_TIMER_CLOCK
#define OS_TIMER_CLOCK      (OS_BUS_CLOCK/4)
#endif

#define OSNanosecondsToTicks( nsec )    (((nsec) * (OS_TIMER_CLOCK / 125000)) / 8000)

OSTick OSGetTick(void);
OSTime OSGetTime(void);

void OSRegisterVersion(const char *);

inline void* OSPhysicalToCached(u32 addr) {
    return (void*)(addr | 0x8000 << 16);
}
void* OSPhysicalToUncached(u32);

void OSReport(const char *, ...);
void OSVReport(const char *, va_list);
void OSPanic(const char *, int, const char *, ...);

#include <revolution/base/PPCArch.h>
#include <revolution/os/OSAlarm.h>
#include <revolution/os/OSCache.h>
#include <revolution/os/OSException.h>
#include <revolution/os/OSInterrupt.h>
#include <revolution/os/OSMessage.h>
#include <revolution/os/OSMutex.h>
#include <revolution/os/OSThread.h>

#endif // OS_H