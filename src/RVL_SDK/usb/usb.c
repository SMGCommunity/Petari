#include "private/iosrestypes.h"
#include "private/iostypes.h"
#include "revolution.h"
#include "revolution/usb.h"
#include "revolution/ipc.h"
#include "revolution/os/OSInterrupt.h"
#include <cstdio>
#include <mem.h>

#define ROUNDUP(sz) (((u32)(sz) + 32 - 1) & ~(u32)(32 - 1))

static IOSHeapId hId = -1;
void *lo = NULL, *hi = NULL;

typedef struct {
    char *data;
    u16 wLength;
} IntBlkCtrlReq;

typedef struct iusb_Ctxt {
    USBCallbackFunc  cb;
    USBIsoCallbackFunc icb;
    void *cbArg;
    void *xfer;
    void *spare;
    void *clean[8];
    u32 nclean;

    union {
        char path[ROUNDUP(64)];
        char des[ROUNDUP(sizeof(USB_DevDescr))];
        char reqBuf[ROUNDUP(sizeof(IntBlkCtrlReq))];
    } u __attribute__ ((aligned(32)));

} iusb_ctxt;

static u8 s_usb_log = 0;
static u8 s_usb_err = 1;

void USB_LOG(char *str, ...) {
    va_list vlist;
    if (s_usb_log) {
        OSReport("USB: ");
        va_start(vlist, str);
        vprintf(str, vlist);
        va_end(vlist);
    } else {
        (void)str;
    }
}

void USB_ERR(char *str, ...) {
    va_list vlist;
    if (s_usb_err) {
        OSReport("USB ERR: ");
        va_start(vlist, str);
        vprintf(str, vlist);
        va_end(vlist);
    } else {
        (void)str;
    }
}

static void* IOSAlloc(u32 size) {
    void *rv;
    if ((rv = iosAllocAligned(hId, size, 32)) == NULL) {
      USB_ERR("iosAllocAligned(%d, %u) failed: %d\n", hId, size, rv);
    }

    return rv;
}

static void IOSFree(void *addr) {
    IOSError rv;
    if (addr) {    
        if ((rv = iosFree(hId, addr)) < 0)  {
            USB_ERR("iosFree(%d, 0x%x) failed: %d\n", hId, addr, rv);
        }
    }
    return;
}  

IOSError IUSB_OpenLib(void) {
    IOSError rc = IOS_ERROR_OK;
    u32 mask = (u32)OSDisableInterrupts();
    if (-1 != hId) {
        USB_LOG("Library is already initialized. Heap Id = %d\n", hId);
        goto out;
    }

    if (NULL == lo) {
        lo = IPCGetBufferLo();
        hi = IPCGetBufferHi();
        USB_LOG("iusb size: %d lo: %x hi: %x\n", sizeof(iusb_ctxt), lo, hi);

        if ((u32)lo + ROUNDUP(16384) > (u32)hi) {
            USB_ERR("Not enough IPC arena\n");
            rc = IOS_ERROR_FAIL_ALLOC;
            goto out;
        }

        IPCSetBufferLo((void*)((u32)lo + ROUNDUP(16384)));
    }

    hId = iosCreateHeap(lo, 16384);

    if (hId < 0) {
        USB_ERR("Not enough heaps\n");
        rc = IOS_ERROR_FAIL_ALLOC;
        goto out;
    }

out:
    OSRestoreInterrupts((BOOL)mask);
    return rc;
}

IOSError IUSB_CloseLib(void) {
    return IOS_ERROR_OK;
}

IOSError _intrBlkCtrlIsoCb(IOSError ret, void *ctxt) {
    u32 i;
    IOSError rv = ret;
    iusb_ctxt *req = (iusb_ctxt*)ctxt;

    USB_LOG("_intrBlkCtrlIsoCb returned: %d\n", rv);
    USB_LOG("_intrBlkCtrlIsoCb: nclean = %d\n", req->nclean);

    if ((req->nclean != 7) && (req->nclean != 3) && (req->nclean != 0) && (req->nclean != 4) && (req->nclean != 2)) {
        USB_ERR("__intrBlkCtrlIsoCb: got invalid nclean\n");
    } 
    else {
        for (i = 0; i < req->nclean; ++i) {
            USB_LOG("Freeing clean[%d] = %x\n", i, req->clean[i]);
            IOSFree(req->clean[i]);
        }
        req->nclean = 0;
    }

    USB_LOG("cb = %x cbArg = %x\n", req->cb, req->cbArg);

    if (req->cb) {
        req->cb(ret, req->cbArg);
    } 
    else if (req->icb) {
        USB_LOG("calling iso callback\n");
        req->icb(ret, req->xfer, req->cbArg);
    }

    IOSFree(req);
    return rv;
}

