#pragma once

#include "JSystem/JKernel/JKRFileLoader.h"

class JKRArchive : public JKRFileLoader {
public:
    virtual ~JKRArchive();

    virtual u32 becomeCurrent(const char *) = 0;
    virtual void* getResource(const char *) = 0;
    virtual void* getResource(u32, const char *) = 0;
    virtual u32 readResource(void *, u32, const char *) = 0;
    virtual u32 readResource(void *, u32, u32, const char *) = 0;
    virtual void removeResourceAll() = 0;
    virtual void removeResource(void *);
    virtual void detatchResource(void *);
    virtual s32 getResSize(const void *) const;
    virtual s32 countFile(const char *) const;
    /* todo */
};