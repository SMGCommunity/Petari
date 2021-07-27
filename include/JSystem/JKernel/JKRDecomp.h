#pragma once

#include "JSystem/JKernel/JKRThread.h"
#include "JSystem/JKernel/JKRAramPiece.h"

class JKRDecompCommand
{
public: 
    typedef void (*AsyncCallback)(u32);

    JKRDecompCommand();

    u32 _0;
    u8* _4;
    u8* _8;
    u32 _C;
    u32 _10;
    JKRDecompCommand::AsyncCallback _14;
    JKRDecompCommand* _18;
    OSMessageQueue* _1C;
    u32 _20;
    JKRAMCommand* _24;
    OSMessageQueue _28;
    OSMessage _48;
};

class JKRDecomp : public JKRThread
{
public:
    JKRDecomp(s32);

    virtual ~JKRDecomp();
    virtual s32 run();

    static JKRDecomp* create(s32);

    static JKRDecompCommand* prepareCommand(u8 *, u8 *, u32, u32, JKRDecompCommand::AsyncCallback);
    static void sendCommand(JKRDecompCommand *);
    static bool sync(JKRDecompCommand *, int);
    static bool orderSync(u8 *, u8 *, u32, u32);

    static void decode(u8 *, u8 *, u32, u32);
    static void decodeSZP(u8 *, u8 *, u32, u32);
    static void decodeSZS(u8 *, u8 *, u32, u32);
    static s32 checkCompressed(u8 *);

    static OSMessage sMessageBuffer;
    static OSMessageQueue sMessageQueue;
    static JKRDecomp* sDecompObject;
};

