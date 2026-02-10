#pragma once

#include "JSystem/JKernel/JKRThread.hpp"

class JKRAMCommand;

enum EJKRCompression { JKR_COMPRESSION_NONE = 0, JKR_COMPRESSION_SZP = 1, JKR_COMPRESSION_SZS = 2, JKR_COMPRESSION_ASR = 3 };

class JKRDecompCommand {
public:
    JKRDecompCommand();

    u8 _0[4];
    u8* mSrc;               // 0x4
    u8* mDst;               // 0x8
    u32 mCompressedSize;    // 0xC
    u32 mDecompressedSize;  // 0x10
    void (*_14)(u32);
    JKRDecompCommand* mThis;  // 0x18
    OSMessageQueue* _1C;
    s32 _20;
    JKRAMCommand* mAmCommand;      // 0x24
    OSMessageQueue mMessageQueue;  // 0x28
    OSMessage mMessage;            // 0x48
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
};
