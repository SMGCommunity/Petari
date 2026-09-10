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

static int JKRDecompressFromDVDToAram(JKRDvdFile*, u32, u32, u32, u32, u32, u32*);
static inline u8* firstSrcData();
static u32 dmaBufferFlush(u32);

JKRAramBlock* JKRDvdAramRipper::loadToAram(s32 entryNumber, u32 address, JKRExpandSwitch expandSwitch, u32 param_3, u32 param_4, u32* param_5) {
    JKRDvdFile dvdFile;
    if (!dvdFile.open(entryNumber)) {
        return NULL;
    } else {
        return loadToAram(&dvdFile, address, expandSwitch, param_3, param_4, param_5);
    }
}

JKRAramBlock* JKRDvdAramRipper::loadToAram(JKRDvdFile* dvdFile, u32 address, JKRExpandSwitch expandSwitch, u32 param_3, u32 param_4, u32* param_5) {
    JKRADCommand* command = loadToAram_Async(dvdFile, address, expandSwitch, NULL, param_3, param_4, param_5);
    syncAram(command, 0);

    if (command->field_0x48 < 0) {
        delete command;
        return NULL;
    }

    if (address) {
        delete command;
        return (JKRAramBlock*)-1;
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
    command->mBlock = NULL;
    command->mExpandSwitch = expandSwitch;
    command->mCallback = callback;
    command->field_0x3c = param_4;
    command->field_0x40 = param_5;
    command->field_0x44 = param_6;

    JKRADCommand* cmd2 = callCommand_Async(command);
    if (!cmd2) {
        delete command;
        return NULL;
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
            u8* bufPtr = (u8*)ALIGN_NEXT((u32)&buffer, 0x20);
            while (true) {
                s32 result = DVDReadPrio(dvdFile->getFileInfo(), bufPtr, 0x20, 0, 2);
                if (result >= 0) {
                    break;
                }

                if (JKRDvdAramRipper::errorRetry == 0) {
                    delete stream;
                    return NULL;
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
            if (command->mAddress == 0 && command->mBlock == NULL) {
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
                dvdFile->field_0x50 = NULL;
                return NULL;
            }
        } else {
            if (command->mAddress == 0 && !command->mBlock) {
                command->mBlock = JKRAllocFromAram(fileSize, JKRAramHeap::HEAD);
            }

            if (command->mBlock) {
                command->mAddress = command->mBlock->getAddress();
            }

            if (command->mAddress == 0) {
                dvdFile->field_0x50 = NULL;
                return NULL;
            }
        }

        if (compression == 0) {
            command->mStreamCommand =
                JKRStreamToAram_Async(stream, command->mAddress, fileSize - command->field_0x3c, command->field_0x3c, NULL, command->field_0x44);
        } else if (compression == 1) {
            command->mStreamCommand =
                JKRStreamToAram_Async(stream, command->mAddress, fileSize - command->field_0x3c, command->field_0x3c, NULL, command->field_0x44);
        } else if (compression == 2) {
            command->mStreamCommand = NULL;
            JKRDecompressFromDVDToAram(command->mDvdFile, command->mAddress, fileSize, uncompressedSize, command->field_0x3c, 0, command->field_0x44);
        }

        if (!command->mCallback) {
            (*((JSUList< JKRADCommand >*)&sDvdAramAsyncList)).append(&command->mLink);
        } else {
            command->mCallback((u32)command);
        }
    }

    OSUnlockMutex(&dvdFile->mMutex2);
    return bVar1 == true ? command : NULL;
}

bool JKRDvdAramRipper::syncAram(JKRADCommand* command, int param_1) {
    JKRDvdFile* dvdFile = command->mDvdFile;
    OSLockMutex(&dvdFile->mMutex2);

    if (command->mStreamCommand) {
        JKRAramStreamCommand* var1 = JKRStreamToAram_Sync(command->mStreamCommand, param_1);
        command->field_0x48 = (var1) ? 0 : -1;

        if (param_1 != 0 && var1 == NULL) {
            OSUnlockMutex(&dvdFile->mMutex2);
            return false;
        }
    }

    (*((JSUList< JKRADCommand >*)&sDvdAramAsyncList)).remove(&command->mLink);
    if (command->mStreamCommand) {
        delete command->mStreamCommand;
    }

    delete dvdFile->mFileStream;
    dvdFile->field_0x50 = NULL;
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
    szpBuf = (u8*)JKRAllocFromSysHeap(bufferSize, 0x20);
    szpEnd = szpBuf + bufferSize;
    refBuf = (u8*)JKRAllocFromSysHeap(0x1120, 0);

    refEnd = refBuf + 0x1120;
    refCurrent = refBuf;
    dmaBuf = (u8*)JKRAllocFromSysHeap(0x100, 0x20);
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
    srcLimit = szpEnd - 0x19;
    u8* first = szpBuf;
    u32 readSize = szpEnd - first;
    if (transLeft < readSize) {
        readSize = transLeft;
    }
    while (true) {
        int readResult = DVDReadPrio(srcFile->getFileInfo(), first, readSize, 0, 2);
        if (readResult >= 0) {
            break;
        }
        if (!JKRDvdAramRipper::isErrorRetry()) {
            first = nullptr;
            goto firstReadComplete;
        }
        VIWaitForRetrace();
    }
    srcOffset += readSize;
    transLeft -= readSize;
firstReadComplete:
    if (first) {
        u8* src;
        u32 dest = param_1;
        u32 dmaAddress = dest;
        u32 written;
        s32 count;
        u32 code;
        s32 validBits;
        u8* copySrc;
        validBits = 0;
        code = 0;
        written = 0;

        if (first[0] != 'Y' || first[1] != 'a' || first[2] != 'z' || first[3] != '0') {
            result = -1;
        } else {
            u32 end = dest + (*reinterpret_cast< u32* >(first + 4) - fileOffset);
            if (end > dest + maxDest) {
                end = dest + maxDest;
            }

            src = first + 0x10;
            do {
                if (validBits == 0) {
                    if (src > srcLimit && transLeft != 0) {
                        u32 size = szpEnd - src;
                        u8* buffer;
                        if (IS_NOT_ALIGNED(size, 0x20)) {
                            buffer = szpBuf + 0x20 - (size & (0x20 - 1));
                        } else {
                            buffer = szpBuf;
                        }

                        memcpy(buffer, src, size);
                        u32 transferSize;
                        u8* readBuffer = buffer + size;
                        transferSize = szpEnd - readBuffer;
                        if (transferSize > transLeft) {
                            transferSize = transLeft;
                        }

                        while (true) {
                            s32 readResult = DVDReadPrio(srcFile->getFileInfo(), readBuffer, transferSize, srcOffset, 2);
                            if (readResult >= 0) {
                                break;
                            }
                            if (!JKRDvdAramRipper::isErrorRetry()) {
                                buffer = nullptr;
                                goto refillComplete;
                            }
                            VIWaitForRetrace();
                        }
                        srcOffset += transferSize;
                        transLeft -= transferSize;
                        if (transLeft == 0) {
                            srcLimit = readBuffer + transferSize;
                        }
                    refillComplete:
                        src = buffer;
                    }
                    code = *src++;
                    validBits = 8;
                }

                if (code & 0x80) {
                    if (readCount >= fileOffset) {
                        *dmaCurrent++ = *src;
                        dest++;
                        written++;
                        if (dmaCurrent == dmaEnd) {
                            dmaAddress += dmaBufferFlush(dmaAddress);
                        }
                        if (dest == end) {
                            break;
                        }
                    }

                    *refCurrent++ = *src;
                    if (refCurrent == refEnd) {
                        refCurrent = refBuf;
                    }
                    readCount++;
                    src++;
                } else {
                    s32 firstByte = *src++;
                    u8 secondByte = *src++;
                    copySrc = refCurrent - (((firstByte & 0xF) << 8) | secondByte) - 1;
                    count = firstByte >> 4;
                    if (copySrc < refBuf) {
                        copySrc += refEnd - refBuf;
                    }

                    if (count == 0) {
                        count = *src++ + 0x12;
                    } else {
                        count += 2;
                    }

                    do {
                        if (readCount >= fileOffset) {
                            *dmaCurrent++ = *copySrc;
                            dest++;
                            written++;
                            if (dmaCurrent == dmaEnd) {
                                dmaAddress += dmaBufferFlush(dmaAddress);
                            }
                            if (dest == end) {
                                break;
                            }
                        }

                        *refCurrent++ = *copySrc;
                        if (refCurrent == refEnd) {
                            refCurrent = refBuf;
                        }
                        copySrc++;
                        if (copySrc == refEnd) {
                            copySrc = refBuf;
                        }
                        readCount++;
                    } while (--count != 0);
                }

                code <<= 1;
                validBits--;
            } while (dest < end);

            dmaBufferFlush(dmaAddress);
            *tsPtr = written;
            result = 0;
        }
    } else {
        result = -1;
    }
    JKRFree(szpBuf);
    JKRFree(refBuf);
    JKRFree(dmaBuf);
    OSUnlockMutex(&decompMutex);
    return result;
}

static u32 dmaBufferFlush(u32 param_1) {
    if (dmaCurrent == dmaBuf) {
        return 0;
    }
    u32 size = ALIGN_NEXT(dmaCurrent - dmaBuf, 0x20);
    JKRAramPcs(0, (u32)dmaBuf, param_1, size, NULL);
    dmaCurrent = dmaBuf;
    return size;
}
