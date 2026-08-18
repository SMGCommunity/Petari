#include "revolution/os/OSInterrupt.h"
#include <cstring>
#include <revolution/aralt.h>
#include <revolution/os.h>

static u32 __AR_init_flag;
static s32 __ARQ_init_flag;
static u32 __ARQChunkSize;
static ARQCallback __ARQCallbackLo;
static ARQCallback __ARQCallbackHi;
static ARQRequest* __ARQRequestPendingLo;
static ARQRequest* __ARQRequestPendingHi;
static ARQRequest* __ARQRequestQueueLo;
static ARQRequest* __ARQRequestQueueHi;
static u32 __ARH_BaseAdr;
static u32 __AR_InternalSize;
static u32 __AR_Size;
static ARCallback __AR_Callback;

static u32 __ARH_MemoryTop = 0x90000000;

void ARStartDMA(u32 type, u32 ram_addr, u32 aram_addr, u32 len) {
    OSDisableScheduler();

    if (type == 0) {
        aram_addr += __ARH_BaseAdr;
        DCInvalidateRange((u32*)ram_addr, len);
        memcpy((u32*)aram_addr, (u32*)ram_addr, len);
        DCFlushRange((u32*)aram_addr, len);
    } else if (type == 1) {
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

u32 ARAlloc(u32 amount) {
    BOOL en = OSDisableInterrupts();
    u32 top = __ARH_MemoryTop;
    OSRestoreInterrupts(en);
    __ARH_MemoryTop += amount;
    return top - __ARH_BaseAdr;
}

u32 ARInit(u32*, u32) {
    BOOL en;
    u32 diff;
    void* memLo;
    void* memHi;

    if (__AR_init_flag != 0) {
        return 0x4000;
    }

    en = OSDisableInterrupts();
    __AR_Callback = 0;
    __AR_init_flag = 1;
    OSRestoreInterrupts(en);

    memLo = OSGetMEM2ArenaLo();
    memHi = OSGetMEM2ArenaHi();
    diff = (u32)memHi - (u32)memLo;

    __ARH_MemoryTop = (u32)memLo + ARGetBaseAddress();
    __ARH_BaseAdr = (u32)memLo;
    OSReport("ARInit : Dummy ARAM enabled (RVL), area %p -> %p (size 0x%x)\n", memLo, memHi, diff);
    __AR_Size = diff;
    __AR_InternalSize = diff;
    return 0x4000;
}

u32 ARGetBaseAddress(void) {
    return 0x4000;
}

u32 ARGetSize(void) {
    return __AR_Size;
}

void __ARQPopTaskQueueHi(void) {
    u32 type;

    if (__ARQRequestQueueHi) {
        type = __ARQRequestQueueHi->type;
        if (type == 0) {
            ARStartDMA(type, __ARQRequestQueueHi->source, __ARQRequestQueueHi->dest, __ARQRequestQueueHi->length);
        } else {
            ARStartDMA(type, __ARQRequestQueueHi->dest, __ARQRequestQueueHi->source, __ARQRequestQueueHi->length);
        }

        __ARQCallbackHi = __ARQRequestQueueHi->callback;
        __ARQRequestPendingHi = __ARQRequestQueueHi;
        __ARQRequestQueueHi = __ARQRequestQueueHi->next;
    }
}

void __ARQServiceQueueLo() {
    u32 length;
    u32 type;
    u32 v1;

    if (!__ARQRequestPendingLo && __ARQRequestQueueLo) {
        __ARQRequestPendingLo = __ARQRequestQueueLo;
        __ARQRequestQueueLo = __ARQRequestQueueLo->next;
    }
    if (__ARQRequestPendingLo) {
        if ((length = __ARQRequestPendingLo->length) <= __ARQChunkSize) {
            v1 = __ARQRequestPendingLo->type;
            if (!v1) {
                ARStartDMA(v1, __ARQRequestPendingLo->source, __ARQRequestPendingLo->dest, length);
            } else {
                ARStartDMA(v1, __ARQRequestPendingLo->dest, __ARQRequestPendingLo->source, length);
            }
            __ARQCallbackLo = __ARQRequestPendingLo->callback;
        } else {
            type = __ARQRequestPendingLo->type;
            if (!type) {
                ARStartDMA(type, __ARQRequestPendingLo->source, __ARQRequestPendingLo->dest, __ARQChunkSize);
            } else {
                ARStartDMA(type, __ARQRequestPendingLo->dest, __ARQRequestPendingLo->source, __ARQChunkSize);
            }
        }
        __ARQRequestPendingLo->length -= __ARQChunkSize;
        __ARQRequestPendingLo->source += __ARQChunkSize;
        __ARQRequestPendingLo->dest += __ARQChunkSize;
    }
}

void __ARQInterruptServiceRoutine(void) {
    if (__ARQCallbackHi) {
        __ARQCallbackHi((u32)__ARQRequestPendingHi);
        __ARQRequestPendingHi = 0;
        __ARQCallbackHi = 0;
    } else if (__ARQCallbackLo) {
        __ARQCallbackLo((u32)__ARQRequestPendingLo);
        __ARQRequestPendingLo = 0;
        __ARQCallbackLo = 0;
    }
    __ARQPopTaskQueueHi();
    if (!__ARQRequestPendingHi) {
        __ARQServiceQueueLo();
    }
}

void ARQInit(void) {
    BOOL en;

    if (__ARQ_init_flag != 1) {
        __ARQRequestQueueLo = 0;
        __ARQRequestQueueHi = 0;
        __ARQChunkSize = 0x1000;
        en = OSDisableInterrupts();
        __AR_Callback = __ARQInterruptServiceRoutine;
        OSRestoreInterrupts(en);
        __ARQRequestPendingHi = 0;
        __ARQRequestPendingLo = 0;
        __ARQCallbackHi = 0;
        __ARQCallbackLo = 0;
        __ARQ_init_flag = 1;
    }
}
