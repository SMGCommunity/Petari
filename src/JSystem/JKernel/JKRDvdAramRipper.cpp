#include "Inline.hpp"
#pragma inline_max_size(250)
#include "JSystem/JKernel/JKRDvdAramRipper.hpp"
#include "JSystem/JKernel/JKRAram.hpp"
#include "JSystem/JKernel/JKRAramHeap.hpp"
#include "JSystem/JKernel/JKRAramPiece.hpp"
#include "JSystem/JKernel/JKRAramStream.hpp"
#include "JSystem/JKernel/JKRDecomp.hpp"
#include "JSystem/JKernel/JKRDvdFile.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"
#include "JSystem/JSupport/JSUFileStream.hpp"
#include <cstring>

int JKRDecompressFromDVDToAram(JKRDvdFile*, u32, u32, u32, u32, u32, u32*);
static inline u8* firstSrcData();
static inline u8* nextSrcData(u8*);
static inline int decompSZS_subroutine(u8*, u32);
static u32 dmaBufferFlush(u32);

JKRAramBlock* JKRDvdAramRipper::loadToAram(s32 entryNumber, u32 address, JKRExpandSwitch expandSwitch, u32 param_3, u32 param_4, u32* param_5) {
    JKRDvdFile dvdFile;
    if (!dvdFile.open(entryNumber)) {
        return nullptr;
    } else {
        return loadToAram(&dvdFile, address, expandSwitch, param_3, param_4, param_5);
    }
}

JKRAramBlock* JKRDvdAramRipper::loadToAram(JKRDvdFile* dvdFile, u32 address, JKRExpandSwitch expandSwitch, u32 param_3, u32 param_4, u32* param_5) {
    JKRADCommand* command = loadToAram_Async(dvdFile, address, expandSwitch, nullptr, param_3, param_4, param_5);
    syncAram(command, 0);

    if (command->field_0x48 < 0) {
        delete command;
        return nullptr;
    }

    if (address) {
        delete command;
        return reinterpret_cast< JKRAramBlock* >(-1);
    }

    JKRAramBlock* result = command->mBlock;
    delete command;
    return result;
}

JKRADCommand* JKRDvdAramRipper::loadToAram_Async(JKRDvdFile* dvdFile, u32 address, JKRExpandSwitch expandSwitch, void (*callback)(u32), u32 param_4,
                                                 u32 param_5, u32* param_6) {
    JKRADCommand* command = new (JKRGetSystemHeap(), -4) JKRADCommand();
    command->mDvdFile = dvdFile;
    command->mAddress = address;
    command->mBlock = nullptr;
    command->mExpandSwitch = expandSwitch;
    command->mCallback = callback;
    command->field_0x3c = param_4;
    command->field_0x40 = param_5;
    command->field_0x44 = param_6;

    JKRADCommand* cmd2 = callCommand_Async(command);
    if (!cmd2) {
        delete command;
        return nullptr;
    }

    return command;
}

JSUList< JKRADCommand > JKRDvdAramRipper::sDvdAramAsyncList;

bool JKRDvdAramRipper::errorRetry = true;