IOSError IUSB_OpenDeviceIds(const char *did, u16 vid, u16 pid, IOSFd *fd) {
    IOSError rv = IOS_ERROR_OK;
    iusb_ctxt *req = 0;

    if (fd == 0) {
        rv = IOS_ERROR_INVALID;
        goto out;
    }

    req = IOSAlloc(ROUNDUP(sizeof(*req)));
    if (req == 0) {
        USB_ERR("OpenDeviceIds: Not enough memory\n");
        rv = IOS_ERROR_FAIL_ALLOC;
        goto out;
    }

    memset(req, 0, sizeof(iusb_ctxt));
    snprintf(req->u.path, 64, "/dev/usb/%s/%x/%x", did, vid, pid);
    USB_LOG("OpenDevice - %s\n", req->u.path);
    rv = IOS_Open(req->u.path, 0);
    USB_LOG("OpenDevice returned: %d\n", rv);
    *fd = rv;

out:
    IOSFree(req);
    return rv;
}

IOSError IUSB_CloseDeviceAsync(IOSFd fd, USBCallbackFunc cb, void *cbArg)
{
    IOSError rv = IOS_ERROR_OK;
    iusb_ctxt *req;

    USB_LOG("CloseDevice\n");
    req = IOSAlloc(ROUNDUP(sizeof(*req)));
    if (req == 0) {
        USB_ERR("CloseDeviceAsync: Not enough memory\n");
        rv = IOS_ERROR_FAIL_ALLOC;
        goto out;
    }
    memset(req, 0, sizeof(iusb_ctxt));

    req->cb = cb;
    req->cbArg = cbArg;
    req->nclean = 0;

    rv = IOS_CloseAsync(fd, _intrBlkCtrlIsoCb, req);
    USB_LOG("CloseDevice returned: %d\n", rv);
    if (rv < 0) {
        IOSFree(req);
        goto out;
    }
out:
    return rv;

}

static IOSError __IntrBlkMsgInt(IOSFd fd, u32 ep, u32 buflen, char *buf, u8 typ, USBCallbackFunc cb, void *cbArg, u8 async) {
    IOSError rv = IOS_ERROR_OK;   
    IOSIoVector *vector = IOSAlloc(0x60);
    u8 *sep = IOSAlloc(0x20);
    u16 *sbuflen = IOSAlloc(0x20);

    if (NULL == vector || NULL == sep || NULL == sbuflen) {
        USB_ERR("__IntrBlkMsgInt: Not enough memory\n");
        rv = IOS_ERROR_FAIL_ALLOC;
        goto clean;
    }

    *sep = (u8)ep;
    *sbuflen = (u16)buflen;
    vector[0].base = sep;
    vector[0].length = sizeof(u8);
    vector[1].base = (u8 *) sbuflen;
    vector[1].length = sizeof(u16);
    vector[2].base = (u8*) buf;
    vector[2].length = buflen;
    
    DCFlushRange(sep, 0x20);
    DCFlushRange(sbuflen, 0x20);
    DCFlushRange(vector, 0x60);

    if (0 == async) {
        rv = IOS_Ioctlv(fd, typ, 2, 1, vector);
        USB_LOG("intr/blk ioctl returned: %d\n", rv);
        goto clean;
    } 
    else {
        iusb_ctxt *req = IOSAlloc(0x80);
        if (NULL == req) {
            USB_ERR("IntBlkMsgInt (async): Not enough memory\n");
            rv = IOS_ERROR_FAIL_ALLOC;
            goto clean;
        } 
        memset(req, 0, sizeof(iusb_ctxt));
        req->cb = cb;
        req->cbArg = cbArg;
        USB_LOG("intrblkmsg: cb = 0x%x cbArg = 0x%x\n", req->cb, req->cbArg);
        req->nclean = 3;
        req->clean[0] = sep;
        req->clean[1] = sbuflen;
        req->clean[2] = vector;
        ((IntBlkCtrlReq *)req->u.reqBuf)->data = buf;
        ((IntBlkCtrlReq *)req->u.reqBuf)->wLength = (u16) buflen;
        
        rv = IOS_IoctlvAsync(fd, typ, 2, 1, vector, _intrBlkCtrlIsoCb, req);
        if (rv < 0) {
            IOSFree(req);
            goto clean;
        }
        goto out;
    }
clean:
    IOSFree(sep);
    IOSFree(sbuflen);
    IOSFree(vector);
out:
    return rv;
}

IOSError IUSB_ReadIntrMsgAsync(IOSFd fd, u32 ep, u32 buflen, char *buf, USBCallbackFunc cb, void *cbArg) {
    DCInvalidateRange(buf, buflen);
    return __IntrBlkMsgInt(fd, ep, buflen, buf, 2, cb, cbArg, 1);
}

IOSError IUSB_ReadBlkMsgAsync(IOSFd fd, u32 ep, u32 buflen, char *buf, USBCallbackFunc cb, void *cbArg) {
    IOSError rv;
    DCInvalidateRange(buf, buflen);
    rv = __IntrBlkMsgInt(fd, ep, buflen, buf, 1, cb, cbArg, 1);
    return rv;
}

