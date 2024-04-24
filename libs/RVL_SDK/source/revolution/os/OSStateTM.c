#include <revolution/os.h>
#include <revolution/os/OSResetSW.h>
#include <revolution/vi.h>
#include <revolution/ipc/ipcclt.h>
#include <private/iosresclt.h>
#include <private/flipper.h>

static u32 StmImInBuf[8] __attribute__((align(32)));
static u32 StmImOutBuf[8] __attribute__((align(32)));

static u32 StmVdInBuf[8] __attribute__((align(32)));
static u32 StmVdOutBuf[8] __attribute__((align(32)));

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
static s32 __OSVIDimReplyHandler(s32, void *);
static void __OSDefaultResetCallback(void);
static void __OSDefaultPowerCallback(void);
static void __OSRegisterStateEvent(void);

#ifdef NON_MATCHING
OSPowerCallback OSSetPowerCallback(OSPowerCallback callback) {
    BOOL enabled;
    OSPowerCallback prevCallback;

    enabled = OSDisableInterrupts();
    prevCallback = PowerCallback;

    if (callback) {
        PowerCallback = callback;
    }
    else {
        PowerCallback = __OSDefaultPowerCallback;
    }

    if (!StmEhRegistered) {
        __OSRegisterStateEvent();
    }

    OSRestoreInterrupts(enabled);

    if (prevCallback == __OSDefaultResetCallback) {
        return NULL;
    }
    else {
        return prevCallback;
    }
}
#endif

// from a debug build of the OS lib, this function is inlined in __OSSetVIForceDimming
static int AccessVIDimRegs(void) {
    int res;
    res = IOS_IoctlAsync(StmImDesc, 0x5001, StmVdInBuf, 0x20, StmVdOutBuf, 0x20, __OSVIDimReplyHandler, 0);
    switch(res) {
        default:
            return res;
        case 0:
            return 1;
    }   
}

int __OSSetVIForceDimming(BOOL isEnabled, u32 yShift, u32 xShift) {
    BOOL en;

    if (!StmReady) {
        return -10;
    }

    en = OSDisableInterrupts();

    if (StmVdInUse) {
        OSRestoreInterrupts(en);
        return 0;
    }

    StmVdInUse = 1;
    OSRestoreInterrupts(en);

    StmVdInBuf[0] = yShift << 3 | xShift | isEnabled << 7;
    StmVdInBuf[1] = 0;
    StmVdInBuf[2] = 0;
    StmVdInBuf[3] = 0;
    StmVdInBuf[4] = 0;
    StmVdInBuf[5] = 0xFFFFFFFF;
    StmVdInBuf[6] = 0xFFFF0000;
    StmVdInBuf[7] = 0;
    
    return AccessVIDimRegs();
}

// this function is inlined but isn't in some games, so thanks
BOOL __OSGetResetButtonStateRaw(void) {
    return (!(__PIRegs[0] & 0x10000)) ? TRUE : FALSE;
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

s32 __OSVIDimReplyHandler(s32 ret, void *pUnused) {
    StmVdInUse = 0;
    return 0;
}

static void __OSRegisterStateEvent(void) {
    int err, enabled;
    enabled = OSDisableInterrupts();

    err = IOS_IoctlAsync(StmEhDesc, 0x1000, StmEhInBuf, 0x20, StmEhOutBuf, 0x20, __OSStateEventHandler, (void*)0);

    if (err == IOS_ERROR_OK) {
        StmEhRegistered = 1;
    }
    else {
        StmEhRegistered = 0;
    }

    OSRestoreInterrupts(enabled);
}

void __OSDefaultResetCallback(void) {

}

void __OSDefaultPowerCallback(void) {

}

// arg seems to be unused and it's only there so we can register our states
//#ifdef NON_MATCHING
static s32 __OSStateEventHandler(s32 ret, void *pUnused) {
    int en;
    OSResetCallback cb;

    if (ret != 0) {
        OSPanic(__FILE__, 0x314, "Error on STM state event handler\n");
    }

    StmEhRegistered = 0;

    if (StmEhOutBuf[0] == 0x20000) {
        // this won't inline properly. sigh
        if (__OSGetResetButtonStateRaw()) {
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
        en = OSDisableInterrupts();
        cb = PowerCallback;
        PowerCallback = __OSDefaultPowerCallback;
        cb();
        OSRestoreInterrupts(en);
    }

    return 0;
}
//#endif

int __OSInitSTM(void) {
    PowerCallback = __OSDefaultPowerCallback;
    ResetCallback = __OSDefaultResetCallback;
    ResetDown = 0;

    if (StmReady) {
        return 1;
    }

    StmVdInUse = 0;
    StmImDesc = IOS_Open("/dev/stm/immediate", 0);

    if (StmImDesc < 0) {
        StmReady = 0;
        return 0;
    }

    StmEhDesc = IOS_Open("/dev/stm/eventhook", 0);

    if (StmEhDesc < 0) {
        StmReady = 0;
        return 0;
    }

    __OSRegisterStateEvent();
    StmReady = 1;
    return 1;
}

static void LockUp(void);

void __OSHotReset(void) {
    int result;
    __VIRegs[1] = 0;

    if (!StmReady) {
        OSPanic(__FILE__, 380, "Error: The firmware doesn't support reboot feature.\n");
    }

    result = IOS_Ioctl(StmImDesc, 0x2001, StmImInBuf, sizeof(StmImInBuf), StmImOutBuf, sizeof(StmImOutBuf));
    LockUp();
}

static void LockUp(void) {
    BOOL en = OSDisableInterrupts();

    ICFlashInvalidate();

    while (1) {

    }
}