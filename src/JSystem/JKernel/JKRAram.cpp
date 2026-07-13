#include "JSystem/JKernel/JKRAram.hpp"
#include "JSystem/JKernel/JKRAramPiece.hpp"
#include "JSystem/JKernel/JKRAramStream.hpp"
#include "JSystem/JKernel/JKRDecomp.hpp"
#include "JSystem/JKernel/JKRExpHeap.hpp"
#include "JSystem/JUtility/JUTException.hpp"
#include <cstring>
#include <revolution.h>

static u8* firstSrcData();
static u8* nextSrcData(u8* param_0);
static int JKRDecompressFromAramToMainRam(u32 src, void* dst, u32 srcLength, u32 dstLength, u32 offset, u32* resourceSize);
int decompSZS_subroutine(u8* src, u8* dest);

JKRAram* JKRAram::sAramObject;

JKRAram* JKRAram::create(u32 aram_audio_buffer_size, u32 aram_audio_graph_size, s32 stream_priority, s32 decomp_priority, s32 piece_priority) {
    if (!sAramObject) {
        sAramObject = new (JKRGetSystemHeap(), 0) JKRAram(aram_audio_buffer_size, aram_audio_graph_size, piece_priority);
    }

    JKRCreateAramStreamManager(stream_priority);
    JKRCreateDecompManager(decomp_priority);
    sAramObject->resume();
    return sAramObject;
}

OSMessage JKRAram::sMessageBuffer[4] = {
    NULL,
    NULL,
    NULL,
    NULL,
};

OSMessageQueue JKRAram::sMessageQueue = {0};

JKRAram::~JKRAram() {
    sAramObject = NULL;
    if (mAramHeap)
        delete mAramHeap;
}

void* JKRAram::run(void) {
    OSInitMessageQueue(&sMessageQueue, sMessageBuffer, 4);
    do {
        OSMessage msg;
        OSReceiveMessage(&sMessageQueue, &msg, OS_MESSAGE_BLOCK);
        JKRAramCommand* message = (JKRAramCommand*)msg;
        int result = message->field_0x00;
        JKRAMCommand* command = (JKRAMCommand*)message->command;
        delete message;

        switch (result) {
        case 1:
            JKRAramPiece::startDMA(command);
            break;
        }
    } while (true);
}

void JKRAram::checkOkAddress(u8* addr, u32 size, JKRAramBlock* block, u32 param_4) {
    if (!IS_ALIGNED((uintptr_t)addr, 0x20) && !IS_ALIGNED(size, 0x20)) {
        JUTException::panic(__FILE__, 219, ":::address not 32Byte aligned.");
    }

    if (block && !IS_ALIGNED((uintptr_t)block->getAddress() + param_4, 0x20)) {
        JUTException::panic(__FILE__, 227, ":::address not 32Byte aligned.");
    }
}

void JKRAram::changeGroupIdIfNeed(u8* data, int groupId) {
    JKRHeap* currentHeap = JKRGetCurrentHeap();
    if (currentHeap->getHeapType() == 'EXPH' && groupId >= 0) {
        JKRExpHeap::CMemBlock* block = (JKRExpHeap::CMemBlock*)(data - sizeof(JKRExpHeap::CMemBlock));
        block->newGroupId(groupId);
    }
}

