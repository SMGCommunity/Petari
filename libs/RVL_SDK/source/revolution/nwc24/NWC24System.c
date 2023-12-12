#include <revolution/nwc24.h>
#include <revolution/sc.h>

static s32 nwc24ShtFd = -1;
static s32 nwc24ShtRetryRest = 0;

static OSShutdownFunctionInfo ShutdownFuncInfo;

BOOL NWC24Shutdown(BOOL, u32);

NWC24Err NWC24iPrepareShutdown(void) {
    NWC24Err result = NWC24_OK;
    u32 status;
    
    NWC24iRegister();
    ShutdownFuncInfo.func = NWC24Shutdown;
    ShutdownFuncInfo.priority = 110;
    OSRegisterShutdownFunction(&ShutdownFuncInfo);

    if (nwc24ShtFd < 0) {
        result = NWC24iOpenResourceManager(__FUNCTION__, "/dev/net/kd/request", &nwc24ShtFd, 1);
    }

    nwc24ShtRetryRest = 5;

    while (1) {
        status = SCCheckStatus();

        if (status == 2) {
            break;
        }

        if (status != 1) {
            SCIdleModeInfo info;
            SCGetIdleMode(&info);
            __OSSetIdleLEDMode(info.led);
            break;
        }
    }

    if ((OSGetAppType() & 0xFF) == 0x40u) {
        NWC24iSetScriptMode(1);
    }

    return result;
}

static NWC24Err NWC24iRequestShutdown(u32 event, IOSError* ret) {
    NWC24Err result = NWC24_OK;
    NWC24Err resultClose = NWC24_OK;
    static s32 shtBuffer[8] __attribute__ ((aligned(32)));
    static s32 shtResult[8] __attribute__ ((aligned(32)));

    shtBuffer[0] = (s32)event;
    return NWC24iIoctlResourceManagerAsync(__FUNCTION__, nwc24ShtFd, 40, (void*)shtBuffer, sizeof(shtBuffer), (void*)shtResult, sizeof(shtResult), ret);
}

BOOL NWC24Shutdown(BOOL final, u32 event) {
    static BOOL shuttingdwn = FALSE;
    static IOSError result = IOS_ERROR_OK;

    if (final) {
        return TRUE;
    }

    if (shuttingdwn) {
        if (NWC24iIsAsyncRequestPending()) {
            return FALSE;
        }
        else {
            if (result >= 0) {
                return TRUE;
            }
            else {
                if (nwc24ShtRetryRest > 0) {
                    shuttingdwn = FALSE;
                    --nwc24ShtRetryRest;
                }
                else {
                    return TRUE;
                }
            }
        }
    }
    else {
        if (NWC24iRequestShutdown(event, &result) >= NWC24_OK) {
            shuttingdwn = TRUE;
        }
    }

    return FALSE;
}