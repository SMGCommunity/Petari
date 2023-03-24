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

IOSError IOS_CloseAsync(IOSFd fd, IOSIpcCb cb, void* cbArg) {
    IOSRpcRequest* rpc;
    IOSError ret = 0;

    ret = __ios_Ipc1(fd, 2, cb, cbArg, &rpc);

    if (ret == 0) {
        ret = __ios__Ipc2(rpc, cb);
    }

    return ret;
}

IOSError IOS_Close(IOSFd fd) {
    IOSRpcRequest* rpc;
    IOSError ret = 0;

    ret = __ios_Ipc1(fd, 2, 0, 0, &rpc);

    if (ret == 0) {
        ret = __ios_Ip2(rpc, 0);
    }

    return ret;
}

static IOSError __ios_Read(IOSRpcRequest* rpc, void* buf, u32 len) {
    IOSError ret = 0;
    IOSResourceRequest* req;

    if (!rpc) {
        ret = -4;
        goto error;
    }

    req = &rpc->request;

    DCInvalidateRange(buf, len);
    req->args.read.outPtr = (buf) ? (u8*)OSVirtualToPhysical(buf) : 0;
    req->args.read.outLen = len;

error:
    return ret;
}

IOSError IOS_ReadAsync(IOSFd fd, void* buf, u32 len, IOSIpcCb cb, void* cbArg) {
    IOSRpcRequest* rpc;
    IOSError ret = 0;

    ret = __ios_Ipc1(fd, 3, cb, cbArg, &rpc);

    if (ret != 0) {
        goto error;
    }

    ret = __ios_Read(rpc, buf, len);

    if (ret != 0) {
        goto error;
    }

    ret = __ios_Ipc2(rpc, cb);

error:
    return ret;
}

IOSError IOS_Read(IOSFd fd, void* buf, u32 len) {
    IOSRpcRequest* rpc;
    IOSError ret = 0;

    ret = __ios_Ipc1(fd, 3, 0, 0, &rpc);

    if (ret != 0) {
        goto error;
    }

    ret = __ios_Read(rpc, buf, len);

    if (ret != 0) {
        goto error;
    }

    ret = __ios_Ipc2(rpc, 0);

error:
    return ret;
}

static IOSError __ios_Write(IOSRpcRequest* rpc, void* buf, u32 len) {
    IOSError ret = 0;
    IOSResourceRequest* req;

    if (!rpc) {
        ret = -4;
        goto error;
    }

    req = &rpc->request;
    req->args.write.inPtr = (buf) ? (u8*)OSVirtualToPhysical(buf) : 0;
    req->args.write.inLen = len;
    DCFlushRange(buf, len);

error:
    return ret;
}

IOSError IOS_WriteAsync(IOSFd fd, void* buf, u32 len, IOSIpcCb cb, void* cbArg) {
    IOSRpcRequest* rpc;
    IOSError ret = 0;

    ret = __ios_Ipc1(fd, 4, cb, cbArg, &rpc);

    if (ret != 0) {
        goto error;
    }

    ret = __ios_Write(rpc, buf, len);

    if (ret != 0) {
        goto error;
    }

    ret = __ios_Ipc2(rpc, cb);

error:
    return ret;
}

IOSError IOS_Write(IOSFd fd, void* buf, u32 len) {
    IOSRpcRequest* rpc;
    IOSError ret = 0;

    ret = __ios_Ipc1(fd, 4, 0, 0, &rpc);

    if (ret != 0) {
        goto error;
    }

    ret = __ios_Write(rpc, buf, len);

    if (ret != 0) {
        goto error;
    }

    ret = __ios_Ipc2(rpc, 0);

error:
    return ret;
}

static IOSError __ios_Seek(IOSRpcRequest* rpc, s32 offset, u32 whence) {
    IOSError ret = 0;
    IOSResourceRequest* req;

    if (!rpc) {
        ret = -4;
        goto error;
    }

    req = &rpc->request;
    req->args.seek.offset = offset;
    req->args.seek.whence = whence;

error:
    return ret;
}

IOSError IOS_SeekAsync(IOSFd fd, s32 offset, u32 whence, IOSIpcCb cb, void* cbArg) {
    IOSRpcRequest* rpc;
    IOSError ret = 0;

    ret = __ios_Ipc1(fd, 5, cb, cbArg, &rpc);

    if (ret != 0) {
        goto error;
    }

    ret = __ios_Seek(rpc, offset, whence);

    if (ret != 0) {
        goto error;
    }

    ret = __ios_Ipc2(rpc, cb);

error:
    return ret;
}

IOSError IOS_Seek(IOSFd fd, s32 offset, u32 whence) {
    IOSRpcRequest* rpc;
    IOSError ret = 0;

    ret = __ios_Ipc1(fd, 5, 0, 0, &rpc);

    if (ret != 0) {
        goto error;
    }

    ret = __ios_Seek(rpc, offset, whence);

    if (ret != 0) {
        goto error;
    }

    ret = __ios_Ipc2(rpc, 0);

error:
    return ret;
}

static IOSError __ios_Ioctl(IOSRpcRequest* rpc, s32 cmd, void* input, u32 inputLen, void* output, u32 outputLen) {
    IOSError ret = 0;
    IOSResourceRequest* req;

    if (!rpc) {
        ret = -4;
        goto error;
    }

    req = &rpc->request;

    req->args.ioctl.cmd = (u32)cmd;
    req->args.ioctl.outPtr = (output) ? (u8*)OSVirtualToPhysical(output) : 0;
    req->args.ioctl.outLen = outputLen;
    req->args.ioctl.inPtr = (input) ? (u8*)OSVirtualToPhysical(input) : 0;
    req->args.ioctl.inLen = inputLen;
    
    DCFlushRange(input, inputLen);
    DCFlushRange(output, outputLen);

error:
    return ret;
}

IOSError IOS_Ioctl(IOSFd fd, s32 cmd, void* input, u32 inputLen, void* output, u32 outputLen) {
    IOSRpcRequest* rpc;
    IOSError ret = 0;

    ret = __ios_Ipc1(fd, 6, 0, 0, &rpc);

    if (ret != 0) {
        goto error;
    }

    ret = __ios_Ioctl(rpc, cmd, input, inputLen, output, outputLen);

    if (ret != 0) {
        goto error;
    }

    ret = __ios_Ipc2(rpc, 0);

error:
    return ret;
}