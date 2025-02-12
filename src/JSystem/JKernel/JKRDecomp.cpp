#include "JSystem/JKernel/JKRDecomp.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"
#include "JSystem/JKernel/JKRAramPiece.hpp"
#include "revolution.h"

#define NR_MESSAGES 8

namespace {
    JKRDecomp *gDecompInstance; // 0x806B71D8
    OSMessage gMessage[NR_MESSAGES]; // 0x8060D038
    OSMessageQueue gMessageQueue; // 0x8060D058
}

JKRDecompCommand::JKRDecompCommand() {
    OSInitMessageQueue(&mMessageQueue, &mMessage, 1);
    mThis = this; // Probably a pointer to the data stored at 0x00 instead
    _14 = 0;
    _1C = nullptr;
    _20 = 0;
}

JKRDecomp::JKRDecomp(long a1) : JKRThread(0x4000, 0x10, a1) {
    OSResumeThread(mThread);
}

JKRDecomp::~JKRDecomp() {

}

s32 JKRDecomp::run() {
    OSInitMessageQueue(&gMessageQueue, &gMessage[0], NR_MESSAGES);

    JKRDecompCommand *commandPtr;

    while (true) {
        OSReceiveMessage(&gMessageQueue, reinterpret_cast<OSMessage *>(&commandPtr), OS_MESSAGE_BLOCK);

        JKRDecompCommand &command = *commandPtr;

        decode(
            command.mSrc,
            command.mDst,
            command.mCompressedSize,
            command.mDecompressedSize
            );

        if (command._20 != 0) {
            if (command._20 == 1) {
                JKRAramPiece::sendCommand(command.mAmCommand);
            }

            continue;
        }

        if (command._14 != nullptr) {
            command._14(reinterpret_cast<u32>(&command));
            continue;
        }

        if (command._1C != nullptr) {
            OSSendMessage(command._1C, reinterpret_cast<OSMessage>(1), OS_MESSAGE_NOBLOCK);
        }
        else {
            OSSendMessage(&command.mMessageQueue, reinterpret_cast<OSMessage>(1), OS_MESSAGE_NOBLOCK);
        }
    }
}

JKRDecomp *JKRDecomp::create(long a1) {
    if (gDecompInstance == nullptr) {
        gDecompInstance = new(JKRHeap::sGameHeap, 0) JKRDecomp(a1);
    }

    return gDecompInstance;
}

JKRDecompCommand *JKRDecomp::prepareCommand(unsigned char *pSrc, unsigned char *pDst, unsigned long compressedSize, unsigned long decompressedSize, void (*a5)(unsigned long)) {
    JKRDecompCommand *command = new(JKRHeap::sGameHeap, -4) JKRDecompCommand();

    command->mSrc = pSrc;
    command->mDst = pDst;
    command->mCompressedSize = compressedSize;
    command->mDecompressedSize = decompressedSize;
    command->_14 = a5;

    return command;
}

void JKRDecomp::sendCommand(JKRDecompCommand *pCommand) {
    OSSendMessage(&gMessageQueue, pCommand, OS_MESSAGE_NOBLOCK);
}

bool JKRDecomp::sync(JKRDecompCommand *pCommand, int noBlock) {
    OSMessage message;

    if (noBlock == 0) {
        OSReceiveMessage(&pCommand->mMessageQueue, &message, OS_MESSAGE_BLOCK);
        return true;
    }
    else {
        return OSReceiveMessage(&pCommand->mMessageQueue, &message, OS_MESSAGE_NOBLOCK) != 0;
    }
}

bool JKRDecomp::orderSync(unsigned char *pSrc, unsigned char *pDst, unsigned long compressedSize, unsigned long decompressedSize) {
    JKRDecompCommand *command = prepareCommand(pSrc, pDst, compressedSize, decompressedSize,nullptr);

    OSSendMessage(&gMessageQueue, command, OS_MESSAGE_NOBLOCK);
    bool received = sync(command, 0);

    delete command;

    return received;
}

void JKRDecomp::decode(unsigned char *pSrc, unsigned char *pDst, unsigned long compressedSize, unsigned long decompressedSize) {
    EJKRCompression compression = checkCompressed(pSrc);

    if (compression == JKR_COMPRESSION_SZP) {
        decodeSZP(pSrc, pDst, compressedSize, decompressedSize);
    }
    else if (compression == JKR_COMPRESSION_SZS) {
        decodeSZS(pSrc, pDst, compressedSize, decompressedSize);
    }
}

/*void JKRDecomp::decodeSZP(unsigned char *pSrc, unsigned char *pDst, unsigned long compressedSize, unsigned long decompressedSize) {

}*/

#ifdef NON_MATCHING // Wrong registers
void JKRDecomp::decodeSZS(u8 *pSrc, u8 *pDst, u32 compressedSize, u32 a4) {
    u32 decompSize = ((s32)pDst + *(u32*)(pSrc + 4)) - a4;
    u8 byte1, byte2;
    s32 validBitCount = 0;
    u32 curBlock;

    if (compressedSize == 0) {
        return;
    }

    if (a4 > *(u32*)pSrc) {
        return;
    }

    pSrc += 0x10;

    do {
        if (validBitCount == 0) {
            curBlock = *pSrc;
            validBitCount = 8;
            pSrc++;
        }

        if ((curBlock & 0x80) != 0) {
            if (a4 == 0) {
                compressedSize--;
                *pDst++ = *pSrc;

                if (compressedSize == 0) {
                    return;
                }
            }
            else {
                a4--;
            }

            pSrc++;
        }
        else {
            // This bit specifically
            byte1 = *pSrc++;
            byte2 = *pSrc++;

            u8* copySrc = pDst;
            copySrc -= ((byte1 & 0xF) << 8) | byte2;;
            u32 numBytes = byte1 >> 4;

            if (numBytes == 0) {
                numBytes = *pSrc++ + 0x12;
            }
            else {
                numBytes += 2;
            }
            
            do {
               if (a4 == 0) {
                    compressedSize--;
                    *pDst = *(copySrc - 1);
                    pDst++;

                    if (compressedSize == 0) {
                        return;
                    }
                }
                else {
                    a4--;
                }

                copySrc++;
            } while (--numBytes != 0);
        }

        curBlock <<= 1;
        validBitCount--;
    } while ((u32)pDst != decompSize);
}
#endif

EJKRCompression JKRDecomp::checkCompressed(unsigned char *pSrc) {
    if (pSrc[0] == 'Y' && pSrc[1] == 'a' && pSrc[3] == '0') {
        if (pSrc[2] == 'y') {
            return JKR_COMPRESSION_SZP;
        }
        
        if (pSrc[2] == 'z') {
            return JKR_COMPRESSION_SZS;
        }
    }

    if (pSrc[0] == 'A' && pSrc[1] == 'S' && pSrc[2] == 'R') {
        return JKR_COMPRESSION_ASR;
    }

    return JKR_COMPRESSION_NONE;
}
