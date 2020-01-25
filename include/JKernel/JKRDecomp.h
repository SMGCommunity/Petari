#ifndef JKRDECOMP_H
#define JKRDECOMP_H

#include "JKernel/JKRThread.h"
#include "revolution/os/OSMessage.h"

class JKRDecompCommand;

class JKRDecomp
{
public:
    JKRDecomp(s32);

    virtual ~JKRDecomp();
    virtual s32 run();

    static void decode(u8 *, u8 *, u32, u32);
    static void decodeSZP(u8 *, u8 *, u32, u32);
    static void decodeSZS(u8 *, u8 *, u32, u32);
    static s32 checkCompressed(u8 *);

    static OSMessage sMessageBuffer;
    static OSMessageQueue sMessageQueue;
};

class JKRDecompCommand
{
public:
    JKRDecompCommand();

    u32 _0;
    u32 _4;
    u32 _8;
    u32 _C;
    u32 _10;
    u32 _14;
    u32 _18;
    u32 _20;
    u32 _24;
    OSMessageQueue _28;
    OSMessage _48;
};

#endif // JKRDECOMP_H