JKRADCommand* JKRDvdAramRipper::callCommand_Async(JKRADCommand* command) {
    bool bVar1 = true;
    JKRDvdFile* dvdFile = command->mDvdFile;
    s32 compression = 0;
    s32 uncompressedSize;
    OSLockMutex(&dvdFile->mMutex2);
    if (command->field_0x44) {
        *command->field_0x44 = 0;
    }

    if (dvdFile->field_0x50) {
        bVar1 = false;
    } else {
        dvdFile->field_0x50 = OSGetCurrentThread();
        JSUFileInputStream* stream = new (JKRGetSystemHeap(), -4) JSUFileInputStream(dvdFile);
        dvdFile->mFileStream = stream;
        u32 fileSize = dvdFile->getFileSize();
        if (command->field_0x40 && fileSize > command->field_0x40) {
            fileSize = command->field_0x40;
        }
        fileSize = ALIGN_NEXT(fileSize, 0x20);
        if (command->mExpandSwitch == 1) {
            u8 buffer[0x40];
            u8* bufPtr = reinterpret_cast< u8* >(ALIGN_NEXT(reinterpret_cast< u32 >(&buffer), 0x20));
            while (true) {
                s32 result = DVDReadPrio(dvdFile->getFileInfo(), bufPtr, 0x20, 0, 2);
                if (result >= 0) {
                    break;
                }

                if (JKRDvdAramRipper::errorRetry == 0) {
                    delete stream;
                    return nullptr;
                }

                VIWaitForRetrace();
            }
            DCInvalidateRange(bufPtr, 0x20);

            JKRCompression detectedCompression = JKRDecomp::checkCompressed(bufPtr);
            if (detectedCompression == COMPRESSION_ASR) {
                detectedCompression = COMPRESSION_NONE;
            }
            compression = detectedCompression;

            u32 expandSize = (bufPtr[4] << 24) | (bufPtr[5] << 16) | (bufPtr[6] << 8) | bufPtr[7];
            uncompressedSize = expandSize;
            if (command->field_0x40 && uncompressedSize > command->field_0x40) {
                uncompressedSize = command->field_0x40;
            }
        }

        if (compression == 0) {
            command->mExpandSwitch = EXPAND_SWITCH_UNKNOWN0;
        }

        if (command->mExpandSwitch == EXPAND_SWITCH_UNKNOWN1) {
            if (command->mAddress == 0 && command->mBlock == nullptr) {
                command->mBlock = JKRAllocFromAram(uncompressedSize, JKRAramHeap::HEAD);
                if (command->mBlock) {
                    command->mAddress = command->mBlock->getAddress();
                }
                dvdFile->mBlock = command->mBlock;
            }

            if (command->mBlock) {
                command->mAddress = command->mBlock->getAddress();
            }

            if (command->mAddress == 0) {
                dvdFile->field_0x50 = nullptr;
                return nullptr;
            }
        } else {
            if (command->mAddress == 0 && !command->mBlock) {
                command->mBlock = JKRAllocFromAram(fileSize, JKRAramHeap::HEAD);
            }

            if (command->mBlock) {
                command->mAddress = command->mBlock->getAddress();
            }

            if (command->mAddress == 0) {
                dvdFile->field_0x50 = nullptr;
                return nullptr;
            }
        }

        if (compression == 0) {
            command->mStreamCommand =
                JKRStreamToAram_Async(stream, command->mAddress, fileSize - command->field_0x3c, command->field_0x3c, nullptr, command->field_0x44);
        } else if (compression == 1) {
            command->mStreamCommand =
                JKRStreamToAram_Async(stream, command->mAddress, fileSize - command->field_0x3c, command->field_0x3c, nullptr, command->field_0x44);
        } else if (compression == 2) {
            command->mStreamCommand = nullptr;
            JKRDecompressFromDVDToAram(command->mDvdFile, command->mAddress, fileSize, uncompressedSize, command->field_0x3c, 0, command->field_0x44);
        }

        if (!command->mCallback) {
            sDvdAramAsyncList.append(&command->mLink);
        } else {
            command->mCallback(reinterpret_cast< u32 >(command));
        }
    }

    OSUnlockMutex(&dvdFile->mMutex2);
    return bVar1 == true ? command : nullptr;
}

bool JKRDvdAramRipper::syncAram(JKRADCommand* command, int param_1) {
    JKRDvdFile* dvdFile = command->mDvdFile;
    OSLockMutex(&dvdFile->mMutex2);

    if (command->mStreamCommand) {
        JKRAramStreamCommand* var1 = JKRStreamToAram_Sync(command->mStreamCommand, param_1);
        command->field_0x48 = (var1) ? 0 : -1;

        if (param_1 != 0 && var1 == nullptr) {
            OSUnlockMutex(&dvdFile->mMutex2);
            return false;
        }
    }

    sDvdAramAsyncList.remove(&command->mLink);
    if (command->mStreamCommand) {
        delete command->mStreamCommand;
    }

    delete dvdFile->mFileStream;
    dvdFile->field_0x50 = nullptr;
    OSUnlockMutex(&dvdFile->mMutex2);
    return true;
}

JKRADCommand::JKRADCommand() : mLink(this) {
    field_0x48 = 0;
    field_0x4c = 0;
}

JKRADCommand::~JKRADCommand() {
    if (field_0x4c == 1) {
        delete mDvdFile;
    }
}

static OSMutex decompMutex;

u32 JKRDvdAramRipper::sSZSBufferSize = 0x00000400;

static u8* szpBuf;

static u8* szpEnd;

static u8* refBuf;

static u8* refEnd;

static u8* refCurrent;

static u8* dmaBuf;

static u8* dmaEnd;

static u8* dmaCurrent;

static u32 srcOffset;

static u32 transLeft;

static u8* srcLimit;

static JKRDvdFile* srcFile;

static u32 fileOffset;

static int readCount;

static u32 maxDest;

static bool data_804514A4;

static u32* tsPtr;

static u32 tsArea;

int JKRDecompressFromDVDToAram(JKRDvdFile* dvdFile, u32 param_1, u32 fileSize, u32 uncompressedSize, u32 param_4, u32 param_5, u32* param_6) {
    BOOL level = OSDisableInterrupts();
    if (!data_804514A4) {
        OSInitMutex(&decompMutex);
        data_804514A4 = true;
    }

    OSRestoreInterrupts(level);
    OSLockMutex(&decompMutex);
    int result = 0;
    u32 bufferSize = JKRDvdAramRipper::getSZSBufferSize();
    szpBuf = static_cast< u8* >(JKRAllocFromSysHeap(bufferSize, 0x20));

    szpEnd = szpBuf + bufferSize;
    refBuf = static_cast< u8* >(JKRAllocFromSysHeap(0x1120, 0));

    refEnd = refBuf + 0x1120;
    refCurrent = refBuf;
    dmaBuf = static_cast< u8* >(JKRAllocFromSysHeap(0x100, 0x20));

    dmaEnd = dmaBuf + 0x100;
    dmaCurrent = dmaBuf;
    srcFile = dvdFile;
    srcOffset = param_5;
    transLeft = fileSize - srcOffset;
    fileOffset = param_4;
    readCount = 0;
    maxDest = uncompressedSize;
    tsPtr = param_6 ? param_6 : &tsArea;
    *tsPtr = 0;
    u8* first = firstSrcData();
    if (first) {
        result = decompSZS_subroutine(first, param_1);
    } else {
        result = -1;
    }
    JKRFree(szpBuf);
    JKRFree(refBuf);
    JKRFree(dmaBuf);
    OSUnlockMutex(&decompMutex);
    return result;
}

