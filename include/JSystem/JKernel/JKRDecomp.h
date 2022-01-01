#pragma once

#include "JSystem/JKernel/JKRThread.h"

enum EJKRCompression {
    JKR_COMPRESSION_NONE = 0,
    JKR_COMPRESSION_SZP = 1,
    JKR_COMPRESSION_SZS = 2,
};

class JKRDecompCommand {
public:
    JKRDecompCommand();
};

class JKRDecomp : public JKRThread {
public:
    JKRDecomp(long);
    virtual ~JKRDecomp();

    virtual s32 run();

    static JKRDecomp *create(long);

    void prepareCommand(unsigned char *, unsigned char *, unsigned long, unsigned long, void(unsigned long));
    void sendCommand(JKRDecompCommand *);
    void sync(JKRDecompCommand *, int);
    void orderSync(unsigned char *, unsigned char *, unsigned long, unsigned long);
    void decode(unsigned char *, unsigned char *, unsigned long, unsigned long);
    void decodeSZP(unsigned char *, unsigned char *, unsigned long, unsigned long);
    void decodeSZS(unsigned char *, unsigned char *, unsigned long, unsigned long);
    EJKRCompression checkCompressed(unsigned char *);

    static JKRDecomp *sDecompInstance; // _806B71D8
};