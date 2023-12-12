#include <revolution/nwc24.h>
#include <revolution/os.h>
#include <mem.h>

static s32 nwc24ScdInitialized;
static OSMutex nwc24ScdCommandMutex;
static OSMutex nwc24ScdCounterMutex;

static s32 nwc24ScdCommonBuffer[8] __attribute__ ((aligned(32)));
static s32 nwc24ScdCommonResult[8] __attribute__ ((aligned(32)));

static s32 nwc24ScdSuspendCnt;
static s32 nwc24ScdOpenCnt;

s32 ExecSuspendScheduler(void);
s32 ExecResumeScheduler(void);
s32 ExecNoParamCommand(const char *, s32, s32 *);

s32 NWC24SuspendScheduler(void) {
    s32 res;

    if (!(nwc24ScdInitialized & 1)) {
        BOOL enabled = OSDisableInterrupts();

        if (!(nwc24ScdInitialized & 1)) {
            OSInitMutex(&nwc24ScdCommandMutex);
            OSInitMutex(&nwc24ScdCounterMutex);
            memset(nwc24ScdCommonBuffer, 0, 0x20);
            memset(nwc24ScdCommonResult, 0, 0x20);
            nwc24ScdInitialized |= 1;
        }

        OSRestoreInterrupts(enabled);
    }

    OSLockMutex(&nwc24ScdCounterMutex);
    res = ExecSuspendScheduler();

    if (res >= 0) {
        ++nwc24ScdSuspendCnt;
        res = res - nwc24ScdOpenCnt;
    }

    OSUnlockMutex(&nwc24ScdCounterMutex);
    return res;
}

static void InitScdMutex(void) {
    BOOL enabled = OSDisableInterrupts();

    if (!(nwc24ScdInitialized & 1)) {
        OSInitMutex(&nwc24ScdCommandMutex);
        OSInitMutex(&nwc24ScdCounterMutex);
        memset(nwc24ScdCommonBuffer, 0, sizeof(nwc24ScdCommonBuffer));
        memset(nwc24ScdCommonResult, 0, sizeof(nwc24ScdCommonResult));
        nwc24ScdInitialized |= 1;
    }

    OSRestoreInterrupts(enabled);
}

static void LockCounters(void) {
    if (!(nwc24ScdInitialized & 1)) {
        InitScdMutex();
    }

    OSLockMutex(&nwc24ScdCounterMutex);
}

static void UnlockCounters(void) {
    OSUnlockMutex(&nwc24ScdCounterMutex);
}

s32 NWC24ResumeScheduler(void) {
    s32 suspend;

    LockCounters();

    if (nwc24ScdOpenCnt > 0 && nwc24ScdSuspendCnt == 0) {
        suspend = 0;
    }
    else {
        suspend = ExecResumeScheduler();
        if (nwc24ScdSuspendCnt > 0) {
            --nwc24ScdSuspendCnt;
            suspend -= nwc24ScdOpenCnt;
        }
    }

    UnlockCounters();
    return suspend;
}

NWC24Err NWC24iSetScriptMode(s32 mode) {
    IOSFd fId;
    NWC24Err err;
    OSThread* thread = OSGetCurrentThread();

    if (thread == 0) {
        err = NWC24_ERR_FATAL;
    }
    else if (NWC24IsMsgLibOpened() || NWC24IsMsgLibOpenedByTool()) {
        err = NWC24_ERR_LIB_OPENED;
    }
    else if (NWC24IsMsgLibOpenBlocking()) {
        err = NWC24_ERR_BUSY;
    }
    else {
        err = NWC24_OK;
    }

    if (err < 0) {
        return err;
    }
    else {
        NWC24Err resErr;

        LockCounters();
        memset(nwc24ScdCommonBuffer, 0, 0x20);
        resErr = NWC24iOpenResourceManager(__FUNCTION__, "/dev/net/kd/request", &fId, 0);

        if (resErr >= 0) {
            nwc24ScdCommonBuffer[0] = mode;
            resErr = NWC24iIoctlResourceManager(__FUNCTION__, fId, 0x22, nwc24ScdCommonBuffer, 0x20, nwc24ScdCommonResult, 0x20);

            if (resErr >= 0) {
                resErr = nwc24ScdCommonResult[0];
            }

            {

                s32 tmp = NWC24iCloseResourceManager(__FUNCTION__, fId);

                if (resErr > 0) {
                    resErr = tmp;
                }
            }
        }

        UnlockCounters();
        return resErr;
    }

    return err;
}

#pragma auto_inline off
s32 ExecSuspendScheduler(void) {
    return ExecNoParamCommand(NULL, 1, NULL);
}

s32 ExecResumeScheduler(void) {
    return ExecNoParamCommand(NULL, 3, NULL);
}
#pragma auto_inline reset

s32 ExecNoParamCommand(const char* funcName, s32 cmd, s32* event) {
    IOSFd fId;
    s32 res;
    OSThread* thread = OSGetCurrentThread();

    if (thread == 0) {
        return -1;
    }

    if (!(nwc24ScdInitialized & 1)) {
        BOOL en = OSDisableInterrupts();

        if (!(nwc24ScdInitialized & 1)) {
            OSInitMutex(&nwc24ScdCommandMutex);
            OSInitMutex(&nwc24ScdCounterMutex);
            memset(nwc24ScdCommonBuffer, 0, 0x20);
            memset(nwc24ScdCommonResult, 0, 0x20);
            nwc24ScdInitialized |= 1;
        }

        OSRestoreInterrupts(en);
    }

    OSLockMutex(&nwc24ScdCommandMutex);
    res = NWC24iOpenResourceManager(funcName, "/dev/net/kd/request", &fId, 0);
    if (res >= 0) {
        res = NWC24iIoctlResourceManager(funcName, fId, cmd, NULL, 0, nwc24ScdCommonResult, 0x20);

        if (res >= 0) {
            res = nwc24ScdCommonResult[0];
            if (res == -2 || res == -33) {
                if (event != NULL) {
                    *event = nwc24ScdCommonResult[1];
                }
            }
        }

        {
            s32 tmp = NWC24iCloseResourceManager(funcName, fId);

            if (tmp < 0) {
                res = tmp;
            }
        }
    }

    OSUnlockMutex(&nwc24ScdCommandMutex);
    return res;
}