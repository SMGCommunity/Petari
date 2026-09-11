#pragma inline_max_size(250)
#include "Inline.hpp"
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
int JKRDecompressFromAramToMainRam(u32 src, void* dst, u32 srcLength, u32 dstLength, u32 offset, u32* resourceSize);
static inline int decompSZS_subroutine(u8* src, u8* dest);

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
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

OSMessageQueue JKRAram::sMessageQueue = {0};

JKRAram::~JKRAram() {
    sAramObject = nullptr;
    if (mAramHeap)
        delete mAramHeap;
}

void* JKRAram::run(void) {
    OSInitMessageQueue(&sMessageQueue, sMessageBuffer, 4);
    do {
        OSMessage msg;
        OSReceiveMessage(&sMessageQueue, &msg, OS_MESSAGE_BLOCK);
        JKRAramCommand* message = reinterpret_cast< JKRAramCommand* >(msg);
        int result = message->field_0x00;
        JKRAMCommand* command = reinterpret_cast< JKRAMCommand* >(message->command);
        delete message;

        switch (result) {
        case 1:
            JKRAramPiece::startDMA(command);
            break;
        }
    } while (true);
}

void JKRAram::checkOkAddress(u8* addr, u32 size, JKRAramBlock* block, u32 param_4) {
    if (!IS_ALIGNED(reinterpret_cast< uintptr_t >(addr), 0x20) && !IS_ALIGNED(size, 0x20)) {
        JUTException::panic(__FILE__, 219, ":::address not 32Byte aligned.");
    }

    if (block && !IS_ALIGNED(static_cast< uintptr_t >(block->getAddress()) + param_4, 0x20)) {
        JUTException::panic(__FILE__, 227, ":::address not 32Byte aligned.");
    }
}

void JKRAram::changeGroupIdIfNeed(u8* data, int groupId) {
    JKRHeap* currentHeap = JKRGetCurrentHeap();
    if (currentHeap->getHeapType() == 'EXPH' && groupId >= 0) {
        JKRExpHeap::CMemBlock* block = reinterpret_cast< JKRExpHeap::CMemBlock* >((data - sizeof(JKRExpHeap::CMemBlock)));
        block->newGroupId(groupId);
    }
}

