#include <revolution/fs.h>
#include <revolution/ipc.h>
#include <private/iosresclt.h>
#include <private/iosrestypes.h>
#include <cstring>

static IOSFd __fsFd = -1;
static u32 __fsInitialized = FALSE;
static char* __devfs = 0;
static IOSHeapId hId;

#define ROUNDUP(sz)     (((u32)(sz) + 31) & \
                        ~(u32)(31))

typedef struct isfs_GetAttr {
    IOSUid* ownerId;
    IOSGid* groupId;
    u32* attr;
    u32* ownerAcc;
    u32* groupAcc;
    u32* othersAcc;
} isfs_GetAttr;

typedef struct isfs_GetUsage {
    u32* nblocks;
    u32* ninodes;
} isfs_GetUsage;

typedef struct __isfsCtxt {
    u8 ioBuf[ROUNDUP(256)] __attribute__((aligned(32)));
    ISFSCallback cb;
    void* ctxt;
    u32 func;

    union {
        ISFSStats* stats;
        ISFSFileStats* fstats;
        u32* num;
        isfs_GetAttr ga;
        isfs_GetUsage gu;
    } args;
} __isfsCtxt;

ISFSError ISFS_OpenLib(void) {
    ISFSError rc = 0;
    static void* lo = 0, *hi = 0;
    __isfsCtxt* __fsCtxt = 0;

    if (!__fsInitialized) {
        lo = IPCGetBufferLo();
        hi = IPCGetBufferHi();
    }

    __devfs = (char*)ROUNDUP(lo);
    if (!__fsInitialized && ((u32)__devfs + ROUNDUP(64)) > (u32)hi) {
        OSReport("APP ERROR: Not enough IPC arena\n");
        rc = -22;
        goto out;
    }

    strcpy(__devfs, "/dev/fs");
    __fsFd = IOS_Open(__devfs, 0);

    if (__fsFd < 0) {
        rc = __fsFd;
        goto out;
    }

    __fsCtxt = (__isfsCtxt*)((u32)__devfs);

    if (!__fsInitialized && ((u32)__fsCtxt + (16 + 1) *  ROUNDUP(sizeof(__isfsCtxt))) > (u32)hi) {
        OSReport("APP ERROR: Not enough IPC arena\n");
        rc = -22;
        goto out;
    }

    if (!__fsInitialized) {
        IPCSetBufferLo((void*)((u32)__fsCtxt + (17) * ROUNDUP(sizeof(__isfsCtxt))));
        __fsInitialized = TRUE;
    }

    hId = iosCreateHeap(__fsCtxt, 17 * ROUNDUP(sizeof(__isfsCtxt)));

    if (hId < 0) {
        rc = -22;
        goto out;
    }

out:
    return rc;
}

s32 ISFS_Read(s32 fd, u8 *pBuffer, u32 bufSize) {
    /* nullptr check and alignment to 0x20 */
    if (pBuffer == NULL || (u32)pBuffer & 31) {
        return -101;
    }

    return IOS_Read(fd, pBuffer, bufSize);
}
