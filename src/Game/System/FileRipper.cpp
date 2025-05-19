#include "Game/System/FileRipper.hpp"
#include "Game/Util/MemoryUtil.hpp"

namespace {
    OSMutex sDecompMutex;
}

namespace {
    u8 *sReadBuffer;
    u8 *sReadBufferEnd;
    u8 *sReadBufferLimit;
    s32 sReadDvdOffset;
    u32 sReadDvdLeftSize;
    DVDFileInfo *sSrcFileInfo;
}

s32 FileRipper::checkCompressed(const u8 *pData)
{
    if (pData[0] == 'Y' && pData[1] == 'a' && pData[3] == '0') {
        if (pData[2] == 'y') {
            return 1;
        }

        if (pData[2] == 'z') {
            return 2;
        }
    }

    return 0;
}

bool FileRipper::decompressFromDVD(
    DVDFileInfo *fileInfo,
    void *dest,
    u32 readSize,
    u32 bufSize,
    const u8 *currentPos,
    u32 copySize
) {
    OSLockMutex(&sDecompMutex);
    
    sSrcFileInfo = fileInfo;
    sReadDvdOffset = 0;
    sReadDvdLeftSize = readSize;
    sReadBufferLimit = sReadBufferEnd - 0x19;
    
    u8 *buf;
    if (currentPos) {
        MR::copyMemory(sReadBufferEnd - copySize, currentPos, copySize);
        sReadDvdOffset += copySize;
        sReadDvdLeftSize -= copySize;
        buf = readSrcDataNext(sReadBufferEnd - copySize);
    }
    else {
        buf = readSrcDataFirst();
    }

    bool result;
    if (buf) {
        result = decompressSzsSub(buf, (u8 *)dest);
    }
    else {
        result = false;
    }

    DCStoreRangeNoSync(dest, bufSize);
    OSUnlockMutex(&sDecompMutex);

    return result;
}

bool FileRipper::decompressSzsSub(u8 *src, u8 *dest) {
    u8 *copy_src;
    u32 group_count = 0;
    u32 group_head = 0;
    
    if (src[0] != 'Y' || src[1] != 'a' || src[2] != 'z' || src[3] != '0') {
        return false;
    }
    
    u32 data_len = *(u32 *)(src + 4);
    u8 *dest_end = dest + data_len;
    src += 0x10;

    do {
        if (!group_count) {
            if (src > sReadBufferLimit && sReadDvdLeftSize) {
                if (!(src = readSrcDataNext(src))) {
                    return false;
                }
            }
            group_head = *src;
            group_count = 8;
            src = src + 1;
        }

        if (group_head & 0x80) {
            u8 b = *src++;
            *dest++ = b;
            if (dest == dest_end) {
                break;
            }
        }
        else {
            u8 b1 = *src++;
            u8 b2 = *src++;

            u32 offset = ((b1 & 0x0f) << 8);
            offset |= b2;

            copy_src = dest - offset - 1;
            u32 copy_len = b1 >> 4;
            if (copy_len == 0) {
                copy_len = *src++ + 0x12;
            }
            else {
                copy_len += 2;
            }
            do {
                *dest++ = *copy_src;
                if (dest == dest_end) {
                    break;
                }
                ++copy_src;
                --copy_len;
            } while (copy_len != 0);
        }
        group_head <<= 1;
        --group_count;
    } while (dest < dest_end);
    
    return true;
}

u8* FileRipper::readSrcDataFirst() {
    u8 *readBuf = sReadBuffer;
    u32 readSize = sReadBufferEnd - sReadBuffer;
    if (sReadDvdLeftSize < readSize) {
        readSize = sReadDvdLeftSize;
    }
    while(true) {
        s32 result = DVDReadPrio(sSrcFileInfo, readBuf, readSize, sReadDvdOffset, 2);
        if (result >= 0) {
            break;
        }
        else if (result == -3) {
            return 0;
        }
        VIWaitForRetrace();
    }
    DCInvalidateRange(readBuf, readSize);
    sReadDvdOffset += readSize;
    sReadDvdLeftSize -= readSize;
    return readBuf;
}

u8* FileRipper::readSrcDataNext(u8 *buf) {
    u32 size = sReadBufferEnd - buf;
    u8 *start;
    if ((size & 0x1f) != 0) {
        start = sReadBuffer + 0x20 - (size & 0x1f);
    }
    else {
        start = sReadBuffer;
    }
    MR::copyMemory(start, buf, size);
    u8 *readBuf = start + size;
    u32 readSize = sReadBufferEnd - readBuf;
    if (sReadDvdLeftSize < readSize) {
        readSize = sReadDvdLeftSize;
    }
    while(true) {
        s32 result = DVDReadPrio(sSrcFileInfo, readBuf, readSize, sReadDvdOffset, 2);
        if (result >= 0) {
            break;
        }
        else if (result == -3) {
            return 0;
        }
        VIWaitForRetrace();
    }
    DCInvalidateRange(readBuf, readSize);
    sReadDvdOffset += readSize;
    sReadDvdLeftSize -= readSize;
    if (!sReadDvdLeftSize) {
        sReadBufferLimit = readBuf + readSize;
    }
    return start;
}
