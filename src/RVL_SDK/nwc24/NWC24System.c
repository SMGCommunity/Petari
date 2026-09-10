#include <revolution/nwc24.h>
#include <revolution/nwc24/NWC24Internal.h>
#include <revolution/os.h>
#include <revolution/sc.h>

#define SHUTDOWN_RETRY_MAX 5

enum {
    NWC24_IOCTL_SHUTDOWN = 40,
};

static s32 nwc24ShtFd = -1;
static s32 nwc24ShtRetryRest = 0;

static OSShutdownFunctionInfo ShutdownFuncInfo;

NWC24Err NWC24EnableLedNotification(BOOL enable) {
    SCIdleModeInfo idleMode;
    u32 status;
    u32 led = 0;

    if (enable) {
        do {
            status = SCCheckStatus();

            if (status == SC_STATUS_FATAL) {
                return NWC24_ERR_FATAL;
            }
        } while (status == SC_STATUS_BUSY);

        SCGetIdleMode(&idleMode);
        led = idleMode.led;
    }

    __OSSetIdleLEDMode(led);
    return NWC24_OK;
}

NWC24Err NWC24iPrepareShutdown(void) {
    NWC24Err result = NWC24_OK;

    NWC24iRegister();

    ShutdownFuncInfo.func = NWC24Shutdown;
    ShutdownFuncInfo.priority = 110;
    OSRegisterShutdownFunction(&ShutdownFuncInfo);

    if (nwc24ShtFd < 0) {
        result = NWC24_OPEN_DEVICE(NWC24i_SCHEDULER_DEVICE, &nwc24ShtFd, 1);
    }

    nwc24ShtRetryRest = SHUTDOWN_RETRY_MAX;
    NWC24EnableLedNotification(TRUE);

    if (OSGetAppType() == 0x40) {
        NWC24iSetScriptMode(1);
    }

    return result;
}

NWC24Err NWC24iRequestShutdown(u32 event, NWC24Err* pResult) {
    static u8 shtBuffer[32] __attribute__((aligned(32)));
    static u8 shtResult[32] __attribute__((aligned(32)));

    *(u32*)shtBuffer = event;
    return NWC24_IOCTL_DEVICE_ASYNC(nwc24ShtFd, NWC24_IOCTL_SHUTDOWN, shtBuffer, sizeof(shtBuffer), shtResult, sizeof(shtResult), pResult);
}

BOOL NWC24Shutdown(BOOL final, u32 event) {
    static BOOL shuttingdown = FALSE;
    static NWC24Err result = NWC24_OK;

    if (final) {
        return TRUE;
    }

    if (shuttingdown) {
        if (NWC24iIsAsyncRequestPending()) {
            return FALSE;
        }

        if (result >= 0) {
            return TRUE;
        }

        if (nwc24ShtRetryRest > 0) {
            shuttingdown = FALSE;
            nwc24ShtRetryRest--;
        } else {
            return TRUE;
        }
    } else if (NWC24iRequestShutdown(event, &result) >= 0) {
        shuttingdown = TRUE;
    }

    return FALSE;
}