JKRAramBlock* JKRAram::mainRamToAram(u8* buf, u32 bufSize, u32 alignedSize, JKRExpandSwitch expandSwitch, u32 fileSize, JKRHeap* heap, int id,
                                     u32* pSize) {
    JKRAramBlock* block = nullptr;
    checkOkAddress(buf, bufSize, nullptr, 0);
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
            if (block == nullptr) {
                return nullptr;
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
        if (allocatedMem == nullptr) {
            if (block != nullptr) {
                JKRFreeToAram(block);
            }
            return nullptr;
        }
        JKRDecompress(buf, reinterpret_cast< u8* >(allocatedMem), fileSize, 0);
        JKRAramPcs(0, reinterpret_cast< uintptr_t >(allocatedMem), bufSize, alignedSize, block);
        JKRFreeToHeap(heap, allocatedMem);
        block = block == nullptr ? reinterpret_cast< JKRAramBlock* >(-1) : block;
        if (pSize != nullptr) {
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
        if (block == nullptr) {
            return nullptr;
        }

        bufSize = block->getAddress();
    }

    JKRAramPcs(0, reinterpret_cast< uintptr_t >(buf), bufSize, alignedSize, block);
    block = block == nullptr ? reinterpret_cast< JKRAramBlock* >(-1) : block;
    if (pSize != nullptr) {
        *pSize = alignedSize;
    }
    return block;
}

u8* JKRAram::aramToMainRam(u32 address, u8* buf, u32 p3, JKRExpandSwitch expandSwitch, u32 p5, JKRHeap* heap, int id, u32* pSize) {
    JKRCompression compression = COMPRESSION_NONE;
    if (pSize != nullptr)
        *pSize = 0;

    checkOkAddress(buf, address, nullptr, 0);

    u32 expandSize;
    if (expandSwitch == EXPAND_SWITCH_UNKNOWN1) {
        u8 buffer[64];
        u8* bufPtr = reinterpret_cast< u8* >(ALIGN_NEXT(reinterpret_cast< uintptr_t >(buffer), 32));
        JKRAramPcs(1, address, reinterpret_cast< uintptr_t >(bufPtr), sizeof(buffer) / 2, nullptr);
        compression = JKRCheckCompressed_noASR(bufPtr);
        expandSize = JKRDecompExpandSize(bufPtr);
    }

    if (compression == COMPRESSION_YAZ0) {
        if (p5 != 0 && p5 < expandSize)
            expandSize = p5;

        u8* r26 = !buf ? reinterpret_cast< u8* >(JKRAllocFromHeap(heap, expandSize, 32)) : buf;
        if (r26 == nullptr)
            return nullptr;
        else {
            changeGroupIdIfNeed(r26, id);
            JKRDecompressFromAramToMainRam(address, r26, p3, expandSize, 0, pSize);
            return r26;
        }
    } else if (compression == COMPRESSION_YAY0) {
        u8* szpSpace = reinterpret_cast< u8* >(JKRAllocFromHeap(heap, p3, -32));
        if (szpSpace == nullptr) {
            return nullptr;
        } else {
            JKRAramPcs(1, address, reinterpret_cast< uintptr_t >(szpSpace), p3, nullptr);
            if (p5 != 0 && p5 < expandSize)
                expandSize = p5;

            u8* rv = !buf ? reinterpret_cast< u8* >(JKRAllocFromHeap(heap, expandSize, 32)) : buf;

            if (rv == nullptr) {
                JKRFree(szpSpace);
                return nullptr;
            } else {
                changeGroupIdIfNeed(rv, id);
                JKRDecompress(szpSpace, rv, expandSize, 0);
                JKRFreeToHeap(heap, szpSpace);
                if (pSize != nullptr) {
                    *pSize = expandSize;
                }
                return rv;
            }
        }
    } else {
        u8* r24 = !buf ? reinterpret_cast< u8* >(JKRAllocFromHeap(heap, p3, 32)) : buf;
        if (r24 == nullptr) {
            return nullptr;
        } else {
            changeGroupIdIfNeed(r24, id);
            JKRAramPcs(1, address, reinterpret_cast< uintptr_t >(r24), p3, nullptr);
            if (pSize != nullptr) {
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

static inline u8* nextSrcData(u8* current) {
    u8* dest;
    u32 left = static_cast< uintptr_t >((szpEnd - current));
    if (IS_NOT_ALIGNED(left, 0x20)) {
        dest = szpBuf + 0x20 - (left & (0x20 - 1));
    } else {
        dest = szpBuf;
    }

    memcpy(dest, current, left);
    u32 transSize = static_cast< uintptr_t >((szpEnd - (dest + left)));
    if (transSize > transLeft)
        transSize = transLeft;

    JKRAramPcs(1, static_cast< uintptr_t >((srcAddress + srcOffset)), (reinterpret_cast< uintptr_t >(dest) + left), ALIGN_NEXT(transSize, 0x20),
               nullptr);
    srcOffset += transSize;
    transLeft -= transSize;

    if (transLeft == 0)
        srcLimit = (dest + left) + transSize;

    return dest;
}

static inline u8* firstSrcData() {
    srcLimit = szpEnd - 0x19;
    u8* buffer = szpBuf;

    u32 size = szpEnd - buffer;
    u32 length = transLeft < size ? transLeft : size;

    JKRAramPcs(1, srcAddress + srcOffset, reinterpret_cast< uintptr_t >(buffer), ALIGN_NEXT(length, 0x20), nullptr);

    srcOffset += length;
    transLeft -= length;
    if (!transLeft) {
        srcLimit = buffer + length;
    }

    return buffer;
}

static inline int decompSZS_subroutine(u8* src, u8* dest) {
    u8* endPtr;
    s32 validBitCount = 0;
    s32 currCodeByte = 0;
    u32 ts = 0;

    if (src[0] != 'Y' || src[1] != 'a' || src[2] != 'z' || src[3] != '0') {
        return -1;
    }

    SYaz0Header* header = reinterpret_cast< SYaz0Header* >(src);
    endPtr = dest + (header->length - fileOffset);
    if (endPtr > dest + maxDest) {
        endPtr = dest + maxDest;
    }

    src += 0x10;
    s32 b1;
    u32 dist;
    u8* copySource;
    s32 numBytes;
    do {
        if (validBitCount == 0) {
            if ((src > srcLimit) && transLeft) {
                src = nextSrcData(src);
            }
            currCodeByte = *src++;
            validBitCount = 8;
        }
        if (currCodeByte & 0x80) {
            if (fileOffset != 0) {
                if (readCount >= fileOffset) {
                    *dest = *src;
                    dest++;
                    ts++;
                    if (dest == endPtr) {
                        break;
                    }
                }
                *(refCurrent++) = *src;
                if (refCurrent == refEnd) {
                    refCurrent = refBuf;
                }
                src++;
            } else {
                *dest++ = *src++;
                ts++;
                if (dest == endPtr) {
                    break;
                }
            }
            readCount++;
        } else {
            b1 = src[0];
            dist = src[1] | ((b1 & 0x0f) << 8);
            numBytes = b1 >> 4;
            src += 2;
            if (fileOffset != 0) {
                copySource = refCurrent - dist - 1;
                if (copySource < refBuf) {
                    copySource += refEnd - refBuf;
                }
            } else {
                copySource = dest - dist - 1;
            }
            if (numBytes == 0) {
                numBytes = (*src++) + 0x12;
            } else {
                numBytes += 2;
            }
            if (fileOffset != 0) {
                do {
                    if (readCount >= fileOffset) {
                        *dest = *copySource;
                        dest++;
                        ts++;
                        if (dest == endPtr) {
                            break;
                        }
                    }
                    *(refCurrent++) = *copySource;
                    if (refCurrent == refEnd) {
                        refCurrent = refBuf;
                    }
                    copySource++;
                    if (copySource == refEnd) {
                        copySource = refBuf;
                    }
                    readCount++;
                    numBytes--;
                } while (numBytes != 0);
            } else {
                do {
                    *dest = *copySource;
                    dest++;
                    ts++;
                    if (dest == endPtr) {
                        break;
                    }
                    copySource++;
                    readCount++;
                    numBytes--;
                } while (numBytes != 0);
            }
        }
        currCodeByte <<= 1;
        validBitCount--;
    } while (dest < endPtr);
    *tsPtr = ts;
    return 0;
}

int JKRDecompressFromAramToMainRam(u32 src, void* dst, u32 srcLength, u32 dstLength, u32 offset, u32* resourceSize) {
    BOOL interrupts = OSDisableInterrupts();
    if (s_is_decompress_mutex_initialized == false) {
        OSInitMutex(&decompMutex);
        s_is_decompress_mutex_initialized = true;
    }
    OSRestoreInterrupts(interrupts);
    OSLockMutex(&decompMutex);

    u32 szsBufferSize = JKRAram::getSZSBufferSize();
    szpBuf = reinterpret_cast< u8* >(JKRAllocFromSysHeap(szsBufferSize, 32));

    szpEnd = szpBuf + szsBufferSize;
    if (offset != 0) {
        refBuf = reinterpret_cast< u8* >(JKRAllocFromSysHeap(0x1120, 0));
        refEnd = refBuf + 0x1120;
        refCurrent = refBuf;
    } else {
        refBuf = nullptr;
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
    decompSZS_subroutine(first, reinterpret_cast< u8* >(dst));
    JKRFree(szpBuf);
    if (refBuf) {
        JKRFree(refBuf);
    }

    DCStoreRangeNoSync(dst, *tsPtr);
    OSUnlockMutex(&decompMutex);

    return 0;
}
