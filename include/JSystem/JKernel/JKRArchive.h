#pragma once

#include "JSystem/JKernel/JKRFileLoader.h"

class JKRArchive {
public:
    virtual ~JKRArchive();

    virtual u32 becomeCurrent(const char *) = 0;
    virtual void* getResource(const char *) = 0;
    virtual void* getResource(u32, const char *) = 0;
    virtual u32 readResource(void *, u32, const char *) = 0;

    /* todo */
};