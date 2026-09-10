#include <revolution/nwc24.h>
#include <revolution/nwc24/NWC24Internal.h>
#include <revolution/sc.h>

#include <mem.h>

#define CHECK_CALLING_STATUS() CheckCallingStatus(__FUNCTION__)

#define NWC24_TIME_DEVICE "/dev/net/kd/time"

enum {
    NWC24_IOCTL_SET_RTC_COUNTER = 23,
    NWC24_IOCTL_GET_TIME_DIFFERENCE = 24,
};

typedef struct CommonBuffer {
    u32 rtc;
    u32 flags;
    u8 padding[32 - 0x8];
} __attribute__((packed)) CommonBuffer;

typedef struct CommonResult {
    s32 result;
    u32 diff[2];
    u8 padding[32 - 0xC];
} __attribute__((packed)) CommonResult;

static BOOL nwc24TimeInitialized = FALSE;
static u32 nwc24TimeRtc = 0;
static s64 nwc24TimeDifference = 0;

static u8 buf[128];

static CommonResult nwc24TimeCommonResult __attribute__((aligned(32)));
static CommonBuffer nwc24TimeCommonBuffer __attribute__((aligned(32)));
static OSMutex nwc24TimeCommandMutex __attribute__((aligned(32)));

static NWC24Err GetRTC(u32* pRTC);
static NWC24Err CheckCallingStatus(const char* pUser);
static void InitMutex(void);
static void LockRight(void);
static void UnlockRight(void);

NWC24Err NWC24iGetUniversalTime(s64* pTime) NO_INLINE;

char* NWC24iStrTimeR(s64 seconds, char* buffer, int length);

char* NWC24iStrTime(s64 seconds) {
    return NWC24iStrTimeR(seconds, (char*)buf, sizeof(buf));
}

NWC24Err NWC24iGetUniversalTimeMinute(s32* minutes) {
    s64 seconds = 0;
    NWC24Err result = NWC24iGetUniversalTime(&seconds);
    if (result < 0) {
        return result;
    }
    return NWC24iEpochSecondsToMinutes(minutes, seconds);
}

NWC24Err NWC24iGetUniversalTime(s64* pTime) {
    static s64 whenCached = 0;
    NWC24Err result;

    if (pTime == NULL) {
        nwc24TimeDifference = 0;
        return NWC24_ERR_INVALID_VALUE;
    }

    if (whenCached == 0 || whenCached + OS_SEC_TO_TICKS(1) <= __OSGetSystemTime() || nwc24TimeRtc == 0) {
        result = NWC24iGetTimeDifference(&nwc24TimeDifference);
        if (result < 0) {
            return result;
        }

        result = GetRTC(&nwc24TimeRtc);
        if (result != NWC24_OK) {
            return result;
        }

        whenCached = __OSGetSystemTime();
    }

    *pTime = nwc24TimeDifference + nwc24TimeRtc;
    return NWC24_OK;
}

NWC24Err NWC24iGetTimeDifference(s64* pTimeDiff) {
    s32 fd;
    NWC24Err result;
    NWC24Err close;

    result = CHECK_CALLING_STATUS();
    if (result < 0) {
        return result;
    }

    LockRight();
    {
        result = NWC24_OPEN_DEVICE(NWC24_TIME_DEVICE, &fd, 0);

        if (result >= 0) {
            result = NWC24_IOCTL_DEVICE(fd, NWC24_IOCTL_GET_TIME_DIFFERENCE, NULL, 0, &nwc24TimeCommonResult, sizeof(CommonResult));

            if (result >= 0) {
                result = nwc24TimeCommonResult.result;

                if (result == 0 && pTimeDiff != (void*)NULL) {
                    *pTimeDiff = *(s64*)nwc24TimeCommonResult.diff;
                }
            }

            close = NWC24_CLOSE_DEVICE(fd);
            if (result >= 0) {
                result = close;
            }
        }
    }
    UnlockRight();

    return result;
}

NWC24Err NWC24iSetRtcCounter(u32 rtc, u32 flags) {
    s32 fd;
    NWC24Err result;
    NWC24Err close;

    result = CHECK_CALLING_STATUS();
    if (result < 0) {
        return result;
    }

    LockRight();
    {
        result = NWC24_OPEN_DEVICE(NWC24_TIME_DEVICE, &fd, 0);

        if (result >= 0) {
            nwc24TimeCommonBuffer.rtc = rtc;
            nwc24TimeCommonBuffer.flags = flags;

            result = NWC24_IOCTL_DEVICE(fd, NWC24_IOCTL_SET_RTC_COUNTER, &nwc24TimeCommonBuffer, sizeof(CommonBuffer), &nwc24TimeCommonResult,
                                        sizeof(CommonResult));

            if (result >= 0) {
                result = nwc24TimeCommonResult.result;
            }

            close = NWC24_CLOSE_DEVICE(fd);
            if (result >= 0) {
                result = close;
            }
        }
    }
    UnlockRight();

    return result;
}

NWC24Err NWC24iSynchronizeRtcCounter(BOOL forceSave) {
    u32 rtc;
    NWC24Err result;

    result = GetRTC(&rtc);
    if (result != NWC24_OK) {
        return result;
    }

    return NWC24iSetRtcCounter(rtc, forceSave ? 1 : 0);
}

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

static NWC24Err CheckCallingStatus(const char* pUser) {
    OSThread* thread = OSGetCurrentThread();
    if (thread == 0) {
        return NWC24_ERR_FATAL;
    }

    return NWC24_OK;
}

static void InitMutex(void) {
    BOOL enabled = OSDisableInterrupts();

    if (!nwc24TimeInitialized) {
        OSInitMutex(&nwc24TimeCommandMutex);

        memset(&nwc24TimeCommonBuffer, 0, sizeof(CommonBuffer));
        memset(&nwc24TimeCommonResult, 0, sizeof(CommonResult));

        nwc24TimeInitialized = TRUE;
    }

    OSRestoreInterrupts(enabled);
}

static void LockRight(void) {
    if (!nwc24TimeInitialized) {
        InitMutex();
    }

    OSLockMutex(&nwc24TimeCommandMutex);
}

static void UnlockRight(void) {
    OSUnlockMutex(&nwc24TimeCommandMutex);
}
