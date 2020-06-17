#ifndef JKRARAMSTREAM_H
#define JKRARAMSTREAM_H

#include "JKernel/JKRThread.h"

class JKRAramStream : public JKRThread
{
public:
    JKRAramStream(s32);

    virtual ~JKRAramStream();
    virtual s32 run();

    static JKRAramStream* create(s32);
};

#endif // JKRARAMSTREAM_H