IOSError IUSB_WriteBlkMsgAsync(IOSFd fd, u32 ep, u32 buflen, char *buf, USBCallbackFunc cb, void *cbArg) {
    IOSError rv;
    DCFlushRange((void*)buf, buflen);
    rv = __IntrBlkMsgInt(fd, ep, buflen, buf, 1, cb, cbArg, 1);
    return rv;
}

IOSError __CtrlMsgInt(IOSFd fd, u8 reqType, u8 request, u16 value, u16 index, u16 length, char *data, USBCallbackFunc cb, void *cbArg, u8 async) {
    IOSError rv = IOS_ERROR_OK;
    IOSIoVector *vector;
    u8 *sreqType, *srequest, *sep;
    u16 *svalue, *sindex, *slength;
    
    if ((data == 0 && length != 0) || ((u32)data & (u32)(32 - 1))) {
        rv = IOS_ERROR_INVALID;
        USB_ERR("ctrlmsg: bad data buffer\n");
        goto out;
    }

    vector = IOSAlloc(0x20*7);
    sreqType = IOSAlloc(0x20);
    srequest = IOSAlloc(0x20);
    sep = IOSAlloc(0x20);
    svalue = IOSAlloc(0x20);
    sindex = IOSAlloc(0x20);
    slength = IOSAlloc(0x20);

    if (NULL == sreqType || NULL == srequest || NULL == sep
        || NULL == svalue || NULL == sindex || NULL == slength
        || NULL == vector) {
        USB_ERR("Ctrl Msg: Not enough memory\n");
        rv = IOS_ERROR_FAIL_ALLOC;
        goto clean;
    }
    
    *sreqType = reqType;
    *srequest = request;
    *svalue = SWAP16(value);
    *sindex = SWAP16(index);
    *slength = SWAP16(length);
    *sep = 0;

    vector[0].base = sreqType;
    vector[0].length = sizeof(u8);
    vector[1].base = srequest;
    vector[1].length = sizeof(u8);
    vector[2].base =  (u8 *) svalue;
    vector[2].length = sizeof(u16);
    vector[3].base = (u8 *) sindex;
    vector[3].length = sizeof(u16);
    vector[4].base = (u8 *) slength;
    vector[4].length = sizeof(u16);
    vector[5].base =  sep;
    vector[5].length = sizeof(u8);
    vector[6].base = (u8 *)data;
    vector[6].length = length;

    DCFlushRange(sreqType, 0x20);
    DCFlushRange(srequest, 0x20);
    DCFlushRange(sep, 0x20);
    DCFlushRange(svalue, 0x20);
    DCFlushRange(sindex, 0x20);
    DCFlushRange(slength, 0x20);
    DCFlushRange(vector, 0x20*7);

    if (0 == async) {
        rv = IOS_Ioctlv(fd, 0, 6, 1, vector);
        goto clean;
    } 
    else {
        iusb_ctxt *req = IOSAlloc(0x80);
        if (NULL == req) {
            USB_ERR("CtrlMsgInt (async): Not enough memory\n");
            rv = IOS_ERROR_FAIL_ALLOC;
            goto clean;
        }

        memset(req, 0, sizeof(iusb_ctxt));
        req->cb = cb;
        req->cbArg = cbArg;
        USB_LOG("ctrlmsgint: cb = 0x%x cbArg = 0x%x\n", req->cb, req->cbArg);
        req->nclean = 7;
        req->clean[0] = sreqType;
        req->clean[1] = srequest;
        req->clean[2] = svalue;
        req->clean[3] = sindex;
        req->clean[4] = slength;
        req->clean[5] = sep;
        req->clean[6] = vector;
        ((IntBlkCtrlReq *)req->u.reqBuf)->data = data;
        ((IntBlkCtrlReq *)req->u.reqBuf)->wLength = length;
        
        rv = IOS_IoctlvAsync(fd, 0, 6, 1, vector, _intrBlkCtrlIsoCb, req);

        USB_LOG("Ctrl Msg async returned: %d\n", rv);
    
        if (rv < 0) {
            IOSFree(req);
            goto clean;
        } 

        goto out;
    }
clean:
    IOSFree(sreqType);
    IOSFree(srequest);
    IOSFree(svalue);
    IOSFree(sindex);
    IOSFree(slength);
    IOSFree(sep);
    IOSFree(vector);
out:
    return rv;
}

IOSError IUSB_WriteCtrlMsgAsync(IOSFd fd, u8 reqType, u8 request, u16 value, u16 index, u16 buflen, char *buf, USBCallbackFunc cb, void *cbArg) {
    DCFlushRange((void*)buf, buflen);
    return __CtrlMsgInt(fd, reqType, request, value, index, buflen, buf, cb, cbArg, 1);
}
