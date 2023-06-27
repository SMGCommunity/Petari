#include <revolution.h>

static u32 IpcNumPendingReqs = 0;
static u32 IpcNumUnIssuedReqs = 0;
static s32 IpcFdArray[32];
static u32 IpcReqPtrArray[32];

static void AddReqInfo(void *, s32);
static void DelReqInfo(void *, s32);

void IPCiProfInit(void) {
    u32 i;

    IpcNumPendingReqs = 0;
    IpcNumUnIssuedReqs = 0;

    for (i = 0; i < 32; ++i) {
        IpcReqPtrArray[i] = 0;
        IpcFdArray[i] = -1;
    }
}

void IPCiProfQueueReq(void *req, s32 fd) {
    ++IpcNumPendingReqs;
    ++IpcNumUnIssuedReqs;
    AddReqInfo(req, fd);
}

static void AddReqInfo(void* ptr, s32 fd) {
    u32 i;

    for (i = 0; i < 32; ++i) {
        if (IpcReqPtrArray[i] == 0 && IpcFdArray[i] == -1) {
            IpcReqPtrArray[i] = (u32)ptr;
            IpcFdArray[i] = fd;
            break;
        }
    }
}

void IPCiProfAck(void) {
    --IpcNumUnIssuedReqs;
}

void IPCiProfReply(void* req, s32 fd) {
    --IpcNumPendingReqs;
    DelReqInfo(req, fd);
}

static void DelReqInfo(void* ptr, s32 fd) {
    u32 i;

    for (i = 0; i < 32; ++i) {
        if (IpcReqPtrArray[i] == (u32)ptr && IpcFdArray[i] == fd) {
            IpcReqPtrArray[i] = 0;
            IpcFdArray[i] = -1;
            break;
        }
    }
}