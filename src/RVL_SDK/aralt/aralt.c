#include <revolution/aralt.h>
#include <revolution/os.h>
#include <cstring>

static u32 __ARH_BaseAdr;

static ARCallback __AR_Callback;

#ifndef NON_MATCHING
// for some reason has _savegpr
void ARStartDMA(u32 type, u32 ram_addr, u32 aram_addr, u32 len) {
    OSDisableScheduler();

    if (type == 0) {
        aram_addr += __ARH_BaseAdr;
        DCInvalidateRange((u32*)ram_addr, len);
        memcpy((u32*)aram_addr, (u32*)ram_addr, len);
        DCFlushRange((u32*)aram_addr, len);
    }
    else if (type == 1) {
        ram_addr += __ARH_BaseAdr;
        DCFlushRange((u32*)ram_addr, len);
        memcpy((u32*)aram_addr, (u32*)ram_addr, len);
        DCFlushRange((u32*)aram_addr, len);
    }

    OSEnableScheduler();

    if (__AR_Callback) {
        (*__AR_Callback)();
    }
}
#endif