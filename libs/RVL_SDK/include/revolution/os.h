#ifndef OS_H
#define OS_H

#include <revolution/types.h>
#include <cstdarg>

#ifdef __cplusplus
extern "C" {
#endif

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

void* OSPhysicalToUncached(u32);
u32 OSCachedToPhysical(const void* caddr);

#define OS_CACHED_REGION_PREFIX 0x8000
#define OS_BASE_CACHED (OS_CACHED_REGION_PREFIX << 16)
#define OSPhysicalToCached(paddr) ((void*)((u32)(paddr) + OS_BASE_CACHED))
#define OSCachedToPhysical(caddr) ((u32)((u8*)(caddr) - OS_BASE_CACHED))

#define OSRoundUp32B(x) (((u32)(x) + 32 - 1) & ~(32 - 1))
#define OSRoundDown32B(x) (((u32)(x)) & ~(32 - 1))

OSTick OSGetTick(void);
OSTime OSGetTime(void);

void OSRegisterVersion(const char *);

void OSReport(const char *, ...);
void OSVReport(const char *, va_list);
void OSPanic(const char *, int, const char *, ...);

void* OSGetArenaHi(void);
void* OSGetArenaLo(void);

void OSSetArenaHi(void *);
void OSSetArenaLo(void *);

typedef struct OSIOSRev {
    u8 _0;
    u8 major;
    u8 minor;
    u8 micro;
    u8 month;
    u8 date;
    u16 year;
} OSIOSRev;

void __OSGetIOSRev(OSIOSRev *);

#include <revolution/base/PPCArch.h>
#include <revolution/os/OSAlarm.h>
#include <revolution/os/OSAlloc.h>
#include <revolution/os/OSCache.h>
#include <revolution/os/OSException.h>
#include <revolution/os/OSInterrupt.h>
#include <revolution/os/OSMessage.h>
#include <revolution/os/OSMutex.h>
#include <revolution/os/OSThread.h>

#ifdef __cplusplus
}
#endif

#endif // OS_H