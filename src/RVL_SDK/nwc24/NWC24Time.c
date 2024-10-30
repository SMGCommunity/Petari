#include <revolution/nwc24.h>
#include <revolution/os.h>
#include <revolution/sc.h>
#include <mem.h>

static s32 nwc24TimeInitialized;
static OSMutex nwc24TimeCommandMutex;
static s32 nwc24TimeCommonBuffer[8] __attribute__((align(32)));
static s32 nwc24TimeCommonResult[8] __attribute__((align(32)));
static s32 nwc24ShtFd = -1;
static s32 nwc24ShtRetryRest = 0;
static BOOL NWC24iIsRequestPending = FALSE;
static OSShutdownFunctionInfo ShutdownFuncInfo;

static NWC24Err GetRTC(u32* rtcCounter) {
    OSTime time;
    u32 bias, status;

    do {
        status = SCCheckStatus();
        if (status == 2) {
            return NWC24_ERR_FATAL;
        }
    } while (status != 0);

    bias = SCGetCounterBias();
    time = OSGetTime();
    *rtcCounter = (u32)(OSTicksToSeconds(time) - bias);
    return NWC24_OK;
}

NWC24Err NWC24iSetRtcCounter(u32 rtcCounter, u32 flags) {
    NWC24Err result = NWC24_OK;
    NWC24Err resultClose = NWC24_OK;
    IOSFd fd;

    if (OSGetCurrentThread() != NULL) {
        result = 0;
    }
    else {
        result = -1;
    }

    if (result < NWC24_OK) {
        return result;
    }

    {
        if (nwc24TimeInitialized == 0) {
            BOOL en = OSDisableInterrupts();

            if (nwc24TimeInitialized == 0) {
                OSInitMutex(&nwc24TimeCommandMutex);
                memset(nwc24TimeCommonBuffer, 0, 0x20);
                memset(nwc24TimeCommonResult, 0, 0x20);
                nwc24TimeInitialized = TRUE;
            }

            OSRestoreInterrupts(en);
        }

        OSLockMutex(&nwc24TimeCommandMutex);

        result = NWC24iOpenResourceManager(__FUNCTION__, "/dev/net/kd/time", &fd, 0);

        if (result >= NWC24_OK) {
            nwc24TimeCommonBuffer[0] = rtcCounter;
            nwc24TimeCommonBuffer[1] = flags;
            result = NWC24iIoctlResourceManager(__FUNCTION__, fd, 23, nwc24TimeCommonBuffer, 0x20, nwc24TimeCommonResult, 0x20);

            if (result >= NWC24_OK) {
                result = nwc24TimeCommonResult[0];
            }

            resultClose = NWC24iCloseResourceManager(__FUNCTION__, fd);

            if (result >= NWC24_OK) {
                result = resultClose;
            }

            
        }

        OSUnlockMutex(&nwc24TimeCommandMutex);
        return result;
    }
}

NWC24Err NWC24iSynchronizeRtcCounter(BOOL forceSave) {
    NWC24Err err;
    u32 rtcCounter;

    err = GetRTC(&rtcCounter);

    if (err != NWC24_OK) {
        return err;
    }

    return NWC24iSetRtcCounter(rtcCounter, forceSave ? 1 : 0);
}