static inline int decompSZS_subroutine(u8* src, u32 dest) {
    u32 endAddr;
    u8* copySource;
    s32 validBitCount;
    u32 currCodeByte;
    s32 numBytes;
    u32 bytesWritten;
    u32 startDest;

    validBitCount = 0;
    currCodeByte = 0;
    bytesWritten = 0;
    startDest = dest;

    if (src[0] != 'Y' || src[1] != 'a' || src[2] != 'z' || src[3] != '0') {
        return -1;
    }

    SYaz0Header* header = reinterpret_cast< SYaz0Header* >(src);
    endAddr = dest + (header->length - fileOffset);
    if (endAddr > dest + maxDest) {
        endAddr = dest + maxDest;
    }

    src += 0x10;
    s32 b1;
    u32 dist;
    do {
        if (validBitCount == 0) {
            if ((src > srcLimit) && transLeft) {
                src = nextSrcData(src);
            }
            currCodeByte = *src;
            validBitCount = 8;
            src++;
        }
        if (currCodeByte & 0x80) {
            if (readCount >= fileOffset) {
                *(dmaCurrent++) = *src;
                dest++;
                bytesWritten++;
                if (dmaCurrent == dmaEnd) {
                    startDest += dmaBufferFlush(startDest);
                }
                if (dest == endAddr) {
                    break;
                }
            }
            *(refCurrent++) = *src;
            if (refCurrent == refEnd) {
                refCurrent = refBuf;
            }
            src++;
            readCount++;
        } else {
            b1 = src[0];
            dist = src[1] | ((b1 & 0x0f) << 8);
            numBytes = b1 >> 4;
            src += 2;
            copySource = refCurrent - dist - 1;
            if (copySource < refBuf) {
                copySource += refEnd - refBuf;
            }
            if (numBytes == 0) {
                numBytes = *(src++);
                numBytes += 0x12;
            } else {
                numBytes += 2;
            }
            do {
                if (readCount >= fileOffset) {
                    *(dmaCurrent++) = *copySource;
                    dest++;
                    bytesWritten++;
                    if (dmaCurrent == dmaEnd) {
                        startDest += dmaBufferFlush(startDest);
                    }
                    if (dest == endAddr) {
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
        }
        currCodeByte <<= 1;
        validBitCount--;
    } while (dest < endAddr);

    dmaBufferFlush(startDest);
    *tsPtr = bytesWritten;
    return 0;
}

static u8* firstSrcData() {
    srcLimit = szpEnd - 0x19;
    u8* buffer = szpBuf;
    u32 bufSize = szpEnd - buffer;
    u32 length = transLeft < bufSize ? transLeft : bufSize;
    while (true) {
        int result = DVDReadPrio(srcFile->getFileInfo(), buffer, length, 0, 2);
        if (result >= 0) {
            break;
        }
        if (!JKRDvdAramRipper::isErrorRetry()) {
            return nullptr;
        }
        VIWaitForRetrace();
    }
    srcOffset += length;
    transLeft -= length;
    return buffer;
}

static inline u8* nextSrcData(u8* src) {
    u32 size = szpEnd - src;
    u8* dest;
    if (IS_NOT_ALIGNED(size, 0x20)) {
        dest = szpBuf + 0x20 - (size & (0x20 - 1));
    } else {
        dest = szpBuf;
    }

    memcpy(dest, src, size);
    u32 transSize = szpEnd - (dest + size);
    if (transSize > transLeft) {
        transSize = transLeft;
    }

    while (true) {
        s32 result = DVDReadPrio(srcFile->getFileInfo(), dest + size, transSize, srcOffset, 2);
        if (result >= 0) {
            break;
        }
        if (!JKRDvdAramRipper::isErrorRetry()) {
            return nullptr;
        }
        VIWaitForRetrace();
    }
    srcOffset += transSize;
    transLeft -= transSize;
    if (transLeft == 0) {
        srcLimit = dest + size + transSize;
    }
    return dest;
}

static u32 dmaBufferFlush(u32 param_1) {
    if (dmaCurrent == dmaBuf) {
        return 0;
    }
    u32 size = ALIGN_NEXT(dmaCurrent - dmaBuf, 0x20);
    JKRAramPcs(0, reinterpret_cast< u32 >(dmaBuf), param_1, size, nullptr);
    dmaCurrent = dmaBuf;
    return size;
}
