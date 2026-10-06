#pragma once

#include "JSystem/JKernel/JKRCompression.hpp"
#include "JSystem/JKernel/JKRThread.hpp"

class JKRAMCommand;

enum EJKRCompression { JKR_COMPRESSION_NONE = 0, JKR_COMPRESSION_SZP = 1, JKR_COMPRESSION_SZS = 2, JKR_COMPRESSION_ASR = 3 };

class JKRDecompCommand {
public:
    JKRDecompCommand();

    /* 0x00 */ u8 _0[4];
    /* 0x04 */ u8* mSrc;
    /* 0x08 */ u8* mDst;
    /* 0x0C */ u32 mCompressedSize;
    /* 0x10 */ u32 mDecompressedSize;
    /* 0x14 */ void (*_14)(u32);
    /* 0x18 */ JKRDecompCommand* mThis;
    /* 0x1C */ OSMessageQueue* _1C;
    /* 0x20 */ s32 _20;
    /* 0x24 */ JKRAMCommand* mAmCommand;
    /* 0x28 */ OSMessageQueue mMessageQueue;
    /* 0x48 */ OSMessage mMessage;
};

class JKRDecomp : public JKRThread {
public:
    JKRDecomp(long);
    virtual ~JKRDecomp();

    virtual void* run();

    static JKRDecomp* create(long);
    static JKRDecompCommand* prepareCommand(unsigned char*, unsigned char*, unsigned long, unsigned long, void (*)(unsigned long));
    static void sendCommand(JKRDecompCommand*);
    static bool sync(JKRDecompCommand*, int);
    static bool orderSync(unsigned char*, unsigned char*, unsigned long, unsigned long);
    static void decode(unsigned char*, unsigned char*, unsigned long, unsigned long);
    static void decodeSZP(unsigned char*, unsigned char*, unsigned long, unsigned long);
    static void decodeSZS(u8*, u8*, u32, u32);
    static EJKRCompression checkCompressed(unsigned char*);

    static JKRDecomp* sDecompObject;
    static OSMessage sMessageBuffer[8];
    static OSMessageQueue sMessageQueue;
};

inline void JKRDecompress(u8* srcBuffer, u8* dstBuffer, u32 srcLength, u32 dstLength) {
    JKRDecomp::orderSync(srcBuffer, dstBuffer, srcLength, dstLength);
}

inline JKRDecomp* JKRCreateDecompManager(s32 priority) {
    return JKRDecomp::create(priority);
}

inline JKRCompression JKRCheckCompressed_noASR(u8* pBuf) {
    JKRCompression compression = JKRDecomp::checkCompressed(pBuf);

    if (compression == COMPRESSION_ASR) {
        compression = COMPRESSION_NONE;
    }

    return compression;
}

inline u32 JKRDecompExpandSize(u8* pBuf) {
    return (pBuf[4] << 0x18) | (pBuf[5] << 0x10) | (pBuf[6] << 8) | pBuf[7];
}

inline void JKRDecompress_SendCommand(JKRDecompCommand* command) {
    JKRDecomp::sendCommand(command);
}
