#ifndef JUTEXCEPTION_H
#define JUTEXCEPTION_H

#include "types.h"

class JUTException
{
public:
    static void panic_f(const char *, s32, const char *, ...);
};

#endif // JUTEXCEPTION_H