JKRAramBlock* JKRAram::mainRamToAram(u8* buf, u32 bufSize, u32 alignedSize, JKRExpandSwitch expandSwitch, u32 fileSize, JKRHeap* heap, int id,
                                     u32* pSize) {
    JKRAramBlock* block = NULL;
    checkOkAddress(buf, bufSize, NULL, 0);
    if (expandSwitch == EXPAND_SWITCH_UNKNOWN1) {
        expandSwitch = (JKRCheckCompressed_noASR(buf) == COMPRESSION_NONE) ? EXPAND_SWITCH_UNKNOWN0 : EXPAND_SWITCH_UNKNOWN1;
    }
    if (expandSwitch == EXPAND_SWITCH_UNKNOWN1) {
        u32 expandSize = JKRDecompExpandSize(buf);
        if (fileSize == 0 || fileSize > expandSize) {
            fileSize = ALIGN_NEXT(expandSize, 32);
        }
        if (bufSize == 0) {
            block = JKRAllocFromAram(fileSize, JKRAramHeap::HEAD);
            if (block == NULL) {
                return NULL;
            }

            block->newGroupID(decideAramGroupId(id));
            bufSize = block->getAddress();
        }
        if (alignedSize == 0 || alignedSize > expandSize) {
            alignedSize = ALIGN_NEXT(expandSize, 32);
        }

        if (alignedSize > fileSize) {
            alignedSize = fileSize;
        }

        void* allocatedMem = JKRAllocFromHeap(heap, fileSize, -32);
        if (allocatedMem == NULL) {
            if (block != NULL) {
                JKRFreeToAram(block);
            }
            return NULL;
        }
        JKRDecompress(buf, (u8*)allocatedMem, fileSize, 0);
        JKRAramPcs(0, (uintptr_t)allocatedMem, bufSize, alignedSize, block);
        JKRFreeToHeap(heap, allocatedMem);
        block = block == NULL ? (JKRAramBlock*)-1 : block;
        if (pSize != NULL) {
            *pSize = alignedSize;
        }
        return block;
    }
    if (fileSize != 0 && alignedSize > fileSize) {
        alignedSize = fileSize;
    }
    if (bufSize == 0) {
        block = JKRAllocFromAram(alignedSize, JKRAramHeap::HEAD);
        block->newGroupID(decideAramGroupId(id));
        if (block == NULL) {
            return NULL;
        }

        bufSize = block->getAddress();
    }

    JKRAramPcs(0, (uintptr_t)buf, bufSize, alignedSize, block);
    block = block == NULL ? (JKRAramBlock*)-1 : block;
    if (pSize != NULL) {
        *pSize = alignedSize;
    }
    return block;
}

u8* JKRAram::aramToMainRam(u32 address, u8* buf, u32 p3, JKRExpandSwitch expandSwitch, u32 p5, JKRHeap* heap, int id, u32* pSize) {
    JKRCompression compression = COMPRESSION_NONE;
    if (pSize != NULL)
        *pSize = 0;

    checkOkAddress(buf, address, NULL, 0);

    u32 expandSize;
    if (expandSwitch == EXPAND_SWITCH_UNKNOWN1) {
        u8 buffer[64];
        u8* bufPtr = (u8*)ALIGN_NEXT((uintptr_t)buffer, 32);
        JKRAramPcs(1, address, (uintptr_t)bufPtr, sizeof(buffer) / 2,
                   NULL);  // probably change sizeof(buffer) / 2 to 32
        compression = JKRCheckCompressed_noASR(bufPtr);
        expandSize = JKRDecompExpandSize(bufPtr);
    }

    if (compression == COMPRESSION_YAZ0) {  // SZS
        if (p5 != 0 && p5 < expandSize)
            expandSize = p5;

        u8* r26 = !buf ? (u8*)JKRAllocFromHeap(heap, expandSize, 32) : buf;
        if (r26 == NULL)
            return NULL;
        else {
            changeGroupIdIfNeed(r26, id);
            JKRDecompressFromAramToMainRam(address, r26, p3, expandSize, 0, pSize);
            return r26;
        }
    } else if (compression == COMPRESSION_YAY0) {  // SZP
        u8* szpSpace = (u8*)JKRAllocFromHeap(heap, p3, -32);
        if (szpSpace == NULL) {
            return NULL;
        } else {
            JKRAramPcs(1, address, (uintptr_t)szpSpace, p3, NULL);
            if (p5 != 0 && p5 < expandSize)
                expandSize = p5;

            u8* rv = !buf ? (u8*)JKRAllocFromHeap(heap, expandSize, 32) : buf;

            if (rv == NULL) {
                JKRFree(szpSpace);
                return NULL;
            } else {
                changeGroupIdIfNeed(rv, id);
                JKRDecompress(szpSpace, rv, expandSize, 0);
                JKRFreeToHeap(heap, szpSpace);
                if (pSize != NULL) {
                    *pSize = expandSize;
                }
                return rv;
            }
        }
    } else {  // Not compressed or ASR
        u8* r24 = !buf ? (u8*)JKRAllocFromHeap(heap, p3, 32) : buf;
        if (r24 == NULL) {
            return NULL;
        } else {
            changeGroupIdIfNeed(r24, id);
            JKRAramPcs(1, address, (uintptr_t)r24, p3, NULL);
            if (pSize != NULL) {
                *pSize = p3;
            }
            return r24;
        }
    }
}

