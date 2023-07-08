#include <revolution/nwc24.h>
#include <revolution/os.h>
#include <mem.h>

static s32 nwc24ScdInitialized;
static OSMutex nwc24ScdCommandMutex;
static OSMutex nwc24ScdCounterMutex;

static void* nwc24ScdCommonBuffer;
static void* nwc24ScdCommonResult;

static s32 nwc24ScdSuspendCnt;
static s32 nwc24ScdOpenCnt;

s32 ExecSuspendScheduler(void);

s32 NWC24SuspendScheduler(void) {
    BOOL enabled;
    s32 res;

    if (!(nwc24ScdInitialized & 1)) {
        enabled = OSDisableInterrupts();

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