#include <revolution/ai.h>
#include <revolution/os.h>
#include <private/flipper.h>

static AIDCallback __AID_Callback;
static u8* __CallbackStack;
static u8* __OldStack;
static volatile BOOL __AI_init_flag = 0;
static volatile BOOL __AID_Active = 0;
static void __AIDHandler(__OSInterrupt, OSContext *);
static void __AICallbackStackSwitch(register void *);
static void __AI_SRC_INIT(void);

const char* __AIVersion = "<< RVL_SDK - AI \trelease build: Aug  8 2007 01:58:12 (0x4199_60831) >>";

AIDCallback AIRegisterDMACallback(AIDCallback cb) {
    AIDCallback old;
    BOOL intr;

    old = __AID_Callback;
    intr = OSDisableInterrupts();
    __AID_Callback = cb;
    OSRestoreInterrupts(intr);
    return old;
}

void AIInitDMA(u32 start, u32 len) {
    BOOL intr;

    intr = OSDisableInterrupts();
    __DSPRegs[0x18] = (__DSPRegs[0x18] & ~0x1FFF) | (start >> 16);
    __DSPRegs[0x19] = (__DSPRegs[0x19] & ~0xFFE0) | (0xFFFF & start);
    __DSPRegs[0x1B] = (__DSPRegs[0x1B] & ~0x7FFF) | ((len >> 5) & 0xFFFF);
    OSRestoreInterrupts(intr);
}

void AIStartDMA(void) {
    __DSPRegs[0x1B] |= 0x8000;
}

void AIStopDMA(void) {
    __DSPRegs[0x1B] &= ~0x8000;
}

u32 AIGetDMAStartAddr(void) {
    return (((__DSPRegs[0x1B] & 0x1FFF) << 16) | (__DSPRegs[0x19] & 0xFFE0));
}

u32 AIGetDMALength(void) {
    return ((__DSPRegs[0x1B] & 0x7FFF) << 5);
}

// this ends up being inlined into AISetDSPSampleRate in the condition
u32 AIGetDSPSampleRate(void) {
    return ((__AIRegs[0] & 0x40) >> 6) ^ 1;
}

void AISetDSPSampleRate(u32 sample_rate) {
    BOOL intr;

    if (sample_rate != AIGetDSPSampleRate()) {
        __AIRegs[0] &= ~0x40;

        if (sample_rate == 0) {
            intr = OSDisableInterrupts();
            __AI_SRC_INIT();
            __AIRegs[0] |= 0x40;
            OSRestoreInterrupts(intr);
        }
    }
}

static OSTime bound_32KHz;
static OSTime bound_48KHz;
static OSTime min_wait;
static OSTime max_wait;
static OSTime buffer;

void AIInit(u8 *pStack) {
    u32 reg;

    if (__AI_init_flag == 1) {
        return;
    }
 
    OSRegisterVersion(__AIVersion);

    bound_32KHz = OSNanosecondsToTicks(31524);
    bound_48KHz = OSNanosecondsToTicks(42024);
    min_wait = OSNanosecondsToTicks(42000);
    max_wait = OSNanosecondsToTicks(63000);
    buffer = OSNanosecondsToTicks(3000);

    reg = __AIRegs[0];
    reg &= ~(0x1 | 0x4 | 0x10);
    __AIRegs[0] = reg;
    __AIRegs[1] = 0;
    __AIRegs[3] = 0;
    __AIRegs[0] = (__AIRegs[0] & ~0x20) | (0x1 << 5);

    AISetDSPSampleRate(0);

    __AID_Callback = NULL;
    __CallbackStack = pStack;
    __OSSetInterruptHandler(5, __AIDHandler);
    __OSUnmaskInterrupts(0x4000000);
    __AI_init_flag = 1;
}

static void __AIDHandler(__OSInterrupt intr, OSContext *pContext) {
    OSContext context;
    u16 reg;

    reg = __DSPRegs[5];
    reg = (reg & ~(0x80 | 0x20) | 0x8);
    __DSPRegs[5] = reg;

    OSClearContext(&context);
    OSSetCurrentContext(&context);

    if (__AID_Callback) {
        if (!__AID_Active) {
            __AID_Active = 1;

            if (__CallbackStack != NULL) {
                __AICallbackStackSwitch(__AID_Callback);
            }
            else {
                (*__AID_Callback)();
            }

            __AID_Active = 0;
        }
    }

    OSClearContext(&context);
    OSSetCurrentContext(pContext);
}

/* this function is written in assembly due to the direct stack changes */
asm static void __AICallbackStackSwitch(register void *cb) {
    fralloc
    lis     r5, __OldStack@ha
    addi    r5, r5, __OldStack@l
    stw     r1, 0(r5)
    lis     r5, __CallbackStack@ha
    addi    r5, r5, __CallbackStack@l
    lwz     r1, 0(r5)
    subi    r1, r1, 8
    mtlr    cb
    blrl
    lis     r5, __OldStack@ha
    addi    r5, r5, __OldStack@l
    lwz     r1, 0(r5)
    frfree
    blr 
}

/* honestly I have no idea what's going on here */
void __AI_SRC_INIT(void) {
    OSTime v0 = 0;
    OSTime v1 = 0;
    OSTime v2 = 0;
    OSTime v4;
    u32 v5;
    u32 v6;
    u32 isDone = 0;
    u32 i = 0;
    u32 v8 = 0;

    v8 = 0;
    i = 0;

    v4 = 0;   

    while(isDone == 0) {
        SET_FLAG(__AIRegs[0], 1, 0x20, 5);
        SET_FLAG(__AIRegs[0], 0, 2, 1); 
        SET_FLAG(__AIRegs[0], 1, 1, 0);

        v5 = GET_FLAG(__AIRegs[2], 0x7FFFFFFF, 0);

        while(v5 == GET_FLAG(__AIRegs[2], 0x7FFFFFFF, 0)) { } 

        v0 = OSGetTime();
        SET_FLAG(__AIRegs[0], 1, 2, 1);
        SET_FLAG(__AIRegs[0], 1, 1, 0);

        v6 = GET_FLAG(__AIRegs[2], 0x7FFFFFFF, 0);

        while (v6 == GET_FLAG(__AIRegs[2], 0x7FFFFFFF, 0)) { }

        v1 = OSGetTime();

        v2 = v1 - v0;
        SET_FLAG(__AIRegs[0], 0, 2, 1);
        SET_FLAG(__AIRegs[0], 0, 1, 0); 

        if (v2 < bound_32KHz - buffer) {
            v4 = min_wait;
            isDone = 1;
            i++;
        }
        else {
            if (v2 >= (bound_32KHz + buffer) && v2 < (bound_48KHz - buffer)) {
                v4 = max_wait;
                isDone = 1;
                i++;
            }
            else {
                isDone = 0;
                v8 = 1;
                i++;
            }
        }
    }

    while ((v1 + v4) > OSGetTime()) { }
}