#ifndef CHARSTRMREADER_H
#define CHARSTRMREADER_H

#include "types.h"

class CharStrmReader
{
public:
    typedef u16 (CharStrmReader::*ReadNextCharFunc)();

    u16 ReadNextCharUTF8();
    u16 ReadNextCharUTF16();
    u16 ReadNextCharCP1252();
    u16 ReadNextCharSJIS();

    const void* mStream;
    const ReadNextCharFunc mReadFunc;

    explicit CharStrmReader(ReadNextCharFunc func) : mStream(NULL), mReadFunc(func)
    { }
};

#endif // CHARSTRMREADER_H