JSUList< JKRAMCommand > JKRAram::sAramCommandList;

static OSMutex decompMutex;

u32 JKRAram::sSZSBufferSize = 0x00000400;

static u8* szpBuf;

static u8* szpEnd;

static u8* refBuf;

static u8* refEnd;

static u8* refCurrent;

static u32 srcOffset;

static u32 transLeft;

static u8* srcLimit;

static u32 srcAddress;

static u32 fileOffset;

static u32 readCount;

static u32 maxDest;

static bool s_is_decompress_mutex_initialized;

static u32* tsPtr;

static u32 tsArea;

static int JKRDecompressFromAramToMainRam(u32 src, void* dst, u32 srcLength, u32 dstLength, u32 offset, u32* resourceSize) {
    BOOL interrupts = OSDisableInterrupts();
    if (s_is_decompress_mutex_initialized == false) {
        OSInitMutex(&decompMutex);
        s_is_decompress_mutex_initialized = true;
    }
    OSRestoreInterrupts(interrupts);
    OSLockMutex(&decompMutex);

    u32 szsBufferSize = JKRAram::getSZSBufferSize();
    szpBuf = (u8*)JKRAllocFromSysHeap(szsBufferSize, 32);

    szpEnd = szpBuf + szsBufferSize;
    if (offset != 0) {
        refBuf = (u8*)JKRAllocFromSysHeap(0x1120, 0);
        refEnd = refBuf + 0x1120;
        refCurrent = refBuf;
    } else {
        refBuf = NULL;
    }
    srcAddress = src;
    srcOffset = 0;
    transLeft = (srcLength != 0) ? srcLength : -1;
    fileOffset = offset;
    readCount = 0;
    maxDest = dstLength;
    tsPtr = (resourceSize != 0) ? resourceSize : &tsArea;
    *tsPtr = 0;

    u8* first = firstSrcData();
    int r25 = decompSZS_subroutine(first, (u8*)dst);
    JKRFree(szpBuf);
    if (refBuf) {
        JKRFree(refBuf);
    }

    DCStoreRangeNoSync(dst, *tsPtr);
    OSUnlockMutex(&decompMutex);

    return 0;
}

// this won't inline
// shrug
static u8* firstSrcData() {
    srcLimit = szpEnd - 0x19;
    u8* buffer = szpBuf;

    u32 size = szpEnd - buffer;
    u32 length = transLeft < size ? transLeft : size;

    JKRAramPcs(1, srcAddress + srcOffset, uintptr_t(buffer), ALIGN_NEXT(length, 0x20), NULL);

    srcOffset += length;
    transLeft -= length;
    if (!transLeft) {
        srcLimit = buffer + length;
    }

    return buffer;
}

static u8* nextSrcData(u8* current) {
    u8* dest;
    u32 left = (uintptr_t)(szpEnd - current);
    if (IS_NOT_ALIGNED(left, 0x20)) {
        dest = szpBuf + 0x20 - (left & (0x20 - 1));
    } else {
        dest = szpBuf;
    }

    memcpy(dest, current, left);
    u32 transSize = (uintptr_t)(szpEnd - (dest + left));
    if (transSize > transLeft)
        transSize = transLeft;

    JKRAramPcs(1, (uintptr_t)(srcAddress + srcOffset), ((uintptr_t)dest + left), ALIGN_NEXT(transSize, 0x20), NULL);
    srcOffset += transSize;
    transLeft -= transSize;

    if (transLeft == 0)
        srcLimit = (dest + left) + transSize;

    return dest;
}
