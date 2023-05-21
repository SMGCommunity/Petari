#ifndef OS_H
#define OS_H

#include <revolution/types.h>
#include <cstdarg>

#ifdef __cplusplus
extern "C" {
#endif

#define OSHalt(msg) OSPanic(__FILE__, __LINE__, msg)

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

#define OSNanosecondsToTicks(nsec) (((nsec) * (OS_TIMER_CLOCK / 125000)) / 8000)

#define OSTicksToSeconds(ticks) ((ticks) / OS_TIMER_CLOCK)
#define OSTicksToMilliseconds(ticks) ((ticks) / (OS_TIMER_CLOCK / 1000))
#define OSTicksToMicroseconds(ticks) (((ticks) * 8) / (OS_TIMER_CLOCK / 125000))
#define OSSecondsToTicks(sec) ((sec)  * OS_TIMER_CLOCK)
#define OSMillisecondsToTicks(msec) ((msec) * (OS_TIMER_CLOCK / 1000))
#define OSMicrosecondsToTicks(usec) (((usec) * (OS_TIMER_CLOCK / 125000)) / 8)

void* OSPhysicalToUncached(u32);
u32 OSCachedToPhysical(const void* caddr);

#define OS_CACHED_REGION_PREFIX 0x8000
#define OS_UNCACHED_REGION_PREFIX 0xC000

#define OS_BASE_CACHED (OS_CACHED_REGION_PREFIX << 16)
#define OS_BASE_UNCACHED (OS_UNCACHED_REGION_PREFIX << 16)

#define OSPhysicalToCached(paddr) ((void*)((u32)(paddr) + OS_BASE_CACHED))
#define OSCachedToPhysical(caddr) ((u32)((u8*)(caddr) - OS_BASE_CACHED))
#define OSPhysicalToUncached(paddr) ((void*) ((u32)(paddr) + OS_BASE_UNCACHED))

#define OSIsMEM1Region(addr) (((u32)(addr) & 0x30000000) == 0x00000000)
#define OSIsMEM2Region(addr) (((u32)(addr) & 0x30000000) == 0x10000000)

#define OSRoundUp32B(x) (((u32)(x) + 32 - 1) & ~(32 - 1))
#define OSRoundDown32B(x) (((u32)(x)) & ~(32 - 1))
#define OSDiffTick(tick1, tick0) ((s32) (tick1) - (s32) (tick0))

OSTick OSGetTick(void);
OSTime OSGetTime(void);

u32 OSGetConsoleType(void);

void OSRegisterVersion(const char *);

void OSReport(const char *, ...);
void OSVReport(const char *, va_list);
void OSPanic(const char *, int, const char *, ...);

void* OSGetArenaHi(void);
void* OSGetArenaLo(void);

void OSSetArenaHi(void *);
void OSSetArenaLo(void *);

typedef struct OSIOSRev {
    u8 reserved;
    u8 major;
    u8 minor;
    u8 micro;
    u8 month;
    u8 date;
    u16 year;
} OSIOSRev;

void __OSGetIOSRev(OSIOSRev *);

#ifndef ASSERT
#define ASSERT(exp) ((void) 0)
#endif

#ifndef ASSERTMSG
#define ASSERTMSG(exp, msg) ((void) 0)
#endif

#include <revolution/base/PPCArch.h>
#include <revolution/os/OSAlarm.h>
#include <revolution/os/OSAlloc.h>
#include <revolution/os/OSCache.h>
#include <revolution/os/OSContext.h>
#include <revolution/os/OSException.h>
#include <revolution/os/OSExecParams.h>
#include <revolution/os/OSFastCast.h>
#include <revolution/os/OSInterrupt.h>
#include <revolution/os/OSMessage.h>
#include <revolution/os/OSMutex.h>
#include <revolution/os/OSRtc.h>
#include <revolution/os/OSReset.h>
#include <revolution/os/OSTime.h>
#include <revolution/os/OSThread.h>

/* PRIVATE OS FUNCTIONS */
void __OSInitSystemCall(void);
void __OSUnhandledException(__OSException, OSContext *, u32, u32);
void __OSReschedule(void);
void __OSInitAlarm(void);
void __OSModuleInit(void);
void __OSInterruptInit(void);
void __OSContextInit(void);
void __OSInitAudioSystem(void);
void __OSInitMemoryProtection(void);

extern void __RAS_OSDisableInterrupts_begin(void);
extern void __RAS_OSDisableInterrupts_end(void);

#ifdef __cplusplus
}
#endif

#endif // OS_H