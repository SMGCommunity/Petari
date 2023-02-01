#include <revolution/os.h>
#include <revolution/os/OSResetSW.h>
#include <revolution/vi.h>
#include <revolution/ipc/ipcclt.h>
#include <private/iosresclt.h>
#include <private/flipper.h>

static u32 StmImInBuf[8] __attribute__((align(32)));
static u32 StmImOutBuf[8] __attribute__((align(32)));

static u32 StmEhInBuf[8]__attribute__((align(32)));
static u32 StmEhOutBuf[8] __attribute__((align(32)));

static int StmReady = 0;
static int StmImDesc = 0;
static int StmEhDesc = 0;
static int StmEhRegistered = 0;

static int StmVdInUse = 0;

static BOOL ResetDown;

static OSResetCallback ResetCallback;
static OSPowerCallback PowerCallback;

static BOOL __OSGetResetButtonStateRaw(void);
static s32 __OSStateEventHandler(s32, void *);

// this function is inlined but isn't in some games, so thanks
BOOL __OSGetResetButtonStateRaw(void) {
    u32 ret;
    ret = __PIRegs[0];

    if(!(ret & 0x10000)) {
        return TRUE;
    } 
    else {
        return FALSE;
    }
}

// same with this one
inline void __OSRegisterStateEvent(void) {
    s32 error;
    s32 en;
    en = OSDisableInterrupts();
    error = IOS_IoctlAsync(StmEhDesc, 0x1000, StmEhInBuf, 0x20, StmEhOutBuf, 0x20, __OSStateEventHandler, (void*)0);

    StmEhRegistered = (error == 0) ? TRUE : FALSE;

    OSRestoreInterrupts(en);
}

s32 __OSSetIdleLEDMode(u32 led_mode) {
    s32 ret;

    if (StmReady == 0) {
        return -6;
    }

    StmImInBuf[0] = led_mode;
    ret = IOS_Ioctl(StmImDesc, 0x6002, StmImInBuf, 0x20, StmImOutBuf, 0x20);
    return ret;
}

s32 __OSUnRegisterStateEvent(void) {
    s32 ret;

    if (StmEhRegistered == 0) {
        return 0;
    }

    if (StmReady == 0) {
        return -6;
    }

    ret = IOS_Ioctl(StmImDesc, 0x3002, StmImInBuf, 0x20, StmImOutBuf, 0x20);

    if (ret == 0) {
        StmEhRegistered = 0;
    }

    return ret;
}

s32 __OSVIDimReplyHandler(void) {
    StmVdInUse = 0;
    return 0;
}

void __OSDefaultResetCallback(void) {

}

void __OSDefaultPowerCallback(void) {

}

// arg seems to be unused and it's only there so we can register our states
#ifdef NON_MATCHING
s32 __OSStateEventHandler(s32 ret, void *pUnused) {
    int en;

    if (ret != 0) {
        OSPanic(__FILE__, 0x314, "Error on STM state event handler\n");
    }

    StmEhRegistered = 0;

    if (StmEhOutBuf[0] == 0x20000) {
        // this won't inline properly. sigh
        if (__OSGetResetButtonStateRaw()) {
            OSResetCallback cb;
            en = OSDisableInterrupts();
            ResetDown = TRUE;
            cb = ResetCallback;
            ResetCallback = __OSDefaultResetCallback;
            cb();
            OSRestoreInterrupts(en);
            VIResetDimmingCount();
        }

        __OSRegisterStateEvent();
    }

    if (StmEhOutBuf[0] == 0x800) {
        OSPowerCallback cb;
        en = OSDisableInterrupts();
        cb = PowerCallback;
        PowerCallback = __OSDefaultPowerCallback;
        cb();
        OSRestoreInterrupts(en);
    }

    return 0;
}
#endif
