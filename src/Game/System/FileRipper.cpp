#include "Game/System/FileRipper.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/FileUtil.hpp"

namespace {
    OSMutex sDecompMutex;

    u8 *sReadBuffer;
    u8 *sReadBufferEnd;
    u8 *sReadBufferLimit;
    s32 sReadDvdOffset;
    u32 sReadDvdLeftSize;
    DVDFileInfo *sSrcFileInfo;
}

void FileRipper::setup(u32 size, JKRHeap *pHeap) {
    sReadBuffer = new (pHeap, 0x40) u8[size];
    sReadBufferEnd = sReadBuffer + size;
    OSInitMutex(&sDecompMutex);
}

s32 FileRipper::checkCompressed(const u8 *pData) {
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

void* FileRipper::loadToMainRAM(const char *fpath, u8 *dest, bool decompress, JKRHeap *pHeap, AllocDirection allocDir) {
    bool alloced = false;
    s32 compression = 0;
    u8 *copySrc = 0;
    u32 decompressedSize;
    
    if (!MR::isFileExist(fpath, false)) {
        OSPanic(__FILE__, 0x70, "File isn't exist.");
    }

    u32 fsize = (MR::getFileSize(fpath, false) + 0x1f) / 0x20 * 0x20;

    DVDFileInfo fileInfo;
    if (!DVDOpen(fpath, &fileInfo)) {
        OSPanic(__FILE__, 0x77, "DVDOpen() failed");
    }

    if (decompress) {
        u8 buf[0x60];
        copySrc = (u8 *)((u32)(buf + 0x3f) / 0x40 * 0x40);
        while (true) {
            s32 result = DVDReadPrio(&fileInfo, copySrc, 0x20, 0, 2);
            if (result >= 0) {
                break;
            }
            else if (result == -3) {
                return 0;
            }
            VIWaitForRetrace();
        }
        DCInvalidateRange(copySrc, 0x20);
        compression = checkCompressed(copySrc);
        decompressedSize = (copySrc[4] << 24) | (copySrc[5] << 16) | (copySrc[6] << 8) | copySrc[7];
    }

    if (decompress && compression) {
        if (!dest) {
            int align = allocDir == 0 ? 0x40 : -0x40;
            dest = (u8 *)JKRHeap::alloc(decompressedSize, align, pHeap);
            alloced = true;
        }
        if (!dest) {
            return 0;
        }
    }
    else {
        if (!dest) {
            int align = allocDir == 0 ? 0x40 : -0x40;
            dest = (u8 *)JKRHeap::alloc(fsize, align, pHeap);
            alloced = true;
        }
        if (!dest) {
            return 0;
        }
    }

    if (!compression) {
        u8* readDest;
        s32 readOffset;
        u32 readSize;
        while (true) {
            readDest = dest;
            readSize = fsize;
            readOffset = 0;
            if (copySrc) {
                readDest += 0x20;
                readSize -= 0x20;
                readOffset = 0x20;
                MR::copyMemory(dest, copySrc, 0x20);
            }
            s32 result = DVDReadPrio(&fileInfo, readDest, readSize, readOffset, 2);
            if (result >= 0) {
                break;
            }
            if (result == -3) {
                if (alloced == true) {
                    JKRHeap::free(dest, 0);
                }
                return 0;
            }
            VIWaitForRetrace();
        }
        return dest;
    }
    else if (compression == 2) {
        bool success = decompressFromDVD(&fileInfo, dest, fsize, decompressedSize, copySrc, copySrc ? 0x20 : 0);
        if (!success) {
            if (alloced) {
                JKRHeap::free(dest, 0);
            }
            dest = 0;
        }
        return dest;
    }
    else {
        if (alloced) {
            JKRHeap::free(dest, 0);
        }
        return 0;
    }
}

bool FileRipper::decompressFromDVD(
    DVDFileInfo *fileInfo,
    void *dest,
    u32 readSize,
    u32 destSize,
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

    DCStoreRangeNoSync(dest, destSize);
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
