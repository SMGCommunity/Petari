#include "private/iostypes.h"
#include "private/iosrestypes.h"
#include "private/iosresclt.h"
#include "revolution/os.h"
#include "revolution/ipc/memory.h"

/* a few of these functions were grabbed from debug builds, because many of these are inlined into functions and they are complex */

/* macro for matching __ipcQueueRequest */
#define diff(a, b)                                                           \
    ((a) < (b)) ? ((u32)0xffffffff - (b) + (a) + 1) : ((a) - (b))

static s32 __mailboxAck = 1;
static u32 __relnchFl = 0;

#define OSVirtualToPhysical(addr)    OSCachedToPhysical(addr)
#define OSPhysicalToVirtual(addr)    OSPhysicalToCached(addr)

typedef struct IOSRpcRequest {
    IOSResourceRequest request;
    IOSIpcCb cb __attribute__((aligned(32))); // I am assuming this is aligned due to where cbArg is stored, and I see nothing between cb and callback_arg?
    void* callback_arg;
    u32 relaunch_flag;
    OSThreadQueue thread_queue;
} IOSRpcRequest;

static struct {
    u32 rcount;
    u32 wcount;
    u32 rptr;
    u32 wptr;
    IOSResourceRequest *buf[16];
} __responses;

static IOSHeapId hid = -1;

extern void IPCiProfQueueReq(void *, s32);

/* the MSL_C version of strnlen doesn't match when inlined, cool */
static u32 strnlen(const u8 *str, u32 n) {
    const u8 *s = str;
    while (*s && n-- > 0)
        ++s;
    return (s - str);
}

/* this call seems to be inlined */
static inline IOSRpcRequest* ipcAllocReq(void ){
    IOSRpcRequest* req = NULL;
    req = iosAllocAligned(hid, 0x40, 0x20);
    return req;
}

static inline IOSError ipcFree(IOSRpcRequest *rpc) {
    IOSError ret = 0;
    iosFree(hid, rpc);
    return ret;
}

static inline IOSError __ipcQueueRequest(IOSResourceRequest *req) {
    IOSError ret = 0;

    if (diff(__responses.wcount, __responses.rcount) >= sizeof(__responses.buf) / sizeof(__responses.buf[0])) {
        ret = -8;
    }
    else {
        __responses.buf[__responses.wptr] = req;
        __responses.wptr = (__responses.wptr + 1) % (sizeof(__responses.buf) / sizeof(__responses.buf[0]));
        __responses.wcount++;
        IPCiProfQueueReq(req, (s32)req->handle);
    }

    return ret;
}

static inline void __ipcSendRequest(void) {
    IOSRpcRequest *rpc;

    if (diff(__responses.wcount, __responses.rcount) <= 0) {
        return;
    }

    rpc = (IOSRpcRequest*)__responses.buf[__responses.rptr];

    if (rpc == NULL) {
        return;
    }

    if (rpc->relaunch_flag) {
        __mailboxAck--;
    }

    IPCWriteReg(0, OSVirtualToPhysical(rpc));
    __responses.rptr = (__responses.rptr + 1) % (sizeof(__responses.buf) / sizeof(__responses.buf[0]));
    __responses.rcount++;
    __mailboxAck--;

    IPCWriteReg(1, (IPCReadReg(1) & (1 << 5 | 1 << 4)) | 1 << 0);
}

static inline IOSError __ios_Open(IOSRpcRequest *rpc, const char *path, u32 flags) {
    IOSError ret = 0;
    IOSResourceRequest* req;

    if (!rpc) {
        ret = -4;
        goto error;
    }

    req = &rpc->request;
    /* regswap with the strnlen call... */
    DCFlushRange((void*)path, strnlen((const u8*)path, 64) + 1);
    req->args.open.path = (u8*)OSVirtualToPhysical(path);
    req->args.open.flags = flags;

error:
    return ret;
}

// the existance of Ipc2 implies the existance of Ipc1, which has a common pattern
// so this code is what I am assuming is a part of Ipc1
static inline IOSError __ios_Ipc1(IOSFd fd, u32 cmd, IOSIpcCb cb, void *cbArg, IOSRpcRequest **rpc) {
    IOSError ret = 0;
    IOSResourceRequest *req;

    if (rpc == 0) {
        ret = -4;
        goto error;
    }
    
    *rpc = (IOSRpcRequest*)ipcAllocReq();
    
    if (*rpc == 0) {
        ret = -22;
        goto error;
    }

    req = &(*rpc)->request;
    (*rpc)->cb = cb;
    (*rpc)->callback_arg = cbArg;
    (*rpc)->relaunch_flag = 0;
    req->cmd = cmd;
    req->handle = (u32)fd;

error:
    return ret;
}

// this one, however, is not inlined
static IOSError __ios_Ipc2(IOSRpcRequest *rpc, IOSIpcCb cb) {
    IOSError ret = 0;
    u32 inten;
    IOSResourceRequest *req;

    if (rpc == NULL) {
        ret = -4;
    }
    else {
        req = &rpc->request;

        if (!cb) {
            OSInitThreadQueue(&rpc->thread_queue);
        }

        DCFlushRange(req, sizeof(*req));
        inten = OSDisableInterrupts();
        ret = __ipcQueueRequest(req);

        if (ret != 0) {
            OSRestoreInterrupts(inten);

            if (cb) {
                ipcFree(rpc);
            }
        }
        else {
            if (__mailboxAck > 0) {
                __ipcSendRequest();
            }

            if (!cb) {
                OSSleepThread(&rpc->thread_queue);
            }

            OSRestoreInterrupts(inten);

            if (!cb) {
                ret = req->status;
            }
        }
    }

    if (rpc && !cb) {
        ipcFree(rpc);
    }

    return ret;
}

IOSError IOS_OpenAsync(const char *pPath, u32 flags, IOSIpcCb cb, void *callback_arg) {
    IOSRpcRequest* rpc;
    IOSError ret = 0;

    ret = __ios_Ipc1(0, 1, cb, callback_arg, &rpc);

    if (ret != 0) {
        goto error;
    }

    ret = __ios_Open(rpc, pPath, flags);

    if (ret != 0) {
        goto error;
    }

    ret = __ios_Ipc2(rpc, cb);
error:
    return ret;
}

IOSError IOS_Open(const char *path, u32 flags) {
    IOSRpcRequest  *rpc;
    IOSError ret = 0;

    ret = __ios_Ipc1(0, 1, 0, 0, &rpc);
    
    if (ret != 0) {
        goto error;
    }

    ret = __ios_Open(rpc, path, flags);
    
    if (ret != 0) {
        goto error;
    }
    
    ret = __ios_Ipc2(rpc, 0);

error:
    return ret;
}